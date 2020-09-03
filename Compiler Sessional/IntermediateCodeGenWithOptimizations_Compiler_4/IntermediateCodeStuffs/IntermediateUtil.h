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
#include <streambuf>
#include <set>
//#include "../Miscalenous/Converter.h"
#include "../Bison_Stuffs/1505022_SemanticUtil.h"

using namespace std ;

class IUtil{

public:
	std::vector<string> codeLines;
	std::vector<string> mainLines;
	std::vector<string> dataLines;
	std::vector<string> otherFunctions;
	std::set<string> dataSet ;

	std::vector<SymbolInfoNode *> argumentsOfFunction;
	std::vector<SymbolInfoNode *> parametersOfFunction;

	std::vector<FunResult> funResults;

	std::vector<SymbolInfoNode *> declaredVariables;

	string currentResultVar;
	string currentFunName;

	ofstream codeOut ;
	ofstream main; 
	ofstream data;

	ofstream deb ;
	ofstream fun;

	ifstream input;
  
	string code ;
 	Converter converter ;
	int labelNum = 0;
	int tempNum = 0;
	int resNum = 0;
 	int mainTab = 0;

	bool doPrint = true;
	
	void pushArgument(SymbolInfoNode *node){
		SymbolInfoNode *src = new SymbolInfoNode();
		setSymbol(src, node);
		this->argumentsOfFunction.push_back(src);
	}

	IUtil(){
		codeOut.open("IntermediateCodeStuffs/ICode.asm");
		main.open("IntermediateCodeStuffs/Main.asm");
		data.open("IntermediateCodeStuffs/Data.asm");
		deb.open("IntermediateCodeStuffs/DEBUG.txt");
		input.open("IntermediateCodeStuffs/InputFunctions.asm");
		fun.open("IntermediateCodeStuffs/OutFunctions.asm");
		printInitialCode();
	}
 
	void printPrintFunction(){
 
		std::string str((std::istreambuf_iterator<char>(input)), std::istreambuf_iterator<char>());
		fun << str << endl << endl ;
		otherFunctions.push_back(str + "\n\n");
	}

	void setUtils(const BisonUtil &b, const SemanticUtil &s){
 
		//butil.printSemantic("SIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIUUUU!!\n");
	}

	void printCode(string s){
		codeLines.push_back(s);
		codeOut << s << endl ;
		if(doPrint){
			cout << s << endl ;
		}
	}
	void printInitialCode(){

		string initial = ".DATA";
		//printData(initial);
		makeVar("primary_variable");
		printPrintFunction();
	}

	void printData(string s){
		if(s == ".DATA"){
			return;
		}
		//data << "\t" << s << endl ;
		dataLines.push_back("\t" + s);
		dataSet.insert("\t" + s);
		
		if(doPrint){
			cout << "\t" <<  s << endl ;
		}
	}

	void printMain(string s){
		if(false){
			main << s << endl ;
			if(doPrint){
				cout << s << endl ;
			}
		}else{
			mainLines.push_back(s);
			main <<  s << endl ;
			if(doPrint){
				cout << s << endl ;
			}
		}
	}

	void printDeb(string s, bool flag){
		deb << s << endl;
		if(flag){
			cout << s << endl ;
		}
	}
	void printDeb(string s){
		printDeb(s, true);
	}

	string getTabMain(int t){
		string s = "";
		for (int i = 0; i < t; i++)
		{
			s += "\t";
		}
		return s;
	}

	string newLabel(){
		labelNum ++ ;
		string s = "Label_" ;	//generate a new label.
		s += converter.integerToString(labelNum);
		
		return s ;
	}

	string newTemp(){
		tempNum ++ ;
		string s = "tempVar_" ;	//generate a new temporary variable.
		s += converter.integerToString(tempNum);
		makeVar(s);
		return s ;
	}
	string newResult( ){
		resNum ++;
		string s = "resVar_" ;
		s += converter.integerToString(resNum);
		makeVar(s);
		return s; 
	}
	string newResult(string str){
		resNum ++;
		string s = str ;
		makeVar(s);
		return s; 
	}

	void printInitialMain(){
		mainTab++ ;
		string initial = "MAIN PROC\n";
		initial += getTabMain(mainTab);
		initial += "MOV DX, @DATA\n";
		initial += getTabMain(mainTab);
		initial += "MOV DS, DX\n\n";
		printMain(initial);
	}

	void declareVariable(SymbolInfoNode *node1, const BisonUtil &butil){
		printDeb("Inside declareVariable .... node1 = " + node1->getName());
		
		string temp = makeName(node1, butil);
		node1->symbol = temp;
		printDeb("\t=>>After code. node1->symbol = " + node1->symbol);
		string c = node1->symbol + "   DW   ?";
		printDeb("\t\t===>Data must be: " + c);
		printData(c);
	}

	void declareArray(SymbolInfoNode *node1, SymbolInfoNode *node2, const BisonUtil &butil){
		printDeb("Inside declareArray .... node1 = " + node1->getName() + " , and node2 = " + node2->getName());

		string temp = makeName(node1, butil);
		node1->symbol = temp ;
		printDeb("\t=>>After code. node1->symbol = " + node1->symbol);
		string c = node1->symbol + "   DW  " + node2->getName() + "  DUP(0) ";	//? dile problem hoy
		printDeb("\t\t===>Data must be: " + c);
		printData(c);
	}

	string makeName(SymbolInfoNode *node1, const BisonUtil &butil){
		string s = node1->getName() + "_" + converter.integerToString(butil.tab->getCurrentScopeID());
		return s; 
	}

	void printID(SymbolInfoNode *src, SymbolInfoNode *node, string currentFunc){
		printDeb("---->>Inside printID .. node->name = " + node->symbol_name + " , and node->symbol = " 
			+ node->symbol + " , and currentFunc = " + currentFunc);
		printDeb("PRINTING For Table node = " + node->getPrintForTable());
		
		string s = "\n\t\t;Printing " + node->symbol_name + "\n";
		s += "\tPUSH AX\n";	//For Temporary use things
		string s2 = "\tMOV AX, " + node->symbol + "\n";	//To enter this number
		s += s2;
		s += "\tCALL OUTDEC\n";	//To print that number 
		s += "\tPOP AX\n\t\t;Printing " + node->symbol_name + " is done " + "\n\n";	//To restore AX value
		src->code = s;

		//printCode(s, currentFunc);
	}
	void printDeb(string s, SymbolInfoNode *n){
		printDeb(s + ", <<< Name : " + n->symbol_name + ", symbol : " + n->symbol + ", code : \n" + n->code + 
			 ", isArray : " + converter.boolToString(n->isArray) + ", n->dataType = " + n->dataType + " >>>>");
	}
	bool doesNodeExist(SymbolInfoNode *n, const BisonUtil &butil){
		SymbolInfoNode *res = butil.tab->LookUp(n->symbol_name);
		if(res){
			return true;	
		}else{
			return false;
		}
	}

