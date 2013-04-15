//connection to websocket lobby
var connection = null;

//WebSocket logic
//--WebSocket Connection Handlers--//
function WSLobbyConnect(){
	connection = new WebSocket('ws://192.168.10.105:8888','echo');
	connection.onopen = myWSOnOpen;
	connection.onmessage = myWSOnMessage;
	connection.onerror = myWSOnError;

	//window.setInterval(updateNameListWidget,2000);
	document.onmousemove=updateMouseEvent;
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
	var patternUpdateWidget=/^\/widgetupdate /g;

	if(patternJS.test(message.data)){
		eval(message.data.replace(patternJS,""));
	}else if(patternUpdateWidget.test(message.data)){
		var messageFragment = message.data.replace(patternUpdateWidget,"");//pop the first command string and continue with the rest
		var patternMouse=/mouse /g;

		if(patternMouse.test(messageFragment)){
			messageFragment = messageFragment.replace(patternMouse,"");//pop the first command string and continue with the rest
			var coords = messageFragment.split(" ");
			document.getElementById('reticle').style.position = "absolute";
			document.getElementById('reticle').style.left = coords[0]-47;
			document.getElementById('reticle').style.top = coords[1]-47;
		}else{
			document.getElementById('nameList').innerHTML="Online Users:<br>"+messageFragment;
		}
	
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
