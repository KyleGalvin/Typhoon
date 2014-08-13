define(['clientConnection'], function(){
	var widget = function(wFactory,model,Hashes){
		var id = wFactory.counter
		console.log("factory id:",id)
		var socket = wFactory.socket
		console.log('hashes',Hashes)
		var loggedIn = $("<div id='loggedIn'>")
			.css({
				"width":"100%"
			})
		var save = $("<div id='save'>")
			.css({
				"position":"relative",
				"float":"left",
				"width":"50px",
				"height":"50px",
				"background":"blue",
			}).append("SAVE")
		var messages = $("<div id='messages'>")
			.css({
				"position":"relative",
				"float":"left",
				"width":"50px",
				"height":"50px",
				"background":"green",
			}).append("MSGS 0")
		loggedIn.append(save)
		loggedIn.append(messages)
		var loggedOut = $("<div id='loggedOut'>")
		this.handlemessage = function(message){
			console.log("widget_login message returned!",message)
			if (message.success && message.action =="login"){
				var name = message.name
				//var contents = $("#login")
				//var contentsClone = contents.clone()
				//contents.empty()
				loggedOut.hide()
				var statusBox = $("<div id='status'>")
					.css({
						"position":"relative",
						"float":"right",
						"width":"100px",
						"height":"50px",
					}).append("Logged in as " + name)
				loggedIn.append(statusBox)
				loggedIn.show()
			}
		}
		var loginaction = function(){
			console.log("Logging In",socket)
			var username = $("#loginname").val()
			var password = $("#loginpass").val()
			var login = {}
			login.address = id
			login.command='login'
			login.args=[username, Hashes.hex(username+password)]
			console.log('going to socket with message',username,login)
			socket.write(login)
		}
		var signupaction = function(){
			console.log("Creating User")
			var username = $("#signupname").val()
			var password = $("#signuppass").val()
			console.log("signing up "+$("#signupname").val()+" with pass "+ $("#signuppass").val())
			var makeUser = {}
			makeUser.address = id
			makeUser.command='make_user',
			makeUser.args=[username, Hashes.hex(username+password)]
			socket.write(makeUser)	
		}
		this.view = $("<div id='login'/>")
			.css({
				"border-radius":"25px 0px 0px 25px",
				"position":"relative",
				"background":"rgba(50,0,0,0.75)",
				"height":"100%",
				"color":"#FFF",
				"width":"450px",
				"float":"right"	
			})
		var leftside = $("<div id='leftside'>")
			.css({
				"padding":"10px",
				"float":"left",
				"width":"200px",
			})

		var rightside = $("<div id='rightside'>")
			.css({
				"padding":"3px",
				"float":"right",
				"width":"200px",
			})
		
		this.view.append(loggedOut)
		this.view.append(loggedIn)
		loggedIn.hide()
		loggedOut.append(leftside)
		loggedOut.append(rightside)
		var loginheader = $("<div id='loginheader'>")
			.css({
				"text-align":"center",
				"font-size":"20px",			
			}).append("Login")

		var logintop = $("<div id='logintop'>")
			.css({
				"width":"100%",
				"height":"120px",
			})
		logintop.append(loginheader)

		leftside.append(logintop)
		var logininputs = $("<div id='logininputs'>")
			.css({
				"text-align":"right",
				"height":"20px",
				"float":"right",
				"width":"50%",
			})
		var nameinput = $("<input id='loginname' type='text'>").css({
			"width":"80%",
			"height":"15px",
			"float":"left",
		})
		var passinput = $("<input id='loginpass' type='password'>").css({
			"width":"80%",
			"float":"left",
			"height":"15px",
		})
		var loginbutton = $("<button>",{text:'Login'}).css({
			"width":"80%",
			"height":"20px",
			"float":"left",

		})
		loginbutton.click(loginaction)
		logininputs.append(nameinput)
		logininputs.append(passinput)
		logininputs.append(loginbutton)

		logintop.append(logininputs)

		var loginlabels = $("<div id='loginlabels'>")
			.css({
				"text-align":"center",
				"float":"left",
				"width":"50%",
			})

		var namelabel =$("<div>").css({
			"width":"100%",
			"font-size":"10px",
			}).append("User:")
		var passlabel =$("<div>").css({
				"font-size":"10px",
			}).append("Pass:")
		loginlabels.append(namelabel)
		loginlabels.append(passlabel)
		
		logintop.append(loginlabels)

		var option=$("<div>")
			.css({
				"position":"relative",
				"width":"100%",
				"text-align":"center",
				"font-size":"32px",			
			}).append("~or~")
		var loginbottom = $("<div id='loginbottom'>").css
			({
				"width":"100%",
			})
		//this.view.append(logintop)
		//this.view.append(option)
		var loginbottom = $("<div id='loginbottom'>")
			.css({
				"width":"100%",
			})
		var signupinputs = $("<div id='signupinputs'>")
			.css({
				"text-align":"right",
				"float":"right",
				"width":"50%",
			})
		var signupnameinput = $("<input id='signupname' type='text'>").css({
			"width":"80%",
			"height":"15px",
			"font-size":"8px",
			"float":"left"
		})
		var signuppassinput = $("<input id='signuppass' type='password'>").css({
			"width":"80%",
			"height":"15px",
			"float":"left",
		})
		var signuppassinput2 = $("<input id='signuppass' type='password'>").css({
			"width":"80%",
			"height":"15px",
			"float":"left",

		})
		var signupbutton = $("<button>",{text:'Sign Up'}).css({
			"width":"80%",
			"height":"20px",
			"float":"left",
			})
		signupbutton.click(signupaction)
		signupinputs.append(signupnameinput)
		signupinputs.append(signuppassinput)
		signupinputs.append(signuppassinput2)
		signupinputs.append(signupbutton)


		var signupheader = $("<div id='signupheader'>")
			.css({
				"width":"100%",
				"text-align":"center",
				"font-size":"20px",			
			}).append("Sign Up")
		loginbottom.append(signupheader)
		loginbottom.append(signupinputs)
		rightside.append(loginbottom)
		var signupnameinput = $("<input type='text'>").css({"width":"100%"})
		var signuppassinput = $("<input type='password'>").css({"width":"100%"})
		var signupinputs = $("<div id='signupinputs'>")
			.css({
				"text-align":"right",
				"float":"right",
				"width":"50%",
			})
		var signuplabels = $("<div id='signuplabels'>")
			.css({
				"text-align":"center",
				"float":"left",
				"width":"50%",
			})

		var signupnamelabel =$("<div>").css({
				"font-size":"10px",
				"width":"100%",
			}).append("User:")
		var signuppasslabel =$("<div>").css({
				"font-size":"10px",
			}).append("Pass:")
		var signuppasslabel2 =$("<div>").css({
				"font-size":"10px",
			}).append("Pass(2):")
		signuplabels.append(signupnamelabel)
		signuplabels.append(signuppasslabel)
		signuplabels.append(signuppasslabel2)
		
		loginbottom.append(signuplabels)
		
	}
	
	return widget
})
