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
#define DODO "DODO"
#include "../SymbolTable/1505022_SymbolTable.h"

using namespace std ;

class BisonUtil{

public:
    ofstream logout;
    ofstream errout ;
    ofstream err ;
    //ofstream debug ;
    ofstream sem ;
    //ofstream deb;
    ofstream arr; 
    string returnFlag = "false" ;
	int voidCount = -1000;
	int secondVoidCount = -1000;
	int numErrors = 0;
	ArgumentList currentArgumentList;
	vector<SymbolInfoNode *> list;
	vector<SymbolInfoNode *> currentFunctionList;
	string currentFunctionName ;
	string currentFunctionReturnType;
	int currentFunctionLine;
	SymbolInfoNode *firstNode ;
    SymbolTable *tab ;
	string currentDataType = "NIL" ;
	int zeroError = 0;
	string currentFunctionSymbol;
	BisonUtil(){

	}
	BisonUtil(SymbolTable *table)
	{
		char* errorFile = "Bison_Stuffs/1505022_BisonError.txt";
		char* logFile = "Bison_Stuffs/1505022_BisonLog.txt";
        errout.open(errorFile);
        logout.open(logFile);
        //debug.open("DEBUG_BISON.txt");
        arr.open("DebugStuffs/Array_Debug.txt");
        sem.open("DebugStuffs/1505022_Semantic.txt");
        err.open("DebugStuffs/1505022_ZeroError.txt");
        //deb.open("ANOTEHR_DEBUG.txt");
        tab = table ;
        if(tab)
        {
        	delete tab ;
        }
        tab = new SymbolTable(50);
        //Initially Should enter a new scope..
      	enterNewScope();
	}
	std::string numToString ( int  Number )
	{
	 std::ostringstream ss;
	 ss << Number;
	 return ss.str();
	}

	void closeFiles()
	{
		errout.close();
		logout.close();
		err.close();
		sem.close();
	}
	void addFirstNode(int flag)
	{
		if(flag != 0)
			return ;
		if(firstNode)
			pushToSymbolTable(firstNode);
	}
	void printLog(string whatToPrint)
	{
		logout << whatToPrint << endl ;
		//cout << whatToPrint << endl ;
		voidCount++ ;
		//logout << "==============>>>>Here, butil.voidCount = " << voidCount << endl << endl ;
		
	}
	void printArrayDebug(string s, SymbolInfoNode *node){
		string str = s ;
		str += "\n";
		str += node->getPrintForTable();
		arr << str << endl << endl;
		//cout << str << endl;

	}
	void printArrayDebug(string s){
		arr << s << endl << endl;
		//cout << s << endl; 
	}
	void printDebug(string s)
	{
		//debug << s << endl ;
		//cout << s << endl ;
	}
	void printDeb(string s){
		//deb << s << endl;
		//cout << s << endl;
	}
	void convertToDataAndPrintLog(SymbolInfoNode *node)
	{

		string str = node->getName();
		string dataType = node->getDataType(); 
		string type = "ID" ;
 		int len = str.length() ;
 		int lineBegin = node->getLineBegin() ;
		char *cstr = new char[str.length() + 1];
		strcpy(cstr, str.c_str());

		//process in cstr
		string name = "" ; 
		for(int i=0; i<len; i++){
			
			if(isSafe(cstr[i]) == true)
			{
			
				name += cstr[i] ;
				
			}else{
				pushToSymbolTable(name, type, dataType, lineBegin);
				name = "" ;
			}
	
		}

		pushToSymbolTable(name, type, dataType, lineBegin);
		delete []cstr ;
	}
	void processAndPushToST(SymbolInfoNode *node)
	{
		string str = node->getName();
		char *cstr = new char[str.length() + 1];
		strcpy(cstr, str.c_str());
		int len = str.length();
		string temp = "";
		for(int i=0; i<len; i++){
			if(cstr[i] == ',')
			{
				myFunction(temp, node->getLineBegin());
				temp = "";
			}
			else{
				temp += cstr[i];
			}
		}
		myFunction(temp, node->getLineBegin());
		//printf("\n\n");
	}
	void myFunction(string str, int line)
	{
		printLog("Inside myFunction .. str = " + str);
		char *cstr = new char[str.length() + 1];
		strcpy(cstr, str.c_str());
		int len = str.length();
		string dataType = "";
		string name = "";
		string type = "ID";

		int temp = 0;
		int pos1 = 0;
		for(int i=0; i<len; i++){
			if(cstr[i] == ' ')
				continue;
			else {
				pos1 = i ;
				break ;
			}
		}
		for(int i=pos1; i<len; i++){
			if(cstr[i] == ' '){
				temp = i ;
				break ;	
			}else{
				dataType += (cstr[i] + 'A' - 'a');
			}
		}
		for(int i=temp; i<len; i++){
			if(cstr[i] == ' '){
				continue;
			}
			name += cstr[i];
		}
		SymbolInfoNode *node = new SymbolInfoNode(name, type, line, dataType);
		////printf("==---++===>>>.. ");
		//node->printNodeB();
		pushToSymbolTable(node);
	}
	bool isSafe(char c)
	{
		int num = 9 ;
		char  toAvoid[] = {',' , '+', '-', '*', '%', '/', '&', '|', ' '}; 
		for(int i=0; i<num; i++){
			if(c == toAvoid[i])
				return false ;
		}	
		return true ;
	}
	void pushToSymbolTable(SymbolInfoNode *node)
	{

		pushToSymbolTable(node->getName(), node->getType(), node->getDataType(), node->getLineBegin(),
			node->isFunction, node->isArray, node->functionItems, node->arrayItems, node->actualValue, 
			node->symbol, node->code);
		return ;

		if(node->getName() == "")
		{
			return ;
		}
	 
		//printf("\n----------------------------------------------\n");
		//printf("Inside pushToSymbolTable , insert to table and printing node ..... ");
		node->printNodeB();

		//tab->Insert(node);
		string s = tab->printCurrentNonEmpty();
		printLog(s);
		//printf("\n-----------------------------------------------\n");
	}
	bool myProcess(SymbolInfoNode *node){
		string name = node->getName();
		if(name == ""){
			////printf("name == empty so do nothing..\n");
			return false;
		}
		char *cstr = new char[name.length() + 1];
		strcpy(cstr, name.c_str());

		for(int i=0; i<name.length(); i++){
			if(cstr[i] == ',')
				return false;
		}
		return true ;
	}
	
