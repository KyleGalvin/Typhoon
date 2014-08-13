//var _WidgetTemplates = {}
var Hashes = {}
var _WidgetTemplates
requirejs(['jquery-1.10.2.min.js','widget_templates','hashes.min','clientConnection','modelHandler'], function(jquery,WidgetTemplates,incomingHashes,ClientConnection,modelHandler){
	console.log("Done collecting requirements")
	console.log("after init",incomingHashes)
	_WidgetTemplates = WidgetTemplates
	//_WidgetTemplates = WidgetTemplates
	Hashes = incomingHashes
	$(document).ready(function(){
		new clientConnection('www.littlereddevshed.com',80,runApplication)
		console.log("Document Ready")
	})
})

var runApplication = function() {
	var WidgetTemplates = _WidgetTemplates
  	console.log("widget templates:",WidgetTemplates)
	console.log("Beginning Application")

	var logo = $("<div class='logo'>")
		.css({
			"font-family":"Times New Roman",
			"font-size":"55px",
			"font-weight":"bold",
			"position":"relative",
			"border-radius":"45px",
			"height":"90px",
			"width":"90px",
			"text-align":"center",
			"border":"2px solid white",
			"line-height":"90px",
			"margin-top":"-45px",
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
            "height": "100px",
            "top": "20px",
            "overflow": "hidden",
        })
    var main = $("<div id='main'/>")
        .css({
            "position":"absolute",
            "background": "url('./img/background.jpg')",
            "height":"100%",
            "width": "100%",
            "overflow": "hidden",
        })

	var content = $("<div id='content'/>")
		.css({
			"position":"absolute",
			"top":"170px",
			"height":"60%",
			    "width": "100%",
			    "background": "rgba(0,0,0,0.75)",
			
		})
   /* var footer = $("<div id='footer'/>")
        .css({
            "position": "absolute",
            "background": "rgba(0,0,0,0.75)",
            "width": "100%",
            "height": "10%",
            "bottom": "5%",
            "overflow": "hidden",

        })*/
    $("body").css({
        "padding":"0px",
        "margin": "0px",
    })
    $("body").append(main)
	$("body").append(content)
    $("body").append(header)
   // $("body").append(footer)

    var topmenu = $("<div id='topmenu'>")
        .css({
	    "font-family":"Lucida",
            //"line-height":"16px",
            "width":"75%",
            //"margin-top":"-8px",
            //"vertical-align":"middle",
            "text-align":"right",
            "padding":"0px 0px 0px 0px",
            "position":"relative",
            //"top":"50%",
            "color":"white",
            "float":"right",
        })
    var exitbutton = $("<a href='./'>Exit</a>")
        .css({
            "color":"#FFF"
        })
	var messages = $('<a href="http://www.youtube.com/watch?v=DE0C4kQ_ygI" target="_blank">Messages</a> ')
	.css({
		"color":"#FFF"
	})
	var calls = $('<a href="./Kyle_Galvin_Thesis.pdf" target="_blank">Calls</a> ')
	.css({
		"color":"#FFF"
	})

	var files = $('<a href="./APPresentation.pptx" target="_blank">Files</a>')
	.css({
		"color":"#FFF"
	})
	var column1 = $("<div>")
	.css({
		'position':'relative'
	})
	var column2 = $("<div>")
	.css({
		'position':'relative',
		'left':'100px'
	})
	//topmenu.append(messages)
	//topmenu.append(calls)
	//topmenu.append(files)
	//topmenu.append(exitbutton)
	//topmenu.append(column1)
//        topmenu.append(" | Curriculum Vitae | Image Gallery | News")
    header.append(topmenu)

	/*var bottommenu = $("<div id='bottommenu'>")
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
	footer.append(bottommenu)*/
	logocontainer.appendTo("#footer, #header")

	//client.write({'val':'TEST CONNECTION1'})

	//client.connect("www.littlereddevshed.com",80)
	//client.write(0,{'command':'TEST CONNECTION2','args':['arg1','arg2']})
	var leftPanel_widget = WidgetTemplates.create('leftPanel')
	content.append(leftPanel_widget.view)
	
	//console.log("setting leftpanel view1")	
	//var contactList_widget = WidgetTemplates.create('contactList')
	//leftPanel_widget.view.append(contactList_widget.view)

//	var rtc = WidgetTemplates.create('rtc', null)	

	var center = WidgetTemplates.create('centerPanel')
	content.append(center.view)
	console.log("hashes:",Hashes)
	var sha = new Hashes.SHA256()
	console.log('sha',sha)
	var login_widget = WidgetTemplates.create('login',sha)
	console.log('WIDGET"',center)
	topmenu.append(login_widget.view)


}
