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

#include "1505022_PairItems.h"
 
using namespace std ;



class ArrayItems
{
public:
    bool isArray ;
    string arrayName;
    int lineNumberBegin ;
    int size ;
    vector<PairItems> arrayValues;
    ArrayItems()
    {
    	lineNumberBegin = -1;
    	size = -1;
        arrayName = "NIL";
        isArray = false ;
    }
    std::string getString ( int  Number )
    {
     std::ostringstream ss;
     ss << Number;
     return ss.str();
    }
   /* void addValue(string s){
        PairItems pair(-1, s);
        this->arrayValues.push_back(pair);
    }
    */
    bool addValue(int i, string s){
    	printf("INSIDE addValue ... i = %d, s = ", i); cout << s ;
    	if(i >= size){
    		return false ;	
    	}
    	if(i < 0){
    		return false;
    	}
        PairItems pair(i, s);
        cout << pair.getString() << endl ;
        
        //this->arrayValues.push_back(pair);
        
        //this->arrayValues.push_back(pair);
        return true; 
    }

    string getArrayValues(){
        string s = "";
        for(int i=0; i<(int)arrayValues.size(); i++){
            s += "(" ;
            s += arrayValues[i].getString();
            s += ")  ";
        }
        s += "\n";
        return s ;
    }
    string getArrayItem(){
        string s = "\n-----------------------------------------------------\n";
        s += "arrayName = " ;
        s += arrayName ;
        s += " , size = ";
        s += getString(size) ;
        s += " , lineNumberBegin = ";
        s += getString(lineNumberBegin);
        s += "\nArrayValues = ";
        s += getArrayValues();
        s += "\n-----------------------------------------------------\n";
        return s ;
    }
    void printArrayItem( )
    {
        string s = getArrayItem();
        cout << s ;
    }
    void makeArray(string name, int s, int lineNum)
    {
        arrayName = name;
        this->size = s ;
        lineNumberBegin = lineNum;
        isArray = true ;
    }
    void changeArray(ArrayItems arr2){
        this->isArray = arr2.isArray;
        this->arrayName = arr2.arrayName;
        this->lineNumberBegin = arr2.lineNumberBegin;
        this->size = arr2.size ;
        this->arrayValues.clear();
        for(int i=0; i<(int)arr2.arrayValues.size(); i++){
            this->arrayValues[i].index = arr2.arrayValues[i].index;
            this->arrayValues[i].value = arr2.arrayValues[i].value;
        }
    }
    bool equals(ArrayItems ar2){
        if(this->isArray != ar2.isArray)
            return false;
        if(this->arrayName != ar2.arrayName)
            return false;
        if(this->lineNumberBegin != ar2.lineNumberBegin)
            return false;
        if(this->size != ar2.size)
            return false;
        if(this->arrayValues.size() != ar2.arrayValues.size())
            return false;
        for(int i=0; i<(int) arrayValues.size(); i++){
            PairItems p1 = arrayValues[i];
            PairItems p2 = ar2.arrayValues[i];
            if(p1.index != p2.index)
                return false;
            if(p1.value != p2.value)
                return false;
        }

        return true;
    }
};
class Variable
{
public:
    string name;
    string type;
    string actualValue;
    string symbol;
    string code ;
    int lineBegin ;
    bool isArray ;

