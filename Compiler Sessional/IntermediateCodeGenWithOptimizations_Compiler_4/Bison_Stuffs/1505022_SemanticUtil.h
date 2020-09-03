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

#include "1505022_BisonUtil.h"
#include "../Miscalenous/Converter.h"

#define DODO "DODO"
#define DUMMY "dumVal"
using namespace std ;


class SemanticUtil{
 
public :
 	Converter converter;
 	bool checkFloatFlag ;
 	string errorVoid = "NOTHING ASSIGNED YET!!" ;
	void printSemantic(string s, BisonUtil butil)
	{
		butil.printSemantic(s);
	}

	string getString(int x){
		return converter.integerToString(x);
	}
	string getString(float x){
		return converter.floatToString(x);
	}

	void printSemantic(string str, const BisonUtil &butil, SymbolInfoNode *node1, SymbolInfoNode *node2){
		string s = "";
		s += "\n=======================<< In printSemantic function >>============================\n";
		s += str ;
		s += "\n";
		s += node1->getPrintForTable();
		s += "\n";
		s += node2->getPrintForTable();
		
		s += "\n===================== << DONE >> =================================\n";
		butil.printSemantic(s);
	}
	void printSemantic(SymbolInfoNode *node, const BisonUtil &butil){
		string s = "";
		s += "\n=======================<< In printSemantic function >>============================\n";

		s += node->getPrintForTable();

		s += "\n===================== << DONE >> =================================\n";
		butil.printSemantic(s);
	}
	void checkAndChangeInSymbolTable(SymbolInfoNode *node1,  const BisonUtil &butil){
		string s ;
 
		butil.printSemantic("Line 53 done.");
		SymbolInfoNode *res = butil.searchForUndeclaredID(node1);
		butil.printSemantic("Line 57 done.");
 
		if(res){
			node1->makeNode(res);
		}
		else{
			s = "At Line. " ;
			s += butil.getString(node1->getLineBegin());
			s += " , variable undeclared.";
			s += " variable name is " ;
			s += node1->getName();
			butil.printError(s);
		}
	}
	string getNameOfType(string type){
		if(type == "INT"){
			return "int";
		}
		if(type == "FLOAT")
			return "float";
		if(type == "VOID")
			return "void";
		return "NIL";
	}
	string integerTrimmed(string s){
		float x = converter.stringToFloat(s);
		int z = (int)x;
		return converter.integerToString(z);
	}
	bool isValidNameOfID(string id, const BisonUtil &butil){
		butil.printSemantic("\n??????????=>>>>>> Inside isValidNameOfID(..) .. printing id = " + id);
		int len = id.length(); 
		for(int i=0; i<len; i++){
			if((id[i] == '{') ||(id[i] == '}') ||(id[i] == '(') ||(id[i] == ')') ||(id[i] == '[') ||(id[i] == ']') ||(id[i] == '+') 
				||(id[i] == '-')||(id[i] == '/')||(id[i] == '?')||(id[i] == '<')||(id[i] == '>')||(id[i] == '=')){
				return false;
			}
		}
		char charArr[11] = {'0','1','2','3','4','5','6','7','8','9'};
		for(int i=0; i<11; i++){
			if(id[0] == charArr[i])
				return false;
		}
		butil.printSemantic("RETURNING true.\n");
		return true ;
	}	
	void assignOpArray(SymbolInfoNode *src, SymbolInfoNode *lhs, SymbolInfoNode *rhs, const BisonUtil &butil){


		string lName = lhs->getName();
 		string name = "";
 		for(int i=0; i<lName.length(); i++){
 			if(lName[i] == '[')
 				break ;
 			name += lName[i];
 		}

 		SymbolInfoNode *checkNode = new SymbolInfoNode(name, lhs->getType(), lhs->getLineBegin(), lhs->dataType);
 		checkNode->isArray = lhs->isArray;
 		checkNode->isFunction = lhs->isFunction;
 		checkNode->actualValue = lhs->actualValue;

		SymbolInfoNode *res = checkDeclaredIDArray(checkNode, butil, false);
		SymbolInfoNode *l ;
		if(res){
			l = res ;
			butil.printSemantic("FOUND res.. printing res =" + res->getPrintForTable());
		}else{
			l = lhs ;
			butil.printSemantic("RES DOESNOT EXIST HERE...");
		}

		string s = "++==----++==--->>> Inside assignOpArray ... printing lhs(res), rhs\n";
		s += l->getPrintForTable();
		s += "\n";
		s += rhs->getPrintForTable();
		s += "\n";
		butil.printSemantic(s);

		if(res && rhs)
			butil.printSemantic("===========+>>>>>> Line 137 sutil. res->dataType = " + res->dataType + " , and rhs->dataType = " + rhs->dataType);
		if(res){
			if(res->dataType == "INT"){
				if(rhs->dataType == "FLOAT"){
					butil.printError("SEMANTIC WARNING at line. " + converter.integerToString(lhs->getLineBegin())
						+ ", Trying to assign float with int array.");
				}
			}
			else{
				if(checkFloatFlag == true){
					if(res->dataType == "FLOAT"){
						if(rhs->dataType == "INT"){
							butil.printError("SEMANTIC WARNING, at line. " + converter.integerToString(lhs->getLineBegin())
								+ ", Trying to assign int with float array.");
						}
					}
				}
			}
		}
		s += "\nFINALLY PRINTING src=" ;

		//DO THING FOR SOURCE AND PUSH IN VECTOR's INDEX...
		//,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,<<NNNNNEEEED TO EDIIT HEREEE >>>>> //////////////////////////////

		s += src->getPrintForTable();


		s += "\n<<<<<<<<<<<<<< DONE assignOpArray DONE >>>>>>>>>>>>>>>\n";

		butil.printSemantic(s);

	}

