int arr[10];
int size;
int isSorted;   //kind of like flag, gets activated once sort() is called.
void sort();
int getMax();
int getMin();
void print(int arr){
    println(arr);
}
void printArray();
void assign(){
    arr[0] = 22; arr[1] = -5; arr[2] = 7; arr[3] = -110; arr[4] = 31; arr[5] = 28; arr[6] = 15; arr[7] = 33; arr[8] = 460; arr[9] = 145; 
}
void runner(){
    size = 10;
    assign();
    printArray();       //Normally after assigning print array
    sort(); 
    printArray();       //After assigning print array
    int max, min;
    max = getMax();     
    min = getMin();     
    println(max);       //450
    println(min);       //-110
}
int main(){
    isSorted = 0;
    runner();
}
void printArray(){
    size = 10;
    int i;
    print(7777);       //Print array start bujhar jonne
    for(i=0; i<size; i++){
        print(arr[i]);
    }
    print(-7777);      //To end printarray bujhar jonne
}
int getMax(){
    if(isSorted == 1){
        return arr[size - 1];
    }
    else{
        sort();
        return arr[size - 1];
    }
}
int getMin(){
    if(isSorted == 0){
        sort();
    }
    return arr[0];
}   
void sort(){
    //Ascending order insertion sort
    int c, d, t;
    int n ;
    n = 10;
    for (c = 1 ; c <= (n - 1); c++){     //Outer Loop
        d = c;

        while ( (d > 0) && (arr[d-1] > arr[d])){   //Inner Loop
            t  = arr[d];
            arr[d]   = arr[d-1];
            arr[d-1] = t;

            d--;
        }
    }
    isSorted = 1;
}

/*
Net theke newaa
7777                                                                                                                                                                                    
22                                                                                                                                                                                      
-5                                                                                                                                                                                      
7                                                                                                                                                                                       
-110                                                                                                                                                                                    
31                                                                                                                                                                                      
28                                                                                                                                                                                      
15                                                                                                                                                                                      
33                                                                                                                                                                                      
460                                                                                                                                                                                     
145                                                                                                                                                                                     
-7777                                                                                                                                                                                   
7777                                                                                                                                                                                    
-110                                                                                                                                                                                    
-5                                                                                                                                                                                      
7                                                                                                                                                                                       
15                                                                                                                                                                                      
22                                                                                                                                                                                      
28                                                                                                                                                                                      
31                                                                                                                                                                                      
33                                                                                                                                                                                      
145                                                                                                                                                                                     
460                                                                                                                                                                                     
-7777                                                                                                                                                                                   
460                                                                                                                                                                                     
-110
*/