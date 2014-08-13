var modelHandler = require('./modelHandler')
var path = require('path')
var git = require('../node_modules/nodegit/')
var fs = require('fs')
var repositoryDir = '../../../..'
//var Repo = function(){
//	this.rawRepo = new git.raw.Repo()
//}
//Repo.prototype.open = function(directory, function(error,repository){
//})

module.exports = {
	call: function(cmd,sessionID,args,address){
		console.log("calling command:",cmd,address)
		if(_cmds[cmd]){
			var result = _cmds[cmd](sessionID,args,address)
			if(result){
				console.log("returning result to sender",result)
				return result
			}
		}else{
			console.log("command not found!",cmd)
			return {"test":"command not found returned"}
		}
	}

}
function stringToModelID(string){
	return string.replace(' ','_')
}

_cmds = {

	git_log: function(id,args){
		console.log("git_log called.")
		console.log("id:")
		console.log(id)
		console.log("args:")
		console.log(args)
		console.log('git:',git)
		git.Repository.open('../../../../.git',function(error,repo){
			console.log("repo open. In callback now")
			console.log(repo)
			if (error) throw error
			repo.getBranch('master', function(error,branch){
				if (error) throw error
				console.log("found branch")
				console.log(branch)
				var history = branch.history()

				history.on("commit",function(commit){
					console.log('commit ',commit.sha())
					console.log(commit.author().name())
					console.log(commit.message())
				})

				history.start()
			})
		})
		//git.repo('./',function(error,repo){
		//	console.log('repo callback')
		//})
		//var repo = new git.Repo()
		//var dir = repo.open("../../../.git",function(){

		//})
		return {success:true,note:"getTree"}

	},
	git_getDirectory: function(id,args){
		console.log("git_getDirectory called still.")
		var userResult = modelHandler.query('read',//action
			["model","sessions"]
		)

		console.log('user result: ', userResult) 
		var path = args[0]
		var dir = fs.readdirSync(path)
		var list = []
		for(var i = 0; i < dir.length; i++){
			var loc = path +'/' + dir[i]
			var item = fs.statSync(loc)
			console.log("inner:", item)
			if (item.isDirectory()){
				list.push( {type:'dir',path:loc})
			}else if (item.isFile()){
				list.push( {type:'file',path:loc})
			}
			else{
				console.log('wtf')
			}
			console.log('item:',item)

		}
		return {success:true,identifier:true, list:list,path:path}
		console.log('finished dir: ',dir)
	},
	git_retrieveDocument: function(id,args){
		console.log('args into readFile:',args[0])
		var data = fs.readFileSync(args[0],"utf8")
		console.log("git_retrieveDocument called")
		return {success:true,data:data}
	},
	git_commitDocument: function(id,args){

	},
	disconnect: function(id,args){
		modelHandler.query('delete',["model","sessions",stringToModelID(id)])
	},
	//function (sessionID, modelPath){
	subscribe: function (id,args,address){
		console.log("SUBSCRIBED")
		var subscriptionList = modelHandler.query('read',
			["model","subscriptions",stringToModelID(args.toString())]
		)
		if(!subscriptionList){
			subscriptionList = {}
		}
		//if(!subscriptionList[args.toString()]){
		//	subscriptionList[args.toString()]={}	
		//}

		//subscriptionList[args.toString()][id] = 'true'
		console.log("SUBSCRIPTION DATA:", id, address)
		subscriptionList[id] = address

		var result = modelHandler.query('create',
			["model","subscriptions",stringToModelID(args.toString())],
			subscriptionList
		)
		return {sucess:true}
	
	},

	add_session: function(id,args){
		var result = modelHandler.query('create',
			["model","sessions",id],
			{socket: args[0], created: new Date()}
		)
	},
	logout : function(id,args){
		var groupResult = modelHandler.query('create',
			["model","group","users",stringToModelID(args[0]),"online"],
			false
		)
		var userResult = modelHandler.query('create',//action
			["model","users",stringToModelID(args[0])],//location
			{username:args[0] ,passwordSignature:stringToModelID(args[1]), group:["model","group","users"], online:false} // data
		)

	},
	make_user : function(id,args){
		if(args[0]!=null && args[1] !=null){
			var isExistingUser = modelHandler.query('read',["model","users",stringToModelID(args[0])])
			if(!isExistingUser){
				//check if the main user group already exists
				var groupExists = modelHandler.query('read',
					["model","group","users"]
				)
				if(!groupExists){//create it if it does not
					var groupExists = modelHandler.query('create',
						["model","group","users"],
						{} //group starts out empty
					)
				
				}
				//make the user account
				var userResult = modelHandler.query('create',//action
					["model","users",stringToModelID(args[0])],//location
					{username:args[0] ,passwordSignature:stringToModelID(args[1]), group:["model","group","users"],online:false} // data
				)
				//make the user in users group (default status offline)
				var groupResult = modelHandler.query('create',
					["model","group","users",stringToModelID(args[0]),"online"],
					false
				)
				if (userResult && groupExists){
					return {success:true}

				}else if(!groupExists){
					return {success:false,error:"could not create group"}
				}else{
					return {success:false,error:"could not create user"}

				}
			}else{
				return {success:false,error:"user already exists"}
			}
		}else{
			return {success:false,error:"passwords do not match"}
		}
	},
	login : function(id,args,address){
		console.log("LOGIN ADDRESS:",address)
		var userData = modelHandler.query('read',['model','users',stringToModelID(args[0])])
		if (userData && userData.passwordSignature == args[1]){
			//set the user to online
			modelHandler.query('create',['model','users',stringToModelID(args[0]),'online'],true)
			//alert the user's group they are online
			modelHandler.query('create',["model","group","users",stringToModelID(args[0]),'online'],true)
			//this.subscribe(id,["model","group","users"],address)
			return {success:true,action:"login",name:stringToModelID(args[0])}

		}else{
			return {success:false,error:"invalid credentials"}

		}
	}
}
