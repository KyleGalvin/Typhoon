function ServerConnection(){
	
	//WebSocket logic
	//--Browser compatibility abstraction--//
	window.WebSocket = window.WebSocket || window.MozWebSocket;

	//connection to websocket lobby
	var connection = null;


	//--WebSocket Connection Handlers--//
	this.connect = function (ip,port){
		connection = new WebSocket('ws://'+ip+':'+port,'echo');
		connection.onopen = myWSOnOpen;
		connection.onmessage = myWSOnMessage;
		connection.onerror = myWSOnError;
	}
	this.createUser = function(){}
	this.login = function(){}

	function myWSOnOpen() {
	}

	function myWSOnError(error) {
		// an error occurred when sending/receiving data
		console.log("myWSOnError: "+error);
	}

	function myWSOnMessage(message) {
	}

	return this;
}
var server = new ServerConnection()
server.connect("192.168.10.105",80)
