var WebSocketServer = require('websocket').server;
var http = require("http");

//array of active connections
var clients = [];
var names = [];
function HTTPMsgHandler(request, response) {}//since we are using apache to serve the page, nothing goes here.

var server = http.createServer(HTTPMsgHandler).listen(8888);

wsServer = new WebSocketServer({httpServer: server});//listen for incoming WebSocket HTTP upgrade requests

function syncNameList(request){

	console.log("LIST WIDGIT");
	var namesList = "";
	for(var k in names){
		var n = names[k];//grab each individual name by key
		namesList+= n +"<br>";
	}
	for(var k in clients){
		var c = clients[k];
		c.sendUTF("/widgetupdate "+namesList);
	}
}

//on the request event we create a connection, define our response to the connection's event handlers, and store the connection for later
wsServer.on('request', function(request) {
	var connection = request.accept('echo',request.origin);
	connection.sendUTF('/msg Youve connected to my remote server from:'+request.remoteAddress+'. The connection is '+connection.readyState);

	console.log('request!!!!');
	console.log("request key:" + request.key);
	console.log(request.remoteAddress);
	//console.log(request);//This is informative but noisy
	var index = clients.length;
	names[request.key.toString()] = "GuestAnonymous"+ Object.keys(names).length;//associative array length is weird when javascript doesnt 'technically' have associative arrays
	clients[request.key.toString()] = connection;

	//when a user is added, inform other users
	syncNameList(request);

	//parse and handle incoming messages
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
			WidgitNames[1]=/^mouse/g;

			//widget query (real-time AJAX replacement)
			if(patternWidgetQuery.test(message.utf8Data)){				
				message.utf8Data = message.utf8Data.replace(patternWidgetQuery,"");//pop the first command string and continue with the rest
				if(WidgitNames[0].test(message.utf8Data)){
					syncNameList(request);	
				}else if(WidgitNames[1].test(message.utf8Data)){
					console.log("Mouse WIDGIT");
					for(var k in clients){
						var c = clients[k];//grab each individual client by key
						if(c != connection){
							c.sendUTF("/widgetupdate "+ message.utf8Data);
						}
					}
					
				}

			//chat relay message
			}else if(patternMSG.test(message.utf8Data)){
				message.utf8Data = message.utf8Data.replace(patternMSG,"");
				if(patternJS.test(message.utf8Data)){
					console.log(names[request.key.toString()]+" sending script to clients:"+message.utf8Data);
					//if the client broadcasts a javascript command, we can't append the name without breaking the command's syntax
					for(var k in clients){
						var c = clients[k];
						c.sendUTF(message.utf8Data);
					}
				
				}else if(patternName.test(message.utf8Data)){
					console.log(names[request.key.toString()]+' changing name to'+message.utf8Data.replace(patternName,""));
					var oldName = names[request.key.toString()];
					names[request.key.toString()] = message.utf8Data.replace(patternName,"");
					
					for(var k in clients){
						var c = clients[k];
						c.sendUTF('/msg '+oldName+' has changed names to '+names[request.key.toString()]);
					}
				
					syncNameList(request);
				}else{
					console.log(names[request.key.toString()]+" broadcasting message to all clients:"+message.utf8Data);
					for(var k in clients){
						var c = clients[k];
						console.log('outbound');
						c.sendUTF('/msg '+names[request.key.toString()]+':'+message.utf8Data);
					}
				}
			//p2p video signalling message
			}else{
					for(var k in clients){
						c = clients[k];
						n = names[k];
						if(c != connection){
							console.log('outbound');
							console.log('from: '+names[request.key.toString()]+' to:'+ n);
							c.sendUTF(message.utf8Data);
						}
					}
				console.log('video signalling!');
			}
				
		}
	});

	connection.on('close',function(reasonCode, description){
		
		console.log('LOSING CONNECTION!');
		var targetIndex;
		console.log('index scope test');
		console.log(index);

		delete(clients[request.key.toString()]);
		delete(names[request.key.toString()]);

		console.log('description:');
		console.log(JSON.stringify(description));
		console.log('reasonCode:');
		console.log(JSON.stringify(reasonCode));

		syncNameList(request);
	});
});