	void assignOp(SymbolInfoNode *src, SymbolInfoNode *left, SymbolInfoNode *right, const BisonUtil &butil){
		/*
			$$=$1;
			$$->code=$3->code+$1->code;
			$$->code+="mov ax, "+$3->getSymbol()+"\n";
			if($$->getType()=="notarray"){ 
				$$->code+= "mov "+$1->getSymbol()+", ax\n";
			}
			else{
				$$->code+= "mov  "+$1->getSymbol()+"[bx], ax\n";
			}
			delete $3;
		*/
 
		
		printDeb("=================+>>>Before assignOp\nleft = ", left); printDeb("right = ", right);
		src->setSymbol(left);	src->setUnion(" = "); src->setUnion(right);

		src->symbol = left->symbol ;

		string code = "\t\t;This is the starting of assignment op. " + left->symbol_name + " = " + right->symbol_name + "\n";
		code += (right->code + left->code); 
		code += ("\tMOV AX, " + right->symbol + "\n");
		if(src->isArray == false){
			code += ("\tMOV " + left->symbol + ", AX\n");
		}
		else{
			code += ("\tMOV " + left->symbol + "[BX], AX\n");
		}
		code += "\t\t;Assignment op over " + left->symbol_name + " = " + right->symbol_name;
		code += "\n\n";
		src->code = code ;
 		printDeb("-->>>After assignOp , we are printing the code now .. , src = ", src);
 		//printCode(code, butil);

	}

	bool isArray(SymbolInfoNode *n){
		//APATOTO RETURN n->isArray
		return n->isArray ;
	}

	void arrayMaking(SymbolInfoNode *src, SymbolInfoNode *arrayName, SymbolInfoNode *exp, const BisonUtil &butil){
	 	//$$->code=$3->code+"mov bx, " +$3->getSymbol() +"\nadd bx, bx\n";

		bool flag = false;
		string temp = "";
 	
		

		src->makeNode(arrayName);

		string finalCode = "";
		finalCode = exp->code + "\n";
		finalCode += ("\tMOV BX, " + exp->symbol + "\n");
		//finalCode += ("\tADD BX, BX\n");
		//Can use SHL BX, 1
		finalCode += ("\tSHL BX, 1\n");
		if(arrayName){
			arrayName->code = finalCode;
			arrayName->symbol = temp ;
			//src->symbol = "";
			SymbolInfoNode *res = getSymbolFromST(arrayName, butil);
			if(res){
				src->symbol = res->symbol;
			}else{
				src->symbol = arrayName->symbol;
			}
			src->code = finalCode;
			printDeb("=>Line 278 Array Making, For src = " + src->getName() + ", symbol = " + src->symbol + ", code: \n" + src->code);
			//printCode(finalCode, butil.currentFunctionName);
		}

	}

	void setArraySymbol(SymbolInfoNode *src, SymbolInfoNode *var, const BisonUtil &butil){
		/*
		factor -> variable rule .. 
			char *temp= newTemp();
			$$->code+="mov ax, " + $1->getSymbol() + "[bx]\n";
			$$->code+= "mov " + string(temp) + ", ax\n";
			$$->setSymbol(temp);
		*/

		printDeb("==-->>Line 293. Iutil.h, factor ->variable rule .. \nvar =", var);
		src->makeNode(var);
		printDeb("->src = ", src);
		if(var->isArray == true){
			string temp = newTemp();


			string code = "";
			string name = var->symbol_name; 
			code += ("\n\t;This starts array setting " + temp + " has the value of " + var->symbol_name);
			code += var->code ;
			var = getSymbolFromST(var, butil);
			if(!var){
				printDeb("var with name : " + var->symbol_name + " does not exist in ST.");
				return ;
			}
			printDeb("==-->>Line 301 arraycheck. Iutil.h, \nvar =", var);

			
			
			code += ("\tMOV AX, " + var->symbol + "[BX]\n");
			code += ("\tMOV " + temp + ", AX\n");
			code += ("\t;Now " + temp + " has the value of " + name + "\n");
			//MY EDIT 6 45 pm Friday
			//code += ("\tMOV " + temp + ", AX");

			src->code = code;
			src->symbol = temp; 
			//printCode(src->code, butil);
		}
		else{
			//DO NOTHING IF NOT AN ARRAY! 
		}
		printDeb("->After (factor -> var ) setting src, src = ", src);
	}

