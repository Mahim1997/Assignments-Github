int a[10];
int f1(int a){
    return a;
}
void print(int x){
    println(x);
}
void printArray();
void sort(){
    //Insertion Sort..
    int c, d, t;
    int n ;
    n = 10;
    for (c = 1 ; c <= (n - 1); c++){     //Outer Loop
        d = c;

        while ( (d > 0) && (a[d-1] > a[d])){   //Inner Loop
            t  = a[d];
            a[d]   = a[d-1];
            a[d-1] = t;

            d--;
        }
    }
}
int main(){
    int i;
    for(i=0; i<10; i++){
        a[i] = (10 - i) % 3;
    }
    printArray();       //Should print array as ..
    print(-777);
    a[0] = 22; a[1] = -5; a[2] = 7; a[3] = 110; a[4] = 31; a[5] = 28; a[6] = 15; a[7] = 33; a[8] = 4460; a[9] = 1445; 
    printArray();       //Should print array as ..
    print(-555);        //Should print -10000
    sort();             //Should sort => No printing
    printArray();       //Should print array as ..
    print(-333);
}
void printArray(){
    int i;
    for(i=0; i<10; i++){
        print(a[i]);
    }
}

/*Net theke output
1                                                                                                                                                                                       
0                                                                                                                                                                                       
2                                                                                                                                                                                       
1                                                                                                                                                                                       
0                                                                                                                                                                                       
2                                                                                                                                                                                       
1                                                                                                                                                                                       
0                                                                                                                                                                                       
2                                                                                                                                                                                       
1                                                                                                                                                                                     
-777                                                                                                                                                                                  
22                                                                                                                                                                                      
-5                                                                                                                                                                                      
7                                                                                                                                                                                       
110                                                                                                                                                                                     
31                                                                                                                                                                                      
28                                                                                                                                                                                      
15                                                                                                                                                                                      
33                                                                                                                                                                                      
4460                                                                                                                                                                                    
1445                                                                                                                                                                                    
-777                                                                                                                                                                                 
-5                                                                                                                                                                                      
7                                                                                                                                                                                       
15                                                                                                                                                                                      
22                                                                                                                                                                                      
28                                                                                                                                                                                      
31                                                                                                                                                                                      
33                                                                                                                                                                                      
110                                                                                                                                                                                     
1445                                                                                                                                                                                    
4460   

*/