//--Browser compatibility abstraction--//
window.WebSocket = window.WebSocket || window.MozWebSocket;
navigator.getUserMedia = navigator.getUserMedia || navigator.webkitGetUserMedia || navigator.mozGetUserMedia || navigator.msGetUserMedia;
window.URL = window.URL || window.webkitURL;

var camTrueScreenFalse = true;

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

//screen cast
function vid(){
	camTrueScreenFalse = false;
}
//video call
function cam(){
	camTrueScreenFalse = true;
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
	if(camTrueScreenFalse){
		var constraints = {audio:true, video:true};
	}else{
		var constraints = {video:{mandatory:{chromeMediaSource:'screen'}}};
	}
	navigator.getUserMedia(
		constraints, 
		function(mediaStream){
			console.log("local stream enabled");

			videoLocal = document.getElementById("vidLocal");
			videoRemote = document.getElementById("vidRemote");//initialize remote viewer too
			localStream = mediaStream;
			P2PEndpoint.addStream(localStream);
			videoLocal.src = window.URL.createObjectURL(localStream);
			//document.getElementById("myCanvas").context.drawImage(videoLocal,0,0);
			document.getElementById("call").disabled=false;
		//	document.getElementById("myCanvas").webkitRequestFullScreen(Element.ALLOW_KEYBOARD_INPUT); //Chrome
		}, 
		function(error) {
			console.log(error);
		}
	);

}

function hangup(){
}

//NameList Widget
function updateNameListWidget(){
	connection.send("/updatewidget list");
}

//MouseShare Widget
function updateMouseEvent(){
	connection.send("/updatewidget mouse "+window.event.clientX+ " "+window.event.clientY);
}

window.resize( function(){
    var w = window.width();
    var h = window.height();

    $("#myCanvas").css("width", w + "px");
    $("#myCanvas").css("height", h + "px"); 
});

function draw(v,c,w,h) {
    if(v.paused || v.ended) return false;
    c.drawImage(v,0,0,w,h);
    setTimeout(draw,20,v,c,w,h);
}
