define(['clientConnection'], function(){
	var widget = function(id,socket){
		this.msgHandler = function(cmd,args){

		}
		this.view = $("<div id='contacts'/>")
			.css({
				"border-radius":"0px 25px 25px 0px",
				"position":"relative",
				"background":"rgba(50,0,0,0.75)",
				"height":"100%",
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


		var name1 = $("<div id='name1'>")
			.css({
				"text-align":"left",
				"float":"left",
				"padding":"2px",
				"width":"100%",
			}).append("User 1")

		var menu1 = $("<div id='menu1'>")
			.css({
				"text-align":"right",
				"float":"right",
				"padding":"2px",
				"width":"100%",
			}).append("button")

		var name2 = $("<div id='name2'>")
			.css({
				"text-align":"left",
				"float":"left",
				"padding":"2px",
				"width":"100%",
			}).append("User 2")

		var menu2 = $("<div id='menu2'>")
			.css({
				"text-align":"right",
				"float":"right",
				"padding":"2px",
				"width":"100%",
			}).append("button")

		loginnames.append(name1)
		loginnames.append(name2)
		loginmenus.append(menu1)
		loginmenus.append(menu2)
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
