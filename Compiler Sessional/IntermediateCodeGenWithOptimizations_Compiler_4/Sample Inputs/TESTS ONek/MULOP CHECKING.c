
int main(){
	int i, c, b ;
	c = 19;
	b = 5;

	i = c * b;
	println(i);			// 95
	i = c/b;
	println(i);			//3

	i = c % b; 
	println(i);			//4
	
	int a[10];
	
	a[2] = 7; a[3] = 3;
	
	i = a[2] * a[3];
	println(i);			//21

	a[4] = a[2] % a[3];
	i = a[4];
	println(i);			//1
	

	a[5] = a[2] / a[3];
	i = a[5];
	println(i);			//2
	
}
