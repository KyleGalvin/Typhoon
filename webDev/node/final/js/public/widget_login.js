define(['clientConnection'], function(){
	var widget = function(id,socket,Hashes){
		console.log('hashes',Hashes)
		var handlemessage = function(message){
			console.log("widget_login message returned!")
		}
		var loginaction = function(){
			console.log("Logging In",socket)
			var username = $("#loginname").val()
			var password = $("#loginpass").val()
			var login = {}
			login.address = id
			login.command='login'
			login.args=[username, Hashes.hex(username+password)]
			console.log('going to socket with message',login)
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
		this.msgHandler = function(cmd,args){

		}
		this.view = $("<div id='login'/>")
			.css({
				"border-radius":"25px 0px 0px 25px",
				"position":"relative",
				"background":"rgba(50,0,0,0.75)",
				"height":"100%",
				"color":"#FFF",
				"width":"200px",
				"float":"right"	
			})
		
		var loginheader = $("<div id='loginheader'>")
			.css({
				"text-align":"center",
				"font-size":"32px",			
			}).append("Login")

		var logintop = $("<div id='logintop'>")
			.css({
				"width":"100%",
				"height":"120px",
			})
		logintop.append(loginheader)
		this.view.append(logintop)
		var logininputs = $("<div id='logininputs'>")
			.css({
				"text-align":"right",
				"float":"right",
				"width":"50%",
			})
		var nameinput = $("<input id='loginname' type='text'>").css({
			"width":"80%",
			"float":"left",
		})
		var passinput = $("<input id='loginpass' type='password'>").css({
			"width":"80%",
			"float":"left",
		})
		var loginbutton = $("<button>",{text:'Login'}).css({
			"width":"80%",
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

		var namelabel =$("<div>").css({"width":"100%"}).append("User:")
		var passlabel =$("<div>").append("Pass:")
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
		this.view.append(logintop)
		this.view.append(option)
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
			"float":"left"
		})
		var signuppassinput = $("<input id='signuppass' type='password'>").css({
			"width":"80%",
			"float":"left",

		})
		var signuppassinput2 = $("<input id='signuppass' type='password'>").css({
			"width":"80%",
			"float":"left",

		})
		var signupbutton = $("<button>",{text:'Sign Up'}).css({
			"width":"80%",
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
				"font-size":"32px",			
			}).append("Sign Up")
		loginbottom.append(signupheader)
		loginbottom.append(signupinputs)
		this.view.append(loginbottom)
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

		var signupnamelabel =$("<div>").css({"width":"100%"}).append("User:")
		var signuppasslabel =$("<div>").append("Pass:")
		var signuppasslabel2 =$("<div>").append("Pass(2):")
		signuplabels.append(signupnamelabel)
		signuplabels.append(signuppasslabel)
		signuplabels.append(signuppasslabel2)
		
		loginbottom.append(signuplabels)
		
	}
	
	return widget
})
