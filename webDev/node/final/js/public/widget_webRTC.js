define(['clientConnection','dropzone'], function(con,dz){



	var widget = function(wFactory,model, libScreen){
		var dropzone = dz
		console.log("dropzone:",dropzone)
		var handleFileChange = function(){
			var file = button2.prop('files')[0]
			console.log("new file selected!",file)
			var reader = new window.FileReader();
			reader.readAsDataURL(file);
			reader.onload = onReadAsDataURL;
			var chunkLength = 1000;
			function onReadAsDataURL(event, text) {
			    var data = {}; // data object to transmit over data channel

			    if (event) text = event.target.result; // on first invocation

			    if (text.length > chunkLength) {
				data.message = text.slice(0, chunkLength); // getting chunk using predefined chunk length
			    } else {
				data.message = text;
				data.last = true;
			    }

			    dataChannel.send(JSON.stringify(data)); // use JSON.stringify for chrome!

			    var remainingDataURL = text.slice(data.message.length);
			    if (remainingDataURL.length) setTimeout(function () {
				onReadAsDataURL(null, remainingDataURL); // continue transmitting
			    }, 500)
			}



		}
						var dropzoneForm= $("<form action='javascript:handleFileChange', class='dropzone'>")
						var button2 = $( "<input type='file', class='dropzone',id='file'>" )
							.css({
								"width":"50px",
								"float":"left",
								"height":"50px",
								"border":"2px solid black",
								"background":"#888888",
							})

						button2.change(handleFileChange)
						var loggedIn = $("#loggedIn")
						loggedIn.append(dropzoneForm)
						dropzoneForm.append(button2)
							console.log("my button!: ",button2)
							console.log("loggedin!: ",loggedIn)
						//var myDropZone = new dropzone("#file",{url:""})


			var div = $("#code")
			console.log('code mirror div',div[0])

		function simulateKeyPress(character) {
			console.log("simulating key press:",character)
			var div = $("#code")
			console.log('code mirror div',div[0])
			div.trigger("keypress",{ which : character.charCodeAt(0) });
		}





		var img = $("<img id='remoteMouse' src='https://www.littlereddevshed.com/img/Cursor_Hand.png'>")
		img.css({"position":"absolute"})
		img.appendTo('body')
		    document.onmousemove = handleMouseMove;
		    function handleMouseMove(event) {
			console.log("mouse moving")
			var dot, eventDoc, doc, body, pageX, pageY;

			event = event || window.event; 

			if (event.pageX == null && event.clientX != null) {
			    eventDoc = (event.target && event.target.ownerDocument) || document;
			    doc = eventDoc.documentElement;
			    body = eventDoc.body;

			    event.pageX = event.clientX +
			      (doc && doc.scrollLeft || body && body.scrollLeft || 0) -
			      (doc && doc.clientLeft || body && body.clientLeft || 0);
			    event.pageY = event.clientY +
			      (doc && doc.scrollTop  || body && body.scrollTop  || 0) -
			      (doc && doc.clientTop  || body && body.clientTop  || 0 );
			 }
			if(dataChannelOn){
				dataChannel.send(JSON.stringify({x:event.pageX,y:event.pageY}))
			}

		    }

		var screen = libScreen
		var id = wFactory.counter
		var socket = wFactory.socket
		this.handlemessage = function(message){
			console.log("webRTC message recieved. calling:",message)

			if (message.command == "share"){
				nameRemote = message.args[1]
				nameLocal = $("#loginname").val()

				call()
				//P2PEndpoint.createOffer(localDescriptionCreated,message.args[2])
			}else if(message.command == "toggleScreen"){
				console.log("TOGGLING SHARE!")
				if(sharingScreen){
					constraints = {audio:true, video: true} 
					navigator.getUserMedia(
						constraints, 
						function(mediaStream){
							if(myMediaStream){
							//	myMediaStream.stop()
								P2PEndpoint.removeStream(myMediaStream)
							}
							myMediaStream = mediaStream
							console.log("local stream enabled");
							videoLocal = document.getElementById("screen1");
							videoLocal.muted = true
							videoRemote = document.getElementById("screen2");//initialize remote viewer too
							localStream = mediaStream;
							P2PEndpoint.addStream(localStream);
							videoLocal.src = window.URL.createObjectURL(mediaStream);
						if(nameRemote){
							startLocal(call)
						}else{
							startLocal()
						}
						//call()
						}, 
						function(error) {
							console.log("error details:", error)
						}
					);
				}else{
					constraints = 
						{
							audio: false,
							video: {
								mandatory: {
									chromeMediaSource: 'screen',
								}
							}
						}
					navigator.getUserMedia(
					 	constraints,
						function(mediaStream){
							if(myMediaStream){
							//	myMediaStream.stop()
								P2PEndpoint.removeStream(myMediaStream)
							}
							myMediaStream = mediaStream
							console.log("local stream enabled");
							videoLocal = document.getElementById("screen1");
							videoLocal.muted = true
							videoRemote = document.getElementById("screen2");//initialize remote viewer too
							localStream = mediaStream;
							P2PEndpoint.addStream(localStream);
							videoLocal.src = window.URL.createObjectURL(mediaStream);
						if(nameRemote){
							startLocal(call)
						}else{
							startLocal()
						}
						//call()
						}, 
						function(error) {
							console.log("error details:", error)
						}
					);
				}
				sharingScreen = !sharingScreen
			}else if(message.command == "take_call"){
				console.log("taking call:",message.args)
					if(message.args[2].sdp.type === 'answer'){
						nameRemote = message.args[0]
						nameLocal = message.args[1]
						console.log("--Accepting answer and finalizing handshake--");
						P2PEndpoint.setRemoteDescription(new RTCSessionDescription(message.args[2].sdp));
					}else if(message.args[2].sdp.type === 'offer'){
						nameRemote = message.args[0]
						nameLocal = message.args[1]
					//	document.getElementById("accept").disabled=false;
						console.log('--Recieved offer information. Waiting for the signal to answer--');
						P2PEndpoint.setRemoteDescription(new RTCSessionDescription(message.args[2].sdp),
						function(){
							P2PEndpoint.createAnswer(localDescriptionCreated);
						},
						function(){
							console.log('failed to set remote description');
						});
					}
			}else if(message.command == "ice_candidate"){
				console.log("ice candidate:",message.args)
				P2PEndpoint.addIceCandidate(new RTCIceCandidate(message.args.candidate));
				//WRTCOnIceCandidate(message.args)
			}else if(message.command == "message"){
				console.log("webRTC widget recieving message command:", message.args)
				dataChannel.send(JSON.stringify({address:"message",args:message.args}))

			}
		}
		console.log("webRTC module created")
		//--Browser compatibility abstraction--//
		window.RTC = window.webkitRTCPeerConnection || window.mozRTCPeerConnection
		window.WebSocket = window.WebSocket || window.MozWebSocket;
		navigator.getUserMedia = navigator.getUserMedia || navigator.webkitGetUserMedia || navigator.mozGetUserMedia || navigator.msGetUserMedia;
		window.URL = window.URL || window.webkitURL;

		//webrtc video streams & p2p connection handler
		var P2PEndpoint = null;
		var arrayToStoreChunks = []
		var dataChannel = null
		var offerSDP = null
		var videoLocal;
		var myMediaStream=null
		var dataChannelOn = false;
		var localStream = null;
		var sharingScreen = true;
		var videoRemote;
		var remoteStream = null;
		var isCaller=false;
		var nameLocal=null;
		var nameRemote=null;

		var constraints = 
				{
					audio: false,
					video: {
						mandatory: {
							chromeMediaSource: 'screen',
						}
					}
				}
		var servers = {iceServers:[{url:'stun:stun.l.google.com:19302'}]};//remote ICE servers for path discovery

		/*function myIceEscape(key,val) {
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
		};*/

		//--WebRTC media & connection handlers--//
		function WRTCOnAddStream(event){
			console.log('--Adding Incoming Stream--',event);
			videoRemote.src = webkitURL.createObjectURL(event.stream);
			remoteStream = event.stream;
		}


		function WRTCOnError(event){
			alert('error: '+event);
		}

		function localDescriptionCreated(description){
			offerSDP = description
			console.log("--local information gathered. Relaying to remote peer--",description);
			P2PEndpoint.setLocalDescription(description);
				var JSONString = { "sdp": description};//wrap in JSON and send to remote peeer
				//connection.send(JSONString);
				console.log("sharing to: ",nameRemote," from ",nameLocal)
				var share = {}
				share.command='p2p_sdp_connect',
				share.args=[nameLocal,nameRemote,JSONString]
				socket.write(share)	

		}

		function WRTCOnIceCandidate(event){
			if(event.candidate){
				console.log('Time:'+new Date().getTime()+': Recieved Ice Candidate');
				if(P2PEndpoint.localDescription != null){
					var share = {}
					share.command='p2p_ice_candidate',
					share.args=[nameLocal,nameRemote,{"candidate":event.candidate}]
					socket.write(share)
					//socket.write(JSON.stringify({"candidate": event.candidate},myIceEscape));//wrap the event in JSON and let the other side call p2pendpoint.addIceCandidate()
				}
			}else{
				console.log('end of candidates');
			}
		}

		function WRTCInit(callbackcallback,callback){
			console.log('--Initializing peer connection--');
			P2PEndpoint = new window.RTC(servers) //webkitRTCPeerConnection(servers);
			dataChannel = P2PEndpoint.createDataChannel("myLabel",{ordered:true,id:0,maxRetrasmitTime:3000})

			dataChannel.onerror = function(error){
				console.log("Data Channel Error:",error)
			}
			dataChannel.onmessage = function(event){
			
				if(event.data){
				//	console.log("p2p data:",event.data)
					var data = JSON.parse(event.data)
					if(data.address == "message"){
						console.log("recieved P2P message. Routing to message widget",data.args)
						msg = {}
						msg.local = true
						msg.type= "messaging"
						msg.args = data.args
						socket.write(msg)
					}
					if (data["key"]){
						console.log("data channel recieved key press:",data)
						var packet = {}
						packet.local = true
						packet.type =  "editor"
						packet.command = "keypress"
						packet.args = {}
						packet.args[0] = data['key']
						socket.write(packet)
						//simulateKeyPress(data["key"])	
					}
					if (data["x"]){
						//console.log("exists x")
						img.css({"left":data.x + 'px'})
					}
					if(data["y"]){
						//console.log("exists y")
						img.css({'top':data.y+'px'})
					}
					if(data.message){
						    arrayToStoreChunks.push(data.message); // pushing chunks in array

						    if (data.last) {
							saveToDisk(arrayToStoreChunks.join(''), 'fileName');
							arrayToStoreChunks = []; // resetting array
						    }
						console.log("Got data channel message! event:",data)

					}
	
				}
			}
			dataChannel.onopen = function(){
				console.log("new connection opened",dataChannel)
				dataChannelOn = true;
				//dataChannel.send("hello world")
				if(dataChannelOn){
					$('body').keypress(function(e) {
						console.log("sending keystroke to remote endpoint:",e.which)
						dataChannel.send(JSON.stringify({key:e.which}))
					});

				}
			}
			dataChannel.onclose = function(){
				dataChannelOn = false;
				console.log("data channel closed")
			}

			P2PEndpoint.onicecandidate = WRTCOnIceCandidate;
			P2PEndpoint.onaddstream = WRTCOnAddStream;
			callback(callbackcallback)
		}
		function saveToDisk(fileUrl, fileName) {
		    var save = document.createElement('a');
		    save.href = fileUrl;
		    save.target = '_blank';
		    save.download = fileName || fileUrl;

		    var event = document.createEvent('Event');
		    event.initEvent('click', true, true);

		    save.dispatchEvent(event);
		    (window.URL || window.webkitURL).revokeObjectURL(save.href);
		}


		//--Triggers & User Interface Callbacks--//
		function sendMsg(message){
			connection.send('/msg '+message);//the msg flag is passed for general websocket chatroom use
		}


		function answer(){
			console.log('--answering--');
		}

		function call(){
			isCaller=true;
			console.log('--creating offer--');
			P2PEndpoint.createOffer(localDescriptionCreated);
		}

		function startLocal(callback){
			WRTCInit(callback,startLocalVideo);
		}

		function startLocalVideo(callback){
			console.log('libscreen:',screen)
			navigator.getUserMedia(
			 	constraints,
				function(mediaStream){

					if(myMediaStream){
					//	myMediaStream.stop()
						P2PEndpoint.removeStream(myMediaStream)
					}
					myMediaStream = mediaStream
					console.log("local stream enabled");
					videoLocal = document.getElementById("screen1");
					videoLocal.muted = true
					videoRemote = document.getElementById("screen2");//initialize remote viewer too
					localStream = mediaStream;
					P2PEndpoint.addStream(localStream);
					videoLocal.src = window.URL.createObjectURL(mediaStream);
				}, 
				function(error) {
					console.log("error details:", error)
				}
			);
			if(callback){
				callback()
			}
		}

		function hangup(){
		}

		this.view = $("<div id='RTCwidget'>")
		var screen1 = $("<video autoplay id='screen1'>")
			.css({
				//"height":"100",
				"background":"black",
				"float":"left",
				"height":"100%",
			})
		var screen2 = $("<video id='screen2'>")
			.css({
				"position":"absolute",
				
				"float":"right",
				"background":"#555555",
				"height":"100%",
			})
		this.view.append(screen1)
		this.view.append(screen2)
		startLocal()

	}
	return widget
})