	void pushUncondition(SymbolInfoNode *n2)
	{
		string name = n2->getName();
		string type = n2->getType();
		string dataType = n2->getDataType();
		int lineBegin = n2->getLineBegin();
		//tokenType is ID 
		if(name == ""){
			//printf("name == empty so do nothing..\n");
			return ;
		}
		/*char *cstr = new char[name.length() + 1];
		strcpy(cstr, name.c_str());

		for(int i=0; i<name.length(); i++){
			if(cstr[i] == ',')
				return ;
		}*/

		//printf("\n------------------------------------------------\n");
		

		SymbolInfoNode *node = new SymbolInfoNode(name, type, lineBegin, dataType);

		if(lookup(node) == true)
			return ;

		//printf("Inside pushToSymbolTable ... symbol to push is .. ");
		node->printNodeB();
		tab->Insert(node);
		string s = tab->printCurrentNonEmpty();
		printLog(s);
		//printf("\n-----------------------------------------------------------------\n");
	}

	void pushToSymbolTable(string name, string type ,string dataType, int lineBegin)
	{
		string n = "";
		for(int i=0; i<name.length(); i++){
			if(name[i] == ' ')
			{
				continue ;
			}
			else{
				n += name[i];
			}
		}
		name = n ;
		//tokenType is ID 
		string toPrint = "";
		if(name == ""){
			printDebug("name == empty so do nothing..\n");
			return ;
		}

		printDebug("\n----------------- <<< INSIDE pushToSymbolTable >>>> -------------------------------\n");
		

		SymbolInfoNode *node = new SymbolInfoNode(name, type, lineBegin, dataType);

		if(lookup(node) == true)
			return ;

		printDebug("Inside pushToSymbolTable ... symbol to push is .. ");

	

		node->printNodeB();
	 	tab->Insert(node);

		string s = tab->printEverything();
		printSemantic(s);
		printDebug("\n--------------------------------<<DONE pushToSymbolTable>>---------------------------------\n");
	}
	void pushToSymbolTable(string name, string type ,string dataType, int lineBegin, 
		bool isFunction, bool isArray, FunctionItems func, ArrayItems arr, string actualValue, 
		string symbol, string code)
	{
		string n = "";
		for(int i=0; i<name.length(); i++){
			if(name[i] == ' ')
			{
				continue ;
			}
			else{
				n += name[i];
			}
		}
		name = n ;
	 
		if(name == ""){
			//printf("name == empty so do nothing..\n");
			return ;
		}
		printSemantic("==========++++>>>>> INSERTING IN ST IS Begin !! ");		

		SymbolInfoNode *node = new SymbolInfoNode(name, type, lineBegin, dataType);

		node->isFunction = isFunction;
		node->isArray = isArray;
		node->functionItems = func;
		node->arrayItems = arr;
		node->actualValue = actualValue;
		node->code = code;
		node->symbol = symbol;
		SymbolInfoNode *res = tab->LookUp(node->getName());
		if(res){
			printSemantic("\n\n========>>>lookup(node)  " +  node->getName() +  " is true  \n\n");
		}
		else{
			printSemantic("\n\n========>>>lookup(node)  " +  node->getName() +  " is false  \n\n");

		 	tab->Insert(node);

			string s = tab->printCurrentNonEmpty();
			printSemantic(s);
			printSemantic("\n---------------------((DONE pushToSymbolTable))--------------------------------------------\n");
		}
		printSemantic("==========++++>>>>> INSERTING IN ST IS DONE !! ");
	}
	void checkAndPrint(SymbolInfoNode *node){
		SymbolInfoNode *res = tab->LookUp(node->getName());
		if(res){
			string s = "\nERROR [Variable already declared before] at line ";
			s += getString(node->getLineBegin());
			s += " , Variable name is ";
			s += res->getName();
			s += " , and trying to declare again at line. ";
			s += res->getLineBegin();
			s += "\n";
			printError(s);
		}
	}
	void checkUniquenessAndPushST(SymbolInfoNode *node)
	{
		SymbolInfoNode *res = tab->searchCurrent(node) ;
		
		if(res){
			string name = res->getName();
			int lineBegin = node->getLineBegin();
			printError( "[NOT UNIQUE VAR NAME] : " + name + " already exists at line no " + getString(res->getLineBegin())+ " , trying to declare again at " + getString(lineBegin));
			return;
		}
		//pushToSymbolTable(node->getName(), node->getType(), node->getDataType(), node->getLineBegin());
		pushToSymbolTable(node);
	}
	void printLog(string whatToPrint, SymbolInfoNode *node)
	{

		int lineNumber = node->getLineBegin();
		int lineEnd = node->getLineEnd();
		string s = "At Line Number : " ; 
		if(lineNumber == lineEnd)
		{
			s += numToString(lineNumber);
			s += " , ";
			s += whatToPrint ;
			s += "\n";
			s += node->getName();
			//s += "<<==>>Type: " ;
			//s += node->getDataType();
		}
		else{
			//s = "LINE NUMBERS DONT MATCH...\n" ;
			s += numToString(lineNumber);
			s += " , ";
			s += whatToPrint ;
			s += "\n";
			s += node->getName();
		}
		s += "\n";

		s += ("Symbol : " + node->symbol);
		s += "\nCode: \n";
		s += node->code;
		s += "\n"; 
		
		logout << s ;
		//cout << s ;
		voidCount++ ;
		//logout << "==============>>>>Here, butil.voidCount = " << voidCount << endl << endl ;
	}
	void enterNewScope()
	{
		string s = tab->enterScope();
		printLog(s);
	}
	void exitScope()
	{
		//printLog("\n==-->>>Calling exitScope()... \n");
/*		int currId = tab->getCurrentScopeID();
		string x = tab->getAllNonEmpty();
		printLog(x);
		string s = tab->exitScope();
		printLog(s);
*/	//THIS IS ACTUAL


		//DEBUG
		int currId = tab->getCurrentScopeID();
		string x = tab->printCurrentNonEmpty();
		printLog(x);
		string s = tab->exitScope();
		printLog(s);
	 
	}
	void push(string name, string type)
	{
		tab->Insert(name, type);
	}
	void printST()
	{
		string s = tab->getAllNonEmpty();
		printLog(s);
	}
	bool lookup(SymbolInfoNode *n)
	{
		return false ;
		SymbolInfoNode *node = tab->LookUp(n->getName());
		if(node != 0){
			return true ;
		}
		return false ;
	}
    std::string getString ( int  Number )
    {
     std::ostringstream ss;
     ss << Number;
     return ss.str();
    }
    void printError(string msg)
    {
		numErrors++ ;
		string s = "\n--------------------------------------\n";

		s += getString(numErrors);
		s += ". ";
		s += msg ;
		s += "\n--------------------------------------\n";
    	errout << s << endl ;
    	//cout << s << endl ;

    }
    void printError(string s, bool flag){
    	errout << s << endl ;
    }
    void typeCheck(SymbolInfoNode *node1, SymbolInfoNode *node2)
    {

    }
    bool search(SymbolInfoNode *node)
    {
    	SymbolInfoNode *n = tab->LookUp(node->getName());
    	if(n)
    		return true;
    	return false ;
    }

