function clientConnection(ip,port,callback){
	//var socket = new io.Socket()
	var socket = io.connect('https://'+ip+":")
	var _this = this //we need to access parent scope from socket.on('connect')
	/*this.connect = function(ip,port){
		console.log("connecting")
		socket = io.connect('http://'+ip+":"+port)
		console.log('connected socket:',socket)
		socket.emit('message',{'command':'test','args':['just connected. test send']})
	}*/
	this.write = function(json){
		console.log("what am I writing?",json)	
		if(json.local && (json.address || json.type)){
			handleMessage(JSON.stringify(json))//communication between local widgets do not need to go over network
		}else if(json.local){
			console.log("local message with no address?")
		}else{

			console.log("writing",json)
			//json.address = id
			socket.emit('message',json)
		}
	}

	var handleMessage = function(data,callback){
		data = jQuery.parseJSON(data)
		console.log("type:",data)
		if(data.address){
			console.log("sending to address...",data.address)
			widget = _WidgetTemplates.widgetLookupTable
			console.log("is there a widget?",widget)
			widget[data.address].handlemessage(data)
		}else if(data.type){
			console.log("sending to widget...",data.type)
			widget = _WidgetTemplates.widgetNameTable
			console.log("is there a widget?",widget)
			var namedWidgets = widget[data.type]
			console.log('widgets with name ',data.type,': ',namedWidgets)
			widget[data.type][0].handlemessage(data)
			
		}else if(data.broadcast){

			console.log("broadcast recieved. Current model:",_model.query('read',["model"]))
			//add to local model, hope a widget is subscribed to the info
		}else{
			console.log("lost packet in the system:",data)
		}	

	}

	socket.on('message', function(data,callback){
		console.log("incoming data:",data)
		handleMessage(data,callback)
	})
	socket.on('error',function(){
		console.log("error!")
	})
	socket.on('connect',function(data){
		console.log('client is connected',data)
		//grab scope of parent function rather than this function
		_WidgetTemplates.socket = _this
		callback()
	})
	socket.on('disconnect',function(){})
	return this
}
