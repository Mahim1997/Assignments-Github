#include<iostream>
#include<cstdio>
#include<cstdlib>
#include<cmath>
#include<fstream>
#include <set>
#include <iterator>
#include<vector>
#include<algorithm>
#include<istream>
#include <sstream>
#include<iostream>
#include<cstdio>
#include<cstdlib>
#include<cmath>
#include<fstream>
#include <set>
#include <iterator>
#include<vector>
#include<algorithm>
#include<istream>
#include <sstream>
#include<iostream>
#include <queue>
#define NULL_VALUE -99999
#define MAX_VALUE 10000
#define INF_VALUE 9999
#define LOCAL_MAX 100
using namespace std ;

bool doWeTakeSet[MAX_VALUE];
int UniversalSet[MAX_VALUE];
int cardinalityArray[MAX_VALUE];
int m, n;
bool isElementCovered[MAX_VALUE]; ///

std::vector<std::string> splitString(const std::string& a, char delim)
{
    std::vector<std::string> tokens;
    std::string token;
    std::istringstream tokenStream(a);
    while (std::getline(tokenStream, token, delim))
    {
        tokens.push_back(token);
    }
    return tokens;
}

void printLine()
{
    printf("\n");
    int n = 25;
    int x = 20;
    for(int i=0; i<n; i++)
    {
        printf("-");
    }
    for(int i=0; i<x; i++)
    {
        printf("*");
    }
    for(int i=0; i<n; i++)
    {
        printf("-");
    }
    printf("\n\n");
}
void printArray(int n, int start,bool arr[])
{
    if(start == 0)
    {
        for(int i=0; i<n; i++)
        {
            printf("%d, ", arr[i]);
        }
    }
    else
    {
        for(int i=1; i<=n; i++)
        {
            printf("%d, ", arr[i]);
        }
    }
    cout << endl;
}
void printArray(int n, int start,int arr[])
{
    if(start == 0)
    {
        for(int i=0; i<n; i++)
        {
            printf("%d, ", arr[i]);
        }
    }
    else
    {
        for(int i=1; i<=n; i++)
        {
            printf("%d, ", arr[i]);
        }
    }
    cout << endl ;
}
void printSet(set<int> mySet)
{
    set<int> :: iterator itr ;
    for(itr = mySet.begin(); itr != mySet.end(); itr++)
    {
        printf("%d, ", *itr);
    }
    printf("\n\n");
}

void printArrayOfSets(int m, set<int> arrayOfSets[])
{
    for(int i=0; i<m; i++)
    {
        printf("PRINTING SET : %d\n", i);
        printSet(arrayOfSets[i]);
        printf("\n\n");
    }
}
class Answer
{
public :
    int numberOfSubsetsUsed;

    Answer()
    {
        numberOfSubsetsUsed = 0;
    }
    void printAnswer(int m, set<int>arrayOfSets[] )
    {

        printLine();
        printf("OUTPUT: \n");
        printf("Number of subsets used = %d\n", numberOfSubsetsUsed);

        for(int i=0; i<m; i++)
        {
            if(doWeTakeSet[i] == true)
            {
                printSet(arrayOfSets[i]);
            }
        }

        printLine();
    }
};
class Node
{
public:
    int level ; ///index in the subsets[array];
    double  lowerBound ;
    int current_number_of_UniversalSet_Elements;

    int currentUniverse[LOCAL_MAX];
    bool whichSetsAreUsed_Local[LOCAL_MAX];
    double LBWithoutThis;
    double lowerBoundWithSet;

    set<int> currentUniversalSet ;

