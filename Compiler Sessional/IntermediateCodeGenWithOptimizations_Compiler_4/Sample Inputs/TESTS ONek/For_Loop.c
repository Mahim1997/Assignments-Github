
int main(){
	int i, c, b ;		//Program finds max and the index of the max element of the array. 
	int max;
	max = -1;
	int a[10];
	for(i=0; i<10; i++){
		a[i] = (i * 10);
	}
	for(i=0; i<10; i++){
		b = a[i];
		println(b);
	}
	for(i=0; i<10; i++){
		if(a[i] >= max){
			max = a[i];
			c = i;
		}
	}
	println(max);
	println(i);
}
