
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
#include<cstring>

#include "1505022_SymbolInfo.h"

using namespace std ;

class ScopeTableNode
{
private:
	int numberOfBuckets;
	//Array Of Pointers of SymbolInfoNode i.e. double pointers each single pointed to initially 1.
	SymbolInfoNode **array_of_pointers ;
	int uniqueID ;

	ScopeTableNode *parentScope;
public:
    static int global_scope ;
	//Constructor
    std::string numToStringInST ( int  Number )
    {
     std::ostringstream ss;
     ss << Number;
     return ss.str();
    }
	ScopeTableNode()
	{
	    array_of_pointers = 0;
	    parentScope = 0;
	}
	ScopeTableNode(int n)
	{
		numberOfBuckets = n;
		uniqueID = 0;
//		current_scope_number++ ;
		initialiseArray(n);
		parentScope = 0;
	}
	ScopeTableNode(int n, int id)
	{
		numberOfBuckets = n;
		uniqueID = id;
 
//		current_scope_number = id + 1 ;
		initialiseArray(n);
		parentScope = 0;
	}
	//Destructor
	~ScopeTableNode()
	{
//	    //printf("++-->>Calling destructor for id = %d\n", getID());
		if(array_of_pointers)
			delete [] array_of_pointers ;
		array_of_pointers = 0 ;
		if(parentScope)
            delete  parentScope ;

        parentScope = 0;
	}
	//Functions
    int getNumberOfBuckets(){return numberOfBuckets;}
    void initialiseArray(int n)
	{
		array_of_pointers = new SymbolInfoNode*[n];
		for(int i=0; i<n; i++){
            array_of_pointers[i] = new SymbolInfoNode(); /// dummy nodes.
		}
	}

    int getID(){return uniqueID;}

    int hash2(string s){
	 
//	    char a[s.size() + 1] ;
	    char *a = new char[s.size() + 1];
        copy(s.begin(), s.end(), a);
        
   

        long long int  tot = 0;
         
        for(int i=0; i<s.size(); i++)
        {
            tot += a[i]*pow(prime_number, i);
             
            tot = tot%numberOfBuckets ;
//            //printf("tot += a[i]*pow(prime_number, i); =--> %d += %d*pow(%d, %d)\n", tot, a[i], prime_number, i);
        }
        int final = (int) ( tot%numberOfBuckets ) ;
         
        return final ;
    }

    int hashFunction(string s)
	{
		return hash2(s);
//	    char a[s.size() + 1] ;
	    char *a = new char[s.size() + 1];
        copy(s.begin(), s.end(), a);
//        printArray(a, s.size());
        int tot = 0;
        for(int i=0; i<s.size(); i++)
        {
            tot += a[i]*pow(prime_number, i);
//            //printf("tot += a[i]*pow(prime_number, i); =--> %d += %d*pow(%d, %d)\n", tot, a[i], prime_number, i);
        }
        tot = tot%numberOfBuckets ;
//        //cout << "tot = " << tot << endl ;
        return tot ;
	}

