module.exports = {
	query: function(transactionType,id,data){
		console.log("calling command:",transactionType)
		console.log("id:",id)
		console.log("data:",data)
		if(transactionType == 'create'){
			console.log("create!")
			create(id,data)
		}else if(transactionType == 'read'){
			console.log("read!")
			read(id)
		}else if(transactionType == 'update'){
			console.log("update!")
			update(id,data)
		}else if(transactionType == 'delete'){
			console.log("delete!")
			del(id)
		}else{
			console.log("unknown REST command:",transactionType)
		}
	}
}

create = function (id,data){

}

read = function (id){

}

update = function (id,data){

}

del = function (id){

}
