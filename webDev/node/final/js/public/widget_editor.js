define(['codemirror-4.3/lib/codemirror','codemirror-4.3/mode/javascript/javascript','clientConnection'], function(CodeMirror){

	function loadCss(url) {
	    var link = document.createElement("link");
	    link.type = "text/css";
	    link.rel = "stylesheet";
	    link.href = url;
	    document.getElementsByTagName("head")[0].appendChild(link);
	}

	var widget = function(wFactory,model){
		loadCss('codemirror-4.3/lib/codemirror.css')
		var id = wFactory.counter
		var socket = wFactory.socket
		this.handlemessage = function(message){
			console.log("editor recieving command...",message)

			if(message.command && message.command == "open"){
				var fileRequest = {}
				fileRequest.address = id
				fileRequest.command='git_retrieveDocument',
				console.log("sending args?",message.args[0])
				fileRequest.args=[message.args[0]]
				socket.write(fileRequest)	
			}else if(message.data){
				console.log("recieved data:",message.data)
				editor.getDoc().setValue(message.data)
			}
		}
	
		var textArea = $("<textarea id='code', name='code' />")
			.css({
				"width":"80%",
				"height":"100%",
				"float":"right"
			})
		this.view = $("<div id='codemirror'/>")
		this.view.append(textArea)
		console.log("code mirror:",CodeMirror)
		console.log("code mirror done",textArea)
		var editor = CodeMirror.fromTextArea(textArea[0],{
			mode:"javascript",
			lineNumbers:true,
			lineWrapping:true
		})
		//$("#codemirror").css({"height":'100%'})
		//editor.doc.setValue( textArea[0].value);
		//console.log("editor:",editor)
	}
	return widget
})
