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
#include <sstream>
#define prime_number 13
#define dummy "dummy"
using namespace std ;

int bucketSize = 0 ;
int current_scope_number = 1;


std::string numToString ( int  Number )
{
 std::ostringstream ss;
 ss << Number;
 return ss.str();
}
void printArray(char c[], int len)
{
    cout << "LENGTH = " << len << endl ;
    for(int i=0; i<len; i++){
        cout << c[i] << " , " ;
    }
    cout << endl ;
}
class SymbolInfoNode
{
private:
	string symbol_name; //key used for hash table.
	string symbol_type;
	SymbolInfoNode *next;
public:
	//Constructor
	SymbolInfoNode(string name, string type)
	{
		setName(name);
		setType(type);
		next = 0 ;
	}
	SymbolInfoNode()
	{
		setName(dummy);
		setType(dummy);
		next = 0;
	}
	bool isDummy()
	{
	    if(symbol_name == dummy && symbol_type == dummy)
            return true ;
        return false ;
	}
	SymbolInfoNode(string name, string type, SymbolInfoNode *next_pointer)
	{
		setName(name);
		setType(type);
		next = 0 ;
		next = next_pointer ;
	}
	//Destructor
	~SymbolInfoNode()
	{
//	    printf("==-->>Calling Destructor for Node = "); printNode();
		deleteNextNode();
	}
	//Getter and Setter
	void setName(string name)
	{
		symbol_name = name;
	}
	void setType(string type)
	{
		symbol_type = type;
	}
	string getName()
	{
		return symbol_name;
	}
	string getType()
	{
		return symbol_type;
	}
	void deleteNextNode()
	{
		if(next)
			delete next ;
		next = 0;
	}
	void printNode()
	{
		cout << "<" << symbol_name << " : " << symbol_type << ">" << endl ;
	}
	void printForTable()
	{
	    cout << "<" << symbol_name << " : " << symbol_type << ">" <<  "  " ;
	}
	string getPrintForTable()
	{
	    if(symbol_name == dummy)
            return 0 ;
        string s = "<" ;
        s = s + symbol_type;
        s = s + " : ";
        s = s + symbol_name ;
        s = s + ">";
        s = s + " ";
        return s ;
	}
	SymbolInfoNode *makeNewNode(string name, string type)
	{
	    SymbolInfoNode *node = new SymbolInfoNode(name, type);
	    node->next = 0;
        return node ;
	}
	void printLinkedList(SymbolInfoNode *head){
        while(head->next != 0){
            if(head->isDummy() == true){
                head = head->next;
            }
            else{
                head->printForTable();
                printf("   ");
                head = head->next;
            }
        }
	}
    SymbolInfoNode *getNext(){return next;};
    void setNext(SymbolInfoNode *next_ptr)
    {
//        deleteNextNode();
        next = next_ptr;
    }
};
class ScopeTableNode
{
private:
	int numberOfBuckets;
	//Array Of Pointers of SymbolInfoNode i.e. double pointers each single pointed to initially 1.
	SymbolInfoNode **array_of_pointers ;
	int uniqueID ;

	ScopeTableNode *parentScope;
public:
	//Constructor
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
//	    printf("++-->>Calling destructor for id = %d\n", getID());
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
    int hashFunction(string s)
	{
//	    char a[s.size() + 1] ;
	    char *a = new char[s.size() + 1];
        copy(s.begin(), s.end(), a);
//        printArray(a, s.size());
        int tot = 0;
        for(int i=0; i<s.size(); i++)
        {
            tot += a[i]*pow(prime_number, i);
//            printf("tot += a[i]*pow(prime_number, i); =--> %d += %d*pow(%d, %d)\n", tot, a[i], prime_number, i);
        }
        tot = tot%numberOfBuckets ;
//        cout << "tot = " << tot << endl ;
        return tot ;
	}

