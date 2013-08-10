var codeMirror;

window.onload = function (){
	codeMirror = CodeMirror.fromTextArea(document.getElementById("code"),{
        lineNumbers: true,
        matchBrackets: true,
        extraKeys: {"Enter": "newlineAndIndentContinueComment"}});//different elementID?
	//document.getElementById("tab-2").nextSibling.appendChild(codeMirror);
	codeMirror.doc.setValue( document.getElementById("code").value);
}
