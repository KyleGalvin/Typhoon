
define(['clientConnection'], function(){
	var widget = function(wFactory,model){
		var id = wFactory.counter
		var socket = wFactory.socket
		var view = $("<div id='git' />").append("messaging")
		this.view = view
		var i = 0
		this.handlemessage = function(message){
			console.log("widget_messaging got your message!",message.args[0])
			var messageDom = $("<div id='message"+i+"'>").text(message.args[0])
				.css({
					"width":"100%",
					"height":"30px",
					"float":"right",
					"text-align":"right"
				})
			this.view.append(messageDom)
			i=i+1
		}
		var inputBox = $("<input type='textarea', id='messageBox'>")
		var inputButton = $("<div id='inputButtonMessage'>")
			.css({
				"height":"30px",
				"width":"100px",
				"text-align":"center",
				"background":"#555"
			})
			.text("Submit")
			.click(function(){
				console.log('sending message:',inputBox.val())
				var msg = {}
				msg.local = true
				msg.type = "rtc"
				msg.command = "message"
				msg.args = []
				msg.args[0] = inputBox.val()
				socket.write(msg)
				var messageDom = $("<div id='message"+i+"'>").text(inputBox.val())
					.css({
						"width":"100%",
						"height":"30px"
					})
				view.append(messageDom)
				i=i+1
			})
		this.view.append(inputBox)
		this.view.append(inputButton)
	}
	return widget
})
