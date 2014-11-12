define(['clientConnection'], function(){
	var widget = function(wFactory,model,screen){
		var id = wFactory.counter
		var socket = wFactory.socket
		this.handlemessage = function(message){
			console.log("SUBSCRIPTION MESSAGE!", message)
			if(message.command =='video'){
				editor.view.hide()
				rtc.view.show()
			}else{
				rtc.view.hide()
				editor.view.show()
			}
		}

		var view = $("<div id='centerpanel'/>")
		this.view = view
		var editor = wFactory.create('editor')
		editor.view.hide()
		view.append(editor.view)
		var rtc = wFactory.create('rtc',screen)
		view.append(rtc.view)

	}
	return widget
})
