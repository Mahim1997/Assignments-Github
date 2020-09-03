int f2(int);
int f3(int);
int f1(int a){
    if(a<10){
        return f2(a);   //return 2*a
    }else{
        return f3(a);   //return 3*a
    }
}
int main(){
    int a[10];
    int i ;
    i = 0;
    while(i < 10){
        a[i] = i; 
        i++;
    }
    int x, y;
    x = f1(a[0]);   //a[0] = 0
    y = f2(a[1]);   //a[1] = 1
    println(x);
    println(y);
}
int f2(int x){
    if(x != 0){
        return 2*x; 
    }else{
        return 2;
    }
}

int f3(int b){
    if(b != 0){
        return 3*b;
    }else{
        return 3;
    }

}