	SymbolInfoNode *lookup(string s)
	{
        int idx = hashFunction(s);
        SymbolInfoNode *nullNode = 0;
        SymbolInfoNode *head = array_of_pointers[idx];
        int pos = lookup(s, idx);
        if(pos == -1){
//            printf("\tNot Found in ScopeTableNode #%d\n", uniqueID);
            return nullNode ;
        }
        else{
//            printf("\tFound in ScopeTableNode #%d at position %d, %d\n", uniqueID, idx, pos);
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
//            printf("    ");
//            x.printForTable();
//            printf(" already exists in ScopeTableNode # %d at position %d, %d\n", uniqueID, hashFunction(s), flag);
            return false ;
	    }
        int f = insert(s,type,hashFunction(s));
//        printf("    Inserted in ScopeTableNode # %d at position %d, %d\n", uniqueID, hashFunction(s), f);
        return true;
	}
	int insert(string s, string type, int index)
	{
        SymbolInfoNode *ptr = array_of_pointers[index];
        ///get the SymbolInfo pointer of the array of pointers...
        SymbolInfoNode *newNode = ptr->makeNewNode(s, type);
//        printf("Inside insert at idx = %d.. newNode is ", index); newNode->printNode();

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
//            printf("    Cannot delete since "); cout << s ; printf(" doesn't exist.\n");
            return false ;
        }
        else{
//            ptr->printForTable(); printf(" is deleted from ScopeTableNode #%d at position %d, %d\n",
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
//        printf("==--->>>==>>PREV is found , printing prev.."); previous->printNode();
//        printf("Printng priv->next.. "); previous->getNext()->printNode();
        previous->setNext(pointer->getNext());
        pointer->setNext(0);
        if(pointer)
            delete pointer ;
        pointer = 0;

	}
    void printTable()
    {
        cout << endl << endl ;
        cout << "ScopeTable #" << getID() << endl ;
        //printf("Number of Buckets = %d\n", getNumberOfBuckets());
        for(int i=0; i<numberOfBuckets; i++)
        {

            SymbolInfoNode *ptr = array_of_pointers[i]; //0th is a head of linked list or pointers
            cout << i << " :---> " ;


            while (ptr!=0)
            {
                if(ptr->isDummy() == true)
                    ptr = ptr->getNext();
                else{
                    ptr->printForTable();
                    ptr = ptr->getNext();
                }
            }
            cout << endl;
        }
        cout << "------------------*****------------------" << endl ;
    }
    string printNonEmpty()
    {
        string s = "    ";
        s = s + "ScopeTable #" ;
        s = s + numToString(getID());
        s = s + "\n";
        //printf("Number of Buckets = %d\n", getNumberOfBuckets());
        for(int i=0; i<numberOfBuckets; i++)
        {

            SymbolInfoNode *ptr = array_of_pointers[i]; //0th is a head of linked list or pointers
//            cout << i << " :---> " ;
            while (ptr!=0)
            {
                if(ptr->isDummy() == true){
                    ptr = ptr->getNext();
                }
                else{
                    s = s + "    ";
                    s = s + numToString(i);
                    s = s + " :---> " ;
                    s = s + ptr->getPrintForTable();
                    ptr = ptr->getNext();
                    s = s + "\n";
                }
            }
//            s = s + "\n";
//            cout << endl;
        }
        s = s + "\n";
        return s ;
//        s = s + "---------------------******-----------------\n";
//        cout << "------------------*****------------------" << endl ;
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
};

class SymbolTable
{
private:
    int number_of_buckets_for_all;
    int counter_scopeTable ;
//    ScopeTableNode *initialScopeTableNode ; ///same as head
    ScopeTableNode *currentScopeTable ; ///This pointer changes with new scope or exit scope

public:
    SymbolTable(int n)
    {
        number_of_buckets_for_all = n ;
        currentScopeTable = 0 ;
        counter_scopeTable = 1 ;
    }
    ~SymbolTable()
    {
        if(currentScopeTable)
            delete currentScopeTable ;
        currentScopeTable = 0 ;
    }
    void enterScope()
    {
        ScopeTableNode *tab = new ScopeTableNode(number_of_buckets_for_all, counter_scopeTable);
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
        cout << "New Scope Table with id = " << currentScopeTable->getID() << " is created.\n" ;
//        printf("New Scope Table with id = %d is created.\n", currentScopeTable->getID());
    }
    void exitScope()
    {
        ///Removes current scope table
        ScopeTableNode *curr = currentScopeTable ;
        ///If empty
        if(curr == 0){
            cout << "\tSymbol Table is empty.\n";
            return ;
        }
        ///This is the head
        else if(curr->getParent() == 0){
            cout << "\tScope Table with id = " << curr->getID() <<  " is removed.\n" ;
            curr->setParent(0);
            if(curr)
                delete curr ;
            curr = 0 ;
            currentScopeTable = 0 ;
            counter_scopeTable--;
//            printf("RETURNING FROM <ELSE-IF> in EXIT_SCOPE FUNC\n");
            return ;
        }
        currentScopeTable = curr->getParent() ;

        cout << "\tScope Table with id = " << curr->getID() <<  " is removed.\n";

//        printf("\tScope Table with id = %d is removed.\n", curr->getID());
        curr->setParent(0);
        if(curr)
            delete curr ;
        curr = 0;
        counter_scopeTable-- ;

        return ;
    }
    bool Insert(string symbol_name, string symbol_type)
    {
        if(currentScopeTable == 0){
            cout << "Can't Insert in Empty Symbol Table\n";
//            printf("Can't Insert since Symbol Table is empty\n");
            return false ;
        }

        SymbolInfoNode *node = currentScopeTable->lookup(symbol_name);
        if(node != 0){
            node->printForTable() ;cout << " already exists in the current ScopeTable\n" ;
            return false ;
        }
        bool flag = currentScopeTable->insert(symbol_name, symbol_type);
        int pos = currentScopeTable->lookup(symbol_name, currentScopeTable->hashFunction(symbol_name));

        cout << "\tInserted in ScopeTable #" << currentScopeTable->getID() << " at position " <<
                currentScopeTable->hashFunction(symbol_name) << " , " << pos << endl ;
//        printf("\tInserted in ScopeTable #%d at position %d, %d\n",
//               currentScopeTable->getID(), currentScopeTable->hashFunction(symbol_name), pos);
        return true ;
    }
    bool Remove(string symbolName)
    {
        if(currentScopeTable == 0){
            cout << "SymbolTable is empty." << endl ;
            return false ;
        }
        int pos = currentScopeTable->lookup(symbolName, currentScopeTable->hashFunction(symbolName));
        SymbolInfoNode *node = currentScopeTable->lookup(symbolName);
        if(node == 0){
            cout << "\tNot present in SymbolTable" << endl ;
            return false ;
        }
        cout << "\t" << symbolName << " " ;
        cout << "is present in ScopeTable #" << currentScopeTable->getID() << " at position " << currentScopeTable->hashFunction(symbolName)
             << " , " << pos << endl ;
//        printf("is present in ScopeTable #%d at position %d, %d\n", currentScopeTable->getID(),
//               currentScopeTable->hashFunction(symbolName), pos);
        ///Function call..
        currentScopeTable->deleteFromTable(symbolName);

//        printf("Deleted entry at %d, %d from current ScopeTable\n", currentScopeTable->hashFunction(symbolName), pos);
        cout << "Deleted entry at " << currentScopeTable->hashFunction(symbolName) << ", " << pos << " from current ScopeTable\n";
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
//                printf("Found at ScopeTable %d")
                int pos = itr->lookup(name, itr->hashFunction(name));
//                printf("\tFound in ScopeTable %d at position %d, %d\n", itr->getID(), itr->hashFunction(name), pos);
                cout << "\tFound in ScopeTable " << itr->getID() << " at position " << itr->hashFunction(name) << " , " << pos << endl ;
                return node ;
            }
            itr = itr->getParent();
        }
//        printf("\tDoes not exist in the Symbol Table\n");
        cout << "\tDoes not exist in the Symbol Table\n" << endl ;
        return 0 ;
    }
    void printCurrent()
    {
        if(currentScopeTable == 0){
//            printf("Empty SymbolTable\n");
            cout << "Empty SymbolTable" << endl ;
            return ;
        }
        currentScopeTable ->printTable();
    }
    void printAll()
    {
        if(currentScopeTable == 0){
//            printf("Empty SymbolTable\n");
            cout << "Empty SymbolTable" << endl ;
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
};

/// ------------------*****------------------------------------*****------------------
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
//

void printArray(string arr[], int n)
{
    cout << endl ;
    for(int i=0; i<n; i++)
    {
        cout << arr[i] << " " ;
    }
    cout << endl << endl  ;
}
void printArray(string s)
{
    cout << endl ;
    cout << s << endl << endl  ;
}