    void checkPrintAndPush(SymbolInfoNode *node)
    {
    	
    	string name = node->getName();
    	SymbolInfoNode *n = tab->LookUp(node->getName());
    	//printf("===>>>>Inside checkPrintAndPush ... printing node->getName  ...");
    	//cout << node->getName() << endl ;
    	if(n){
    		string s = "SEMANTIC ERROR [DOUBLE DEFINITION] " + name + " already exists at line : " + getString(n->getLineBegin()) ;
			s += "\nTrying to declare " ;
			s += name ;
			s += " again at line ";
			s += getString(node->getLineBegin());
			printError(s);
			return ;
    	}
    	pushToSymbolTable(node);

    }

    void printErrorToLog(const char* str, int line, char * token)
    {
    	string msg(str);
    	string lineNum = getString(line);
    	string tokenName(token);
    	string red = "\n-------------------------------*****-------------------------------\n===>>>>";

    	string fullMsg = red + "SYNTAX ERROR at line " + lineNum ;
    	fullMsg += " , Error message is : ";
    	fullMsg += msg ;
    	fullMsg += "\nRecent most token is \"" ;
    	fullMsg += token ;
    	fullMsg += "\"" ;
    	fullMsg += "\n-------------------------------*****-------------------------------\n";
    	printLog(fullMsg);
    }
    void declareFunction(SymbolInfoNode *n)
    {
    	string s = "\n-------------------\n";
    	n->printNodeB();
    	s += n->getName();
 

    	string name = n->getName();

		name = getTrimmedName(name);

		char *cstr = new char[name.length() + 1];
		strcpy(cstr, name.c_str());

		FunctionItems func ;


 		int i = 0;
		int nextPos = 0;
		if(cstr[i] == 'v'){
			func.returnType = "VOID" ;
			nextPos = 4 ; //3 + 1 for extra space
		}
		else if(cstr[i] == 'i'){
			func.returnType = "INT" ;
			nextPos = 3 ; //2 + 1 for extra space
		}
		else if(cstr[i] == 'f'){
			func.returnType = "FLOAT" ;
			nextPos = 5 ; //4 + 1 for extra space 
		}
 		func.functionName = "";
 		for(i = nextPos; i<name.length(); i++){
 			if(cstr[i] == '('){
				nextPos = i;
				break ;
			}
 			else{
				func.functionName += cstr[i];
 			}
 		}
 

 		string thisVar = "";
 		nextPos ++ ; //TO GET RID OF THE FIRST LPAREN / '('
 	
 		func.numberParameters = 0; 
 	 		
 		if(cstr[i + 1] == ')'){
 			func.numberParameters = 0;
 		}
 		else{
			for(i = nextPos; i<name.length(); i++){
				if((cstr[i] == ',') || cstr[i] == ')'){
					addToFunction(func, thisVar, n->getLineBegin());
					thisVar = "";
					func.numberParameters++;
				}
				else{
					thisVar += cstr[i];
				}	
 			}
 		}

 		//UPORE PORJONTO RETURN TYPE , AND PARAMETER LIST DONE..
 		func.lineNumberBegin = n->getLineBegin();
 		func.lineNumberEnd = n->getLineEnd();

 		func.declareOrDefine = "DECLARE";


 		//FUNCTION IS GOING TO BE MADE..
 		SymbolInfoNode *node = new SymbolInfoNode(func.functionName, func.returnType);
 		node->functionItems = func;
 		node->setDataType(func.returnType) ;
 		node->isFunction = true ;
 		node->setLineBegin(func.lineNumberBegin);
 		node->setLineEnd(func.lineNumberEnd);

 		//FUNCTION IS MADE.. 
 		/*string x = node->functionItems.getFunction();
 		printDebug(x);
 		string x2 = node->getPrintForTable();
 		printDebug(x2);
    	*/

    	//PUSH TO SYMBOL TABLE
    	checkPrintAndPush(node);

    	//printf("\n-------------------------------------------------------\n");
    }
 void defineFunction(SymbolInfoNode *n)
    {
    	string s = "\n-------------------------------------------------\nInside defineFunction..printing n->getName..";
    	//n->printNodeB();
    	s += n->getName();
 		printSemantic(s);

    	string name = n->getName();
    	//name = getTrimmedName(name);
		
		char *cstr = new char[name.length() + 1];
		strcpy(cstr, name.c_str());

		FunctionItems func ;



 		int i = 0;
		int nextPos = 0;
		
		for(int i=0; i<name.length(); i++){
			if(name[i] == ' ')
				continue;
			else{
				nextPos = i;
				break ;
			}
		}
		i = nextPos ;

		if(cstr[i] == 'v'){
			func.returnType = "VOID" ;
			nextPos += 4 ; //3 + 1 for extra space
		}
		else if(cstr[i] == 'i'){
			func.returnType = "INT" ;
			nextPos += 3 ; //2 + 1 for extra space
		}
		else if(cstr[i] == 'f'){
			func.returnType = "FLOAT" ;
			nextPos += 5 ; //4 + 1 for extra space 
		}

 		func.functionName = "";
 		for(i = nextPos; i<name.length(); i++){
 			if(cstr[i] == '('){
				nextPos = i;
				break ;
			}
 			else{
				func.functionName += cstr[i];
 			}
 		}

 		string thisVar = "";
 		nextPos ++ ; //TO GET RID OF THE FIRST LPAREN / '('


 		func.numberParameters = 0; 

 		if(cstr[i + 1] == ')'){
 			func.numberParameters = 0;
 		}
 		else{
			for(i = nextPos; i<name.length(); i++){
				 
				if(cstr[i] == ')'){
					func.numberParameters++;
					addToFunction(func, thisVar, n->getLineBegin());					
					break ;
				}
				else if((cstr[i] == ',')){
					addToFunction(func, thisVar, n->getLineBegin());
					thisVar = "";
					func.numberParameters++;
				}
				else{
					thisVar += cstr[i];

				}	
 			}
 		}

 		//UPORE PORJONTO RETURN TYPE , AND PARAMETER LIST DONE..
 		//func.lineNumberBegin = n->getLineBegin();
 		func.lineNumberBegin = currentFunctionLine ;
 		func.lineNumberEnd = n->getLineEnd();

 		func.declareOrDefine = "DEFINE";

 	
 		//FUNCTION IS GOING TO BE MADE..
 		SymbolInfoNode *node = new SymbolInfoNode(func.functionName, func.returnType);
 		node->functionItems = func;
 		node->setDataType(func.returnType) ;
 		node->isFunction = true ;
 		node->setLineBegin(func.lineNumberBegin);
 		node->setLineEnd(func.lineNumberEnd);
 		node->symbol = "res_" + getTrimmedName(func.functionName);
 		//FUNCTION IS MADE.. 
 		printSemantic("INSIDE defineFunction ... \n");
	 	string x = node->functionItems.getFunction();
 		printSemantic(x);
 		string x2 = node->getPrintForTable();
 		printSemantic(x2);
    	 

 		printSemantic("===>>IMMEDIATELY BEFORE checkForDeclareAndPush .... \n");
    	//CHECK FOR DECLARATION .. OTHERWISE PUSH TO SYMBOL TABLE
    	checkForDeclareAndPush(node);
    	//checkPrintAndPush(node);

    	printSemantic("\n--------------------[[ DONE WITH defineFunction ]] -----------------------------------\n");
    }
    void addToFunction(FunctionItems &fun, string var, int lineBegin)
    {
    	//string s = "\n-------------------------************------------------------\n";
    	string s = "";
    	s += "Inside addToFunction , var =";
    	s += var ;
    	s += "\n" ;
    	printDebug(s);

    	int start = 0;
    	for(int i=0; i<var.length(); i++){
    		if(var[i] == ' '){
				start ++ ;
    		}else{
    			start = i;
    			break ;
    		}
    	}

		char *str = new char[var.length() + 1];
		strcpy(str, var.c_str());

		int i = start; 
		int nextPos = start;

		string dataType = "";

		bool isValid = false ;
		if(var[i] == 'f'){
			dataType = "FLOAT";
			nextPos += 5;
			isValid = true ;
		}else if(var[i] == 'i'){
			dataType = "INT";
			nextPos += 3;
			isValid = true ;
		}

		if(!isValid){
			printDebug("Unidentified data type .. \n");
		}

		string name = "";
		for(i = nextPos; i<var.length(); i++){
			if(var[i] == ' ')
				continue;
			name += var[i];
		}
		//printDebug("Name : " + name + " , dataType = " + dataType + "\n");
    	Variable variable;
    	variable.makeVariable(name, dataType, lineBegin);
    	
    	fun.addFunctionItem(variable);

    	//s += "\n-------------------------************------------------------\n";
    }