    double  calcLBWithThis()
    {
        return lowerBound;
    }
    double calcWithoutLB()
    {
        return lowerBound;
    }
    void calculateLowerBound()
    {
        printf(">>>Inside calculate Lower Bound of node level = %d\n", level);
//        printf("PRINTING NODE.. ."); this->printNode();
        int numSetsPickedSoFar = 0;
        int numElementsNotCovered = n - current_number_of_UniversalSet_Elements;

        double maxCardinality = 0;

        for (int i=0; i<m; i++)
        {
            if(whichSetsAreUsed_Local[i]==true)
            {
                numSetsPickedSoFar++;
            }
            else
            {
                if( (cardinalityArray[i] > maxCardinality) && (i!=level) )
                    maxCardinality = cardinalityArray[i];
            }
        }
        if(maxCardinality != 0)
        {
            printf("=->Inside calculateLowerBound()... <Line 162> Remaining elements number = %d, max Card = %lf, num sets picked so far = %d\n",
                   numElementsNotCovered, maxCardinality, numSetsPickedSoFar);

            double temp = (double)numElementsNotCovered/(double)maxCardinality;

            lowerBound = numSetsPickedSoFar + temp ;
        }
        else
        {
            lowerBound = numSetsPickedSoFar;
        }
    }

    Node()
    {
        for(int i=0; i<m; i++)
        {
            this->whichSetsAreUsed_Local[i] = false;
        }
        for(int i=0; i<m; i++)
        {
            this->currentUniverse[i] = -1;
        }
        level = -1;
        lowerBound = NULL_VALUE;
        current_number_of_UniversalSet_Elements = 0;

    }
    void setSet(set<int>mySet)
    {
        set<int> :: iterator itr ;
        for(itr = mySet.begin(); itr != mySet.end(); itr++)
        {
            currentUniversalSet.insert(*itr) ;
//            printf("%d, ", *itr);
        }
    }
    void printNode()
    {
        printLine();
        printf("Printing node... Level = %d\n", level);
        printf("Printing set... >> ");
        printThisSet();
        printf("lowerBound = %lf\n", lowerBound);
        printf("current_number_of_UniversalSet_Elements = %d\n", current_number_of_UniversalSet_Elements);
        printf("printing ... currentUniverse  = >>> ");
        printSet(currentUniversalSet);
        printf("printing.. currentUniverseArray >>> ");
        printArray(current_number_of_UniversalSet_Elements, 1, currentUniverse);
        printf("doWeTakeSets array >>> ");
        printArray(m, 0, doWeTakeSet);
        printf("whichSetsAreUsed_Local array >>> ");
        printArray(current_number_of_UniversalSet_Elements, 0, whichSetsAreUsed_Local);
        printLine();
    }
    void printThisSet()
    {
        printSet(currentUniversalSet);
    }
};
bool operator < (Node n1, Node n2)    ///Comparison operator for min heap
{
    if(n1.lowerBound == n2.lowerBound)
    {
        if(n1.current_number_of_UniversalSet_Elements < n2.current_number_of_UniversalSet_Elements)
        {
            return true ;
        }
        else
        {
            return false ;
        }
    }
    else
    {
        if(n1.lowerBound < n2.lowerBound)
            return false ;
        else
            return true ;
    }

}

