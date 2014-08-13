define(['clientConnection'], function(){
	var widget = function(wFactory,model){
		var id = wFactory.counter
		var socket = wFactory.socket
		this.handlemessage = function(message){
			if(message.path && message.data){
				model.query("create",message.path,message.data)
				var update = model.query("read",["model","group","users"])

				console.log("update?!",update)
				loginnames.empty()
				loginmenus.empty()	
				for(var item in update){
					if(update[item].online == true){
						console.log("item:",item)
						console.log("LOGIN MESSAGE!", message)
						var name = $("<div id='"+item+"'>")
							.css({
								"text-align":"left",
								"float":"left",
								"padding":"2px",
								"width":"100%",
							}).append(item)

						var menu = $("<div id='"+item+"_menu'>")
							.css({
								"text-align":"right",
								"float":"right",
								"padding":"2px",
								"width":"100%",
							})
						var button2 = $("<div id='"+item+"_button2'>")
							.css({
								"width":"20px",
								"float":"right",
								"height":"100%",
								"border":"2px solid black",
								//"border-color":"#000000",
								"background-color":"#0000FF",
							})
						menu.append(button2)
						var button1 = $("<div id='"+item+"_button1'>")
							.css({
								"width":"20px",
								"float":"right",
								"height":"100%",
								"border":"2px solid black",
								//"border-color":"#000000",
								"background-color":"#FF0000",
							})
						menu.append(button1)
						loginnames.append(name)
						loginmenus.append(menu)
					}
				}
			}
		}

		var toggle_leftmenu = function(){
			console.log("toggling menu")
		}
		//model.query('create',["model","data"],{})
		//var data = model.query('read',['model'])
		//console.log('data:',data)

		var groupSubscribe = {}
		groupSubscribe.address = id
		groupSubscribe.command='subscribe'
		groupSubscribe.args=["model","group","users"]
		socket.write(groupSubscribe)

		this.view = $("<div id='contacts'/>")
			.css({
				"border-radius":"0px 25px 25px 0px",
				"position":"relative",
				"background":"rgba(50,0,0,0.75)",
				"height":"80%",
				"color":"#FFF",
				"width":"200px",
				"float":"left"	
			})
		
		var loginheader = $("<div id='contactsheader'>")
			.css({
				"text-align":"center",
				"font-size":"32px",		
				"position":"relative",
			}).append("Online")


		var loginnames = $("<div id='loginnames'>")
			.css({
				"float":"left",
				"text-align":"left",
				"width":"50%",
				"height":"20px",	
			})
		var loginmenus = $("<div id='loginmenus'>")
			.css({
				"float":"right",
				"text-align":"right",
				"width":"50%",	
				"height":"20px",	
			})
		var logintop = $("<div id='contactstop'>")
			.css({
				"width":"100%",
				"height":"33%",
			})
		logintop.append(loginheader)
		logintop.append(loginmenus)
		logintop.append(loginnames)
		this.view.append(logintop)

	}
	return widget
})
