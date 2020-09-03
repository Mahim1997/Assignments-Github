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

 
#include "1505022_ScopeTable.h"
using namespace std ;

//int bucketSize = 0 ;
//int current_scope_number = 1;


class SymbolTable
{
private:
    int number_of_buckets_for_all;
    int counter_scopeTable ;
//    ScopeTableNode *initialScopeTableNode ; ///same as head
    ScopeTableNode *currentScopeTable ; ///This pointer changes with new scope or exit scope

public:
    int global_scope = 1;
    int getCurrentScopeID(){
        return currentScopeTable->getID();
    }
    SymbolTable(int n)
    {
        number_of_buckets_for_all = n ;    //Start with 50 buckets
        currentScopeTable = 0 ;
        counter_scopeTable = 1 ;
    }
    ~SymbolTable()
    {
        if(currentScopeTable)
            delete currentScopeTable ;
        currentScopeTable = 0 ;
    }
    string enterScope()
    {
//        ScopeTableNode *tab = new ScopeTableNode(number_of_buckets_for_all, counter_scopeTable);
        ScopeTableNode *tab = new ScopeTableNode(number_of_buckets_for_all, global_scope);
        global_scope++ ;
        if(currentScopeTable == 0){
            /// or we couldve used if(counter == 0)
            //then initialize the initialScopeTable
            currentScopeTable = tab ;
            currentScopeTable->setParent(0);
        }
        else{
            ///create a new scope table and insert parent pointer accordingly
            tab->setParent(currentScopeTable) ;
            currentScopeTable = tab ;
        }
        counter_scopeTable ++ ;
        ////cout << "New Scope Table with id = " << currentScopeTable->getID() << " is created.\n" ;
        
        string s = "\n\tNew Scope Table with id = " ;
        s += numToString(currentScopeTable->getID());
        s += " is created. \n";
        
        return s ;
//        //printf("New Scope Table with id = %d is created.\n", currentScopeTable->getID());
    }
    string exitScope()
    {
        ///Removes current scope table

        ScopeTableNode *curr = currentScopeTable ;


        ///If empty
        if(curr == 0){
            //cout << "\tSymbol Table is empty.\n";
            return "\tEmpty Symbol Table\n";
        }
        ///This is the head
        else if(curr->getParent() == 0){
            
            ////cout << "\tScope Table with id = " << curr->getID() <<  " is removed.\n" ;
            curr->setParent(0);
            int currentScopeID = curr->getID();
            if(curr)
                delete curr ;
            //curr = 0 ;
            currentScopeTable = 0 ;

            //---**EDITED HERE**---- nicher ta commentout chilo nah
            counter_scopeTable--;

            string s = "\n\tScope Table with id = " ;
            s+= numToString(currentScopeID) ;
            s += " is removed.\n";
//            //printf("RETURNING FROM <ELSE-IF> in EXIT_SCOPE FUNC\n");
            return s;
        }
        //NORMAL CASE .. 

        currentScopeTable = curr->getParent() ;

       

//        //cout << "\tScope Table with id = " << curr->getID() <<  " is removed.\n";

//        //printf("\tScope Table with id = %d is removed.\n", curr->getID());
        int c_id = curr->getID(); 
        curr->setParent(0);
        if(curr)
            delete curr ;
        curr = 0;

        counter_scopeTable-- ;

        string s = "\n\tScope Table with id = " ;
        s+= numToString(c_id) ;
        s += " is removed.\n";

        return s;
    }
    std::string numToString ( int  Number )
    {
     std::ostringstream ss;
     ss << Number;
     return ss.str();
    }
    SymbolInfoNode *searchCurrent(SymbolInfoNode *n)
    {
        SymbolInfoNode *node = 0 ;
        ScopeTableNode *itr = currentScopeTable ;
        string name = n->getName();
        if(name == "")
            return 0 ;
        node = itr->lookup(name);
        int pos = itr->lookup(name, itr->hashFunction(name));
        return node ;
    }