	void relAndLogicOp(SymbolInfoNode *src, SymbolInfoNode *lNode, SymbolInfoNode *rNode, SymbolInfoNode *operation, 
 		const BisonUtil &butil){

		printDeb("\n================================-------=>>Inside Line 264 relAndLogicOp ... ");
		printDeb("lNode = ", lNode);
		printDeb("rNode = ", rNode);
		printDeb("operation = ", operation);
		printDeb("src = ", src);

		src->makeNode(lNode);

		string code = "\n\t\t;This starts the relation op " + operation->symbol_name + " between " + lNode->symbol_name + " and " + rNode->symbol_name + "\n";
		code += (lNode->code + rNode->code);

		if(operation->getName() == "&&"){
			//Make And Operation
			//NEED TO CHECK WITH 1 Of Both sides then make AND
			string temp1 = newTemp();

			string l1 = "\tMOV AX, " + lNode->symbol + "\n";	
			l1 += ("\tAND AX, " + rNode->symbol + "\n");
			l1 += ("\tMOV " + temp1 + ", AX\n"); 
		//	string l2 = "\tMOV DX, " + rNode->symbol + "\n";
			code += l1;
 
 
			src->symbol = temp1; 
		}
		else if(operation->getName() == "||"){
			//Make Or Operation
			string temp1 = newTemp();
		

			string l1 = "\tMOV AX, " + lNode->symbol + "\n";	
			l1 += ("\tOR AX, " + rNode->symbol + "\n");
			l1 += ("\tMOV " + temp1 + ", AX\n"); 
		//	string l2 = "\tMOV DX, " + rNode->symbol + "\n";
			code += l1;
  
			src->symbol = temp1; 
		}
		else{
			// / if(operation->getName() == "<"){}
			string l1 = "\tMOV AX, " + lNode->symbol + "\n";
			l1 += ("\tCMP AX, " + rNode->symbol + "\n");

			string temp = newTemp();


			string label1 = newLabel();
			string label2 = newLabel();
			
			string op = "";
			src->symbol = temp; 
			l1 += ("\tMOV " + src->symbol + ", 0\n");	//Negative Logic First
			
			string opName = operation->getName();
			
			if(opName == "<"){
				op =  ("\tJL  " +  label1 + "\n");
			}
			else if(opName == "<="){
				op =  ("\tJLE  " +  label1 + "\n");
			}	//ARO OPERATIONS BAAKI..
			else if(opName == ">"){
				op =  ("\tJG  " +  label1 + "\n");
			}
			else if(opName == ">="){
				op =  ("\tJGE  " +  label1 + "\n");
			}
			else if(opName == "=="){
				op =  ("\tJE  " +  label1 + "\n");
			}
			else if(opName == "!="){
				op =  ("\tJNE  " +  label1 + "\n");
			}
			

			l1 += op ;
			l1 += ("\tJMP " + label2 + "\n");	//Jump to END_IF
			l1 += (" " + label1 + ": \n");
			l1 += ("\tMOV " + src->symbol + ", 1\n");
		 	l1 += (" " + label2 + ": 			;this label continues with original code\n");

			code += l1; 
			
		}

		src->symbol_name = (lNode->symbol_name + " " + operation->symbol_name + " " + rNode->symbol_name);
		
		code += ("\t\t; This ends the relation op " + operation->symbol_name + " between " + lNode->symbol_name + " and " + rNode->symbol_name + "\n\n");

		src->code = code ;
		//printCode(code, butil);
		printDeb("AFTER relAndLogicOp ... printing source = ", src);
	}

	string makeVar(string varName, string val, const BisonUtil &butil){
		string s =  varName ;
		s += "    DW   ";
		s += val;
		printData(s);
		return varName; 
	}

	string makeVar(string varName, const BisonUtil &butil){
		return makeVar(varName, "?", butil);
	}
	string makeVar(string varName){
		string val = "?";
		string s =  varName ;
		s += "    DW   ";
		s += val;
		printData(s);
		return varName;		
	}



	SymbolInfoNode *addop(SymbolInfoNode *src, SymbolInfoNode *left, SymbolInfoNode *expression, SymbolInfoNode *right, 
		const BisonUtil &butil){

		/*
				$$=$1;
				$$->code+=$3->code;
				
				// move one of the operands to a register, perform addition or subtraction 
				with the other operand and move the result in a temporary variable  
				
				if($2->getSymbol()=="+"){
				
				}
				else{
				
				}
				delete $3;
				cout << endl;
			 
	
		*/
 
		src = new SymbolInfoNode();
		setSymbol(src, left, (left->symbol_name + expression->symbol_name + right->symbol_name));
		 
	 	string initCode = (left->code + right->code) ;
		string symbol = newTemp();
		src->symbol = symbol ;
	 	string code = "";
		if(expression->getName() == "+"){
			//ADDITION
			code += ("\n\t;Addition begins between " + left->getName() + " , " + right->getName() + "\n"); 

				// move one of the operands to a register, perform addition or subtraction 
				//with the other operand and move the result in a temporary variable  
			code += ("\tMOV AX, " + left->symbol + "\n");
			code += ("\tADD AX, " + right->symbol + "\n");
			code += ("\tMOV " + src->symbol + ", AX\n");
			code += ("\t;Addition ENDS between " + left->getName() + " , " + right->getName() + "\n"); 
		}
		else if(expression->getName() == "-"){
			//SUBTRACTION
			code += ("\n\t;Subtraction begins between " + left->getName() + " , " + right->getName() + "\n");
			code += ("\tMOV AX, " + left->symbol + "\n");
			code += ("\tSUB AX, " + right->symbol + "\n");
			code += ("\tMOV " + src->symbol + ", AX\n");
			code += ("\t;Subtraction ENDS between " + left->getName() + " , " + right->getName() + "\n");
		}
		string finalCode = initCode + code ;
		src->code = finalCode; 
		printDeb("\n======================Inside addop printing   left, exp, right, src serially.=======================\n");
		printDeb("Left: " + left->getPrintForTable() + "\nRight: " + right->getPrintForTable() + "\nexp: " + expression->getName()
			 + "\nSrc: " + src->getPrintForTable());
		printDeb("\n==>>>After addop, src = " + src->getPrintForTable() + "\n===================[DONE ADDOP]================================\n");

		return src; 
	}
	