	void assignRightArray(SymbolInfoNode *src, SymbolInfoNode *lhs, SymbolInfoNode *rhs, const BisonUtil &butil)
	{
		string s = "\n\n===>>> Inside ==>> assignRightArray(...) \n";
		s += "Printing lhs, rhs\n lhs: ";
		s += lhs->getPrintForTable();
		s += "\nrhs: ";
		s += rhs->getPrintForTable();
		s += "\n<<<< DONE assignRightArray ... >>>> \n\n";
		butil.printArrayDebug(s);

		lhs = butil.tab->LookUp(lhs->getName());
		string lType, rType;
		if(lhs){
			if(rhs){
				if(rhs->isArray){
					lType = lhs->dataType;	rType = rhs->dataType;
					if((lType == "INT") && (rType == "FLOAT")){
						butil.printError("SEMANTIC WARNING at line. " + converter.integerToString(rhs->getLineBegin()) + 
							", trying to assign a float to an integer.");
					}else{
						if(this->checkFloatFlag == true){
							if((lType == "FLOAT") && (rType == "INT")){
								butil.printError("SEMANTIC WARNING at line. " + converter.integerToString(rhs->getLineBegin()) + 
									", trying to assign an integer to a float.");	
							}
						}
					}
				}
			}
		}
	}

	//>>>>>>>>>>>>>>>>NEED TO CHANGE HERE<<<<<<<<<<<<<<<<<< KISU EKTA BHUL HOISE ((4 * 3 ) == (3 * 4)) KAJ KORTESE NA
	void assignOp(SymbolInfoNode *src, SymbolInfoNode *lhs, SymbolInfoNode *rhs, const BisonUtil &butil)
	{
		//src is $$, lhs is $1, ASSIGNOP is $2 and rhs is $3
		string lhsType = lhs->dataType;
		string rhsType = rhs->dataType;
 		
		string lType = getNameOfType(lhsType);
		string rType = getNameOfType(rhsType);
		
		lhs->actualValue = rhs->actualValue;
		src->actualValue = lhs->actualValue;


		string strToPrint = "???????????? ========+>>>> INSIDE assignOp() <<<<<<<<===============\n";
		strToPrint += "lhs =" ;
		strToPrint += lhs->getPrintForTable();
		strToPrint += "\nrhs =";
		strToPrint += rhs->getPrintForTable();

		strToPrint += "?>?>?>?><?><?><?><?><?> DONE assignOp ><><>?>??????\n";
		


		butil.printArrayDebug(strToPrint);
		butil.printSemantic(strToPrint);

		

		string lName = lhs->getName();
		bool isThisArray = false;

		for(int i=0; i<lName.length(); i++){
			if(lName[i] == '['){
				isThisArray = true;
			}
		}
		
		

		if(isThisArray == true){
			assignOpArray(src, lhs, rhs, butil);
			return ;
		}


		//printf("2nd BLA BLA BLA BLA\n\n");

		bool flag = isValidNameOfID(lName, butil);

		if(lType == "NIL")
			return;
		if(rType == "NIL")
			return ;
		if((lhsType == "INT") && (rhsType == "FLOAT")){
			butil.printError("SEMANTIC WARNING at line. " + butil.getString(rhs->getLineBegin()) +  " , Trying to assign " + 
				lType + " with " + rType);
			return ;
			lhs->actualValue = integerTrimmed(rhs->actualValue);
		}
		else if((lhsType == "FLOAT") && (rhsType == "INT")){
			if(checkFloatFlag){
				butil.printError("SEMANTIC WARNING, at line. " + butil.getString(rhs->getLineBegin()) +  " , Trying to assign " + 
					lType + " with " + rType);
			}
			return ;
			lhs->actualValue = rhs->actualValue ;
		}
		else{
			lhs->actualValue = rhs->actualValue;
		}
		src->actualValue = lhs->actualValue;
		src->dataType = lhsType;
	

		SymbolInfoNode *res ;
		if(flag == true){
			res = butil.tab->LookUp(lhs->getName());
			
			//res->makeNode(lhs);

			string str = "========+>>>>>>INSIDE ASSIGNOP .. printing src, lhs, rhs, res \n";
			str += src->getPrintForTable();
			str += "\n";
			str += lhs->getPrintForTable();
			str += "\n";
			str += rhs->getPrintForTable();
			str += "\n";
			str += res->getPrintForTable();
			str += "\n??????????? <<<<<<<<<<< DONE >>>>>>>>>>>>>>>> ????????????????\n";
			butil.printSemantic(str);
		}
		else{
			//DO NOTHING...
			string str = "========+>>>>>>INSIDE ASSIGNOP .. printing src, lhs, rhs, res is null \n";
			str += src->getPrintForTable();
			str += "\n";
			str += lhs->getPrintForTable();
			str += "\n";
			str += rhs->getPrintForTable();
			str += "\n";
			
			str += "\n??????????? <<<<<<<<<<< DONE >>>>>>>>>>>>>>>> ????????????????\n";
			butil.printSemantic(str);

		}



		string s = "After ASSIGNOP of " + lhs->getName() + " with " + rhs->getName() + " , and printing table.\n" ;
		s += butil.tab->printCurrentNonEmpty();
		butil.printSemantic(s);	
		//butil.printArrayDebug(s);
		//$$->setSymbol($1); $$->setUnion(" ") ; $$->setUnion($2); $$->setUnion(" ") ;$$->setUnion($3);
	}



