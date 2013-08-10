window.onload= function(){

	document.getElementById("dbchooser").addEventListener("DbxChooserSuccess",function(e) {
		alert("Here's the public link to the chosen file: " + e.files[0].link);
	}, false);

}
