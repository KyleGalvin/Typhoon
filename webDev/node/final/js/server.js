//create our webserver instance
var express = require('express')
var app = express()
var server = require('http').createServer(app)
var dispatcher = require('./cmdDispatch') 
var sessions = {}
//allow the user full read access of our public javascript directory
//this allows us to link/embed custom javascript into the client application
app.use(express.static(__dirname + '/public'))
console.log("dirname:"+__dirname)
//configure and initialize web server view template engine
app.set( 'views', __dirname + '/../views' )
app.set( 'view engine', 'ejs' )

//serve our index.html page when the user requests the root of our webserver
app.get('/',function(req,res){
	res.render('index')
})
//serve our index.html page when the user request the root of our webserver
app.get('/voip',function(req,res){
	res.render('voip')
})
server.listen(80)

//create websocket server/listener overtop the existing express.js web server
var io = require('socket.io').listen(server,{log:false})
console.log("server stuff")
//when an incoming connection is detected, we save the connection in our sessions table.
//when we recieve a message from the client, we send the message to our command dispatcher.
//if the dispatcher returns a response, we relay it back to the client application via the open socket.
io.sockets.on('connection',function(socket){

	//put these into the object model instead
	//sessions[socket.id] = {}
	//sessions[socket.id].created = new Date()
	//sessions[socket.id].socket = socket


	socket.on('message',function(data){
		console.log("incoming Data:",data)
		if(data && data.command != null && data.args != null){
			var response = dispatcher.call(data.command,socket.id,data.args)
			console.log("response?",response,typeof(response))
			if(response){
				response.address = data.address
				console.log('sending response',response)
				socket.send(JSON.stringify(response))
				//console.log('socket:',socket)
				
			}
		}
	})

	socket.on('disconnect', function(){
		console.log("disconnecting:",socket.id)
		dispatcher.call("disconnect", socket.id)	
	})

	dispatcher.call("add_session",socket.id, [socket])
})
//var WocketServer = require('websocket').server
//var http = require("http")

//array of active connections
//var clients = [];
//var names = [];
//function HTTPMsgHandler(request, response) {}//since we are using apache to serve the page, nothing goes here.

//var server = http.createServer(HTTPMsgHandler).listen(8888);

//wsServer = new WebSocketServer({httpServer: server});//listen for incoming WebSocket HTTP upgrade requests