	SymbolInfoNode* mulop(SymbolInfoNode *src, SymbolInfoNode *left, SymbolInfoNode *expression, SymbolInfoNode *right, 
		const BisonUtil &butil){
		/*
			$$=$1;
			$$->code += $3->code;
			$$->code += "mov ax, "+ $1->getSymbol()+"\n";
			$$->code += "mov bx, "+ $3->getSymbol() +"\n";
			char *temp=newTemp();
			if($2->getSymbol()=="*"){
				$$->code += "mul bx\n";
				$$->code += "mov "+ string(temp) + ", ax\n";
			}
			else if($2->getSymbol()=="/"){
				// clear dx, perform 'div bx' and mov ax to temp
			}
			else{
				// clear dx, perform 'div bx' and mov dx to temp
			}
			$$->setSymbol(temp);
			cout << endl << $$->code << endl;
			delete $3;
			}
		*/
		src = new SymbolInfoNode();
		setSymbol(src, left, (left->symbol_name + expression->symbol_name + right->symbol_name));

		string symbol = newTemp();
		src->symbol = symbol ;

		string code_init = (left->code + right->code);
		string code = "\t;Beginning Mulop " + expression->getName() + " between " + left->symbol_name + " , " + right->symbol_name + "\n";

		code += ("\tMOV AX, " + left->symbol + "\n");

		code += ("\tMOV BX, " + right->symbol + "\n");

		if(expression->getName() == "*"){
			//This is multiplication
			code += ("\tMUL BX\n");
			code += ("\tMOV " + src->symbol + ", AX\n");
		}
		else if(expression->getName() == "/"){
			//This is division
			/*
				code += ("\tXOR DX, DX\n"); //Clear DX
				code += ("\tDIV BX\n");
				code += ("\tMOV " + src->symbol + ", AX\n");
			*/
			//WE do SIGNED Division
			code += ("\tCWD\n");
			code += ("\tIDIV BX\n");
			code += ("\tMOV " + src->symbol + ", AX\n");

		}
		else if(expression->getName() == "%"){
			//This is modulus
			/*
			code += ("\tXOR DX, DX\n"); //Clear DX
			code += ("\tDIV BX\n");
			code += ("\tMOV " + src->symbol + ", DX\n");
			*/
			//Signed Division performed
			code += ("\tCWD\n");
			code += ("\tIDIV BX\n");
			code += ("\tMOV " + src->symbol + ", DX\n");
		}
		code += "\t;Ending Mulop " + expression->getName() + " between " + left->symbol_name + " , " + right->symbol_name + "\n";

		string finalCode = code_init + code ;
		src->code = finalCode; 

		printDeb("\n*******************Inside mulop printing   left, exp, right, src serially**************n");
		printDeb("Left: " + left->getPrintForTable() + "\nRight: " + right->getPrintForTable() + "\nexp: " + expression->getName()
			 + "\nSrc: " + src->getPrintForTable());
		printDeb("\n==>>>After addop, src = " + src->getPrintForTable() +
		 "\n****************************DONE mulop****************************n");

		return src; 
	}





	SymbolInfoNode* unary(SymbolInfoNode *src, SymbolInfoNode *expression, SymbolInfoNode *left){

/*	NOT
		$$=$2;
		char *temp=newTemp();
		$$->code="mov ax, " + $2->getSymbol() + "\n";
		$$->code+="not ax\n";
		$$->code+="mov "+string(temp)+", ax";
*/


		src = new SymbolInfoNode();
		setSymbol(src, left, (expression->symbol_name + left->symbol_name));

/*
		string symbol = newTemp();
		src->symbol = symbol ;
*/
		string op = expression->getName();
		if(op == "!"){
			//NOT OPERATION
			string symbol = newTemp();
			src->symbol = symbol ;
			string code = left->code;
			code += ("\t\t\t;NOT OPERATION starts for " + left->getName() + "\n");
			
			string label1 = newLabel();
			string label2 = newLabel();
			/*code += ("\tMOV AX, " + left->symbol + "\n");
			code += ("\tNOT AX\n");
			code += ("\tMOV " + src->symbol + ", AX\n");*/
			code += indent("MOV AX, " + left->symbol);
			code += indent("CMP AX, 0");
			code += indent("JE " + label1);
			code += indent("MOV AX, 0");
			code += indent("JMP " + label2);
			code += (label1 + ":\n");
			code += indent("MOV AX, 1");
			code += (label2 + ":\n");
			code += indent("MOV " + src->symbol + ", AX");
			code += ("\t\t\t;NOT OPERATION ENDS for " + left->getName() + "\n");			
			src->code = code ;
		}
		else if(op == "-"){
			//NEG OPERATION
			string symbol = newTemp();
			src->symbol = symbol ;
			string code = left->code;
			code += ("\t\t\t;NEG OPERATION starts for " + left->getName() + "\n");
			code += ("\tMOV AX, " + left->symbol + "\n");
			code += ("\tNEG AX\n");
			code += ("\tMOV " + src->symbol + ", AX\n");
			code += ("\t\t\t;NEG OPERATION ENDS for " + left->getName() + "\n");			
			src->code = code ;

		}

		printDeb("\n*******************Inside mulop printing   left, exp, right, src serially**************n");
		printDeb("Left: " + left->getPrintForTable() + "\nexp: " + expression->getName()
			 + "\nSrc: " + src->getPrintForTable());
		printDeb("\n==>>>After addop, src = " + src->getPrintForTable() +
		 "\n****************************DONE mulop****************************n");



		return src; 

	}

	SymbolInfoNode *if_op(SymbolInfoNode *src,  SymbolInfoNode *expression, SymbolInfoNode *statement){

/*
	$$=$3;
	
	char *label=newLabel();
	$$->code+="mov ax, "+$3->getSymbol()+"\n";
	$$->code+="cmp ax, 0\n";
	$$->code+="je "+string(label)+"\n";
	$$->code+=$5->code;
	$$->code+=string(label)+":\n";

*/

		//dol_3 is expression
		src = new SymbolInfoNode();
		setSymbol(src, expression, ("if(" + expression->getName() + ")\n" + statement->getName()));
		src->symbol = "IF_SYMBOL";

		string code = expression->code ;
		string label = newLabel();
		code += ("\t\t; IF starts ( " + expression->getName() + ")\n");
		code += ("\tMOV AX, " + expression->symbol + "\n");
		code += ("\tCMP AX, 0\n");
		code += ("\tJE " + label + "\n");
		code += statement->code ;
		code += (label + ":\n");

		src->code = code ;
		printDeb("\n======++>>>=======Inside if printing  exp, statement, src  <<<<<<<<===========n");
		printDeb("exp: " + expression->getPrintForTable() + "\n");
		printDeb("statement: " + statement->getPrintForTable());
		printDeb("\n==>>>After op, src = " + src->getPrintForTable() +
		 "\n=============++>>>>>> DONE if<<===========================n");



		return src; 
	}


	SymbolInfoNode *ifElse_op(SymbolInfoNode *src, SymbolInfoNode *expression, SymbolInfoNode *ifStatement, 
		SymbolInfoNode *elseStatement){
		
		/*
			$$=$3;
			
			char *label=newLabel();
			$$->code+="mov ax, "+$3->getSymbol()+"\n";
			$$->code+="cmp ax, 0\n";
			$$->code+="je "+string(label)+"\n";
			$$->code+=$5->code;
			$$->code+=string(label)+":\n";

		*/

		src = new SymbolInfoNode();
		setSymbol(src, expression, ("if(" + expression->getName() + ")\n" + ifStatement->getName() + "else\n" + elseStatement->getName()));

		src->symbol = "IF_ELSE_SYMBOL";

		string comp = "if(" + expression->getName() + ")" ;

		string code = expression->code ;
		string else_label = newLabel();
		string end_label = newLabel();
		code += ("\t\t; IF_ELSE starts ( " + expression->getName() + ")\n");
		code += ("\tMOV AX, " + expression->symbol + "\n");
		code += ("\tCMP AX, 0\n");
		code += ("\tJE " + else_label + "\n");
		code += ifStatement->code ;
		code += ("\tJMP " + end_label + "\n");
		code += (else_label + ":\t; This is the else label \n");
		code += elseStatement->code;
		code += (end_label + ":\t;This is the END_IF Label for the " + comp + " \n");
		src->code = code ;
		printDeb("\n======++>>>=======Inside else if printing   src  <<<<<<<<===========n");
	 
		printDeb("\n==>>>After op, src = " + src->getPrintForTable() +
		 "\n=============++>>>>>> DONE  else if<<===========================n");

		return src; 

	}


