int max(int x, int y);
int func(int a, int b, int c){
    if(a >= b){
        return max(a, c);
    }
    else{
        return max(b, c);
    }
}
int main()
{
    int a, b, c;
    a = func(1, 2, 3);
    println(a);
    int arr[10];
    int i;
    for(i=0; i<10; i++){
    	arr[i] = 10-i;
    }
    b = func(arr[0], arr[4], arr[6]);
    println(b);
}
int max(int x, int y){
    if(x >= y){
        return x;
    }
    return y;
}