    void setVariable(Variable v){
        name = v.name;
        type = v.type;
        lineBegin = v.lineBegin;
        isArray = v.isArray;
        symbol = v.symbol;
        code = v.code ;
    }
    Variable()
    {
        name = "NIL" ;
        type = "NIL" ;
        isArray = false ;
    }
    Variable(string n, string t, int l, string ac, string symbol1, string code1 )
    {
        name = n;
        type = t;
        lineBegin = l;
        actualValue = ac;
        isArray = false ; 
        symbol = symbol1; 
        code = code1;
    }
    Variable(string n, string t, int l, string ac, string symbol1)
    {
        name = n;
        type = t;
        lineBegin = l;
        actualValue = ac;
        isArray = false ; 
        symbol = symbol1; 
    }
    Variable(string n, string t, int l, string ac )
    {
        name = n;
        type = t;
        lineBegin = l;
        actualValue = ac;
        isArray = false ; 
    }
    Variable(string n, string t, int l)
    {
        name = n;
        type = t;
        lineBegin = l;
        isArray = false ;
    }
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
    string getVariable( )
    {   
        string s = "<<Name = " + name + " , Type = " + type + " symbol : " + symbol ;
        s += " , line = ";
        s += getString(lineBegin);
        s += ">>\n" ;
        return s; 
    }
    string getVariableForArg(){
        string s = "<<Name = " + name + " , Type = " + type ;
        s += " , line = ";
        s += getString(lineBegin);
        s += " , actualValue = ";
        s += actualValue;
        s += " , isArray = ";
        s += getString(isArray) ;
        s += " , symbol = ";
        s += symbol ;
        s += ">>\n" ;
        return s; 
    }
    void printVariable( )
    {
        string s = getVariable();
 
        cout << s ;        
    }
    void makeVariable(string n, string t, int l)
    {
        name = n;
        type = t;
        lineBegin = l;
    }
};


class FunctionItems
{
private:
    //ofstream funOut ;
public:
    bool isFunction ;
    string functionName ;
    int numberParameters;
    int lineNumberBegin;
    int lineNumberEnd ;
    string returnType;
    string declareOrDefine ;
    vector<Variable> list;
   //ofstream out ;

    FunctionItems(){
    	isFunction = false;
    	functionName = "NIL";
        
        numberParameters = -1;
   		lineNumberBegin = -1;
	 	lineNumberEnd = -1;
	    returnType = "NIL";
	    declareOrDefine = "NIL";

        //funOut.open("Function_Debug.txt");
    }


    void changeFunction(FunctionItems f){
        printf("Inside change function .. printing parameter's function : \n");
        cout << f.getFunction() << endl ;

        this->isFunction = f.isFunction;
        this->functionName = f.functionName;
        this->numberParameters = f.numberParameters;
        this->lineNumberBegin = f.lineNumberBegin;
        this->lineNumberEnd = f.lineNumberEnd;
        this->returnType = f.returnType;
        this->declareOrDefine = f.declareOrDefine;
        int x = (int)this->list.size();
        if(x >= 1){
            this->list.clear();
        }
        cout << "MIDDLE SETTING!!\n";
        for(int i=0; i<(int)f.list.size(); i++){

            Variable var(f.list[i].name, f.list[i].type, f.list[i].lineBegin, f.list[i].symbol);
            //Variable var();
            //var.setVariable(f.list[i]);
            list.push_back(var);
        }
        cout << "AFTER SETTING!!\n";
        cout << this->getFunction() << endl ;
    }
    string equals(FunctionItems f2)
    {
        if(this->returnType != f2.returnType){
            string s =  "Return types do not match  \n";
            s += f2.declareOrDefine ;
            s += " at line. ";
            s += getString(f2.lineNumberBegin);
            s += " is " ;
            s += f2.returnType ;
            s += " AND ";
            s += this->declareOrDefine ;
            s += " at line.";
            s += getString(this->lineNumberBegin);
            s += " is ";
            s += this->returnType;
            return s ;
        }
        if(this->numberParameters != f2.numberParameters){
            string s =  "Number of parameters do not match \n";
			s += "Number of parameters is ";
			s += getString(f2.numberParameters);
			s += " at line. ";
			s += getString(f2.lineNumberBegin);
			s += " AND , number of parameters is ";
			s += getString(this->numberParameters);
			s += " at line. ";
			s += getString(this->lineNumberBegin);
            return s ;
        }
        for(int i=0; i<(int)list.size(); i++){
            Variable v1 = list[i];
            Variable v2 = f2.list[i];
            if(v1.type != v2.type){
                string s =  "Data Types of parameter list do not match of declared and defined , example ... \n" ;
                s += "At " ; s += f2.declareOrDefine ; s += " is '" ;
                string x12 = v2.type;
                transform(x12.begin(), x12.end(), x12.begin(), ::tolower);
                s += x12 ;
                s += " " ;s += v2.name ;
                s += "' AND at " ; s += this->declareOrDefine ; s += " is '" ; 
                x12 = v1.type;
                transform(x12.begin(), x12.end(), x12.begin(), ::tolower);
                s += x12 ;
                s += " "; s += v1.name; s+="'\n" ; 
                return s ;
            }
        }
        return "Matched" ;
    }