	SymbolInfoNode *incOrdec(SymbolInfoNode *src, SymbolInfoNode *var, SymbolInfoNode* operation){
		src = new SymbolInfoNode();
		string name = var->getName() + operation->getName();
		setSymbol(src, var, name);
		//string temp = newTemp();
		src->symbol = var->symbol;
		string code = var->code ;

		printDeb("\n====>>>>Inside incOrdec .. printing var : " + var->getPrintForTable() + ", op : " + operation->getName());

		string op = operation->getName();

		if(op == "++"){
			code += ("\tINC " + src->symbol + "\n");
		}
		else if(op == "--"){
			code += ("\tDEC " + src->symbol + "\n");
		}

		src->code = code ;
		return src ;
	}



	SymbolInfoNode *ForLoop(SymbolInfoNode *src, SymbolInfoNode *initial, SymbolInfoNode *middle, SymbolInfoNode *final, 
		SymbolInfoNode *statement){
		src = new SymbolInfoNode();
		string name = "for(" + initial->getName() + middle->getName() + final->getName() + ")\n" + statement->getName(); 
		setSymbol(src, initial, name);
		

		//$$->setSymbol($1);$$->setUnion($2);$$->setUnion($3);$$->setUnion($4);$$->setUnion($5);$$->setUnion($6);
	  	//$$->setUnion("\n"); $$->setUnion($7);

		/*
		$$ -> FOR LPAREN expression_statement expression_statement expression RPAREN statement
								$3					$4					$5				$7
							initial 				middle				final 			statement
			$3's code at first, which is already done by assigning $$ = $3
			create two labels and append one of them in $$->code
			compare $4's symbol with 0
			if equal jump to 2nd label
			append $7's code
			append $5's code
			append the second label in the code
		*/
		printDeb("=========================== <<<< FOR LOOP STARTS !! >>> ===========================");
		printDeb("initialization : " + initial->getPrintForTable());
		printDeb("condition: " + middle->getPrintForTable());
		printDeb("increment/decrement: " + final->getPrintForTable());
		printDeb("statements : " + statement->getPrintForTable());
		

		string loop_label = newLabel();
		string end_label = newLabel();

		string code = "\t\t;For loop starts ... for(" + initial->getName() + middle->getName() + final->getName() + ")\n"; 
		code += initial->code ;
		//code += ("\tMOV AX, " + initial->symbol + "\n");
		code += (loop_label + ":\t\t;First Label of the for loop\n");
		code += middle->code ;
		code += ("\t\t;Checking The conditions in loop " + middle->getName() + "\n");
		code += ("\tMOV AX, " + middle->symbol + "\n");
		code += ("\tCMP AX, 0\n");
		code += ("\tJE " + end_label);
		code += ("\t\t;Writing the statements of for loop now ..\n");
		code += (statement->code);
		code += ("\t\t;Writing the final conditions i.e. increment/decrement..\n");
		code += (final->code);
		code += ("\tJMP " + loop_label + "\n");
		code += (end_label + ":\t\t;This is the end loop label .\n");

		src->symbol = "FOR_LOOP";
		src->code = code ;

		printDeb("Before exit. src: " + src->getPrintForTable());
		printDeb("\n=========================== <<<< DONE WITH FOR LOOP >>> ===========================\n");
		return src ;
	}

	SymbolInfoNode *whileLoop(SymbolInfoNode *src, SymbolInfoNode *exp, SymbolInfoNode *statement){
		//$$->setSymbol($1);$$->setUnion($2);$$->setUnion($3);$$->setUnion($4);$$->setUnion("\n");$$->setUnion($5);
		//WHILE LPAREN expression RPAREN statement

		src = new SymbolInfoNode();
		string name = "while(" + exp->getName() + ")\n" + statement->getName(); 
		setSymbol(src, exp, name);

		string loop_label = newLabel();
		string end_label = newLabel();

		string code = "\t\t;WHILE LOOP STARTS .. while(" + exp->getName() + ")\n";
		code += (loop_label + ":\t\t;This is Loop Label\n");
		code += exp->code ;
		code += ("\tMOV AX, " + exp->symbol + "\n");
		code += ("\tCMP AX, 0\n");
		code += ("\tJE " + end_label + "\n");
		code += (statement->code);
		code += ("\tJMP " + loop_label + "\n");
		code += (end_label + ":\t\t;This is end label\n");
		//code += statement->code ;
		//code += ("\t\tThe above is to make sure while lop runs perfect number of times.\n");
		code += ("\t\t\t;While Loop Over.\n\n");

		src->symbol = "WHILE_LOOP";
		src->code = code ;

		return src; 
	}