void function_runner(int n, int m, set<int> arrayOfSets[])
{
    Node nodeArray[m] ;
    for(int i=0; i<m; i++)
    {
        nodeArray[i].setSet(arrayOfSets[i]);
    }
    printf("Inside function_runner... printing sets\n");
    for(int i=0; i<m; i++)
    {
        nodeArray[i].printThisSet();
    }
    printf("Printing cardinality.. \n");
    printArray(m, 0, cardinalityArray);
    for(int i=1; i<=n; i++)
    {
        UniversalSet[i] = i ;
        isElementCovered[i] = false ;
    }

    Node u ;
    Node take_set_node ;
    Node not_take_set_node;
    priority_queue <Node> heap;

    //initializer node
    u.level = -1;
    u.calculateLowerBound();
    u.printNode();


    heap.push(u);

    while(heap.empty() == false)
    {
        printf(">>>Peaking at top Element from Heap... \n");
        u = heap.top();
        printf("\n -----XXXXXX------------ LEVEL = %d ------------XXXXXX----- \n", u.level);
        u.printNode();

        if(u.current_number_of_UniversalSet_Elements == n)
        {
            printf("==-->>>> u.currentNumUnversalSetElements = n so   break  ..\n");
            Answer ans;
            ans.numberOfSubsetsUsed = 0;
            for(int i=0; i<m; i++)
            {
                if(u.whichSetsAreUsed_Local[i] == true)
                {
                    ans.numberOfSubsetsUsed ++ ;
                }
                doWeTakeSet[i] = u.whichSetsAreUsed_Local[i];
            }
            ans.printAnswer(m, arrayOfSets);
            break ;
        }
        printf("<><><>NOW we pop from Heap\n");
        heap.pop();
        printf(">>><LINE 335> u.level = %d, u.lowerBound = %lf\n", u.level, u.lowerBound);
//        printf("InitialLowerBound = %lf\n", initialLowerBound);
        if(u.level == m - 1)
        {
            printf("---++++>>>>WE HAVE REACHED LEAVES BUT UniversalSet not covered so we backtrack... line 266 ..   ...  \n");
        }
        else
        {

            printf(">>>>>LINE 343 LEAVES NOT REACHED! ... still decision to be taken... \n");

            not_take_set_node.level = u.level + 1;  ///NOT USING u Set
            take_set_node.level = u.level + 1;  ///USING u Set


            for (int i=0; i<m; i++)     ///COPY BOOLEAN WHICH SETS USED FROM u
            {
                take_set_node.whichSetsAreUsed_Local[i] = u.whichSetsAreUsed_Local[i];
                not_take_set_node.whichSetsAreUsed_Local[i] = u.whichSetsAreUsed_Local[i];
            }
            ///COPY NUM ELEMENTS OF UNIVERSAL SET FROM u
            take_set_node.current_number_of_UniversalSet_Elements = u.current_number_of_UniversalSet_Elements;
            not_take_set_node.current_number_of_UniversalSet_Elements = u.current_number_of_UniversalSet_Elements;

            for (int i=0; i<u.current_number_of_UniversalSet_Elements; i++) ///COPY Local Universe From u
            {
                take_set_node.currentUniverse[i] = u.currentUniverse[i];
                not_take_set_node.currentUniverse[i] = u.currentUniverse[i];
            }
            printf("<><>LINE 66 Setting universal sets of children to `this` node's universal set _ local... \n");
            take_set_node.setSet(u.currentUniversalSet);
            not_take_set_node.setSet(u.currentUniversalSet);


            /// BRANCH NOT TAKE..
            not_take_set_node.whichSetsAreUsed_Local[not_take_set_node.level] = false;
            printf(">>>>>_--->>>>Pushing & Printing not_take_set_node.. \n");
            not_take_set_node.calculateLowerBound();
            heap.push(not_take_set_node);
            not_take_set_node.printNode();


            ///BRANCH TAKE
            int index = take_set_node.level;
            take_set_node.whichSetsAreUsed_Local[index] = true; ///SINCE WE ARE USING "THIS" SET



            int currentElOfV = take_set_node.current_number_of_UniversalSet_Elements ;
            int tempArray[currentElOfV + 1];

            int previousCountOfV = currentElOfV;

            set<int> :: iterator iter ; int cnt = 0;
            for(iter = take_set_node.currentUniversalSet.begin(); iter != take_set_node.currentUniversalSet.end(); iter++ )
            {
                tempArray[cnt++] = *iter ;
            }
            for(int i=0; i<currentElOfV; i++)
            {
                tempArray[i] = take_set_node.currentUniverse[i];
            }
            printf("<Line 384>PRINTING take_set_node.currentUniverse... =-->> ");
//            printSet(take_set_node.currentUniversalSet);

//            printf("<Line 388>PRINTING take_set_node.currentUniverse array... =-->> ");
            printArray(take_set_node.current_number_of_UniversalSet_Elements, 0, take_set_node.currentUniverse);

            set<int> :: iterator itr ;
            for(itr = arrayOfSets[take_set_node.level].begin(); itr != arrayOfSets[take_set_node.level].end(); itr++)
            {

                bool flag = false ;
//            cout  <<subsets[v.level][i] <<" ";
                for(int j=0; j<=previousCountOfV; j++)
                {

                    if(tempArray[j]== *itr)
                    {

                        flag = true ;
                        //  break;
                    }


                }
                if(flag == false)
                {
                    take_set_node.currentUniverse[currentElOfV]=*itr;
                    currentElOfV++;
                }

            }
            take_set_node.current_number_of_UniversalSet_Elements = currentElOfV ;

            printf("|||>>>Line 342..  take_set_node.current_number_of_UniversalSet_Elements = %d .... printing take_set_node array ////>> ",
                    take_set_node.current_number_of_UniversalSet_Elements);
            printArray(take_set_node.current_number_of_UniversalSet_Elements, 0, take_set_node.currentUniverse);

            take_set_node.calculateLowerBound();

            printf("||}}}>>>>PUSHING take_set_node... printing take_set_node ... \n");
            take_set_node.printNode();
            heap.push(take_set_node);

//            not_take_set_node.whichSetsAreUsed_Local[not_take_set_node.level] = false;
//            printf(">>>>>_--->>>>Pushing & Printing not_take_set_node.. \n");
//            not_take_set_node.calculateLowerBound();
//            heap.push(not_take_set_node);
//            not_take_set_node.printNode();
        }


    }
}