	void assignRightArrayWithArray(SymbolInfoNode *src, SymbolInfoNode *lhs, SymbolInfoNode *rhs, const BisonUtil &butil){

	}





	SymbolInfoNode *findFromST(SymbolInfoNode *node, const BisonUtil &butil){
		butil.printSemantic("--->>>Inside findFromST, printing node=" , node);
		string name = node->getName();
		SymbolInfoNode *res = butil.searchForUndeclaredID(node);

		//EDITED HERE FOR CHECKING LINE NUMBERS THIKASE NAKI NA!.
		//res->setLineBegin(node->getLineBegin());
		return res;
	}
	void checkType(SymbolInfoNode *lhs, SymbolInfoNode *rhs, const BisonUtil &butil){
		string lType = getNameOfType(lhs->dataType);
		string rType = getNameOfType(rhs->dataType);
		if(lType != rType){
			butil.printError("SEMANTIC WARNING at line. " + butil.getString(lhs->getLineBegin()) +  " , Trying to assign " + 
				lType + " with " + rType);
		}
	}
	void makeSymbol(SymbolInfoNode *src, SymbolInfoNode *dest){
		if(dest){
			src->setName(dest->getName());
			src->setLineBegin(dest->getLineBegin());
			src->dataType = dest->dataType;
			src->symbol = dest->symbol;
			src->code = dest->code ;
		}
	}
	void makeUnion(SymbolInfoNode *src, SymbolInfoNode *dest){
		if(dest){
			src->setName(src->getName() + dest->getName());
			src->setLineBegin(max(src->getLineBegin(), dest->getLineBegin()));
			src->code += dest->code;
		}
	}
	void operate(SymbolInfoNode *src, SymbolInfoNode *node1, SymbolInfoNode *operation, const BisonUtil &butil){
		butil.printSemantic("----------------->>Inside sutil.operate ... printing node1  =" + node1->getPrintForTable() + 
			"\nPrinting *operation =" + operation->getPrintForTable());
		SymbolInfoNode *res = butil.searchForUndeclaredID(node1);
	 	butil.printSemantic("===>>>printin res =" + res->getPrintForTable());

		string newVal ;
		string op = operation->getType();
		if(res->equals(node1)){
			newVal = operate(res, op);
			res->actualValue = newVal;
			//src->makeNode(res);
			//makeSymbol(src, node1); makeUnion(src, operation);
		}
		else{
			butil.printError("SEMANTIC Error at line " + getString(node1->getLineBegin()) + " , variable " + node1->getName()
			 + " is undeclared");
		}
	}
	string operate(SymbolInfoNode *node1, string operation){
		string thisValue = node1->actualValue;
		string newValue = DUMMY;
		string dataType = node1->dataType;
		if(dataType == "FLOAT"){
			if(operation == "INCOP"){
				float newVal = converter.stringToFloat(thisValue);
				newVal++;
				newValue = converter.floatToString(newVal);
			}
			else if(operation == "DECOP"){
				float newVal = converter.stringToFloat(thisValue);
				newVal--;
				newValue = converter.floatToString(newVal);
			}
		}
		else if(dataType == "INT"){
			if(operation == "INCOP"){
				int newVal = converter.stringToInteger(thisValue);
				newVal++;
				newValue = converter.integerToString(newVal);
			}
			else if(operation == "DECOP"){
				int newVal = converter.stringToInteger(thisValue);
				newVal--;
				newValue = converter.integerToString(newVal);
			}
		}
		return newValue;
	}
	void makeMulop(SymbolInfoNode *src, SymbolInfoNode *nodeLeft, SymbolInfoNode *nodeRight, SymbolInfoNode *operation, 
		const BisonUtil &butil){
		string op = operation->getName();
		
		if(nodeLeft->getType() != "CONST_INT"){
			if(nodeLeft->getType() != "CONST_FLOAT"){
				nodeLeft = butil.searchForUndeclaredID(nodeLeft);
			}
		}

		if(nodeRight->getType() != "CONST_INT"){
			if(nodeRight->getType() != "CONST_FLOAT"){
				nodeRight = butil.searchForUndeclaredID(nodeRight);
			}
		}

		string lhsType = nodeLeft->dataType;
		string rhsType = nodeRight->dataType;
		
		string s = "===>>>>>>>>>>>>Inside makeMulop ... printing nodeLeft, nodeRight, operation\n";
		s += nodeLeft->getPrintForTable();
		s += "\n"; s += nodeRight->getPrintForTable();
		s += "\n"; s += operation->getPrintForTable();
		string x = "\nlhsType = " + lhsType + " , rhsType = " + rhsType;
		s += x; 
		s += "\n=----------------== DONE ==-------------------=\n";
		butil.printSemantic(s);



		if(op == "%"){
			//Check both sides integer or not
			if((lhsType != "INT")||(rhsType != "INT")){
				//BOTH SIDES ARE NOT INTEGERS of MOD OPERATION..
				butil.printError("SEMANTIC Error at line. " + butil.getString(nodeLeft->getLineBegin()) 
					+ " , Both sides of modulus operator are not integer.");
				return ;
			}
			int lVal = converter.stringToInteger(nodeLeft->actualValue);
			int rVal = converter.stringToInteger(nodeRight->actualValue);
			int resVal ;
			
			if(rVal == 0){
				butil.printZero("ERROR, Trying to mod by 0 at line. " + butil.getString(nodeRight->getLineBegin()));
				resVal = -1;
				
			}else{
				resVal = lVal % rVal; 
			}

			string newString = converter.integerToString(resVal);
			src->actualValue = newString;
			src->dataType = "INT";
			butil.printSemantic("===>>>>>After MULOP ... printing without fullName\n$$=" + src->getPrintForTable());
		}
		else if(op == "*"){
			//Multiplication operation..
			butil.printSemantic("--->>> INSIDE MULOP(*) .. printing nodeLeft and nodeRight actualValues : ");
			butil.printSemantic( nodeLeft->actualValue + " , " + nodeRight->actualValue);
			string finalDataType ;	string result ;
			if(nodeLeft->dataType == "INT"){
				int lVal = converter.stringToInteger(nodeLeft->actualValue);
				if(nodeRight->dataType == "INT"){
					//int * int 
					int rVal = converter.stringToInteger(nodeRight->actualValue);
					int res = lVal * rVal;
					result = converter.integerToString(res);
					finalDataType = "INT";
				}
				else{
					int rVal = converter.stringToFloat(nodeRight->actualValue);
					float res = lVal * rVal;
					result = converter.floatToString(res);
					finalDataType = "FLOAT";
				}
			}
		 	else if(nodeLeft->dataType == "FLOAT"){
		 		float lVal = converter.stringToFloat(nodeLeft->actualValue);
				if(nodeRight->dataType == "INT"){
					//float * int
					int rVal = converter.stringToInteger(nodeRight->actualValue);
					float res = lVal * rVal;
					result = converter.floatToString(res);
					finalDataType = "FLOAT";
				}
				else{
					//float * float
					float rVal = converter.stringToFloat(nodeRight->actualValue);
					float res = lVal * rVal;
					result = converter.floatToString(res);
					finalDataType = "FLOAT";
				}
			}
			src->actualValue = result;
			src->dataType = finalDataType;
			butil.printSemantic("===>>>After MULOP (*) .. printing $$ =" + src->getPrintForTable());
		}
		else if(op == "/"){
			float lVal = converter.stringToFloat(nodeLeft->actualValue);
			float rVal = converter.stringToFloat(nodeRight->actualValue);
			float res ;
			if(rVal == 0){
				butil.printZero("ERROR, Trying to divide by 0 at line. " + butil.getString(nodeRight->getLineBegin()));
				res = -1;
				
			}else{
				res = lVal / rVal;
			}
			string result = converter.floatToString(res);
			string finalDataType = "FLOAT";
			src->actualValue = result;
			src->dataType = finalDataType;
			butil.printSemantic("===>>>After MULOP (/) .. printing $$ =" + src->getPrintForTable());

		}
	}
	void unaryOperation(SymbolInfoNode *src, SymbolInfoNode *node, SymbolInfoNode *operation, const BisonUtil &butil){
		string finalRes ;
		string finalDataType ;
		if(node->getType() == "CONST_FLOAT"){
			finalDataType = "FLOAT";
		}
		else if(node->getType() == "CONST_INT"){
			finalDataType = "INT";
		}
		else if(node->getType() == "ID"){
			node = butil.searchForUndeclaredID(node);
		}
		if(operation->getName() == "-"){
			float val = converter.stringToFloat(node->actualValue);
			val = (-1)*val;
			finalRes = converter.floatToString(val);
			finalDataType = node->dataType;
		}
		else if(operation->getName() == "!"){
			float val = converter.stringToFloat(node->actualValue);
			int res;
			if(val != 0){
				res = 0;
			}else{
				res = 1;
			}
			finalRes = converter.integerToString(res);
			finalDataType = "INT";
		}		
		src->actualValue = finalRes;
		src->dataType = finalDataType;
		string s = "=======>>>>> ====>>>> AFTER unaryOperation \n" + node->getPrintForTable() + " \n" ;
		s += "\nSrc =" + src->getPrintForTable();
		butil.printSemantic(s);
	}
	SymbolInfoNode *getNode(SymbolInfoNode * node, const BisonUtil &butil){
		butil.printSemantic("DEBUGGING Line 457 Inside getNode .. node =" + node->getPrintForTable());
		if(node->getType() != "ID"){
			return node;
		}
		SymbolInfoNode *res = butil.searchForUndeclaredID(node);
		return res ;
	}




