
int main(){
	int i, c, b ;
	c = 7;
	b = 2;
	i = c + b;
	println(i);	//9
	int a[10];
	a[2] =7; a[3] = 4;
	i = a[2] - a[3];
	println(i);	//3
	i = a[2] - 1;
	println(i);	//6
	a[3] = a[2] + a[3];
	i = a[3];
	println(i);	//11
}
