function githubGetData(){
	var github = new Github({
	  username: document.getElementById("username").value,
	  password: document.getElementById("password").value,
	  auth: "basic"
	});
	var repo = github.getRepo("darkdigitaldream", document.getElementById("repo").value);

	repo.read('master', document.getElementById("file").value, function(err, data) {codeMirror.doc.setValue( data)});
	//repo.getTree('master?recursive=true', function(err, tree) {document.getElementById("github").innerHTML = JSON.stringify(tree)});
	codeMirror.doc.setValue( document.getElementById("code").value);
	codeMirror.refresh();
}
