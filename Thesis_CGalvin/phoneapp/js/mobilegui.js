$(document).ready(function() {
	console.log("height:",$(window).height())
	$("#underlay").attr('width',$(window).width())
	$("#underlay").attr('height',$(window).height())
	$("#leftslider").css('top', $(window).height()/2.0 - $("#rightslider").height()/2.0)
	$("#rightslider").css('top', $(window).height()/2.0 - $("#rightslider").height()/2.0)
	$("#rightslider").css('left', $(window).width() - $("#rightslider").width())
	$("#mutetoggle").css('top',0 )
	$("#mutetoggle").css('left', $(window).width()/2.0 - $("#mutetoggle").width()/2.0)
});
