int fun(int a){
	//return a;
	return ( (((a + 1) % 10 ) * 2) + 1);	//Always gonna return a num >= 1
}
int main(){
	int x, a;
	a = 7;
	x = fun(a)  ;
	println(x);		//Should print 7 since fun(7)=7  ,  print 17 
	
	x = fun(a) < 10;
	println(x);		//Should print 1 since 7 is Actually < 10;  .... 19 < 10 is false = 0

}
