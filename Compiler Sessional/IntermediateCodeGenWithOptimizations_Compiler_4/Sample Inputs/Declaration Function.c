int f(int a);
int f2();
int main(){
    int x;
    x = f(7);
    println(x);
    x = f2();
    println(x);
}

int f(int a){
    return a;

}
int f2(){
    return -7;
}