    void checkForDeclareAndPush(SymbolInfoNode *node)
    {
    	printSemantic("===>>>>Inside checkForDeclareAndPush ... printing node->getName  ...");
    	string name = node->getName();
    	name = getTrimmedName(name);
    	printSemantic("node->name = " + name);

    	SymbolInfoNode *res = tab->LookUp(name);
    	if(res){
    		printSemantic("res exists in symbol table!!\n");
    		if(res->isFunction == false){
	    		string s = "SEMANTIC ERROR [DOUBLE DEFINITION] " + name + " already exists at line : " ;
	    		s += getString(res->getLineBegin());
				s += "\nTrying to declare " ;
				s += name ;
				s += " again at line ";
				s += getString(node->getLineBegin());
				printSemantic(s);
				printError(s);
    			return ;
    		}
    		else{
    			printSemantic("Name exists but is not function ... Go for matchWithDeclareAndPush(node, res)");
    			matchWithDeclareAndPush(node, res);
    			return;
			} 
    	}else{
			printSemantic("Name doesnot exist so .. Go for pushToSymbolTable( ... )");
			pushToSymbolTable(node->getName(), node->getType(), node->getDataType(), node->getLineBegin(),
				node->isFunction, node->isArray, node->functionItems, node->arrayItems, node->actualValue,
				node->symbol, node->code); 
	   	}

	   	printSemantic("===>>>>AFTER checkForDeclareAndPush Printing the node... ");
	   	if(node){
	   		printSemantic(node->getPrintForTable());
	   		if(node->isFunction){
	   			printSemantic(node->functionItems.getFunction());
	   		}
	   	}
    }

