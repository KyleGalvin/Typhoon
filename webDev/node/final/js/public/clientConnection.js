function clientConnection(ip,port,callback,widgetFactory){

	//var socket = new io.Socket()
	var socket = io.connect('http://'+ip+":"+port)
	var _this = this //we need to access parent scope from socket.on('connect')
	/*this.connect = function(ip,port){
		console.log("connecting")
		socket = io.connect('http://'+ip+":"+port)
		console.log('connected socket:',socket)
		socket.emit('message',{'command':'test','args':['just connected. test send']})
	}*/
	this.write = function(json){
		console.log("writing",json)
		//json.address = id
		socket.emit('message',json)
	}
	socket.on('message', function(data,callback){
		data = jQuery.parseJSON(data)
		console.log("client?",data)
		console.log("type:",data)
		if(data.address){
			console.log("sending to address...",data.address)
			widget = widgetFactory.widgetLookupTable
			console.log("is there a widget?",widget)
		}	
	})
	socket.on('error',function(){
		console.log("error!")
	})
	socket.on('connect',function(data){
		console.log('client is connected',data)
		//grab scope of parent function rather than this function
		widgetFactory.socket = _this
		callback(widgetFactory)
	})
	socket.on('disconnect',function(){})
	this.write({'val':'CONNECTION TEST'})
	return this
}
