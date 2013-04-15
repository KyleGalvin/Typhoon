function loop(x, y, f){
	for(var i=0;i<=x;i++){
		for(var j=0;j<=y;j++){
			console.log('f('+i+','+j+')='+f(i,j));
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

var sum = function (x,y) { return x+y; }

console.log('f=power()');
loop(5,3, power);

console.log('');//newline
console.log('f=sum()');
loop(5,3,sum);