    std::string getString ( int  Number )
    {
     std::ostringstream ss;
     ss << Number;
     return ss.str();
    }

    void printFunction()
    {
        string s = getFunction();
        cout << s ;
    }
    string getFunction()
    {
        string s = "\n-----------------------------------------------------\n";
        s += "functionName = " ;
        s += functionName ;
        s += " , numberParameters = ";
        s += getString(numberParameters) ;
        s += " , list size = " ;
        s += getString(list.size());
        s += "\n";
        s += " , lineNumberBegin = ";
        s += getString(lineNumberBegin);
        s += " , lineNumberEnd = ";
        s += getString(lineNumberEnd);
        s += " , returnType = " ;
        s += returnType ;
        s += " , declareOrDefine = ";
        s += declareOrDefine ;
        s += "\n";
        for(std::vector<Variable> :: iterator it = list.begin(); it != list.end(); it++){
            Variable var = *it;
            s += var.getVariableForArg();
        }
        s += "\n-----------------------------------------------------\n";
        return s ;        
    }
    void addFunctionItem(Variable var){
        list.push_back(var);
    }
    void addFunctionItem(string name, string type, int beginLine){
        Variable var(name, type, beginLine);
        list.push_back(var);
    }

    Variable getNode(string name)
    {
        Variable  node;
        vector<Variable> :: iterator it = list.begin();
        for (; it != list.end(); it++)
        {
            Variable var = *it ;
            if(var.name == name)
                return node ;

        }
        return node ;
    }

    bool doesExist(string name)
    {
        Variable node ;
        node = getNode(name);
        if((node.name == "NIL") && (node.type == "NIL"))
            return true ;
        return false ;
    }
    void makeFunction(string funName, string rT,int numberParameters, int lineBegin, int lEnd)
    {
        this->functionName = funName ;
        this->numberParameters = numberParameters;
        this->lineNumberBegin = lineBegin;
        this->lineNumberEnd = lEnd;
        this->returnType = rT ;
    }
};


class ArgumentList
{
public:
    string functionName;
    vector <Variable> listArguments;
    ArgumentList(){

    }
    void addArgument(string name, string type, int lineNumberBegin, string actualValue, bool isArray,string symbol, string code){
        Variable pair(name, type, lineNumberBegin, actualValue, symbol, code);
        pair.isArray = isArray ;
        listArguments.push_back(pair);
    }
    void addArgument(string name, string type, int lineNumberBegin, string actualValue,   string symbol){
        Variable pair(name, type, lineNumberBegin, actualValue, symbol);
        pair.isArray = false ;
        listArguments.push_back(pair);
    }
    void addArgument(string name, string type, int lineNumberBegin, string actualValue, bool flag, string symbol){
        Variable pair(name, type, lineNumberBegin, actualValue, symbol);
        pair.isArray = flag ;
        listArguments.push_back(pair);
    }
    void addArgument(string name, string type, int lineNumberBegin, string actualValue, bool flag ){
        Variable pair(name, type, lineNumberBegin, actualValue);
        pair.isArray = flag ;
        listArguments.push_back(pair);
    }
    void addArgument(string name, string type, int lineNumberBegin, string actualValue){
        Variable pair(name, type, lineNumberBegin, actualValue);
        listArguments.push_back(pair);
    }
    Variable getArgument(string name){
        Variable pair;
        for(int i=0; i<(int)listArguments.size(); i++){
            pair = listArguments[i];
            if(pair.name  == name)
                return pair ;
        }
        return pair ;
    }
    string getArgumentList(){
        string s = "\nFunctionName =";
        s += functionName;
        s += "\nPrinting all arguments:\n";
        for(int i=0; i<(int)listArguments.size(); i++){
            s += listArguments[i].getVariableForArg();
            s += "  ";
        }
        s += "\n";
        return s;
    }
    void clear(){
    	if(listArguments.size() > 0){
        	listArguments.clear();
    	}
    }
};