define(['clientConnection'], function(){
	var widget = function(wFactory,model){
		var id = wFactory.counter
		var socket = wFactory.socket
		this.handlemessage = function(message){
			console.log("SUBSCRIPTION MESSAGE!", message)
			if(message.command =='startShare'){
				console.log("key share picked up!", message)
				console.log('my socket:', wFactory.socket)

				var groupSubscribe = {}
				groupSubscribe.address = id
				groupSubscribe.command='read'
				groupSubscribe.args=["model","users",message.item]
				socket.write(groupSubscribe)

				wFactory.socket.send()
			}else{
			}
		}
		console.log("KEYSHARE WIDGET CREATED")
		var view = $("<div id='keyshare'/>")
		this.view = view

	}
	return widget
})
