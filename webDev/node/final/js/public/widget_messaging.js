
define(['clientConnection'], function(){
	var widget = function(wFactory,model){
		var id = wFactory.counter
		var socket = wFactory.socket
		var view = $("<div id='git' />").append("messaging")
		this.view = view
	}
	return widget
})