    void matchWithDeclareAndPush(SymbolInfoNode *definition, SymbolInfoNode *declaration)
    {
    	FunctionItems f1 = definition->functionItems ;
    	FunctionItems f2 = declaration->functionItems;
 
    	string s = f1.equals(f2);
 
    	if(s == "Matched"){
    		changeFunction(definition, declaration);
			printDebug("CHANGED .. printing ST ");
			string s = tab->printCurrentNonEmpty();
			printDebug(s);
			s = declaration->functionItems.getFunction();
			printDebug(s);
    	}
    	else{
			//printError("SEMANTIC ERROR [function declaration and definition not match] .. func name declaration is " 
			//	+ f1.functionName + " , definition is " + f2.functionName + "\nError is : " + s);
			printError("SEMANTIC ERROR at line. " + definition->getString(definition->getLineBegin()) + 
			 " : function with name " + f2.functionName + ", declaration and definition don't match\n" + s);
			return ;
    	}
    }
    void changeFunction(SymbolInfoNode *definition, SymbolInfoNode *declaration){
			for(int i=0; i<(int)declaration->functionItems.list.size(); i++){
				declaration->functionItems.list[i].name = definition->functionItems.list[i].name;
			}
			declaration->functionItems.declareOrDefine = definition->functionItems.declareOrDefine;
			declaration->functionItems.lineNumberBegin = definition->functionItems.lineNumberBegin;
			declaration->functionItems.lineNumberEnd = definition->functionItems.lineNumberEnd;
    }
    string getTrimmedName(string s)
    {
    	string str = "";
    	for(int i=0; i<s.length(); i++){
			if(s[i] == ' ')
				continue ;
			str += s[i];
    	}
    	return str ;
    }
    void printDeb(string str, SymbolInfoNode *node){
    	string s = "\n==========>>Inside printDeb <<=========\n";
    	s += str ;
    	s += "\nPRINTING NODE NOW ..\n";
    	s += node->getPrintForTable();
    	if(node->isArray){
    		s += node->arrayItems.getArrayItem();
    	}
    	if(node->isFunction){
    		s += node->functionItems.getFunction();
    	}
    	s += "\nNODE NAME :";
    	s += node->getName();
    	s += "\n<<<<<<<<<<< DONE >>>>>>>>>>>>\n";
    	printDeb(s);
    }
    void printSemantic(string str, SymbolInfoNode *node){
    	string s = "\n";
    	s += str ;
    	s += "\nPRINTING NODE NOW ..\n";
    	s += node->getPrintForTable();
    	if(node->isArray){
    		s += node->arrayItems.getArrayItem();
    	}
    	if(node->isFunction){
    		s += node->functionItems.getFunction();
    	}
    	s += "\nNODE NAME :";
    	s += node->getName();
    	s += "\n";
    	printSemantic(s);
    }
    void printDebug(string str, SymbolInfoNode *node){
    	string s = "\n======================************INSIDE printDebug(node)*****************=======================\n";
    	s += str ;
    	s += "\nPRINTING NODE NOW ..\n";
    	s += node->getPrintForTable();
    	if(node->isArray){
    		s += node->arrayItems.getArrayItem();
    	}
    	if(node->isFunction){
    		s += node->functionItems.getFunction();
    	}
    	s += "\nNODE NAME :";
    	s += node->getName();
    	s += "\n=====================================*****************************==============================================\n";

    	printDebug(s);

    }
    pushUndeclared(SymbolInfoNode *node){
    	list.push_back(node);
    }
    printAllUndeclaredVariables(){
    	string s = "\n---------------------------------Inside printAllUndeclaredVariables() ....-----------------\n";

    	for(int i=0; i<(int)list.size(); i++){
    		SymbolInfoNode *node = list[i];
    		string x = node->getPrintForTable();
    		s += x;
    	}


    	s += "\n----------------------------------------------------------------------------------------------\n";
    	printDebug(s);
    }
    int getSize(string s){	 
	    // object from the class stringstream
	    stringstream geek(s);
	 
	    // The object has the value 12345 and stream
	    // it to the integer x
	    int x = 0;
	    geek >> x;
	    return x ;
    }
    int getSize(SymbolInfoNode *node){
    	string s = node->getName();
		return getSize(s);
    }