	SymbolInfoNode* defineFunc(SymbolInfoNode *src, string funcName, SymbolInfoNode *statements, const BisonUtil &butil, 
		int currentScope ){
		src = new SymbolInfoNode();
		setSymbol(src, new SymbolInfoNode(funcName, "ID"));

		string code = "";
		string initial = "";
		
		

		if(funcName == "main"){
		 	initial += "MAIN PROC\n";
			initial += "\tMOV DX, @DATA\n";
			initial += "\tMOV DS, DX\n\n";
			code += initial;
		}
		else{
			code += ( "\n" + funcName + " PROC");
		}
		printDeb("\n=======================+++ <<<< FUNCTION DEFINITION >>>> ==========================\n");

		printDeb("function name : " + funcName);
		printDeb("statements is : " + statements->getPrintForTable());

		string str = "Paramater list is : size = " + converter.integerToString((int)butil.currentFunctionList.size())+ " \n";
		int size = (int)butil.currentFunctionList.size();
		for(int i=0; i<size; i++){
			str += (butil.currentFunctionList[i]->getPrintForTable());
			str += "\n";
		}
		printDeb(str);


//		string code = indent(statements);

		if(funcName != "main"){
			//Setting the parameters now
			code += ("\n\t\t;Setting parameters from stack By popping in reverse order\n");
			string funcRes = "res_" + funcName; 
			code += ("\tPOP " + funcRes + "\t\t;To pop initial stack item into resultVariable first\n");
			for(int i=(int)butil.currentFunctionList.size()-1; i>=0; i--){
				string symb = butil.currentFunctionList[i]->symbol ;
				code += ("\tPOP " + symb + "\n");
			}
			//Setting parameters done from stack..
			code += ("\tPUSH " + funcRes + "\t\t;To push the initial stack item back into stack\n");
			code += indent(getRegisterPush());
		}

		string statement_indent = (statements->code);		
		code += statement_indent;




		if((funcName != "main")){

			//move the result i.e. content of AX back to the result var associated with this function..
			string resultVar = getResult(funcName);
			
			if((butil.currentFunctionReturnType == "void") || (butil.currentFunctionReturnType == "VOID")){
				//Do nothing				
				code += indent(getRegisterPop());
				code += ("\tRET\n");
				code += (funcName + " ENDP\n\n");
			}else{
				//code += ("\n\tMOV " + resultVar + ", AX\t\t;Store result back to the res var of the function " + funcName + "\n");
				code += (funcName + " ENDP\n\n");
			}

			
		}
		else{
			code += ("\n\t\t;Return DOS Control\n");
			code += ("\tMOV AH, 4CH\n\tINT 21H\nMAIN ENDP\n\n");
			//code += ("\nMAIN ENDP\nJMP LABEL_END\t\t;Jump to END MAIN line \n");
		}

		

		src->code = code;


		//Change parameters' symbols

		SymbolInfoNode *functionSrc = butil.tab->LookUp(funcName);
		if(functionSrc){
			for(int i=0; i<(int)functionSrc->functionItems.list.size(); i++){
				functionSrc->functionItems.list[i].symbol = butil.currentFunctionList[i]->symbol;
			}
		}

		printDeb("AFTER changing parameters ... printing those parameters for the funciton ... ");

		if(functionSrc){
			string x = functionSrc->getEverything_3();
			printDeb(x);
		}else{
			printDeb("Function Name with name: " + funcName + " DOES not exist in ST yet!");
		}



		printDeb("\n=======================+++ <<<< FUNCTION DEFINITION over  >>>> ==========================\n");


		return src ;

	}



	void pushParameters( const BisonUtil &butil){
		if(butil.currentFunctionName == "main"){
			//DO NOTHING
		}
		else{
			int size = (int)butil.currentFunctionList.size();
			for(int i=0; i<size; i++){
				string name = butil.currentFunctionList[i]->getName();
				SymbolInfoNode *res = butil.tab->searchCurrent(butil.currentFunctionList[i]);
				//name += ("_" + converter.integerToString(butil.tab->getCurrentScopeID()));
				name += ("_" + butil.currentFunctionName);
				makeVar(name);
				if(res){
					printDeb("============|]]]]]]]]]]]]]] FOUND in ST parameter ... changin symbol ");
					res->symbol = name;
				}
				butil.currentFunctionList[i]->symbol = name; 
			}
			printDeb("=======================================>>>>>>>>>>>>>>>>>AFTER CHANGING parameterss .. printin them");
			for(int i=0; i<size; i++){
				string s = butil.currentFunctionList[i]->getPrintForTable();
				printDeb(s + "\n");
			}

	 		string funName = butil.currentFunctionName ;
			string resName = newResult("res_" + funName);
			FunResult fr(funName, resName);
			this->funResults.push_back(fr);
		}
	}

	SymbolInfoNode *returnFun(SymbolInfoNode *src, SymbolInfoNode *exp, const BisonUtil &butil){
		  	//$$->setSymbol($1); $$->setUnion(" ") ;$$->setUnion($2);$$->setUnion($3);
		src = new SymbolInfoNode();
		string funName = butil.currentFunctionName;

		string name = "return " + exp->getName() + ";";
		src->symbol_name = name ;
		string code = exp->code ;
		code += ("\tMOV AX, " + exp->symbol + "\t;Store result temporarily in AX\n");
	 	


	 	if(butil.currentFunctionName != "main"){
 			string resultVar = "res_" + butil.currentFunctionName;
		 	string funcName = butil.currentFunctionName;
		 	code += ("\n\tMOV " + resultVar + ", AX\t\t;Store result back to the res var of the function " + funcName + "\n");
			code += indent(getRegisterPop());
			code += ("\tRET\n");
	 	}

		src->code = code ;
		return src ;
	}