 	void addSpace(SymbolInfoNode *src){
 		src->setName(src->getName() + " ");
 	}
 	void relAndLogicOp(SymbolInfoNode *src, SymbolInfoNode *lNode, SymbolInfoNode *rNode, SymbolInfoNode *operation, 
 		const BisonUtil &butil){
 		lNode = getNode(lNode, butil);
 		rNode = getNode(rNode, butil);
 		string finalRes;
 		string finalDataType = "INT";	//RELOP RESULT'S DATA TYPE IS INT
 		string op = operation->getName();

 		float lVal = converter.stringToFloat(lNode->actualValue);
 		float rVal = converter.stringToFloat(rNode->actualValue);
 		int res; 
 		
		if(op == "<"){
			 res = (lVal < rVal) ;
		}
		 else if(op == "<="){
			 res = (lVal <= rVal) ;
		}
		 else if(op == ">"){
			 res = (lVal > rVal) ;
		}
		 else if(op == ">="){
			 res = (lVal >= rVal) ;
		}
		 else if(op == "=="){
			 res = (lVal == rVal) ;
		}
		 else if(op == "!="){
			 res = (lVal != rVal) ;
		}
 		else if(op == "&&"){
			if((lVal == 0) || (rVal == 0)){
				res = 0;
			}else{
				res = 1;
			}
 		}
 		else if(op == "||"){
			if((lVal != 0) || (rVal != 0)){
				res = 1;
			}else{
				res = 0;
			}
 		}
		finalRes = converter.integerToString(res);
		src->dataType = finalDataType;
		src->actualValue = finalRes;
		
		string s = "=======>>>>> ====>>>> AFTER relAndLogicOp of \n" + lNode->getPrintForTable() + " \n" + rNode->getPrintForTable();
		s += "\nSrc =" + src->getPrintForTable();
		butil.printSemantic(s);
 	}