    void pushForFunction(SymbolInfoNode *node){
    	//printDebug("\n===================Inside pushForFunction(node) .. node =" + node->getPrintForTable() + "\n");
    	string name = node->getName();
    	for(int i=0; i<(int)currentFunctionList.size(); i++){
    		if(currentFunctionList[i]->getName() == name)
    			return ;
    	}
    	currentFunctionList.push_back(node);
    }
    SymbolInfoNode* getInCurrentFunction(SymbolInfoNode *node){
    	return getInCurrentFunction(node->getName());
    }
    SymbolInfoNode* getInCurrentFunction(string name){
    	for(int i=0; i<(int)currentFunctionList.size(); i++){
    		if(currentFunctionList[i]->getName() == name){
    			return currentFunctionList[i];
    		}
    	}
    	return 0;    	
    }
    void printAllCurrentFunctionVariables(){
    	string s = "\n=========================<<<<In printAllCurrentFunctionVariables()>>>>>==========================\n";
    	for(int i=0; i<(int)currentFunctionList.size(); i++){
    		s += currentFunctionList[i]->getPrintForTable();
    	}
    	s += "\n====================================<<<<<<<<<<<<<<<<<DONE>>>>>>>>>>>>>>====================================\n";
    	printDebug(s);
    }	

    void removeAllCurrentFunVar()
    {
    	currentFunctionList.clear();
    	//printDebug("=============>>>After removeAllCurrentFunVar ... ");
    	//printAllCurrentFunctionVariables();
    }