	SymbolInfoNode *funcCall(SymbolInfoNode *src, SymbolInfoNode *func,SymbolInfoNode *args ,const BisonUtil &butil){
		src = new SymbolInfoNode();
		int line = func->getLineBegin();
		func = butil.tab->LookUp(func->getName());
		if(!func){
			return 0;
		}
		setSymbol(src, func);
		src->symbol_name = (func->getName() + "(" + args->getName() + ")");
		src->setLineBegin(line);

		string str = "";
		str += ("===========================+++<<<< INSIDE functionCAll >>>=================================\n");
		str += ("func: " + func->getPrintForTable());
		str += ("Printing its parameters ..size = " + converter.integerToString(func->functionItems.list.size()) + "\n");
		for(int i=0; i<(int)func->functionItems.list.size(); i++){
			Variable var = func->functionItems.list[i];
			str += var.getVariable();
			str += "\n";
		}

		str += ("Now printing arguments... , size = " + converter.integerToString((int)butil.currentArgumentList.listArguments.size()) + "\n");

		for(int i=0; i<(int)butil.currentArgumentList.listArguments.size(); i++){
			Variable var = butil.currentArgumentList.listArguments[i];
			str += var.getVariable();
			str += "\n";
		}
		bool isRecursion = false ;
		string code = "\n\t\t;STARTING function call " + func->getName() + "\n";

		//Check for recursion begin
		if(func->getName() == butil.currentFunctionName){
			isRecursion = true;
		}
		if(isRecursion == true){
			code += ("\n\t\t;Recursion has occurred need to push local variables and parameters into stack\n");
			for(int i=0; i<(int)declaredVariables.size(); i++){
				//code += ("\t\t;Name[" + converter.integerToString(i) + "] : " + declaredVariables[i]->getName());
				//code += (" and symbol is " + declaredVariables[i]->symbol + "\n");
				code += getCode(declaredVariables[i], "PUSH");
			}
			code += indent("\t;Local variables pushing is done");
			code += indent("\n\t;Now we push paramters into stack .. ");
			for(int i=0; i<(int)butil.currentFunctionList.size(); i++){
				code += getCode(butil.currentFunctionList[i], "PUSH");
			}
			code += indent("\t;Parameter push is over and Recursion handling is done");
		}
		//Check for recursion done

		code += ("\t\t;Setting args .. number of args = " + converter.integerToString(butil.currentArgumentList.listArguments.size()) + "\n");
		printDeb("==========++>>>>>>>>>>>>>>>>===============+>>>>>> CHECKING Code of args ... ");
		string c = "";
		for(int i=0; i<(int)butil.currentArgumentList.listArguments.size(); i++){
			c += butil.currentArgumentList.listArguments[i].code ;			//4 am Thursday ADDED
			printDeb("CHECK CODE FOR name: " + butil.currentArgumentList.listArguments[i].name +
			   ", symbol: " + butil.currentArgumentList.listArguments[i].symbol + 
				", code :\n" + butil.currentArgumentList.listArguments[i].code + "\nDONE\n");
			c += "\n\tMOV AX, " + butil.currentArgumentList.listArguments[i].symbol + "\n";
			//c += ("\tMOV " + func->functionItems.list[i].symbol + ", AX\n");
			c += ("\tPUSH AX\t\t;Push the argument " + butil.currentArgumentList.listArguments[i].name + " in stack\n");
		}
		code += c;
		code += ("\t\t;Paramater setting with arguments is done, now we call the function .. \n");
		code += ("\tCALL " + func->getName() + "\n");
		
		if(isRecursion == true){
			code += indent("\t;Now we pop parameters but in reverse order.");

			for(int i=(int)butil.currentFunctionList.size()-1; i>=0; i--){
				code += getCode(butil.currentFunctionList[i], "POP");
			}
			code += ("\t\t;Popping is done for parameters\n");

			code += indent("\t;Now pop local variables in reverse order.");

			for(int i=(int)declaredVariables.size()-1; i>=0; i--){
				code += getCode(declaredVariables[i], "POP");
			}
			code += ("\t\t;Popping is over for local variables in reverse order ..\n");
			code += indent("\t\t;Popping is done for recursion.\n");
		}

		src->symbol = func->symbol ;
		src->code = code ;

		printDeb("========++>>>>>>>>>>src->symbol = " + src->symbol);
		str += ("=================================++[[[ DONE functionCAll ]]]================================\n");
		printDeb(str);
		return src; 
	}

	string getCode(SymbolInfoNode *node, string op){
		printDeb("==========>>>Inside getCode for name : " + node->getPrintForTable() + ", symbol is " + node->symbol + 
			"\ncode is " + node->code);
		if(node->isArray == false){
			return ("\t" + op + " " + node->symbol + "\n");
		}

		//This is for array
		int sX = node->arrayItems.size - 1;
		string sizeArray = converter.integerToString(sX);
		string loop_label = newLabel();
		string end_label = newLabel();

		string str = "\t\t;This is array of name : " + node->getName() + " and size : " + converter.integerToString(node->arrayItems.size) + "\n";

		str += indent("MOV primary_variable, CX\t\t;To store the current value of CX");
		str += indent("XOR CX, CX\t\t;CX is the counter variable = 0");
		if(op == "PUSH"){
			str += indent("XOR SI, SI\t\t;Initialize SI as 0");
		}else{
			int final = 2*(node->arrayItems.size) - 2;
			str += indent("MOV SI, " + converter.integerToString(final) + "\t\t;Initialize SI as size*2 - 2");
		}
		str += (loop_label + ":\n");
		if(op == "PUSH"){
			str += indent("MOV AX, " + node->symbol + "[SI]");
			str += indent(op + " AX");
		}else{
			str += indent(op + " AX");
			str += indent("MOV " + node->symbol + "[SI], AX");
		}
		str += indent("CMP CX, " + sizeArray);
		str += indent("JGE " + end_label);
		
		if(op == "PUSH"){
			str += indent("ADD SI, 2");
		}else{
			str += indent("SUB SI, 2");
		}
		str += indent("INC CX");
		str += indent("JMP " + loop_label);
		str += (end_label + ":\n\tMOV CX, primary_variable\t\t;To restore original value of CX\n");
		str += indent("\t\t;Array " +  op + " into stack is done");
		return str ;
	}



	void declareFunction(string funcName, const BisonUtil &butil){
		string result = converter.makeName("res", funcName);
		makeVar(result);
		SymbolInfoNode *res = butil.tab->LookUp(funcName);
		if(res){
			res->symbol = converter.makeName("res", funcName);
		} 
	}

	void declareFunction(string funcName, string paramName, const BisonUtil &butil){

		printDeb("=================================****Inside declareFunction[2]****====================================");

		printDeb("Function name: " + funcName);
		printDeb("Parameter name: " + paramName);

		int size = (int)butil.currentFunctionList.size();
		printDeb("Printing parameters from butil... , size.param = " + converter.integerToString(size));
		for(int i=0; i<size; i++){
			string s = butil.currentFunctionList[i]->getPrintForTable();
 
			string name = butil.currentFunctionList[i]->getName();
			string var = converter.makeName(name, funcName);
			printDeb("Name: " + name + ", var name: " + var);
			makeVar(var);

		}
		string result = converter.makeName("res", funcName);
		printDeb("Result variable name: " + result);
		makeVar(result);

		printDeb("Getting from ST and setting the symbols accordingly..");

		SymbolInfoNode *res = butil.tab->LookUp(funcName);

		if(res){
			printDeb("Result found in ST !!, res->isFunction =  "  + converter.boolToString(res->isFunction));
			int size = res->functionItems.list.size();
			printDeb("Printing parameters ... size = " + converter.integerToString(size));
			for(int i=0; i<size; i++){
				string name = res->functionItems.list[i].name ;
				string varName = converter.makeName(name, funcName);
				res->functionItems.list[i].symbol = varName;
				
				Variable var = res->functionItems.list[i];
				string s = var.getVariable();
				printDeb(s);

				res->symbol = converter.makeName("res", funcName);
			}
		}else{
			printDeb("RESULT not found in ST!!");
		}

		printDeb("=================================****Ending declareFunction[2]****====================================");

	}