    bool Insert(SymbolInfoNode *n)
    {
        printf("SymbolTable.INSERT is BEGINNNING!!!\n");
        if(currentScopeTable == 0){
            //cout << "Can't Insert in Empty Symbol Table\n";
//            //printf("Can't Insert since Symbol Table is empty\n");
            return false ;
        }
        if(!n){
            printf("Line 154.... n is not present !!");
            return;
        }
        //SymbolInfoNode *node = this->LookUp(n->getName());
        if(!n){
            printf("Inside SymbolTable.INSERT ... parameter er node doesnot exist! \n");
            return false ;
        }
        SymbolInfoNode *node = currentScopeTable->lookup(n->getName());
        if(node != 0){
            node->printForTable() ;//cout << " already exists in the current ScopeTable\n" ;
            return false ;
        }
        //bool flag = currentScopeTable->insert(symbol_name, symbol_type);

        SymbolInfoNode *newNode = new SymbolInfoNode(n->getName(), n->getType());
        newNode->dataType = n->dataType;
        newNode->actualValue = n->actualValue;
        newNode->isArray = n->isArray;
        newNode->isFunction = n->isFunction;
        newNode->symbol = n->symbol;
        newNode->code = n->code ;

        //newNode->functionItems.list.size() = n->functionItems.list.size();
        newNode->line_number_begin = n->line_number_begin;
        newNode->line_number_end = n->line_number_end;

        if(newNode->isArray && n->isArray){
            newNode->arrayItems.changeArray(n->arrayItems);
        }

        if(newNode->isFunction && n->isFunction){
            newNode->functionItems.changeFunction(n->functionItems);
        }
        printf("SymbolTable.INSERT is MIDDLE!!!\n");

        currentScopeTable->insert(newNode, currentScopeTable->hashFunction(newNode->getName()));
        string symbol_name = newNode->getName();
        int pos = currentScopeTable->lookup(symbol_name, currentScopeTable->hashFunction(symbol_name));

        //cout << "\tInserted in ScopeTable #" << currentScopeTable->getID() << " at position " <<
                //currentScopeTable->hashFunction(symbol_name) << " , " << pos << endl ;

//        //printf("\tInserted in ScopeTable #%d at position %d, %d\n",
//               currentScopeTable->getID(), currentScopeTable->hashFunction(symbol_name), pos);
        printf("SymbolTable.INSERT is DONE!!!\n");
        return true ;
    }
    bool Insert(string symbol_name, string symbol_type, string dataType, int lineBeg, bool isArray, bool isFunction, 
        string actualValue, FunctionItems f, ArrayItems a)
    {
        if(currentScopeTable == 0){
            //cout << "Can't Insert in Empty Symbol Table\n";
//            //printf("Can't Insert since Symbol Table is empty\n");
            return false ;
        }

        SymbolInfoNode *node = currentScopeTable->lookup(symbol_name);
        if(node != 0){
            node->printForTable() ;//cout << " already exists in the current ScopeTable\n" ;
            return false ;
        }
        bool flag = currentScopeTable->insert(symbol_name, symbol_type, dataType, lineBeg, isArray, isFunction, 
            actualValue, f, a);
        int pos = currentScopeTable->lookup(symbol_name, currentScopeTable->hashFunction(symbol_name));

        ////cout << "\tInserted in ScopeTable #" << currentScopeTable->getID() << " at position " <<
                //currentScopeTable->hashFunction(symbol_name) << " , " << pos << endl ;

//        //printf("\tInserted in ScopeTable #%d at position %d, %d\n",
//               currentScopeTable->getID(), currentScopeTable->hashFunction(symbol_name), pos);
        return true ;
    }
    bool Insert(string symbol_name, string symbol_type)
    {
        if(currentScopeTable == 0){
            //cout << "Can't Insert in Empty Symbol Table\n";
//            //printf("Can't Insert since Symbol Table is empty\n");
            return false ;
        }

        SymbolInfoNode *node = currentScopeTable->lookup(symbol_name);
        if(node != 0){
            node->printForTable() ;//cout << " already exists in the current ScopeTable\n" ;
            return false ;
        }
        bool flag = currentScopeTable->insert(symbol_name, symbol_type);
        int pos = currentScopeTable->lookup(symbol_name, currentScopeTable->hashFunction(symbol_name));

        ////cout << "\tInserted in ScopeTable #" << currentScopeTable->getID() << " at position " <<
                //currentScopeTable->hashFunction(symbol_name) << " , " << pos << endl ;

//        //printf("\tInserted in ScopeTable #%d at position %d, %d\n",
//               currentScopeTable->getID(), currentScopeTable->hashFunction(symbol_name), pos);
        return true ;
    }
    bool Remove(string symbolName)
    {
        if(currentScopeTable == 0){
            //cout << "SymbolTable is empty." << endl ;
            return false ;
        }
        int pos = currentScopeTable->lookup(symbolName, currentScopeTable->hashFunction(symbolName));
        SymbolInfoNode *node = currentScopeTable->lookup(symbolName);
        if(node == 0){
            //cout << "\tNot present in SymbolTable" << endl ;
            return false ;
        }
        //cout << "\t" << symbolName << " " ;
        //cout << "is present in ScopeTable #" << currentScopeTable->getID() << " at position " << currentScopeTable->hashFunction(symbolName)
             //<< " , " << pos << endl ;
//        //printf("is present in ScopeTable #%d at position %d, %d\n", currentScopeTable->getID(),
//               currentScopeTable->hashFunction(symbolName), pos);
        ///Function call..
        //currentScopeTable->deleteFromTable(symbolName);

//        //printf("Deleted entry at %d, %d from current ScopeTable\n", currentScopeTable->hashFunction(symbolName), pos);
        //cout << "Deleted entry at " << currentScopeTable->hashFunction(symbolName) << ", " << pos << " from current ScopeTable\n";
        return true ;
    }