	SymbolInfoNode *lookup(string s)
	{
        int idx = hashFunction(s);
        SymbolInfoNode *nullNode = 0;
        SymbolInfoNode *head = array_of_pointers[idx];
        int pos = lookup(s, idx);
        if(pos == -1){
            //printf("\tNot Found in ScopeTableNode #%d\n", uniqueID);
            return nullNode ;
        }
        else{
            //printf("\tFound in ScopeTableNode #%d at position %d, %d\n", uniqueID, idx, pos);
            while(head != 0){
                if(head->getName() == s)
                    return head ;
                head = head -> getNext();
            }
        }

        return 0 ;
	}
    int lookup(string s, int index)
    {
        SymbolInfoNode *head = array_of_pointers[index];
        ///Then Search head of that linked list.
        int cnt = -1;
        while(head != 0)
        {
            if(head->getName() == s)
                return cnt ;
            else{
                head = head->getNext();
                cnt++;
            }
        }
        return -1 ;
    }
	bool insert(string s, string type )
	{
	    int flag = lookup(s, hashFunction(s));
	    if(flag != -1)///already exists{
        {
            SymbolInfoNode x(s, type);
            //printf("    ");
            x.printForTable();
            //printf(" already exists in ScopeTableNode # %d at position %d, %d\n", uniqueID, hashFunction(s), flag);
            return false ;
	    }
        int f = insert(s,type,hashFunction(s));
        //printf("    Inserted in ScopeTableNode # %d at position %d, %d\n", uniqueID, hashFunction(s), f);
        return true;
	}


    
    //LATEST SHITASS
    bool insert(string symbol_name, string symbol_type, string dataType, int lineBeg, bool isArray, bool isFunction, 
        string actualValue, FunctionItems f, ArrayItems a)
    {
        int flag = lookup(symbol_name, hashFunction(symbol_name));
        if(flag != -1)///already exists{
        {
            SymbolInfoNode x(symbol_name, symbol_type);
            //printf("    ");
            x.printForTable();
            //printf(" already exists in ScopeTableNode # %d at position %d, %d\n", uniqueID, hashFunction(symbol_name), flag);
            return false ;
        }
        int index = hashFunction(symbol_name);
        SymbolInfoNode *ptr = array_of_pointers[index];
        SymbolInfoNode *newNode = ptr->makeNewNode(symbol_name, symbol_type);
        newNode->dataType = dataType; newNode->setLineBegin(lineBeg); newNode->isArray = isArray; newNode->isFunction = isFunction;
        newNode->actualValue = actualValue;
        if(newNode->isFunction == true){
            newNode->functionItems.changeFunction(f);
        }
        if(newNode->isArray == true){
            newNode->arrayItems.changeArray(a);
        }

        int cnt = 0;
        while(ptr->getNext() != 0){
            ptr = ptr->getNext();
            cnt++ ;
        }
        ///Go to the very end ..
        ptr->setNext(newNode);
        newNode->setNext(0);

        return cnt ;
    }





