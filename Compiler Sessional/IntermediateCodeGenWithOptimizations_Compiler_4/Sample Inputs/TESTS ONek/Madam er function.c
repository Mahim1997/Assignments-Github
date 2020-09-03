int f(int a){
    return 2*a;		//f(1) = 2*1 = 2;
    a=9;
}

int g(int a, int b){
    int x;			//a = 1, b = 2;
    x=f(a)+a+b;		// f(1) + 1 + 2 = 2 + 1 + 2 = 5. 
    return x;
}

int main(){
    int a,b;
    a=1;
    b=2;
    a=g(a,b);		// g(1, 2);
    println(a);		//5 print kore .. 
    return 0;
}
