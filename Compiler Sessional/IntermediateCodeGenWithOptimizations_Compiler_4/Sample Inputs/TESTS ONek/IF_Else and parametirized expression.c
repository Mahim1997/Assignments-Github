
int main(){
	int i, c, b ;
	c = 19; b = 5;
	int a[10];
	a[2] = 7; a[3] = 3;
	
	if(c > 2){
		i = 1111;
	}else{
		i = 22222;
	}
	println(i);		//1111
	
	if(a[2] >= a[3]){
		a[4] = -100;
	}else{
		a[4] = -10;
	}
	i = a[4] ;
	println(i);		//-100
	
	if(a[2] > -10){
		a[5] = 5; 
	}
	a[6] = a[5]; 	
	b = a[6];	
	println(b);		//5
	
	if((1<2) && (7>4)){
		i = 1;
	}else{
		i = 2;
	}
	println(i);		//1
	
	a[1] = 1 + (a[3] - 1)*(a[2] - 5);	
	b = a[1];
	println(b);		//5
	if(a[1] >= 0){
		i = -7;
	}
	println(i);		//-7
}