	void removeDeclaredVariables(){
		int size = (int)declaredVariables.size();
		if(size != 0){
			declaredVariables.clear();
		}
	}

	void pushDeclaredVariables(SymbolInfoNode *node){
		SymbolInfoNode *newNode = new SymbolInfoNode();
		printDeb("===>>>Inside pushDeclaredVariables .. node is " + node->getPrintForTable());
		newNode->makeNode(node);
		declaredVariables.push_back(newNode);
	}

















































	string getResult(string fun){
		for(int i=0; i<(int)funResults.size(); i++){
			FunResult fr = funResults[i];
			if(fr.funName == fun){
				return fr.resName;
			}
		}
		return "NO_RESULT";
	}

	string indent(string s){
		vector<string> line = converter.splitString(s, '\n');
		string res = "";
		if(line.size() == 0){
			return ("\t" + s + "\n");
		}
		for(int i=0; i<(int)line.size(); i++){
			string str = line[i];
			string s1 = "\t" + str + "\n";
			res += s1; 
		}
		return res; 
	}



	string getRegisterPop(){
		string s = "\n\t\t;POP ALL REGS begin";
		s += ("\nPOP DX\nPOP CX\nPOP BX\nPOP AX\n");
		s += ("\t\t;POP ALL REGS ends\n\n");
		return s ;
	}

	string getRegisterPush(){
		string s = "";
		s += ("\n\t\t;PUSH ALL REGS begin");
		s += ("\nPUSH AX\nPUSH BX\nPUSH CX\nPUSH DX\n");
		s += ("\t\t;PUSH ALL REGS end\n\n");
		return s ;
	}


	void setSymbol(SymbolInfoNode *n, SymbolInfoNode *node, string name){
		if(!n){
			n = new SymbolInfoNode();
		}
		//n->symbol_name = node->getName();
		n->symbol_type = node->getType();
		n->dataType = node->dataType;
		n->setLineBegin(node->getLineBegin());
		n->isArray = node->isArray;
		n->isFunction = node->isFunction;
		n->code = node->code ;
		n->symbol = node->symbol;

		if(n->isArray){
			n->arrayItems.changeArray(node->arrayItems);
		}
		if(n->isFunction){
			n->functionItems.changeFunction(node->functionItems);
		}
		n->setLineBegin(node->getLineBegin());
		n->next = 0 ;
		n->symbol_name = name; 
		//printDeb("-------------------___>>>>>After setSymbol.. printing src = " + n->getPrintForTable());
	}

	void setSymbol(SymbolInfoNode *n, SymbolInfoNode *node){
 		setSymbol(n, node, node->getName());
	}


	void printFullCode(SymbolInfoNode *node, const BisonUtil &butil){
		printCode(node->code , butil);

	}


	void printCode(string code, const BisonUtil &butil){
		if(butil.currentFunctionName == "main"){
			printMain(code);
		}else{
			printMain(code);
		}
	}

	void printCode(string code, string currentFunc){
		if(currentFunc == "main"){
			printMain(code);
		}else{
			printMain(code);
		}
	}

	SymbolInfoNode *getSymbolFromST(SymbolInfoNode *node, const BisonUtil &butil){
		string name = getArrayName(node->getName());
		SymbolInfoNode *res = butil.tab->LookUp(name);
		return res;

	}
  	string getArrayName(string arr){
  		string lName = "";
  		for(int i=0; i<arr.length(); i++){
  			if(arr[i] == '['){
  				break;
  			}else{
  				lName += arr[i];
  			}
  		}
  		return lName;
  	}


  	void printFinalCode(){
  		



		string initial = ".MODEL SMALL\n.STACK 100H\n";
		codeOut << initial << endl ;


  		//PRINTING DATA SEGMENT !! 
  		
  		//Printing from set.
  		//printDeb("==============+*Printing dataSet*+================ size = " + converter.integerToString(dataSet.size()));
  		
  		codeOut << ".DATA\n";  		
  		for(set<string>::iterator it = dataSet.begin(); it != dataSet.end(); it ++){
  			string line  = *it ;
  			//printDeb(line + "\n");
  			codeOut << line << endl ;
  		}


  		codeOut << "\n";
  		//PRINTING CODE SEGMENT !!
  		codeOut << ".CODE" << endl << endl ;

  		//Other Functions
  		for(int i=0; i<(int)otherFunctions.size(); i++){
  			codeOut << otherFunctions[i] << endl ;
  		}


  		//Main Function
  		for(int i=0; i<(int)mainLines.size(); i++){
  			codeOut << mainLines[i] << endl; 
  		}

  		//codeOut << "\nMAIN ENDP\nEND MAIN\n\n";
  	}

};

/*

		if(op == "++"){
			code += ("\t\t;INCOP of variable " + var->getName() + "begins.\n");
			code += ("\tMOV AX, " + var->symbol + "\n");
			code += ("\tINC AX\n");
			code += ("\tMOV " + src->symbol + ", AX\n");
			code += ("\t\t;INCOP ends of " + var->getName() + "\n");
		}
		else if(op == "--"){
			code += ("\t\t;DECOP of variable " + var->getName() + "begins.\n");
			code += ("\tMOV AX, " + var->symbol + "\n");
			code += ("\tDEC AX\n");
			code += ("\tMOV " + src->symbol + ", AX\n");
			code += ("\t\t;DECOP ends of " + var->getName() + "\n");
		}


*/









	/*string getSymbolFromFunction(SymbolInfoNode *idName, const BisonUtil &butil){
		string s = "";

		string id = idName->getName();

		SymbolInfoNode *res = butil.tab->searchCurrent(idName);
		if(res){
			//Is already defined in this function ...
		}else{
			//Not defined in this function ...
			int size = (int)butil.currentFunctionList.size();
			for(int i=0; i<size; i++){
				if(butil.currentFunctionList[i]->getName() == id){
					//Match found with parameter....
					return butil.currentFunctionList[i]->symbol;
				}
			}
		}

		return s; 
	}*/

	// /DOESNOT WORK 