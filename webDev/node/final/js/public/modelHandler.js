define([],function modelHandler(){
var modelHandler = function(){
console.log("creating model")
	var myModel = {
		model:{},

		query: function(transactionType,id,data){
			if(transactionType == 'create'){
				return this.create(id,data)
			}else if(transactionType == 'read'){
				return this.read(id)
			}else if(transactionType == 'update'){
				return this.update(id,data)
			}else if(transactionType == 'delete'){
				return this.del(id)
			}else{
				console.log("unknown REST command:",transactionType)
			}
		},

		publish: function (id){
			console.log("updated model:",this.model)
			var address = ""
			for (i=0;i<id.length;i++){
				if(!(i==0)){
					address+=","
				}
				address+= id[i]
				var subList = this.read(["model","subscriptions",address])
				if(subList){
					console.log("publish alert:"+address)
					for(var key in subList){
						console.log("publish to:",key)
						var session = read(["model","sessions",key])
						if(!session.socket){
							this.del(["model","sessions",key])//if a dead session is found, we may as well clean it up
							this.del(["model","subscriptions",address,key])
						}else{
							session.socket.send(JSON.stringify(["PUBLISHED MESSAGE! ",id, this.read(id)]))
						}
					}
				}
			}
		},
		create: function (id,data){
			console.log("create:",id,data)
			var node = this.model
			id.forEach(function(entry,i){
				if(i == id.length-1){//last element is the element we apply data to
						node[entry] = data	
				}else{
					if(!node[entry]){//if it doesnt exist, add empty node
						node[entry] = {}
					}	

				}
				node = node[entry]//traverse into next node
			})

			this.publish(id)
			return true
		},
		read: function (id){
			console.log("WIDGET MODEL READ!")
			var node = this.model
			var index
			//var result = id.forEach(function(entry,i){
			for(index=0; index<id.length; ++index){
				if(index == id.length-1){//last element is the element we return
					return node[id[index]]
				}else{
					if(!node[id[index]]){
						return null//path does not exist
					}
				}
				node=node[id[index]]//traverse to the specified element
			}
		},

		update: function (id,data){

			this.publish(id)
		},

		del: function (id){
			var node = this.model
			var index
			for(index=0; index<id.length; ++index){
				if(index == id.length-1){//delete the last element
					delete node[id[index]]
					this.publish(id)
				}else{
					if(!node[id[index]]){
						return null//cannot delete path which does not exist
					}
					node = node[id[index]]//traverse to the specified element
				}
			}
		},


	}
	myModel.create(["model"])
	return myModel

}
return modelHandler
})