void getArray_Initial(string  firstLine, int *firstLineInteger)
{
    vector<string> firstLineVector = splitString(firstLine, ' ');
    vector<string>:: iterator iter2 = firstLineVector.begin();
    int firstLineCount = 0;
    for(iter2 = firstLineVector.begin(); iter2 != firstLineVector.end(); iter2++)
    {
        stringstream geek(*iter2);
        geek >> firstLineInteger[firstLineCount++];
    }
}
void get_sets(int m, set<int> arrayOfSets[], vector<string> list_string)
{
    int counter = 0 ;
    for(int i=0; i<m; i++)
    {
        cardinalityArray[i] = 0;
    }
    vector<string> :: iterator iter1 = list_string.begin();
    iter1 ++ ;
    for( ; iter1!= list_string.end(); iter1++)
    {
        string thisline = *iter1;
        vector<string> vectorOfThisLine = splitString(thisline, ' ');
        vector<string> :: iterator itr2 = vectorOfThisLine.begin();
        int a ;
        for(itr2 = vectorOfThisLine.begin(); itr2 != vectorOfThisLine.end(); itr2 ++)
        {
            stringstream streamer(*itr2);
            streamer >> a;
            cardinalityArray[counter]++ ;
            arrayOfSets[counter].insert(a);
        }


        counter++;
    }
}
int main()
{
    ifstream fin("InputOffline.txt");
    if(!fin)
    {
        printf("ERROR IN READING FILE. EXITING MAIN.\n");
        return -1;
    }

    vector<string> list_string ;

    for(string line; getline( fin, line ) != 0; )
    {

        list_string.push_back(line);

    }

    vector<string>::iterator iter1;

    iter1 = list_string.begin();
    string firstLine = *iter1;
//    int *first_line_int = returnArray(firstLine);
    int *first_line_int = new int[2];
    getArray_Initial(firstLine, first_line_int);

    n = first_line_int[0];
    m = first_line_int[1];
//    printf("n = %d, m = %d\n", n, m);
    set<int> arrayOfSets[m];
    get_sets(m, arrayOfSets, list_string);
    function_runner(n, m, arrayOfSets);
//    printArrayOfSets(m, arrayOfSets);
}
