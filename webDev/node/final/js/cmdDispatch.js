var modelHandler = require('./modelHandler')

module.exports = {
	call: function(cmd,args){
		console.log("calling command:",cmd)
		if(_cmds[cmd]){
			var result = _cmds[cmd](args)
			if(result){
				return result
			}
		}else{
			console.log("command not found!",cmd)
		}
	}
}

_cmds = {
	make_user : function(args){
		console.log('in make user!',args)
		var isExistingUser = modelHandler.query('read','model.users['+args[0]+']')
		if(!isExistingUser){
			modelHandler.query('create','model.users['+args[0]+']',{passwordSignature:args[1]})
		}
	},
	login : function(args){
		console.log('in login!',args)
	}
}