 	SymbolInfoNode * checkDeclaredIDArray(SymbolInfoNode *arrayName,  const BisonUtil &butil, bool doWePrint)
 	{
 		string name = arrayName->getName();
 		SymbolInfoNode *res = butil.tab->LookUp(name);
 		


		butil.printSemantic("=====?????----->>>>>>> INSIDE checkDeclaredIDArray ... \nprinting arrayName name =" + name +
			 "\narrayName =" 
			+ arrayName->getPrintForTable());
		string str = "++--------===++===>>Inside checkDeclaredIDArray ... printing ST\n";
 		str += butil.tab->printCurrentNonEmpty();
 		butil.printSemantic(str);
		if(!res){
			if(doWePrint){
				butil.printError("SEMANTIC Error at line. " + butil.getString(arrayName->getLineBegin()) +
					", variable with name =" + name + " was not declared\n");
			}
			butil.printSemantic("NAME " + name + " DOESNOT EXIST in ST.");	
			return 0;
		}
		else{
			//Check whether array or not.
			butil.printSemantic("=====?????----->>>>>>> INSIDE checkDeclaredIDArray ... printing res =" + res->getPrintForTable());

			bool flag = res->isArray;
			if(flag == false){
				if(doWePrint){
					butil.printError("Semantic Error, Trying to use ID " + name + " as an array in line. " + 
						butil.getString(arrayName->getLineBegin()) + 
						" but it was not declared as an array at line." + 
						butil.getString(res->getLineBegin()));
				}
				return 0;
			}
			//Check for function name.
			if(name == butil.currentFunctionName){
				if(doWePrint){
					butil.printError("Semantic Error, Trying to use ID " + name + " as an array int line. " + butil.getString(arrayName->getLineBegin())+
						" but it is a function in line. " + butil.getString(butil.currentFunctionLine));
				}
				return 0;
			}
			//Check for parameters.
 			for(int i=0; i<(int)butil.currentFunctionList.size(); i++){
 				if(butil.currentFunctionList[i]->getName() == name){
 					if(doWePrint){
	 					butil.printError("SEMANTIC Error, Array name = " + name + " is  same as name of parameter of function " + 
 							butil.currentFunctionName + " at line. " + butil.getString(butil.currentFunctionLine));
 					}
 					return 0;
 				}
 			}
 			//CHANCES OF SEGMENTATION FAULT ... THEN return arrayName instead of res.
 			//arrayName->makeNode(res);
			return res;
		}
		return 0;
 	}

 	void checkForArrayExpression(SymbolInfoNode* arrayName, SymbolInfoNode* expression, const BisonUtil &butil){


		SymbolInfoNode *res = checkDeclaredIDArray(arrayName, butil, true);
		if(!res){
			//DO NOTHING .. 
		}else{
			arrayName->makeNode(res);

		}
 		string rhsType = expression->dataType;
 		string lhsType = arrayName->dataType;


 		string s = "---->>> Inside checkForArrayExpression ... printin array\n";
 		s += arrayName->getPrintForTable();
 		s += "\nPrinting expression\n";
 		s += expression->getPrintForTable();
 		s += "\n=======>>> DONE WITH THIS SHIT NOW CHECKING FOR INVALID dataType\n";
 		butil.printSemantic(s);

 

 		bool flag = (rhsType == "FLOAT") || (rhsType == "INT") ;
 		bool flag2 =  (lhsType == "FLOAT") || (lhsType == "INT") ;
 		if(!((flag == true) && (flag2 == true))){
 			//Invalid data Type
 			butil.printSemantic("INVALID DATA TYPE IN checkForArrayExpression function LINE 483, dataType lhs/arrayName = " 
 								+ lhsType +
 			 					", dataType rhsType/expression = " + rhsType);
 			return;
 		}

 		/*if(lhsType == "FLOAT"){
 			return;
 		}*/

 		if(arrayName->isArray == false){
 			butil.printError("SEMANTIC Error, Trying to use third braces without declaring as an array in line. " + 
 				butil.getString(arrayName->getLineBegin()));
 		}
 		/*if((rhsType != "INT")){
 			butil.printError("SEMANTIC Error, The index of array " + arrayName->getName() + " is not an integer expression at line " 
 				+ butil.getString(arrayName->getLineBegin()));
 		}*/
 	}