	int insert(string s, string type, int index)
	{
        SymbolInfoNode *ptr = array_of_pointers[index];
        ///get the SymbolInfo pointer of the array of pointers...
        SymbolInfoNode *newNode = ptr->makeNewNode(s, type);
//        //printf("Inside insert at idx = %d.. newNode is ", index); newNode->printNode();

        int cnt = 0;
        while(ptr->getNext() != 0){
            ptr = ptr->getNext();
            cnt++ ;
        }
        ///Go to the very end ..
        ptr->setNext(newNode);
        newNode->setNext(0);

        return cnt ;
	}
    int insert(SymbolInfoNode *newNode, int index)
    {
        SymbolInfoNode *ptr = array_of_pointers[index];
        ///get the SymbolInfo pointer of the array of pointers...
        //SymbolInfoNode *newNode = ptr->makeNewNode(s, type);
//        //printf("Inside insert at idx = %d.. newNode is ", index); newNode->printNode();

        int cnt = 0;
        while(ptr->getNext() != 0){
            ptr = ptr->getNext();
            cnt++ ;
        }
        ///Go to the very end ..
        ptr->setNext(newNode);
        newNode->setNext(0);

        return cnt ;
    }
	bool deleteFromTable(string s)
	{
        int firstIdx = hashFunction(s);
        int secondIdx = lookup(s, firstIdx);
        SymbolInfoNode *ptr = lookup(s);
        if(ptr == 0){
//            //printf("    Cannot delete since "); //cout << s ; //printf(" doesn't exist.\n");
            return false ;
        }
        else{
//            ptr->printForTable(); //printf(" is deleted from ScopeTableNode #%d at position %d, %d\n",
//                                         uniqueID, firstIdx, secondIdx);
            actuallyDelete(ptr, firstIdx);
            return true ;
        }
	}
	void actuallyDelete(SymbolInfoNode *pointer, int firstIdx)
	{
        SymbolInfoNode *previous = array_of_pointers[firstIdx];
        while(previous->getNext() != pointer)
            previous = previous->getNext();
//        //printf("==--->>>==>>PREV is found , printing prev.."); previous->printNode();
//        //printf("Printng priv->next.. "); previous->getNext()->printNode();
        previous->setNext(pointer->getNext());
        pointer->setNext(0);
        if(pointer)
            delete pointer ;
        pointer = 0;

	}
    void printTable()
    {
        //cout << endl << endl ;
        //cout << "ScopeTable #" << getID() << endl ;
        ////printf("Number of Buckets = %d\n", getNumberOfBuckets());
        for(int i=0; i<numberOfBuckets; i++)
        {

            SymbolInfoNode *ptr = array_of_pointers[i]; //0th is a head of linked list or pointers
            //cout << i << " :---> " ;


            while (ptr!=0)
            {
                if(ptr->isDummy() == true)
                    ptr = ptr->getNext();
                else{
                    ptr->printForTable();
                    ptr = ptr->getNext();
                }
            }
            //cout << endl;
        }
        //cout << "------------------*****------------------" << endl ;
    }
    string printEverything_2(){
        string s = "    ";
        s = s + "ScopeTable #" ;
        s = s + this->numToStringInST(getID());
        s = s + "\n";
        ////printf("Number of Buckets = %d\n", getNumberOfBuckets());
        for(int i=0; i<numberOfBuckets; i++)
        {

            SymbolInfoNode *ptr = array_of_pointers[i]; //0th is a head of linked list or pointers
//            //cout << i << " :---> " ;
            while (ptr!=0)
            {
                if(ptr->isDummy() == true){
                    ptr = ptr->getNext();
                }
                else{
                    s = s + "    ";
                    s = s + this->numToStringInST(i);
                    s = s + " :---> " ;
                    s = s + ptr->getEverything_3();
                    ptr = ptr->getNext();
                    s = s + "\n";
                }
            }
//            s = s + "\n";
//            //cout << endl;
        }
        s = s + "\n";
        return s ;
    }
    string printNonEmpty()
    {
        string s = "    ";
        s = s + "ScopeTable #" ;
        s = s + this->numToStringInST(getID());
        s = s + "\n";
        ////printf("Number of Buckets = %d\n", getNumberOfBuckets());
        for(int i=0; i<numberOfBuckets; i++)
        {

            SymbolInfoNode *ptr = array_of_pointers[i]; //0th is a head of linked list or pointers
//            //cout << i << " :---> " ;
            while (ptr!=0)
            {
                if(ptr->isDummy() == true){
                    ptr = ptr->getNext();
                }
                else{
                    s = s + "    ";
                    s = s + this->numToStringInST(i);
                    s = s + " :---> " ;
                    s = s + ptr->getPrintForTable();
                    ptr = ptr->getNext();
                    s = s + "\n";
                }
            }
//            s = s + "\n";
//            //cout << endl;
        }
        s = s + "\n";
        return s ;
//        s = s + "---------------------******-----------------\n";
//        //cout << "------------------*****------------------" << endl ;
    }
    void setParent(ScopeTableNode *parent)
    {
//        if(parentScope)
//            delete parentScope ;
        parentScope = parent ;
    }
    ScopeTableNode *getParent()
    {
        return parentScope ;
    }


    string getPrintNonEmpty()
    {
        string s = "    ";
        s = s + "ScopeTable #" ;
        s = s + this->numToStringInST(getID());
        s = s + "\n";
        ////printf("Number of Buckets = %d\n", getNumberOfBuckets());
        for(int i=0; i<numberOfBuckets; i++)
        {

            SymbolInfoNode *ptr = array_of_pointers[i]; //0th is a head of linked list or pointers
//            //cout << i << " :---> " ;
            while (ptr!=0)
            {
                if(ptr->isDummy() == true){
                    ptr = ptr->getNext();
                }
                else{
                    s = s + "    ";
                    s = s + this->numToStringInST(i);
                    s = s + " :---> " ;
                    s = s + ptr->getForTabFinally();
                    ptr = ptr->getNext();
                    s = s + "\n";
                }
            }
//            s = s + "\n";
//            //cout << endl;
        }
        s = s + "\n";
        return s ;
//        s = s + "---------------------******-----------------\n";
//        //cout << "------------------*****------------------" << endl ;
    }
};
 