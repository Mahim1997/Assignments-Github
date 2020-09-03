
int main(){
	int i, a[10], j;
	
	a[2] = 1;
	a[7] = 0;
 
	i = a[2] && a[7];	
	println(i);		// 0
	i = a[2] || 0;
	println(i);		// 1
	
	a[2] = 22;
	a[7] = 31;	
	
	i = a[2] == a[7];	//0
	println(i);
	
	i = a[2] >= 22;		//1
	println(i);
	
	i = a[2] != a[7];  //1
	println(i);
	
	i = a[2] >= 100;	//0
	println(i);
	
	i = a[2] != 4; //1
	println(i);
}