 	bool isDuplicate(SymbolInfoNode *node, const BisonUtil &butil){
 		SymbolInfoNode *res = butil.tab->searchCurrent(node);	//Search for multiple declaration in same scope
 		string name = node->getName();

 		if(res){
 			butil.printError("SEMANTIC Error[Duplicate definition of array], variable named " + node->getName() + 
 				" was declared in line no. " + butil.getString(res->getLineBegin()));
 			return true;
 		}
 		else{
 			if(name == butil.currentFunctionName){
 				butil.printError("SEMANTIC Error[Duplicate definition of array, array name same as function name], variable named " + node->getName() + 
 				" was declared/defined in line no. " + butil.getString(butil.currentFunctionLine));
 				return true;
 			}
 			for(int i=0; i<(int)butil.currentFunctionList.size(); i++){
 				if(butil.currentFunctionList[i]->getName() == name){
 					butil.printError("SEMANTIC Error[Array name = " + name + " is  same as name of parameter of function " + 
 						butil.currentFunctionName + "] at line. " + butil.getString(butil.currentFunctionLine));
 					return true;
 				}
 			}
 		}
 		butil.printSemantic("-------===>>>> Inside isDuplicate .. printin Node =" + node->getName() + " and returning false  <<====== \n");
		return false;

 	}
 	void pushArray(SymbolInfoNode *node, SymbolInfoNode *sizeNode,const BisonUtil &butil){
 		int size = butil.getSize(sizeNode -> getName());
 		node->isArray = true ;
 		node->arrayItems.size = size;
 		node->arrayItems.arrayName = node->getName();
 		
 
 		butil.tab->Insert(node);
 		/*butil.tab->Insert(node->getName(), node->getType(), node->dataType, node->getLineBegin(), node->isArray, 
 			node->isFunction, node->actualValue, node->functionItems, node->arrayItems);*/

 		string s = "====++>>>>>>  AFTER pushArray ... printing symbol table.. ";
 		s += butil.tab->printCurrentNonEmpty();
 		s += "\n ======== >>> DONE WITH pushArray <<< =========\n";

 		butil.printSemantic(s);
 	}

 	void makeSymbol(SymbolInfoNode *src, SymbolInfoNode *n1, SymbolInfoNode *n2, SymbolInfoNode *n3, SymbolInfoNode *n4, 
 		const BisonUtil &butil){
 		string prevName = n1->getName();
 		string name = n1->getName() + n2->getName() + n3->getName() + n4->getName();
 		src->setName(name);
 		src->setLineBegin(min((min(n1->getLineBegin(), n2->getLineBegin()), n3->getLineBegin()) , n4->getLineBegin() ));
 		src->dataType = n1->dataType;
 		src->isArray = true;
 		src->arrayItems.size = butil.getSize(n3);
 		src->arrayItems.arrayName = n1->getName();

 		n1->setName(prevName);

 		if(n1 && n2 && n3 && n4){
	 		src->symbol = n1->symbol;
	 		src->code = (n1->code + n2->code + n3->code + n4->code);
 		}

 	}

 	string checkWhetherArgumentsMatch(SymbolInfoNode *functionNode, SymbolInfoNode *functionDefined ,
 		SymbolInfoNode *argumentList, const BisonUtil &butil){
 		string s = "=====-----+++++----->>>> Inside sutil.checkWhetherArgumentsMatch ... printing\n";
 		s += "functionNode =";
 		s += functionNode->getPrintForTable();
 		s += "\nfunctionDefined =";
 		s += functionDefined->getPrintForTable();
 		s += "\nargumentList =";
 		s += butil.currentArgumentList.getArgumentList();
 		s += "\nparameterList =";
 		s += functionDefined->functionItems.getFunction();
 		s += "\n\n";
 		butil.printSemantic(s);

 		butil.printSemantic("======+>>> sutil Line 746 checkWhetherArgument match done . ");

 		FunctionItems fun = functionDefined->functionItems;
 		if(fun.returnType == "VOID"){
 			string str = "SEMANTIC ERROR, at line. ";
 			str += converter.integerToString(functionNode->getLineBegin());
 			str += " , trying to use void function as part of expression.";
 			str += " with name = ";
 			str += fun.functionName;
 			str += " which was defined at line no.";
 			str += converter.integerToString(fun.lineNumberBegin);
 			//butil.printError(str);
 			this->errorVoid = str ;
 			functionNode->dataType = functionDefined->functionItems.returnType;
 			return "ERROR";
 		}
		int sizeArguments = (int)butil.currentArgumentList.listArguments.size();
		int sizeParam = (int)fun.list.size();

 		butil.printSemantic("======+>>> sutil Line 762 done . ");

		if(sizeParam != sizeArguments){
 			string str = "SEMANTIC ERROR, at line. ";
 			str += converter.integerToString(functionNode->getLineBegin());
 			str += " , function call parameter size and argument size doesnot match.";
 			str += " function was defined/declared at line no.";
 			str += converter.integerToString(fun.lineNumberBegin);
 			butil.printError(str);
	 		butil.printSemantic("======+>>> sutil Line 772 done . ");
 			return "ERROR";
		}
 		butil.printSemantic("======+>>> sutil Line 774 done . ");

		for(int i=0; i<sizeArguments; i++){
			if(fun.list[i].type != butil.currentArgumentList.listArguments[i].type) {
				//|| (fun.list[i].isArray != butil.currentArgumentList.listArguments[i].isArray))
	 			string str = "SEMANTIC ERROR, at line. ";
	 			str += converter.integerToString(functionNode->getLineBegin());
	 			str += " , function call parameter and arguments type mismatch.";
	 			str += " function was defined/declared at line no.";
	 			str += converter.integerToString(fun.lineNumberBegin);
	 			butil.printError(str);
	 			return "ERROR";
			}
		}

 		butil.printSemantic("======+>>> sutil Line 789 done returning fun.returnType = " + fun.returnType);

		return fun.returnType;

 	}

