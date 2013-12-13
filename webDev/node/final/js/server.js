//create our webserver instance
var express = require('express')
var app = express()
var server = require('http').createServer(app)

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
server.listen(80)

//create websocket server/listener overtop the existing express.js web server
var io = require('socket.io').listen(server)

io.sockets.on('connection',function(socket){
	console.log("Incoming Connection!")
})
//var WocketServer = require('websocket').server
//var http = require("http")

//array of active connections
//var clients = [];
//var names = [];
//function HTTPMsgHandler(request, response) {}//since we are using apache to serve the page, nothing goes here.

//var server = http.createServer(HTTPMsgHandler).listen(8888);

//wsServer = new WebSocketServer({httpServer: server});//listen for incoming WebSocket HTTP upgrade requests

