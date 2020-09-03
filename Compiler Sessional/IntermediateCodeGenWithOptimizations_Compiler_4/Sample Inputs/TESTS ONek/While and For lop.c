int main(){
    int a,b,i;
    b=0;
    for(i=0;i<4;i++){
        a=3;
        while(a--){
            b++;
        }
    }
    println(a);		//-1		amar ashe 0
    println(b);		//12		amar ashe 8
    println(i);		//4
}