 	string functionCall(SymbolInfoNode *src, SymbolInfoNode *functionNode, SymbolInfoNode *argumentList, 
 		const BisonUtil &butil){
 		string s = "+++====--->>>Inside sutil.functionCall .. printing <<<>><=====\nfunctionNode =";
 		s += functionNode->getPrintForTable();
 		s += "\nargumentList =";
 		if(argumentList){
 			s += argumentList->getPrintForTable();
 		}else{
 			s += " EMPTY ARGUMENTS.. ";
 		}
 		s += "\n <========= <<< DONE in functionCall >>> ==========>\n";
 		butil.printSemantic(s);


 		SymbolInfoNode *functionChecker = butil.tab->LookUp(functionNode->getName());
 		if(functionChecker){
 			butil.printSemantic("----=====+++++====>>>> Printing functionChecker =" + functionChecker->getPrintForTable());
 		}else{
 			butil.printSemantic("==>> Inside sutil.functionCall .. NOT FOUND WITH THAT NAME " + functionNode->getName());
 		}
 		if(functionChecker){

 			//Check whether it is a function or not.
 			if(functionChecker->isFunction == false){
 				butil.printSemantic("\n===>>INSIDE functionChecker does exist and is not a function !!\n");
 				butil.printSemantic("<><>==>> functionChecker is not a function !!");
 				string str = "SEMANTIC ERROR, ID with name = ";
 				str += functionChecker->getName();
 				str += " was not declared as function in line. ";
 				str += converter.integerToString(functionChecker->getLineBegin());
 				str += " , but is being used as a function in line. ";
 				str += converter.integerToString(functionNode->getLineBegin()); 
 				butil.printError(str);
 				return "ERROR";
 			}else{
 				butil.printSemantic("---->>>> INSIDE functionCall ... found in ST and printing\n", functionNode);
 				//if(functionChecker->functionItems.declareOrDefine == "DECLARE"){
 				if(false){
 					string err = "SEMANTIC ERROR at line. " + converter.integerToString(functionNode->getLineBegin()) ;
 					err += " , function with name " ;
 					err += functionNode->getName();
 					err += " was not defined.";
 					butil.printError(err);
 					return "ERROR";
 				}
 				return checkWhetherArgumentsMatch(functionNode, functionChecker, argumentList, butil);

 				
 			}
 		}
 		else{
 			//CHECK FOR CURRENT FUNCTION
 			string flag3 = checkForRecursion(functionNode, argumentList, butil);
 			if((flag3 == "ERROR") || (flag3 == "RECURSION")){
 				//This is recursion .. 
 				butil.printSemantic("---->>>==>>> Line 866.. recursion Occurs.");
 				return "RECURSION";
 			}else{

	 			string str = "SEMANTIC ERROR, function with name = ";
	 			str += functionNode->getName();
	 			str += " at line. ";
	 			str += converter.integerToString(functionNode->getLineBegin());
	 			str += " , was not declared before.";
	 			butil.printError(str);
	 			butil.printSemantic("------>>>> INSIDE functionCall ... not found in ST hence error.");
	 			return "ERROR";
 			}
 		}

 		return "ERROR";

 	}

