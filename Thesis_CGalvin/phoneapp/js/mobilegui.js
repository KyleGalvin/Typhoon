$(document).ready(function() {
	var optionalRoom

	var video = document.getElementById("underlay")
	navigator.getMedia = ( navigator.getUserMedia ||
                navigator.webkitGetUserMedia ||
                navigator.mozGetUserMedia ||
		navigator.msGetUserMedia);
	// use the chrome specific GetUserMedia function
	console.log(video);
	navigator.getMedia({video:true, audio:true},
            function(stream) {
                video.src = window.URL.createObjectURL(stream);
		console.log("set stream")
		video.onloadedmetadata = function(e){
			console.log("onladedmetadata")
		}
            },
            function(err) {
                console.log("Unable to get video stream!")
            }
    )


	var tracklength = 100
	var trackwidth = 10
	var buttonshort = 85
	var buttonlong = 135
	var hitboxlength = tracklength*4
	var hitboxwidth = trackwidth*15

	var leftslider = $("#leftslider")
	var leftsliderknob = $("#leftsliderknob")
	var rightslider = $("#rightslider")
	var rightsliderknob = $("#rightsliderknob")

	$("#underlay").attr('width',$(window).width())
	$("#underlay").attr('height',$(window).height())
	$("#mutetoggle").css('top',0 )
	$("#mutetoggle").css('left', $(window).width()/2.0 - $("#mutetoggle").width()/2.0)

	leftslider.css('height', trackwidth)
	leftslider.css('width', tracklength) 
	leftslider.css('top', $(window).height()/2.0 - $("#rightslider").height()/2.0)
	leftslider.css('left', leftslider.width()*1.5)
	leftsliderknob.css('height', buttonlong)
	leftsliderknob.css('width', buttonshort) 
	leftsliderknob.css('top', $(window).height()/2.0 - (leftsliderknob.height()-leftslider.height())/2.0)
	leftsliderknob.css('left', $("#leftslider").position().left + leftslider.width()/2.0 - leftsliderknob.width()/2.0)
	$("#leftsliderhitbox").css('height', hitboxwidth)
	$("#leftsliderhitbox").css('width', hitboxlength) 
	$("#leftsliderhitbox").css('top', $(window).height()/2.0 - $("#leftsliderhitbox").height()/2.0)
	$("#leftsliderhitbox").css('left', $("#leftslider").position().left + leftslider.width()/2.0 - $("#leftsliderhitbox").width()/2.0)

	rightslider.css('height', tracklength)
	rightslider.css('width', trackwidth) 
	rightslider.css('top', $(window).height()/2.0 - rightslider.height()/2.0)
	rightslider.css('left', $(window).width() - (rightslider.height()*1.5))
	rightsliderknob.css('height', buttonshort)
	rightsliderknob.css('width', buttonlong) 
	rightsliderknob.css('top', $(window).height()/2.0 - rightsliderknob.height()/2.0)
	rightsliderknob.css('left', $("#rightslider").position().left + rightslider.width()/2.0 - rightsliderknob.width()/2.0)
	$("#rightsliderhitbox").css('height', hitboxlength)
	$("#rightsliderhitbox").css('width', hitboxwidth) 
	$("#rightsliderhitbox").css('top', $(window).height()/2.0 - $("#rightsliderhitbox").height()/2.0)
	$("#rightsliderhitbox").css('left', $("#rightslider").position().left + rightslider.width()/2.0 - $("#rightsliderhitbox").width()/2.0)

	var leftdrag = false
	function leftslide(e){
		if(leftdrag){
			if(e.pageX > $("#leftslider").position().left && e.pageX < $("#leftslider").position().left + leftslider.width()){
				leftsliderknob.css('left',e.pageX- leftsliderknob.width()/2.0)
			}
		}
	}

	leftsliderknob.on("mousedown", function(e){leftdrag = true})

	
	leftslider.on("mouseup", function(e){leftdrag = false})
	leftsliderknob.on("mouseup", function(e){leftdrag = false})
	$("#leftsliderhitbox").on("mouseup", function(e){leftdrag = false})

	leftslider.on("mousemove", leftslide)
	leftsliderknob.on("mousemove", leftslide)
	$("#leftsliderhitbox").on("mousemove", leftslide)

	//$("#leftsliderhitbox").on("mouseout", function(e){})//turn off drag if we are OUTSIDE hit box (must test; event can be triggered over other divs while still inside

	var rightdrag = false
	function rightslide(e){
		if(rightdrag){
			console.log("right sliding active")
			if(e.pageY > $("#rightslider").position().top && e.pageY < $("#rightslider").position().top + rightslider.height()){
				rightsliderknob.css('top',e.pageY- rightsliderknob.height()/2.0)
			}
		}
	}

	rightsliderknob.on("mousedown", function(e){rightdrag = true})

	rightslider.on("mouseup", function(e){rightdrag = false})
	rightsliderknob.on("mouseup", function(e){rightdrag = false})
	$("#rightsliderhitbox").on("mouseup", function(e){rightdrag = false})

	rightslider.on("mousemove", rightslide)
	rightsliderknob.on("mousemove", rightslide)
	$("#rightsliderhitbox").on("mousemove", rightslide)
});
