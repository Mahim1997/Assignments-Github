int globalVariable;
int fun1(int a, int b){
	return (a - b);
}
void f(int b){
	b--;
}
int main(){
	int x, i;

	x = fun1(5, 1);
	println(x);
}
