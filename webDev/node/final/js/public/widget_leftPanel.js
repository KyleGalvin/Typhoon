define(['clientConnection'], function(){
	var widget = function(wFactory,model){
		var id = wFactory.counter
		var socket = wFactory.socket
		this.handlemessage = function(message){
			console.log("SUBSCRIPTION MESSAGE!", message)
		}
		var view1 = $("<div id='view1' />")
		var view2 = $("<div id='view2' />")
		var view3 = $("<div id='view3' />")
		this.view1 = view1
		var toggle = false
		var toggle1 = function(){
			console.log('click')
			if(!toggle){
				view.animate({"width":"200px"},300)
				view.empty()
				view.append(button1.click(toggle1))
				view.append(contactList_widget.view) 

				var openVid = {}
				openVid.type = "centerPanel"
				openVid.command='video',
				openVid.local = true,
				socket.write(openVid)	
			}else{
				view.animate({"width":"35px"},300)
				view.empty()
				view.append(button1.click(toggle1))
				view.append(button2.click(toggle2))
				view.append(button3.click(toggle3))
			}
			toggle = !toggle
		}
		var toggle2 = function(){
			if(!toggle){
				view.animate({"width":"200px"},300)
				view.empty()
				view.append(button2.click(toggle2))
				view.append(git_widget.view) 
				var openEditor = {}
				openEditor.type = "centerPanel"
				openEditor.command='editor',
				openEditor.local = true,
				socket.write(openEditor)	
			}else{
				view.animate({"width":"35px"},300)
				view.empty()
				view.append(button1.click(toggle1))
				view.append(button2.click(toggle2))
				view.append(button3.click(toggle3))

			}
			toggle = !toggle
		}
		var toggle3 = function(){
			if(!toggle){
				view.animate({"width":"200px"},300)
				view.empty()
				view.append(button3.click(toggle3))
				view.append(messaging_widget.view) 
			}else{
				view.animate({"width":"35px"},300)
				view.empty()
				view.append(button1.click(toggle1))
				view.append(button2.click(toggle2))
				view.append(button3.click(toggle3))

			}
			toggle = !toggle
		}

		var view = $("<div id='leftpanel'/>")
			.css({
				"border-radius":"0px 25px 25px 0px",
				"position":"relative",
				"background":"rgba(50,0,0,1)",
				"height":"100%",
				"color":"#FFF",
				"width":"35px",
				"float":"left",
			})	

		this.view = view
		var insideView = $("<div id='insideview0' />")		
		view.append(insideView)

		var button1 = $("<img src='./img/menu_button.png'/>")
			.css({
				"float":"right",
				"width":"30px",
				"height":"30px",
				"padding":"5px",
			})
			.click(toggle1)
		var button2 = $("<img src='./img/menu_button.png'/>")
			.css({
				"float":"right",
				"width":"30px",
				"height":"30px",
				"padding":"5px",
			})
			.click(toggle2)
		var button3 = $("<img src='./img/menu_button.png'/>")
			.css({
				"float":"right",
				"width":"30px",
				"height":"30px",
				"padding":"5px",
			})
			.click(toggle3)
		insideView.append(button1)	
		insideView.append(button2)	
		insideView.append(button3)
	
		var contactList_widget = wFactory.create('contactList')
		var git_widget = wFactory.create('git')
		var messaging_widget = wFactory.create('messaging')
		//leftPanel_widget.view.append(contactList_widget.view)

	}
	return widget
})
