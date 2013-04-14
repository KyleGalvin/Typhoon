var WebSocketServer = require('websocket').server;
var http = require("http");

function HTTPMsgHandler(request, response) {
	//since we're relying on the overlying websocket for communication,
	//we don't handle this request.
}

var server = http.createServer(HTTPMsgHandler).listen(8888);

wsServer = new WebSocketServer({
	httpServer: server,

	// You should not use autoAcceptConnections for production
	// applications, as it defeats all standard cross-origin protection
	// facilities built into the protocol and the browser.  You should
	// *always* verify the connection's origin and decide whether or not
	// to accept it.
	autoAcceptConnections: false
});
//WSMsgHandler!

wsServer.on('request', function(request) {
	var connection = request.accept(null, request.origin);
	connection.sendUTF('You\'ve connected to my remote server from:'+request.origin);

	console.log('request!!!!');
	console.log(request.remoteAddress);
	console.log(request);
});
