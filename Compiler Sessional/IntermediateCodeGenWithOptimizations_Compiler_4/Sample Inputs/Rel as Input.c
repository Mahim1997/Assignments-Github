 
int f1(int a){
 
    return (2*a);       //return 2*a
}
int main(){
    int x, y, a[10];
    a[7] = 7;
    a[5] = 5;
    
    x = a[7];
    x = f1((x + 1));    //16
 
    y = f1(a[5]);   //a[5] = 5
    println(x);     //16
    println(y);     //2&5 = 10
}
