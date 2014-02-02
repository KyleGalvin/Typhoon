_WidgetTemplates = {}
var Hashes = {}
requirejs(['galleria/galleria-1.3.5.min.js','jquery-1.10.2.min.js','widget_templates','hashes.min','clientConnection'], function(jquery,WidgetTemplates,incomingHashes,ClientConnection){
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
    var voiplink = $("<a href='./voip'>VoIP Software</a> | ")
        .css({
            "color":"#FFF"
        })
	var computerVisionLink = $('<a href="http://www.youtube.com/watch?v=DE0C4kQ_ygI" target="_blank">Kinect Computer Vision</a> | ')
	.css({
		"color":"#FFF"
	})
	var paperPdfLink = $('<a href="./Kyle_Galvin_Thesis.pdf" target="_blank">MSc Project Paper PDF format</a> | ')
	.css({
		"color":"#FFF"
	})

	var paperDocLink = $('<a href="./APPresentation.pptx" target="_blank">HBSc Final Project Presentation</a> | ')
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
	topmenu.append(voiplink)
	topmenu.append(" | ")
	topmenu.append(computerVisionLink)
	topmenu.append(" | ")
	topmenu.append(paperDocLink)
	topmenu.append(" | ")
	topmenu.append(paperPdfLink)
	//column1.append(paperDocLink)
	topmenu.append(column1)
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

	var gallery = $("<div class='galleria'>")
	$('<style>.galleria {width:100%; height:100%;}</style>').appendTo(gallery)
	var img1 = $("<img data-title='My Title' data-description='This is my description' src='img/background.png'>")
	gallery.append(img1)
	content.append(gallery)
	Galleria.loadTheme('galleria/themes/classic/galleria.classic.min.js')
	Galleria.run('.galleria')
	
}