    SymbolInfoNode *LookUp(string name)
    {
        SymbolInfoNode *node = 0 ;
        ScopeTableNode *itr = currentScopeTable ;
        while(itr  != 0)
        {
            node = itr->lookup(name);
            if(node != 0){
//                //printf("Found at ScopeTable %d")
                int pos = itr->lookup(name, itr->hashFunction(name));
//                //printf("\tFound in ScopeTable %d at position %d, %d\n", itr->getID(), itr->hashFunction(name), pos);
                //cout << "\tFound in ScopeTable " << itr->getID() << " at position " << itr->hashFunction(name) << " , " << pos << endl ;
                return node ;
            }
            itr = itr->getParent();
        }
//        //printf("\tDoes not exist in the Symbol Table\n");
        //cout << "\tDoes not exist in the Symbol Table\n" << endl ;
        return 0 ;
    }
    void printCurrent()
    {
        if(currentScopeTable == 0){
//            //printf("Empty SymbolTable\n");
            //cout << "Empty SymbolTable" << endl ;
            return ;
        }
        currentScopeTable ->printTable();
    }
    void printAll()
    {
        if(currentScopeTable == 0){
//            //printf("Empty SymbolTable\n");
            //cout << "Empty SymbolTable" << endl ;
            return ;
        }
        ScopeTableNode *itr = currentScopeTable;
        while(itr != 0){
            itr->printTable();
            itr = itr->getParent();
        }
    }
    string printCurrentNonEmpty()
    {
        string s = "";
        if(currentScopeTable == 0){
            s = "Empty SymbolTable\n";
        }
        else{
            s = currentScopeTable->printNonEmpty();
        }
        return s ;
    }
    string printEverything(){
        string s = "";
        if(currentScopeTable == 0){
            s = "Empty SymbolTable\n";
        }
        else{
            s = currentScopeTable->printEverything_2();
        }
        return s ;
    }
    string printAllNonEmpty()
    {
        string s = "" ;
        if(currentScopeTable == 0){
            s = "Empty SymbolTable\n";
            return s ;
        }
        ScopeTableNode *itr = currentScopeTable;
        while(itr != 0){
            s += itr->printNonEmpty();
            itr = itr->getParent();
        }
        return s ;
    }
    string getAllNonEmpty(){
        string s = "" ;
        if(currentScopeTable == 0){
            s = "Empty SymbolTable\n";
            return s ;
        }
        ScopeTableNode *itr = currentScopeTable;
        while(itr != 0){
            s += itr->getPrintNonEmpty();
            itr = itr->getParent();
        }
        return s ;
    }
};

/// ------------------*****------------------------------------*****------------------
/*
vector<string> splitString(const string& a, char delim)
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

void printArray(string arr[], int n)
{
    //cout << endl ;
    for(int i=0; i<n; i++)
    {
        //cout << arr[i] << " " ;
    }
    //cout << endl << endl  ;
}

void printArray(string s)
{
    //cout << endl ;
    //cout << s << endl << endl  ;
}
void printArray(char c[], int len)
{
    //cout << "LENGTH = " << len << endl ;
    for(int i=0; i<len; i++){
        //cout << c[i] << " , " ;
    }
    //cout << endl ;
}
*/