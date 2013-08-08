$(document).ready(function() {
	console.log("height:",$(window).height())
	$("#underlay").attr('width',$(window).width())
	$("#underlay").attr('height',$(window).height())
	$("#leftslider").css('height', 10)
	$("#leftslider").css('width', 100) 
	$("#leftsliderknob").css('height', 15)
	$("#leftsliderknob").css('width', 15) 
	$("#leftslider").css('top', $(window).height()/2.0 - $("#rightslider").height()/2.0)
	$("#leftslider").css('left', $("#leftslider").width()*1.5)
	$("#leftsliderknob").css('top', $(window).height()/2.0 - ($("#leftsliderknob").height()-$("#leftslider").height())/2.0)
	$("#leftsliderknob").css('left', $("#leftslider").position().left + $("#leftslider").width()/2.0 - $("#leftsliderknob").width()/2.0)// + $("#leftslider").width()/2 )
	$("#rightslider").css('height', 100)
	$("#rightslider").css('width', 10) 
	$("#rightsliderknob").css('height', 15)
	$("#rightsliderknob").css('width', 15) 
	$("#rightslider").css('top', $(window).height()/2.0 - $("#rightslider").height()/2.0)
	$("#rightslider").css('left', $(window).width() - ($("#rightslider").height()*1.5))
	$("#rightsliderknob").css('top', $(window).height()/2.0 - $("#rightsliderknob").height()/2.0)
	$("#rightsliderknob").css('left', $("#rightslider").position().left + $("#rightslider").width()/2.0 - $("#rightsliderknob").width()/2.0)
	$("#mutetoggle").css('top',0 )
	$("#mutetoggle").css('left', $(window).width()/2.0 - $("#mutetoggle").width()/2.0)

	var leftdrag = false
	var lastleft = 0
	var rightdrag = false
	var lastright = 0
	$("#leftsliderknob").on("mousedown", function(e){
		console.log("lk mousedown")
		leftdrag = true
		lastleft = e.offsetY
	})
	$("#leftsliderknob").on("mouseup", function(e){
		console.log("lk mouseup")
		leftdrag = false
	})
	$("#leftsliderknob").on("mousemove", function(e){
		console.log("lk mousemove")
		if(leftdrag){
			console.log("dragging", e)
			lastleft = e.pageX - lastleft
			var t1 = $("#leftslider").position().left
			var t2 =  $("#leftslider").position().left+ $("#leftslider").width()
			console.log(t1, " ", t2, " ", e.pageX)
			if(e.pageX > $("#leftslider").position().left && e.pageX < $("#leftslider").position().left+ $("#leftslider").width()){
				console.log("ready!")
				$("#leftsliderknob").css('left',e.pageX- $("#leftsliderknob").width()/2.0)
			}
		}
	})
	$("#leftsliderknob").on("mouseout", function(e){
		console.log("lk mouseout")
			if(e.pageX > $("#leftslider").position().left && e.pageX < $("#leftslider").position().left+ $("#leftslider").width()){
				console.log("ready!")
				$("#leftsliderknob").css('left',e.pageX- $("#leftsliderknob").width()/2.0)
			}
	})
	$("#rightsliderknob").on("mousedown", function(e){
		console.log("rk mousedown")
		rightdrag = true
	})
	$("#rightsliderknob").on("mouseup", function(e){
		console.log("rk mouseup")
	})
	$("#rightsliderknob").on("mousemove", function(e){
		console.log("rk mousemove")
	})
	$("#rightsliderknob").on("mouseout", function(e){
		console.log("rk mouseout")
	})
});
