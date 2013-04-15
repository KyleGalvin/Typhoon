//webrtc video streams & p2p connection handler
var P2PEndpoint = null;
var videoLocal;
var localStream = null;
var videoRemote;
var remoteStream = null;
var isCaller=false;
var servers = {iceServers:[{url:'stun:stun.l.google.com:19302'}]};//remote ICE servers for path discovery

//helper functions for JSONifying ICE and SDP messages
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