    void removeFromCurrentFunction(SymbolInfoNode *node){
    	printDebug("\n======Inside removeFromCurrentFunction .. node->getName = " + node->getName() + "\n");
    	string name = node->getName();
		char *cstr = new char[name.length() + 1];
		strcpy(cstr, name.c_str());
		int i = 0;	//Start with '(';
		string thisVar = "";
 		int nextPos = 1 ;
 		if(cstr[i + 1] == ')'){
 			return ;
 		}
 		else{
			for(i = nextPos; i<name.length(); i++){
				if((cstr[i] == ',') || cstr[i] == ')'){
					removeFromFunction(thisVar);
					thisVar = "";
			 
				}
				else{
					thisVar += cstr[i];
				}	
 			}
 		}
    }

    void removeFromFunction(string var){
    	printDebug("++++++==>>> Inside removeFromFunction(" + var + ")\n");
    	int start = 0;
    	for(int i=0; i<var.length(); i++){
    		if(var[i] == ' '){
				start ++ ;
    		}else{
    			start = i;
    			break ;
    		}
    	}

		char *str = new char[var.length() + 1];
		strcpy(str, var.c_str());

		int i = start; 
		int nextPos = start;

		string dataType = "";

		bool isValid = false ;
		if(var[i] == 'f'){
			dataType = "FLOAT";
			nextPos += 5;
			isValid = true ;
		}else if(var[i] == 'i'){
			dataType = "INT";
			nextPos += 3;
			isValid = true ;
		}


		string name = "";
		for(i = nextPos; i<var.length(); i++){
			if(var[i] == ' ')
				continue;
			name += var[i];
		}

    	for(int i=0; i<(int)currentFunctionList.size(); i++){
    		if(currentFunctionList[i]->getName() == name){
    			currentFunctionList.erase(currentFunctionList.begin() + i);
    		}
    	}
    }

    bool doesExist(SymbolInfoNode *node){
    	string name = node->getName();
    	printDebug("\n==>>> INSIDE doesExist(node) ===========================\n");
    	string s = node->getPrintForTable();
    	printDebug(s);
    	int line = node->getLineBegin();
    	string dataType = node->getType();
    	bool isArray = node->isArray ;

    	int i = 0;
    	int nextPos = 0;

    	string str = name ;

    	for(i=0; i<name.length(); i++){
    		if(str[i] == ' ')
    			continue;
    		else{
    			nextPos = i;
    			break ;
    		}
    	}

    	if(dataType == "INT"){
    		nextPos += 3;
    	}
    	else if(dataType == "FLOAT"){
			nextPos += 5;
    	}

    	string var = "" ;
    	for(i=nextPos; i<str.length(); i++){
    		if((str[i] == ',') || (str[i] == ';')){
    			checkAndPush(var, dataType, isArray, line );
    			var = "";
    			break;
    		}
    		else{
				var += str[i];
    		}
    	}	
    	checkAndPush(var, dataType, isArray, line );
    	//printDebug("\n========================================<<<<< DONE >>>>>>=====================================\n");
    	return false;
    }

