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


#include "../Miscalenous/1505022_Items.h"
#define prime_number 13
#define dummy "dummy"

using namespace std ;

class SymbolInfoNode
{
public:
    string symbol_name; //key used for hash table.
    string symbol_type;
    SymbolInfoNode *next;
    int line_number_begin = -1; 
    int line_number_end = -1;


    string code;
    string symbol;
    
    ArrayItems arrayItems ;
    FunctionItems functionItems ;
    bool isFunction = false;
    bool isArray = false;
    string dataType = "NIL"; // HOW TO GET ??? 
    string faceValue = "UNDEFINED";
    string actualValue = "NOT_GIVEN_YET";




    std::string getString ( int  Number )
    {
     std::ostringstream ss;
     ss << Number;
     return ss.str();
    }
    std::string getString ( bool  Number )
    {
     std::ostringstream ss;
     ss << Number;
     return ss.str();
    }

   // Overloading of Assignment Operator
    void operator=(const SymbolInfoNode &node) { 
        printf("Inside operator[1] = \n");
    }
    void operator =(const SymbolInfoNode *node){
        printf("Inside operator = \n");
    }


    //Constructor
    SymbolInfoNode(string n, string t, int line, string s, string c, bool isA, bool isF, string d){
        setName(n);
        setType(t);
        this->line_number_begin = line ;
        this->line_number_end = line ;
        setDataType(d);
        next = 0 ;
        this->isArray = isA;
        this->isFunction = isF;
        this->code = c;
        this->symbol = s ; 
    }
    SymbolInfoNode(string name, string type, int line , string data)
    {
        setName(name);
        setType(type);
        this->line_number_begin = line ;
        this->line_number_end = line ;
        setDataType(data);
        next = 0 ;
        isArray = false;
        isFunction = false;
        

    } 
    SymbolInfoNode(string name, string type, int line )
    {
        setName(name);
        setType(type);
        this->line_number_begin = line ;
        this->line_number_end = line ;
        next = 0 ;
        isArray = false;
        isFunction = false;
    } 
    SymbolInfoNode(string name, string type, int line, int line2 )
    {
        setName(name);
        setType(type);
        this->line_number_begin = line ;
        this->line_number_end = line2 ;
        next = 0 ;
        isArray = false;
        isFunction = false;
    }   
    SymbolInfoNode(string name, string type)
    {
        setName(name);
        setType(type);
        next = 0 ;
        isArray = false;
        isFunction = false;
    }
    SymbolInfoNode()
    {
        setName(dummy);
        setType(dummy);
        next = 0;
        isArray = false;
        isFunction = false;
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
        isArray = false;
        isFunction = false;
    }
    //Destructor
    ~SymbolInfoNode()
    {
//      //printf("==-->>Calling Destructor for Node = "); printNode();
        deleteNextNode();
    }
    //Getter and Setter
    void setName(string name)
    {
        symbol_name = name;
        symbol = "";
        code = "";
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
    void setLineBegin(int line)
    {
        this->line_number_begin = line ;
    }
    int getLineBegin()
    {
        return this->line_number_begin ;
    }
    void setDataType(string s)
    {
        this->dataType = s ;
    }
    void setDataType(SymbolInfoNode *node){
        this->dataType = node->getDataType();
    }
    string getDataType()
    {
        return this->dataType ;
    }
    void setLineEnd(int line)
    {
        line_number_end = line ;
    }
    int getLineEnd(){
        return line_number_end;
    }
    //GETTER AND SETTER DONE 


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
    void printNodeB()
    {
        cout << "<" << symbol_name << " ::: " << symbol_type << ">" << " , line begin = " << line_number_begin << 
        " , line end = " << line_number_end << " , dataType = " << dataType << endl ;
    }
    void printForTable()
    {
        cout << "<" << symbol_name << " : " << symbol_type << ">" <<  "  " ;
    }
    //Comparison Operator Overload.
    bool equals(SymbolInfoNode *node1){
        //printf("======+<<<<????<<<< INSIDE Comparison Operator >>>>>>?????>>>>>++=========\n");
        //printf("PRINTING THIS NODE ... \n");
        cout << this->getPrintForTable();
        //printf("PRINTING node1...\n");
        cout << node1->getPrintForTable();
        if(symbol_name != node1->symbol_name){
            //printf("symbol_name != node1.symbol_name\n");
            return false;
        }
        if(symbol_type != node1->symbol_type){
            //printf("symbol_type != node1.symbol_type\n");            
            return false;
        }
        if(dataType != node1->dataType){
            //printf("dataType != node1.dataType\n");
            return false;
        }
        if(false){
            ///ACTUAL VALUE NOT CHECKING...
            //printf("actualValue != node1.actualValue\n");
            return false;
        }
        if(isArray != node1->isArray){
            //printf("isArray != node1.isArray\n");
            return false;
        }
        if(isFunction != node1->isFunction){
            //printf("isFunction != node1.isFunction\n");
            return false;
        }
        if(functionItems.list.size() != node1->functionItems.list.size()){
            //printf("functionItems.list.size() donet match\n");
            return false;
        }
        if(line_number_begin != node1->line_number_begin){
            //printf("line_number_begin dont match");
            return false;
        }
        if(line_number_end != node1->line_number_end){
            //printf("line_number_end dont match \n");
            return false ;
        }
        
        FunctionItems f1 = this->functionItems;
        FunctionItems f2 = node1->functionItems;
        if(f1.equals(f2) != "Matched"){
            //printf("FunctionItems dont match\n");
            return false;
        }
        if(arrayItems.equals(node1->arrayItems) == false){
            //printf("ArrayItems dont match\n");
            return false;
        }
        return true;
    }
    void setSymbol(SymbolInfoNode *node2)
    {
        this->symbol_name = node2->getName();
        this->symbol_type = node2->getType();
        this->line_number_begin = node2->getLineBegin();
        this->line_number_end = node2->getLineEnd();
        this->dataType = node2->dataType ;

        this->symbol = node2->symbol;
        this->code = node2->code; 
    }
    void setUnion(string s)
    {
        this->symbol_name += s ;
    }
    void setUnion(SymbolInfoNode *node1)
    {
        this->symbol_name += node1->getName();
        this->line_number_begin = max(this->line_number_begin, node1->getLineBegin());
        this->line_number_end = max(this->line_number_end, node1->getLineEnd()); 
        this->dataType = node1->dataType ;       
        this->actualValue = node1->actualValue;
        this->code += node1->code ;
    }
    void setSymbol(SymbolInfoNode *node1, SymbolInfoNode *node2)
    {
        this->symbol_name = node1->getName();
        this->line_number_begin = node1->getLineBegin();
        this->line_number_end = node1->getLineEnd();
        this->symbol = node1->symbol;
        this->code = node1->code;

        this->code += node2->code ;
        this->symbol_name += " ";
        this->symbol_name += node2->getName();
        this->line_number_begin = max(this->line_number_begin, node2->getLineBegin());
        this->line_number_end = max(this->line_number_end, node2->getLineEnd());
    }
    void setSymbol(SymbolInfoNode *node1, SymbolInfoNode *node2, SymbolInfoNode *node3)
    {
        this->symbol_name = node1->getName();
        this->line_number_begin = node1->getLineBegin();
        this->line_number_end = node1->getLineEnd();
        
        this->symbol = node1->symbol;
        this->code = node1->code;
        this->code += node2->code ;
        this->code += node3->code; 


        this->symbol_name += " ";
        this->symbol_name += node2->getName();
        this->line_number_begin = max(this->line_number_begin, node2->getLineBegin());
        this->line_number_end = max(this->line_number_end, node2->getLineEnd());

        this->symbol_name += " ";
        this->symbol_name += node3->getName();
        this->line_number_begin = max(this->line_number_begin, node3->getLineBegin());
        this->line_number_end = max(this->line_number_end, node3->getLineEnd());
    }
    void setSymbol(vector<SymbolInfoNode *> list)
    {
        this->symbol_name = "" ;
        this->line_number_end = 0; this->line_number_begin = 0; 
        for(int i=0; i<list.size(); i++)
        {
            this->symbol_name += " ";
            this->symbol_name += list[i]->getName();
            this->line_number_begin = max(this->line_number_begin, list[i]->getLineBegin());
            this->line_number_end = max(this->line_number_end, list[i]->getLineEnd());
        }
    }
    void makeNode(SymbolInfoNode *node){
        this->symbol_name = node->symbol_name;
        this->symbol_type = node->symbol_type;
        this->line_number_begin = node->line_number_begin;
        this->line_number_end = node->line_number_end;
        this->symbol = node->symbol;
        this->code = node->code;
        this->dataType = node->dataType;
        this->isArray = node->isArray;
        this->isFunction = node->isFunction;
        this->actualValue = node->actualValue;
        this->faceValue = node->faceValue;
        this->functionItems.changeFunction(node->functionItems);
        this->arrayItems.changeArray(node->arrayItems);
    }
    string getForTabFinally()
    {
         if(symbol_name == dummy)
                return 0 ;
            string s = "<" ;
            s = s + symbol_type;
            s = s + " :: ";
            s = s + symbol_name ;
            
            s = s + ">";
            return s ;   
    }
    string getPrintForTable()
    {
        //return getForTabFinally();
        if(symbol_name == dummy)
            return 0 ;
        string s = "<{[ type=" ;
        s = s + symbol_type;
        s = s + " ::: name =";
        s = s + symbol_name ;
        
        s = s + " ::: dataType =";
        s = s + dataType ;

        s += " ::: symbol = ";
        s += symbol ;

        s += " ::: \ncode = ";
        s += code ;        
 
        s += " :: line num beg = ";
        s += getString(line_number_begin);
        s += " ::: isArray = ";
        s += getString(isArray) ;
        s += " :: isFunction = ";
        s += getString(isFunction) ;
 
        s = s + "]}>";
        return s ;
    }

    string getEverything_3(){
        if(symbol_name == dummy)
            return 0 ;
        string s = "<{[ type=" ;
        s = s + symbol_type;
        s = s + " ::: name =";
        s = s + symbol_name ;
        
        s = s + " ::: dataType =";
        s = s + dataType ;

        s += " ::: actualValue = ";
        s += actualValue ;
        s += " :: line num beg = ";
        s += getString(line_number_begin);
        s += " ::: isArray = ";
        s += getString(isArray) ;
        s += " :: isFunction = ";
        s += getString(isFunction) ;
        s = s + "]}>";
        if(isFunction == true){
        	s += functionItems.getFunction();
        }
        if(isArray == true){
    		s += arrayItems.getArrayItem();
        }
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
                //printf("   ");
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

