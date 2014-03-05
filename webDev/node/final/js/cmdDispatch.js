var modelHandler = require('./modelHandler')

module.exports = {
	call: function(cmd,args){
		console.log("calling command:",cmd)
		if(_cmds[cmd]){
			var result = _cmds[cmd](args)
			if(result){
				console.log("returning result to sender",result)
				return result
			}
		}else{
			console.log("command not found!",cmd)
		}
	}
}
function stringToModelID(string){
	return string.replace(' ','_')
}

_cmds = {
	make_user : function(args){
		console.log('in make user!',args)
		if(args[1] == args[2] && args[1] !=null){
			var isExistingUser = modelHandler.query('read',["model","users",stringToModelID(args[0])])
			console.log("does user exist?",isExistingUser)
			if(!isExistingUser){
				var result = modelHandler.query('create',["model","users",stringToModelID(args[0])],{username:args[0] ,passwordSignature:stringToModelID(args[1])})
				if (result){
					return {success:true}
				}else{
					return {success:false,error:"unknown error with model creation"}

				}
			}else{
				return {success:false,error:"user already exists"}
			}
		}else{
			return {success:false,error:"passwords do not match"}
		}
	},
	login : function(args){
		console.log('in login!',args)
		return {success:false,error:"function not built yet"}
	}
}
