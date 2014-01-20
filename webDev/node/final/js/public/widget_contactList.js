define(['clientConnection'], function(){
	var widget = function(dependencies){
		var socket = dependencies[0]
		var loginaction = function(){
			console.log("CC",socket)
			alert('login action')
		}
		var signupaction = function(){
			alert('signup action')
		}
		this.msgHandler = function(cmd,args){

		}
		this.view = $("<div id='login'/>")
			.css({
				"border-radius":"0px 25px 25px 0px",
				"position":"relative",
				"background":"rgba(50,0,0,0.75)",
				"height":"100%",
				"color":"#FFF",
				"width":"200px",
				"float":"left"	
			})
		
		var loginheader = $("<div id='loginheader'>")
			.css({
				"text-align":"center",
				"font-size":"32px",			
			}).append("Online")

		var logintop = $("<div id='logintop'>")
			.css({
				"width":"100%",
				"height":"33%",
			})
		logintop.append(loginheader)
		this.view.append(logintop)

	}
	return widget
})
