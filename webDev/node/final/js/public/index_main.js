_WidgetTemplates = {}
var Hashes = {}
requirejs(['jquery-1.10.2.min.js','widget_templates','hashes.min','clientConnection'], function(jquery,WidgetTemplates,incomingHashes,ClientConnection){
	console.log("Done collecting requirements")
	_WidgetTemplates = WidgetTemplates
	Hashes = incomingHashes
	$(document).ready(function(){
		console.log("Document Ready")
		runApplication()
	})
})

var runApplication = function() {
    console.log("Beginning Application")

	var logo = $("<div class='logo'>")
		.css({
			"font-family":"Times New Roman",
			"font-size":"30px",
			"font-weight":"bold",
			"position":"relative",
			"border-radius":"25px",
			"height":"50px",
			"width":"50px",
			"text-align":"center",
			"border":"1px solid white",
			"line-height":"50px",
			"margin-top":"-25px",
			"vertical-align":"middle",
			"color":"#FFF",
			"top":"50%",
		})
		.append("KG")
	var halfLogoHeight = logo.height() * -0.5
	var logocontainer = $("<div class='logocontainer'>")
		.css({
			"width":"25%",
			"margin-top": halfLogoHeight ,
			//"vertical-align":"middle",
			"padding":"0px 0px 0px 5%",
			"position":"relative",
			"top":"50%",
			
		})
	logocontainer.append(logo)
    var header = $("<div id='header'>")
        .css({
            "position": "absolute",
            "background": "rgba(0,0,0,0.75)",
            "width": "100%",
            "height": "10%",
            "top": "5%",
            "overflow": "hidden",
        })
    var main = $("<div id='main'/>")
        .css({
            "position":"absolute",
            "background": "url('./img/background.png') no-repeat center",
            "height":"100%",
            "width": "100%",
            "overflow": "hidden",
        })

	var content = $("<div id='content'/>")
		.css({
			"position":"absolute",
			"top":"20%",
			"height":"60%",
			    "width": "100%",
			    "background": "rgba(0,0,0,0.75)",
			
		})
    var footer = $("<div id='footer'/>")
        .css({
            "position": "absolute",
            "background": "rgba(0,0,0,0.75)",
            "width": "100%",
            "height": "10%",
            "bottom": "5%",
            "overflow": "hidden",

        })
    $("body").css({
        "padding":"0px",
        "margin": "0px",
    })
    $("body").append(main)
	$("body").append(content)
    $("body").append(header)
    $("body").append(footer)

    var topmenu = $("<div id='topmenu'>")
        .css({
			"font-family":"Lucida",
            "line-height":"16px",
            "width":"75%",
            "margin-top":"-8px",
            "vertical-align":"middle",
            "text-align":"right",
            "padding":"0px 5% 0px 0px",
            "position":"relative",
            "top":"50%",
            "color":"white",
            "float":"right",
        })
    var voiplink = $("<a href='./voip'>VoIP Software</a>")
        .css({
            "color":"#FFF"
        })
    topmenu.append(voiplink)
//        topmenu.append(" | Curriculum Vitae | Image Gallery | News")
    header.append(topmenu)

	var bottommenu = $("<div id='bottommenu'>")
		.css({
			"font-family":"Lucida",
			"position":"relative",
			"font-size":"10px",
			"width":"25%",
			"vertical-align":"middle",
			"text-align":"right",
			"padding":"10px 5% 0px 0px",
			"color":"#C30",
			"float":"right",
		})
	bottommenu.append("email<br/>public key<br/>")
	footer.append(bottommenu)
	logocontainer.appendTo("#footer, #header")

	var client = new clientConnection()
	client.connect("216.211.23.156",80)

	var contactList_widget = _WidgetTemplates.create('contactList',client)
	content.append(contactList_widget.view)

	var raphGraph = _WidgetTemplates.create('raphaelWindow', null)	
	content.append(raphGraph.view)

	var login_widget = _WidgetTemplates.create('login',client)
	console.log('WIDGET"',raphGraph)
	content.append(login_widget.view)

	console.log("Creating User")
	var username = 'Kyle Galvin'
	var password = '123'
	var makeUser = {}
	makeUser.command='make_user',
	makeUser.args=[username, new Hashes.SHA256().hex(username+password)]
	
	client.write(makeUser)

	console.log("Logging In")
	var login = {}
	login.command='login'
	login.args=[username, new Hashes.SHA256().hex(username+password)]
	
	client.write(login)

}
