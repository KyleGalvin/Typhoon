var WebSocketServer = require('websocket').server;
var http = require("http");

//array of active connections
var clients = [];
var names = [];
function HTTPMsgHandler(request, response) {}//since we are using apache to serve the page, nothing goes here.

var server = http.createServer(HTTPMsgHandler).listen(8888);

wsServer = new WebSocketServer({httpServer: server});//listen for incoming WebSocket HTTP upgrade requests

//on the request event we create a connection, define our response to the connection's event handlers, and store the connection for later
wsServer.on('request', function(request) {
	var connection = request.accept('echo',request.origin);
	connection.sendUTF('/msg Youve connected to my remote server from:'+request.remoteAddress+'. The connection is '+connection.readyState);

	console.log('request!!!!');
	console.log(request.remoteAddress);
	//console.log(request);//This is informative but noisy
	var index = clients.length;
	names.push("GuestAnonymous"+index);
	clients.push(connection);

	connection.on('message', function(message){
		
		if (message.type === 'utf8') {
			console.log('Received Message: ' + message.utf8Data);
			var patternName=/^\/name /g;
			var patternJS=/^\/js /g;
			var patternMSG=/^\/msg /g;
			var patternNameQuery=/^\/namequery /g;

			var patternWidgetQuery=/^\/updatewidget /g;
			var WidgitNames = new Array();
			WidgitNames[0]=/^list/g;

			//widget query (real-time AJAX replacement)
			if(patternWidgetQuery.test(message.utf8Data)){				
				message.utf8Data = message.utf8Data.replace(patternWidgetQuery,"");//pop the first command string and continue with the rest
				if(WidgitNames[0].test(message.utf8Data)){
					console.log("WIDGIT");
					var namesList = "";
					for(var i=0;i<names.length;i++){
						namesList+=names[i]+"<br>";
					}
					clients[index].sendUTF("/widgetupdate "+namesList);
					
				}
			//chat relay message
			}else if(patternMSG.test(message.utf8Data)){
				message.utf8Data = message.utf8Data.replace(patternMSG,"");
				if(patternJS.test(message.utf8Data)){
					console.log(names[index]+" sending script to clients:"+message.utf8Data);
					//if the client broadcasts a javascript command, we can't append the name without breaking the command's syntax
					for(var i=0;i<clients.length;i++){
						clients[i].sendUTF(message.utf8Data);
					}
				
				}else if(patternName.test(message.utf8Data)){
					console.log(names[index]+' changing name to'+message.utf8Data.replace(patternName,""));
					var oldName = names[index];
					names[index] = message.utf8Data.replace(patternName,"");
					
					for(var i=0;i<clients.length;i++){
						clients[i].sendUTF('/msg '+oldName+' has changed names to '+names[index]);
					}
					
				}else{
					console.log(names[index]+" broadcasting message to all clients:"+message.utf8Data);
					for(var i=0;i<clients.length;i++){
						console.log('outbound');
						clients[i].sendUTF('/msg '+names[index]+':'+message.utf8Data);
					}
				}
			//p2p video signalling message
			}else{
					for(var i=0;i<clients.length;i++){
						if(clients[i] != connection){
							console.log('outbound');
							console.log('from: '+names[index]+' to:'+names[i]);
							clients[i].sendUTF(message.utf8Data);
						}
					}
				console.log('video signalling!');
			}
				
		}
	});

	connection.on('close',function(reasonCode, description){
		
		console.log('LOSING CONNECTION!');
		console.log(request);
		var targetIndex;
		console.log('index scope test');
		console.log(index);
		//unfortunately, the client 'indexing' system has 3 locations that must remain in sych.
		//TODO: unify these index locations ?
		//TODO: could this be done by using clients[key.tostring()] instead of registered index numbers?
		//TODO: in the slice, 'index' would have to be found by searching for the true array index number matching the key lookup elementi

		//1-slice the clients array to remove the disconnected user
		clients = clients.slice(0,index).concat(clients.slice(index+1));
		//2-slice the names array to remove the corresponding name and ensure the two arrays match up (same length)
		names = names.slice(0,index).concat(names.slice(index+1));
		//3-match closed connection to an individual in our clients array
		for(var i=index;i<clients.length;i++){//only the members following the closed connection will be altered.
			clients[i].index--;//decrement the internal reference to reflect the upcoming splice in our externally accessible array
		}

		console.log('description:');
		console.log(JSON.stringify(description));
		console.log('reasonCode:');
		console.log(JSON.stringify(reasonCode));
	});
});
