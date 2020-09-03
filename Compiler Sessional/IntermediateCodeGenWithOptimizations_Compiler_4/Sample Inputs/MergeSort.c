int arr[10];
int size;
void print(int x){
	println(x);
}
void assign(){
    arr[0] = 22; arr[1] = -5; arr[2] = 7; arr[3] = -110; arr[4] = 31; arr[5] = 28; arr[6] = 15; arr[7] = 33; arr[8] = 460; arr[9] = 145; 
}
void merge(int l, int m, int r)
{
    int i, j, k;
    int n1, n2;
    n1 = m - l + 1;
    n2 =  r - m;
 
    /* create temp arrays */
    int L[10], R[10];
 
    /* Copy data to temp arrays L[] and R[] */
    for (i = 0; i < n1; i++){
        L[i] = arr[l + i];
	}
    for (j = 0; j < n2; j++){
        R[j] = arr[m + 1+ j];
	}
    /* Merge the temp arrays back into arr[l..r]*/
    i = 0; // Initial index of first subarray
    j = 0; // Initial index of second subarray
    k = l; // Initial index of merged subarray
    while (i < n1 && j < n2)
    {
        if (L[i] <= R[j])
        {
            arr[k] = L[i];
            i++;
        }
        else
        {
            arr[k] = R[j];
            j++;
        }
        k++;
    }
 
    /* Copy the remaining elements of L[], if there
       are any */
    while (i < n1)
    {
        arr[k] = L[i];
        i++;
        k++;
    }
 
    /* Copy the remaining elements of R[], if there
       are any */
    while (j < n2)
    {
        arr[k] = R[j];
        j++;
        k++;
    }
}
 
/* l is for left index and r is right index of the
   sub-array of arr to be sorted */
void mergeSort(int l, int r)
{
    if (l < r)
    {
        // Same as (l+r)/2, but avoids overflow for
        // large l and h
        int m ;
		m = l+(r-l)/2;
 
        // Sort first and second halves
        mergeSort(l, m);
        mergeSort(m+1, r);
 
        merge(l, m, r);
    }
}
void printArray();

int main(){
	assign();
	printArray();
	mergeSort(0,size - 1);
	print(6666);
	printArray();
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

/*
Net theke
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
6666                                                                                                                                                                                    
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




*/