    void checkAndPush(string id, string type, bool isArray, int line ){
    	if(id == "")
    		return ;
    	printDebug("==--===>>Inside checkAndPush, id =" + id + " , type =" + type);
    	string name = "" ;
    	for(int i=0; i<id.length(); i++){
    		if(id[i] == '[')
    			break ;
    		name += id[i];
    	}
    	name = getTrimmedName(name);
    	SymbolInfoNode *res = tab->LookUp(name);
    	bool isFunction = false ;
    	
    	//FUNCTION CHECK
    	if(res){
    		isFunction = res->isFunction;
	    	if(isFunction == true){
    			printError("SEMANTIC ERROR[Variable and Function Names match] at line." + 
				getString(line) +
    				" , name is : \"" + res->getName() + 
    				"\" , function is at line " + 
    			getString(res->getLineBegin()) +
    				" and ID is at line. " + getString(line));
    			return ;
    		}
    	}
    	if(name == currentFunctionName){
			printError("SEMANTIC ERROR [Id and Function Names match] and ID exists at line. " + getString(line) + 
				" and function defined or declared at line " + getString(currentFunctionLine));
			return ;
    	}
    	//FUNCTION CHECK DONE

    	//PARAMETER CHECK
    	printAllCurrentFunctionVariables();
    	SymbolInfoNode *node ;
    	string varName ;
    	for(int i=0; i<(int)currentFunctionList.size(); i++){
    		node = currentFunctionList[i];
    		varName = node->getName();
    		printDebug("+++++================>>>varName =" + varName + " , and name =" + name);
    		if(varName == name){
    		
    			printError("SEMANTIC ERROR[MULTIPLE DECLARATION] ... " + name + " is declared once as PARAMETER also.");
    			return ;
    		}
    	}
    	//PARAMETER CHECK DONE

    	/*SymbolInfoNode *newNode = new SymbolInfoNode(id, "ID", line, type);
    	newNode->isArray = isArray;
    	newNode->isFunction = false;

    	pushToSymbolTable(newNode);
		*/
    }
    void printError(int x){
    	errout << getString(x);
    }
	bool isValidNameOfID(string id ){
 
		int len = id.length(); 
		for(int i=0; i<len; i++){
			if((id[i] == '{') ||(id[i] == '}') ||(id[i] == '(') ||(id[i] == ')') ||(id[i] == '[') ||(id[i] == ']') ||(id[i] == '+') 
				||(id[i] == '-')||(id[i] == '/')||(id[i] == '?')||(id[i] == '<')||(id[i] == '>')||(id[i] == '=') ||
				(id[i] == '%')||(id[i] == '|') || (id[i] == '&')){
				return false;
			}
		}
		char charArr[11] = {'0','1','2','3','4','5','6','7','8','9'};
		for(int i=0; i<11; i++){
			if(id[0] == charArr[i])
				return false;
		}
		 
		return true ;
	}	
    SymbolInfoNode* searchForUndeclaredID(SymbolInfoNode *node2 ){
    

    	//EDITED JUST NOW..
    	if(isValidNameOfID(node2->getName()) == false)
    		return node2;

    	
    	string id = node2->getName();
    	string type = node2->getType();
    	bool isArray = node2->isArray;
    	int line = node2->getLineBegin();


    	SymbolInfoNode *noNode = new SymbolInfoNode(node2->getName(), node2->getType(), node2->getLineBegin(), node2->dataType);
    	//noNode->actualValue = node2->actualValue;
    	noNode->makeNode(node2);
    	
    	if(id == "")
    		return noNode;
    	 
    	string name = "" ;
    	for(int i=0; i<id.length(); i++){
    		if(id[i] == '[')
    			break ;
    		name += id[i];
    	}

    	name = getTrimmedName(name);
    	SymbolInfoNode *res = tab->LookUp(name);
    	bool isFunction = false ;


    	printSemantic("==>>Inside searchForUndeclaredID, BisonUtil Line 1127. CHECKING FOR Node:\n" + node2->getPrintForTable());

    	if(res){
			if(res->getName() == currentFunctionName){	//Check for current function
				printError("SEMANTIC Error , Variable name and function names: \"" + currentFunctionName + "\" are same " + 
					" Function was at line . " + getString(currentFunctionLine) + " and var at " + getString(res->getLineBegin()));
				printSemantic("MATCHES current func name.");
				return noNode;
			}else if(res->isFunction == true){	//Check for is it a function
				printError("SEMANTIC Error, Function " + res->getName()
					+  " used without brackets. at line " + getString(node2->getLineBegin()));
				printSemantic("Matches isFunction == true");
				return noNode;
			}else if(res->isArray == true){	//Check for is it an array
				printError("SEMANTIC Error, Array " + res->getName() + " is used without third brackets at line. " + 
					getString(node2->getLineBegin()));
				printSemantic("Matches isArray == true");
				return noNode;
			}
			else{
				//No errors.
				printSemantic("NO ERRORS..");
				return res; 
			}

    	}
    	else{	
    		//Check for parameters of function.
    		for(int i=0; i<(int)currentFunctionList.size(); i++){
    			SymbolInfoNode *itr = currentFunctionList[i];
    			if(itr->getName() == name){
					printSemantic("Matches param list. ", itr);
    				return itr;
    			}
    		}
    		//UNDECLARED
    		printSemantic("DOES NOT MATCH ANYTHING HENCE ERROR.");
			printError("SEMANTIC Error at line. " + getString(line) + " Variable " + name + " is UNDECLARED.");
			return noNode;
    	}


    	return noNode ;
    }
    void printSemantic(string s)
    {
    	sem << s << endl ;
    	cout << s << endl ;
    }
    void printLog(string s, bool flag){
    	//logout << s << endl;
    	////cout << s << endl ;
    }
    void printZero(string s, bool flag){
    	zeroError++;
    	string str = getString(zeroError);
    	str += ") "; 
    	str += s ;
    	if(flag){
    		err << str << endl ;
    	}else{
    		err << s << endl ;
    	}
    	if(flag){
    		//cout << s << endl;
    	} 
    }
    void printZero(string s){
    	printZero(s, true);
    }
};










//		pushToSymbolTable(node->getName(), node->getType(), node->getDataType(), node->getLineBegin(),
//			node->isFunction, node->isArray, node->functionItems, node->arrayItems);
/*
		char *cstr = new char[name.length() + 1];
		strcpy(cstr, name.c_str());

		for(int i=0; i<name.length(); i++){
			if(cstr[i] == ',')
				return false;
		}
*/
/*
		if(var[i] == 'i'){
			if(var[i+1] == 'n'){
				if(var[i+2] == 't'){
					dataType = "INT";
					nextPos = 3;
				}
			}
		}else if(var[i] == 'f'){
			if(var[i+1] == 'l'){
				if(var[i+2] == 'o'){
					if(var[i+3] == 'a'){
						if(var[i+4] == 't'){
							dataType = "FLOAT";
							nextPos = 5; 
						}
					}
				}
			}
		}
*/