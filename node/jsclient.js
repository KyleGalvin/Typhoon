//--Browser compatibility abstraction--//
window.WebSocket = window.WebSocket || window.MozWebSocket;
navigator.getUserMedia = navigator.getUserMedia || navigator.webkitGetUserMedia || navigator.mozGetUserMedia || navigator.msGetUserMedia;
window.URL = window.URL || window.webkitURL;

//connection to websocket lobby
var connection = null;

//webrtc video streams & p2p connection handler
var P2PEndpoint = null;
var videoLocal;
var localStream = null;
var videoRemote;
var remoteStream = null;
var isCaller=false;
var servers = {iceServers:[{url:'stun:stun.l.google.com:19302'}]};//remote ICE servers for path discovery

function myIceEscape(key,val) {
	if (typeof(val)!="string") return val;
	return val
		.replace(/[\"]/g, '\\"')
		.replace(/[\\]/g, '\\\\')
		.replace(/[\/]/g, '\\/')
		.replace(/[\b]/g, '\\b')
		.replace(/[\f]/g, '\\f')
		.replace(/[\n]/g, '\\n')
		.replace(/[\r]/g, '\\r')
		.replace(/[\t]/g, '\\t')
	; 
};
function mySdpEscape(key,val) {
	if (typeof(val)!="string") return val;
	return val
		.replace(/[\"]/g, '\\"')
		.replace(/[\\]/g, '\\\\')
		//.replace(/[\/]/g, '\\/')
		.replace(/[\b]/g, '\\b')
		.replace(/[\f]/g, '\\f')
		//.replace(/[\n]/g, '\\n')
		//.replace(/[\r]/g, '\\r')
		.replace(/[\t]/g, '\\t')
	; 
};


//--WebRTC media & connection handlers--//
function WRTCOnAddStream(event){
	console.log('--Adding Incoming Stream--');
	videoRemote.src = webkitURL.createObjectURL(event.stream);
	remoteStream = event.stream;
}

function WRTCOnError(event){
	alert('error: '+event);
}

function localDescriptionCreated(description){
	console.log("--local information gathered. Relaying to remote peer--");
	P2PEndpoint.setLocalDescription(description);
		var JSONString = JSON.stringify({ "sdp": description },mySdpEscape);//wrap in JSON and send to remote peeer
		connection.send(JSONString);

}

function WRTCOnIceCandidate(event){
	if(event.candidate){
		console.log('Time:'+new Date().getTime()+': Recieved Ice Candidate');
		if(P2PEndpoint.localDescription != null){
			connection.send(JSON.stringify({"candidate": event.candidate},myIceEscape));//wrap the event in JSON and let the other side call p2pendpoint.addIceCandidate()
		}
	}else{
		console.log('end of candidates');
	}
}

function WRTCInit(){
	console.log('--Initializing peer connection--');
	P2PEndpoint = new webkitRTCPeerConnection(servers);
	P2PEndpoint.onicecandidate = WRTCOnIceCandidate;
	P2PEndpoint.onaddstream = WRTCOnAddStream;
}

//--Triggers & User Interface Callbacks--//
function sendMsg(message){
	connection.send('/msg '+message);//the msg flag is passed for general websocket chatroom use
}

document.onkeypress=function(event){
	var charCode = (event.which) ? event.which : event.keyCode
	if(charCode == 13 ){//the 'enter' key
		sendMsg(document.getElementById('inputField').value);
	}
}

function answer(){
	console.log('--answering--');
}

function call(){
	isCaller=true;
	console.log('--creating offer--');
	P2PEndpoint.createOffer(localDescriptionCreated);
}

function startLocal(){
	WRTCInit();
	startLocalVideo();
	WSLobbyConnect();
}

function startLocalVideo(){
	navigator.getUserMedia(
		{audio:true, video: true}, 
		function(mediaStream){
			console.log("local stream enabled");
			videoLocal = document.getElementById("vidLocal");
			videoRemote = document.getElementById("vidRemote");//initialize remote viewer too
			localStream = mediaStream;
			P2PEndpoint.addStream(localStream);
			videoLocal.src = window.URL.createObjectURL(localStream);
			document.getElementById("call").disabled=false;
		}, 
		function(error) {
			alert('errored:',error);
		}
	);

}

function hangup(){
}

//WebSocket logic
//--WebSocket Connection Handlers--//
function WSLobbyConnect(){
	connection = new WebSocket('ws://192.168.0.20:8888','echo');
	connection.onopen = myWSOnOpen;
	connection.onmessage = myWSOnMessage;
	connection.onerror = myWSOnError;

	window.setInterval(updateNameListWidget,2000);
}

function myWSOnOpen() {
	document.getElementById('textField').innerHTML='';//clear chat text
}

function myWSOnError(error) {
	// an error occurred when sending/receiving data
	console.log(error);
}

function myWSOnMessage(message) {
	var patternJS=/^\/js/g;
	var patternMSG=/^\/msg/g;
	var patternListName=/^\/widgetupdate /g

	if(patternJS.test(message.data)){
		eval(message.data.replace(patternJS,""));
	}else if(patternListName.test(message.data)){
		message.data = message.data.replace(patternListName,"");//pop the first command string and continue with the rest
		
		document.getElementById('nameList').innerHTML="Online Users:<br>"+message.data.replace(patternListName,"");
	
	}else if(patternMSG.test(message.data)){
		document.getElementById('textField').innerHTML+=message.data.replace(patternMSG,"")+'<br>';
	}else{
		//WebRTC signalling communication
		var p2pSignal=JSON.parse(message.data);
		if(p2pSignal.sdp){
			if(p2pSignal.sdp.type === 'answer'){
				console.log("--Accepting answer and finalizing handshake--");
				P2PEndpoint.setRemoteDescription(new RTCSessionDescription(p2pSignal.sdp));
			}else if(p2pSignal.sdp.type === 'offer'){
				document.getElementById("accept").disabled=false;
				console.log('--Recieved offer information. Waiting for the signal to answer--');
				P2PEndpoint.setRemoteDescription(new RTCSessionDescription(p2pSignal.sdp),
				function(){
					P2PEndpoint.createAnswer(localDescriptionCreated);
				},
				function(){
					console.log('failed to set remote description');
				});
			}
		}else if(p2pSignal.candidate){
			console.log('Time:'+new Date().getTime()+': Adding Ice Candidate');
			P2PEndpoint.addIceCandidate(new RTCIceCandidate(p2pSignal.candidate));
		}else{
			console.log('unexpected message');
		}
	}
}

//NameList Widget
function updateNameListWidget(){
	connection.send("/updatewidget list");
}
