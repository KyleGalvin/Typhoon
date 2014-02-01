function clientConnection(){

	var socket = new io.Socket()
	var widgetLookupTable

	this.connect = function(ip,port){
		console.log("connecting")
		socket = io.connect('http://'+ip+":"+port)
	}
	this.write = function(json){
		console.log("writing")
		socket.emit('message',json)
	}
	socket.on('message', function(data){
		console.log("client?")
		if(data.message){
			console.log('client recieved message:',data.message)
		}	
	})
	socket.on('connect',function(data){})
	socket.on('disconnect',function(){})

	return this
}
