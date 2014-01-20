var model = {}

module.exports = {
	query: function(transactionType,id,data){
		if(transactionType == 'create'){
			return create(id,data)
		}else if(transactionType == 'read'){
			console.log("read!")
			return read(id)
		}else if(transactionType == 'update'){
			console.log("update!")
			return update(id,data)
		}else if(transactionType == 'delete'){
			console.log("delete!")
			return del(id)
		}else{
			console.log("unknown REST command:",transactionType)
		}
	}
}

create = function (id,data){
console.log("creating...",id,data)
	var node = model
	console.log("original model:",model)
	id.forEach(function(entry,i){
		if(i == id.length-1){//last element is the element we apply data to
				node[entry] = data	
				console.log("hit bottom",node)
		}else{
			if(!node[entry]){//if it doesnt exist, add empty node
				node[entry] = {}
				console.log("new node:",node)
			}	

		}
		node = node[entry]//traverse into next node
	})
	console.log("updated model:",model)
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
			}else{
				console.log("read traverse:",node[id[index]])
			}
		}
		node=node[id[index]]
	}
	console.log("returned json:",node)
}

update = function (id,data){

}

del = function (id){

}
