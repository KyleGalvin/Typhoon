function loop(x, y, f){
	for(var i=0;i<=x;i++){
		for(var j=0;j<=y;j++){
			console.log(i + ' to the power of ' + j + ' is ' + f(i,j));
		}
	}
}

function power(x,y){
	if(y==0){
		return 1;
	}else{
		return x * power(x,y-1);
	}
}

loop(5,3, power);

