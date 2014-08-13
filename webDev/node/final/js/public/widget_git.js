
define(['clientConnection'], function(){
	var widget = function(wFactory,model){
		var id = wFactory.counter
		var socket = wFactory.socket
		var view = $("<div id='git' />").append("git")
		this.view = view

		this.handlemessage = function(message){
			console.log("widget_git handling message!")
			console.log("message:")
			console.log(message.list)
			if(message.list){	
				fileTree.empty()
				var topLevelPath = message.path
				//var index = topLevelPath.lastIndexOf("/")
				//topLevelPath = topLevelPath.substring(0,index)
				console.log("upper directory: ", topLevelPath)

				if (topLevelPath.split("/").length > 1){
					var parentIndex = topLevelPath.lastIndexOf("/")
					var parentDir = topLevelPath.substring(0,parentIndex)
				
					var item = $("<div id = '"+path+"'>")
						.css({
							"width":"100%"
						})
						.append("../")
					console.log("parent dir action: ",parentDir)
					item.click(parentDir,getTree)
					fileTree.append(item)
				}
				for(var i=0; i< message.list.length; i++){
					var path = message.list[i].path
					var type = message.list[i].type
					var item = $("<div id = '"+path+"'>")
						.css({
							"width":"100%"
						})
						.append(path)

					if(type == "file"){
						item.click(path,getFile)

					}else{
						item.click(path,getTree)
					}

					fileTree.append(item)
				}
			}
		}

		var getFile = function(filename){
			console.log("trying to grab file...")
			var openFile = {}
			openFile.type = "editor"
			openFile.command='open',
			openFile.local = true,
			openFile.args=[filename.data]
			socket.write(openFile)	
		}

		var getTree = function(baseDir){
			console.log("getTree() called")
			path = "."
			if(baseDir.data){
				console.log("get tree has path ",baseDir.data)
				path = baseDir.data
			}else{
				console.log("get tree has no path. using ./")
			}
			var makeUser = {}
			makeUser.address = id
			makeUser.command='git_getDirectory',
			makeUser.args=[path]
			socket.write(makeUser)	

		}
		var groupSubscribe = {}
		groupSubscribe.address = id
		groupSubscribe.command='subscribe'
		groupSubscribe.args=["model","repo","user","dir"]
		socket.write(groupSubscribe)


		var fileTree = $("<div id=file_tree>")
			.css({

				'float':'left',
				'width':'100%',
				'height':'50%',
			})
		view.append(fileTree)
		getTree('.')
	}
	return widget
})
