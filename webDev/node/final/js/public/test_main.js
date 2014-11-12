
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
}
