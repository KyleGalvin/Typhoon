$(document).ready( function() {
    console.log("initialized")
    var header = $("<div id='header'>")
        .css({
            "position": "absolute",
            "background": "rgba(0,0,0,0.75)",
            "width": "100%",
            "height": "100px",
            "top": "10%",
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
    var footer = $("<div id='footer'/>")
        .css({
            "position": "absolute",
            "background": "rgba(0,0,0,0.75)",
            "width": "100%",
            "height": "100px",
            "bottom": "10%",
            "overflow": "hidden",

        })
    $("body").css({
        "padding":"0px",
        "margin": "0px",
    })
    $("body").append(main)
    $("body").append(header)
    $("body").append(footer)

    var topmenu = $("<div id='topmenu'>")
        .css({
            "line-height":"40px",
            "width":"75%",
            "height":"40px",
            "margin-top":"-20px",
            "vertical-align":"middle",
            "text-align":"right",
            "padding":"0px 15% 0px 0px",
            //"background-color":"rgba(50,0,0,0.75)",
            "position":"relative",
            "top":"50%",
            "color":"white",
            "float":"right",
        })
    var voiplink = $("<a href='./voip.html'>VoIP Software</a>")
        .css({
            "color":"#FFF"
        })
    topmenu.append(voiplink)
        topmenu.append(" | Curriculum Vitae | Image Gallery")
    header.append(topmenu)
})
