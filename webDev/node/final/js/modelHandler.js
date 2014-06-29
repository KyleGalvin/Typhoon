var model = {}
module.exports = {
	query: function(transactionType,id,data){
		if(transactionType == 'create'){
			return create(id,data)
		}else if(transactionType == 'read'){
			return read(id)
		}else if(transactionType == 'update'){
			return update(id,data)
		}else if(transactionType == 'delete'){
			return del(id)
		}else{
			console.log("unknown REST command:",transactionType)
		}
	}

}


//TODO: get socket to push to subscribed users
publish = function (id){
	console.log("updated model:",model)
	var address = ""
	for (i=0;i<id.length;i++){
		if(!(i==0)){
			address+=","
		}
		address+= id[i]
		var subList = read(["model","subscriptions",address])
		if(subList){
			console.log("publish list:"+JSON.stringify(subList),address)
			console.log("publish alert:"+address,Object.keys(subList).length)
			for(var key in subList){
				console.log("publish to:",key, subList[key])
				var session = read(["model","sessions",key])
				if(!session || !session.socket){
					console.log("dead session",key)
					del(["model","sessions",key])//if a dead session is found, we may as well clean it up
					del(["model","subscriptions",address,key])
				}else{
					//console.log("session data:",session)
					console.log("ID:",id)
					session.socket.send(JSON.stringify({address:subList[key], path:id, data:read(id)}))
				}
			}
		}
	}
}


create = function (id,data){
	var node = model
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
	console.log("creating data:",data," for id ",id)
	publish(id)
	return true
}

read = function (id){
	var node = model
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
}

update = function (id,data){

	publish(id)
}

del = function (id){
	var node = model
	var index
	for(index=0; index<id.length; ++index){
		if(index == id.length-1){//delete the last element
			delete node[id[index]]
			publish(id)
		}else{
			if(!node[id[index]]){
				return null//cannot delete path which does not exist
			}
			node = node[id[index]]//traverse to the specified element
		}
	}
}