 	string checkForRecursion(SymbolInfoNode *functionNode, SymbolInfoNode *argumentList, const BisonUtil &butil){

 		string s = "\n\n=====-----+++++----->>>> Inside sutil.checkForRecursion ... printing <<<=============\n";
 		s += "functionNode =";
 		s += functionNode->getPrintForTable();
 		s += "\nSize of argumentList = ";
 		s += butil.getString((int)butil.currentArgumentList.listArguments.size());
 		s += "\nargumentList =";
 		s += butil.currentArgumentList.getArgumentList();
 		
 		s += "\nCurrent function name =";
 		s += butil.currentFunctionName;

 		s += "\nSize of current function parameter List = ";
 		s += butil.getString((int)butil.currentFunctionList.size());

 		s += "\nCurrent parameterList =";
 		for(int i=0; i<(int)butil.currentFunctionList.size(); i++){
 			s += butil.currentFunctionList[i]->getPrintForTable();
 			s += "\t";
 		}

		s += "\n\n<< Line 897 Hoise ... >>\n\n";
 		butil.printSemantic(s);

 		
 		bool isRecursion = false ;
 		//CHECKING WHETHER RECURSION HOISE ... 
 		if(functionNode->getName() == butil.currentFunctionName){
 			//Function names match ...
 			butil.printSemantic("FUNCTION NAMES FOR RECURSION MATCHED !!");
 			isRecursion = true ;
 		}
 		if(!isRecursion){
 			return "NAMES_DONT_MATCH" ;
 		}
//AMAR SHURU
 		butil.printSemantic("butil.currentFunctionReturnType = " + butil.currentFunctionReturnType);
 		if((butil.currentFunctionReturnType == "VOID") || (butil.currentFunctionReturnType == "void")){
 			string str = "SEMANTIC ERROR, at line. ";
 			str += converter.integerToString(functionNode->getLineBegin());
 			str += " , trying to use void function as part of expression.";
 			str += " with name = ";
 			str += functionNode->getName();
 			str += " which was defined at line no.";
 			str += converter.integerToString(butil.currentFunctionLine);
 			this->errorVoid = str ;
 			butil.printSemantic("Line 931 .. checkForRecursion .. setting errorVoid = " + errorVoid);
 			return "ERROR" ; 
 		}//This if was for void function as part of expression thing. 
 	
 		//Now Comparing arguments ..
		int sizeArguments = (int)butil.currentArgumentList.listArguments.size();
		int sizeParam = (int)butil.currentFunctionList.size();

		//Comparing size of argument list and parameter list..
		if(sizeParam != sizeArguments){
 			string str = "SEMANTIC ERROR, at line. ";
 			str += converter.integerToString(functionNode->getLineBegin());
 			str += " , function call parameter size and argument size do not match.";
 			str += " function was defined at line no.";
 			str += converter.integerToString(butil.currentFunctionLine);
 			butil.printError(str);
 			return "ERROR" ;
		}

		//Comparing types. ...

		for(int i=0; i<sizeArguments; i++){
			if(butil.currentFunctionList[i]->getType() != butil.currentArgumentList.listArguments[i].type)  	
 			{
 				//||(butil.currentFunctionList[i]->isArray != butil.currentArgumentList.listArguments[i].isArray)){
 				string str = "SEMANTIC ERROR, at line. ";
	 			str += converter.integerToString(functionNode->getLineBegin());
	 			str += " , function call parameter and arguments type mismatch.";
	 			str += " function was defined  at line no.";
	 			str += converter.integerToString(butil.currentFunctionLine);
	 			butil.printError(str);
	 			return "ERROR" ;
			}
		}

	//AMAR SHESH
 
 		return "RECURSION" ;
 	}

 	void pushCurrentParameters(const BisonUtil &butil)
 	{
 		string s = "\n====>>>> sutil.pushCurrentParameters ... ";
 		s += "\nCurrent Func name =" ;
 		s += butil.currentFunctionName;
 		s += "\nCurrent Scope Table ID = ";
 		s += converter.integerToString(butil.tab->getCurrentScopeID());
 		s += "\nparameterList size = ";
 		s += converter.integerToString((int)butil.currentFunctionList.size());
 		s += "\nparameterList is:\n";
 		for(int i=0; i<(int)butil.currentFunctionList.size(); i++){
 			//s += butil.currentFunctionList[i]->getPrintForTable();
 			butil.tab->Insert(butil.currentFunctionList[i]);
 			//s += "\n";
 		}



 		s += "\nAfter pushing, Printing ST now .. \n";
 		s += butil.tab->printAllNonEmpty();
 		s += "\n===========>>> DONE WITH sutil.pushCurrentParameters ... <<<<==================\n";
 		butil.printSemantic(s);

 	}
  	SymbolInfoNode *makeNewNode(string name, string type){
  		SymbolInfoNode *node = new SymbolInfoNode(name, type);
  		return node ;
  	}

  	//EKDOM LATEST EDIT

  	void makeArrayExpression(SymbolInfoNode *array, SymbolInfoNode *expression, const BisonUtil &butil){
  		butil.printArrayDebug("INSIDE makeArrayExpression... expression is " + expression->getName() + " , and dataType = " + expression->dataType);

  		string lName = "";
  		for(int i=0; i<array->getName().length(); i++){
  			if(array->getName()[i] == '['){
  				break;
  			}else{
  				lName += array->getName()[i];
  			}
  		}

  		if(expression->dataType != "INT"){
  			if(expression->dataType != "NIL"){
  				butil.printError("SEMANTIC ERROR at line. " + converter.integerToString(array->getLineBegin()) + 
  					", the index of array " +  lName +" is not an integer.");
  			}
  		}
  	}
  
  	void makeGoodType(SymbolInfoNode *arr, const BisonUtil &butil){
  		string name = getArrayName(arr->getName());
  		SymbolInfoNode *res = butil.tab->LookUp(name);
  		string dataType = "NIL";
  		if(res){
  			arr->dataType = res->dataType;
  			arr->symbol = res->symbol;
  			arr->code = res->code;
  			//butil.printArrayDebug("Inside makeGoodType ... printing arr , and res ");
  			//butil.printArrayDebug(arr->getPrintForTable());
  			//butil.printArrayDebug(res->getPrintForTable());
  		}
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

  	bool getArgument( SymbolInfoNode *node,const BisonUtil &butil){
  		butil.printArrayDebug("Inside getArgument... node = " + node->getPrintForTable());
  		SymbolInfoNode *res = butil.tab->LookUp(node->getName());
  		if(res){
  			node->isArray = res->isArray;
  			string s  = butil.tab->printCurrentNonEmpty();
  			butil.printArrayDebug(s);
			butil.printArrayDebug("Got RES , res = " + res->getPrintForTable());
  			return true ;
  		}
  		return false ;
  	}

};

//END