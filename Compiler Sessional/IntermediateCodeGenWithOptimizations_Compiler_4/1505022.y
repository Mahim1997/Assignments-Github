%{
#include<iostream>
#include<cstdlib>
#include<cstring>
#include<cmath>


 
//#include "Bison_Stuffs/1505022_SemanticUtil.h"
#include "IntermediateCodeStuffs/IntermediateUtil.h"
#define YYSTYPE SymbolInfoNode*
 

using namespace std;

int firstNodeChange = 0;
FILE *fp ;
int yyparse(void);
int yylex(void);

extern FILE *yyin;
extern int yylineno;
extern char* yytext;

SymbolTable *table;
SymbolInfoNode* node;

SymbolInfoNode *debug ;

BisonUtil butil(table) ;
SemanticUtil sutil;

IUtil iutil;

int voidFlag = -1000;
int errorBison = 0;
int numLinesOfInput = 0 ;
int currentScope = -1 ;
string utilityString = "NIL";
int currentLine;
void yyerror(const char *s)
{
	//write your code
	//printf("<><><><>ERROR[yyerror] at line printing error token .. \n"); yyerrok;
	//printf("%s", s);
	butil.printErrorToLog(s, yylineno, yytext);
}


%}
%locations


%token IF ELSE FOR WHILE ID LPAREN RPAREN SEMICOLON COMMA LCURL RCURL INT FLOAT VOID LTHIRD RTHIRD PRINTLN RETURN ASSIGNOP LOGICOP RELOP ADDOP MULOP NOT CONST_INT CONST_FLOAT INCOP DECOP SWITCH DEFAULT BREAK CHAR DOUBLE CASE CONTINUE CONST_CHAR
BITOP STRING DO

//%left
//%right

%define parse.error verbose
%nonassoc Lower_Than_Else
%nonassoc ELSE

//%nonassoc
//%nonassoc	LPAREN
%%

start : program
	{
		//write your code in this block in all the similar blocks below
		$$->setSymbol($1);
		numLinesOfInput = $$->getLineBegin();
		//$$->code += ("\n\nLABEL_END:\nEND MAIN\n\n");
		$$->code += ("\n\nEND MAIN\n\n");
		butil.printLog("start : program", $$);
		iutil.printFullCode($$, butil);
	}
	;

program : program unit 	{
							$$->setSymbol($1);  $$->setUnion($2);
							$$->setUnion("\n");
							butil.printLog("program : program unit", $$);
						}
	| unit				{
							$$->setSymbol($1);
							$$->setUnion("\n");
							butil.printLog("program : unit", $$);
						}
 
	;

unit : var_declaration			{
									$$->setSymbol($1);
									butil.printLog("unit : var_declaration", $$);
								}
     | func_declaration			{
     								$$->setSymbol($1);
     								butil.printLog("unit : func_declaration", $$);
						        }
     | func_definition			{
     								$$->setSymbol($1);
     								butil.printLog("unit : func_definition", $$);
									//butil.printLog("===>>>1. NEED TO EXIT SCOPE HERE>.....");
								}
     ;

func_declaration : type_specifier ID LPAREN parameter_list RPAREN SEMICOLON	{
								//butil.checkAndPrint($2);

								string funcName = $2->getName();
								string paramName = $4->getName();

								$$->setSymbol($1); $$->setUnion(" ");
								$$->setUnion($2); $$->setUnion($3); $$->setUnion($4);$$->setUnion($5); $$->setUnion($6);
								$$->setUnion("\n");
								butil.printLog("func_declaration : type_specifier ID LPAREN parameter_list RPAREN SEMICOLON", $$);

							 	$2->setDataType($1->getDataType());

						 		

		 						
		 						SymbolInfoNode *node = new SymbolInfoNode();
		 						node->setSymbol($3); node->setUnion($4); node->setUnion($5);
							 	butil.removeFromCurrentFunction(node);
								
							 	
							 	butil.declareFunction($$);
							 	$$->symbol = iutil.converter.makeName("res", funcName);
							 	iutil.declareFunction(funcName, paramName, butil);
							}
		| type_specifier ID LPAREN RPAREN SEMICOLON		{

				string funcName = $2->getName();

				$$->setSymbol($1); $$->setUnion(" ");
				$$->setUnion($2); $$->setUnion($3); $$->setUnion($4);$$->setUnion($5);
				$$->setUnion("\n");
				butil.printLog("func_declaration : type_specifier ID LPAREN RPAREN SEMICOLON", $$);

				$2->setDataType($1->getDataType());
		 		
		 		
				butil.declareFunction($$);
			 	$$->symbol = iutil.converter.makeName("res", funcName);
			 	iutil.declareFunction(funcName, butil);
			}
		|	type_specifier ID LPAREN parameter_list RPAREN error 	{

			butil.printError("MISSING SEMICOLON at Line. " + butil.getString($2->getLineBegin()));
			$$->setSymbol($1); $$->setUnion(" ");
			$$->setUnion($2); $$->setUnion($3); $$->setUnion($4);$$->setUnion($5); $$->setUnion(";");
			$$->setUnion("\n");
			butil.printLog("func_declaration : type_specifier ID LPAREN parameter_list RPAREN ERROR", $$);


		}
		| type_specifier ID LPAREN RPAREN error 	{
				butil.printError("MISSING SEMICOLON at Line. " + butil.getString($2->getLineBegin()));
				$$->setSymbol($1); $$->setUnion(" ");
				$$->setUnion($2); $$->setUnion($3); $$->setUnion($4);$$->setUnion(";");
				$$->setUnion("\n");
				butil.printLog("func_declaration : type_specifier ID LPAREN RPAREN ERROR", $$);

		}
		;

func_definition : type_specifier ID LPAREN parameter_list RPAREN 	{
 		currentLine = $1->getLineBegin();
		butil.currentFunctionReturnType = $1->getName();
	 	butil.currentFunctionName = $2->getName();
	 	butil.currentFunctionLine = $2->getLineBegin();
		


	 	butil.printLog("Here, setting, currentFuncName = " + butil.currentFunctionName +
	 	 " and current return type = " + butil.currentFunctionReturnType, false);

	 	//Setting return types and naming the function [NO]
		$2->setDataType($1->getDataType());


		SymbolInfoNode *node = new SymbolInfoNode("", "");
		node->setSymbol($1); node->setUnion(" "); node->setUnion($2); node->setUnion($3); node->setUnion($4); node->setUnion($5);
	 	currentScope = butil.tab->getCurrentScopeID();
		printf("========================++++>>>>>>>> Line 169\n");

		butil.defineFunction(node);
			 	
		printf("Line 173\n");
	}
	compound_statement 	{
	//$$->setSymbol($1);$$->setUnion(" ");$$->setUnion($2);$$->setUnion($3);$$->setUnion($4);$$->setUnion($5); //$$->setUnion($7);
	printf("==================++>>>>> LINE 178!!\n");
	SymbolInfoNode *n = iutil.defineFunc($$, butil.currentFunctionName, $7, butil, currentScope);
	printf("==================++>>>>> LINE 180!!\n");
	n->symbol_name = ($1->getName() + " " + $2->getName() + $3->getName() + $4->getName() + $5->getName() + $7->getName());
	printf("Line 182 done !!\n");
	iutil.setSymbol($$, n);

	printf("Line 185 done !!\n");

	currentLine = $7->getLineBegin();
	$$->setLineBegin(currentLine);

	butil.printLog("func_definition : type_specifier ID LPAREN parameter_list RPAREN compound_statement", $$);

	butil.removeAllCurrentFunVar();	//Upore dileh error ashbe.
	iutil.removeDeclaredVariables();	//For recursion variables..
 
}
		| type_specifier ID LPAREN RPAREN	{
			currentLine = $1->getLineBegin();

	 	  	butil.currentFunctionReturnType = $1->getName();
		 	butil.currentFunctionName = $2->getName();
		 	butil.currentFunctionLine = $2->getLineBegin();

 	 		butil.printLog("Here, setting, currentFuncName = " + butil.currentFunctionName +
	 	 	" and current return type = " + butil.currentFunctionReturnType, false);

	 	 	 	//Setting return types and naming the function [NO]
			$2->setDataType($1->getDataType());

			SymbolInfoNode *node = new SymbolInfoNode("", "");
			node->setSymbol($1); node->setUnion(" "); node->setUnion($2); node->setUnion($3); node->setUnion($4);
			utilityString = node->getName();
			butil.defineFunction( node);
			currentScope = butil.tab->getCurrentScopeID();
 

		}
		 compound_statement 	{
 
	SymbolInfoNode *n = iutil.defineFunc($$, butil.currentFunctionName, $6, butil, currentScope);
 
	if(n){
		printf("n exists!!\n");
	}else{
		printf("n doesnot exist!!\n");
	}
 
	utilityString += $6->getName();
	n->symbol_name = utilityString;
	n->line_number_begin = $6->getLineBegin();
	 
 	currentLine = $6->getLineBegin();
	iutil.setSymbol($$, n);
 	$$->setLineBegin(currentLine);
	butil.printLog("func_definition : type_specifier ID LPAREN RPAREN compound_statement", $$);
	butil.removeAllCurrentFunVar();	//Upore dileh error ashbe.

	iutil.removeDeclaredVariables();	//For recursion variables.. 
}

 		;


parameter_list : parameter_list COMMA type_specifier ID 	{
			$$->setSymbol($1); $$->setUnion($2); $$->setUnion(" "); $$->setUnion($3); $$->setUnion(" "); $$->setUnion($4);
			butil.printLog("parameter_list  : parameter_list COMMA type_specifier ID", $$);

			$4->setDataType($3->getDataType());
			//butil.printDebug("===>>>LINE 159... pushing $4 in ForFunction. ", $4);
	 
			butil.pushForFunction($4);

		}
		| parameter_list COMMA type_specifier 	{
			$$->setSymbol($1); $$->setUnion($2); $$->setUnion(" "); $$->setUnion($3);
			butil.printLog("parameter_list  : parameter_list COMMA type_specifier", $$);
		}
 		| type_specifier ID 	{
			//butil.printDebug("===>>>LINE 189... printin $1 =", $1);
			$2->setDataType($1->getDataType());


			$$->setSymbol($1); $$->setUnion(" ");  $$->setUnion($2);
			butil.printLog("parameter_list  : type_specifier ID", $$);


			//butil.printDebug("===>>>LINE 191... pushing $2 in ForFunction. ", $2);
		 
			butil.pushForFunction($2);
 		}
		| type_specifier 	{
			$$->setSymbol($1);
			butil.printLog("parameter_list : type_specifier", $$);
		}
 		;


compound_statement : LCURL {
								//butil.printLog("==-->>NEED TO ENTER SCOPE HERE.\n");
								butil.enterNewScope();
								butil.returnFlag = "false";
						butil.printSemantic("\n==================+>>>>> AT LINE 225 Bison.y ... LCURL er porey.");
								iutil.pushParameters(butil);
								sutil.pushCurrentParameters(butil);
								
							}
					statements RCURL 	{

		$$->setSymbol($1); $$->setUnion("\n");  $$->setUnion($3); $$->setUnion("\n"); $$->setUnion($4); $$->setUnion("\n");
		butil.printLog("compound_statement : LCURL statements RCURL", $$);

 		//butil.printLog("==--->>>NEED EXIT SCOPE HERE[1]\n");
 		printf("LINE 293 , immediately before butil.exitScope()\n");
 		butil.exitScope();
}
 		    | LCURL {
								butil.enterNewScope();
								butil.returnFlag = "false";
						butil.printSemantic("\n==================+>>>>> AT LINE 235 Bison.y ... LCURL er pore[2].");
								iutil.pushParameters(butil);
								sutil.pushCurrentParameters(butil);
								
							}
					 RCURL 	{

		$$->setSymbol($1); $$->setUnion("\n");  $$->setUnion($3); $$->setUnion("\n");  
		butil.printLog("compound_statement : LCURL RCURL", $$);
 
 		butil.exitScope();
}
 		    ;

var_declaration : type_specifier declaration_list SEMICOLON		{
		$$->setSymbol($1, $2, $3);
		butil.printLog("var_declaration : type_specifier declaration_list SEMICOLON", $$);
}
|	type_specifier declaration_list error 	{
		butil.printError("Missing SEMICOLON at line. " + butil.getString($1->getLineBegin()));
		$$->setSymbol($1); $$->setUnion(" "); $$->setUnion($2); $$->setUnion(" ;");

		butil.printLog("var_declaration : type_specifier declaration_list ERROR", $$);
	
}
 		 ;

type_specifier	: INT 	{
							$$->setSymbol($1);
							$$->setDataType("INT");
							butil.printLog("type_specifier : INT", $$);
							butil.currentDataType = "INT";
 							
							firstNodeChange = 0;
						}
 		| FLOAT 		{
								$$->setSymbol($1);
								$$->setDataType("FLOAT");
								butil.printLog("type_specifier : FLOAT", $$);
								butil.currentDataType = "FLOAT";
 
								firstNodeChange = 0;
			  			}
 		| VOID 			{
									$$->setSymbol($1);
									$$->setDataType("VOID");
									butil.printLog("type_specifier : VOID", $$);
									butil.currentDataType = "VOID";
 
									firstNodeChange = 0;
			  			}
 		;

declaration_list : declaration_list COMMA ID 	{

				$3->setDataType(butil.currentDataType);
				$3->isArray = false;
  				

				$$->setSymbol($1); $$->setUnion($2); $$->setUnion(" "); $$->setUnion($3);

				butil.printLog("declaration_list: declaration_list COMMA ID", $$);
 

				iutil.declareVariable($3, butil);	//Code is being written in $3 node and will be printed inside this func.
				iutil.pushDeclaredVariables($3);	//For recursion purposes ...
				butil.checkUniquenessAndPushST($3); 

		}
 		  | declaration_list COMMA ID LTHIRD CONST_INT RTHIRD 	{
	  		$3->dataType = butil.currentDataType;
			$3->setDataType(butil.currentDataType);
			$3->arrayItems.makeArray($3->getName(), butil.getSize($5), $3->getLineBegin());
			$3->isArray = true;

			

			bool flag = sutil.isDuplicate($3, butil);
			
			if(flag == false){
  				$3->setDataType(butil.currentDataType);
				$3->arrayItems.makeArray($3->getName(), butil.getSize($5), $3->getLineBegin());
				$3->isArray = true;
				iutil.declareArray($3, $5, butil);	//$3 is node name and $5 is size of array
				iutil.pushDeclaredVariables($3);	//For recursion purposes ...
				sutil.pushArray($3, $5, butil);

			}	
			sutil.makeSymbol($$, $1 ); sutil.makeUnion($$, $2 ); sutil.addSpace($$);
		 	sutil.makeUnion($$, $3 ); sutil.makeUnion($$, $4 ); sutil.makeUnion($$, $5 ); sutil.makeUnion($$, $6 ); 

			$$->dataType = $1->dataType;

			butil.printSemantic("AFTER pushARRAY Line286.. printing ST");
			butil.printSemantic(butil.tab->printCurrentNonEmpty());

			butil.printLog("declaration_list : declaration_list COMMA ID LTHIRD CONST_INT RTHIRD", $$);


		}
 		  | ID 	{
 		  			$1->setDataType(butil.currentDataType);

 		  			$$->setSymbol($1);

	  				

	  				butil.printLog("declaration_list : ID", $$);
					iutil.declareVariable($1, butil);
					iutil.pushDeclaredVariables($1);	//For recursion purposes ...
					butil.checkUniquenessAndPushST($1);
					iutil.printDeb("--->>>Inside Bison.y Line 379 after declareVariable, $1= ", $1);
 
		}
 		  | ID LTHIRD CONST_INT RTHIRD 	{
 		  		$1->dataType = butil.currentDataType;
				bool flag = sutil.isDuplicate($1, butil);
				SymbolInfoNode *node = $1;

				if(flag == false){
	  				$1->setDataType(butil.currentDataType);
					$1->arrayItems.makeArray($1->getName(), butil.getSize($3), $1->getLineBegin());
					$1->isArray = true;
					iutil.declareArray($1, $3, butil);

					iutil.printDeb("+==================+>>>Line 436 Bison.y, $1->symbol = " + $1->symbol);
					iutil.pushDeclaredVariables($1);	//For recursion purposes ...
					sutil.pushArray($1, $3, butil);
				}
  				

				butil.printSemantic(butil.tab->printCurrentNonEmpty());
				$$->dataType = $1->dataType;
		  		sutil.makeSymbol($$, node, $2, $3, $4, butil);
 		  		

				butil.printSemantic("AFTER pushARRAY Line319.. printing ST");
				butil.printSemantic(butil.tab->printCurrentNonEmpty());


 		  		butil.printLog("declaration_list : ID LTHIRD CONST_INT RTHIRD", $$);

	  			 
		}
 		  ;

statements : statement 	{
		$$->setSymbol($1);
		butil.printLog("statements : statement", $$);
	}
	   | statements statement 	{
	   $$->setSymbol($1);
	   $$->setUnion("\n");
	   $$->setUnion($2);
	   butil.printLog("statements : statements statement", $$);
   }
	   ;

statement : var_declaration 	{
	SymbolInfoNode* node = $1; 
	$$->setSymbol($1);
	butil.printLog("statement : var_declaration", $$);
	
	butil.printDebug("++>>>=>>Inside statement:var_dec \n");
	bool flag = butil.doesExist(node);

}
	  | expression_statement 	{
	  		$$->setSymbol($1);	
			butil.printLog("statement : expression_statement ", $$);
		
			butil.printSemantic("===-===================+>>>>> Here , voidFLag = " + butil.getString(voidFlag));
		
		 	butil.printLog("--------====--->>>>HERE, butil.voidCount = " + butil.getString(butil.voidCount) + 
		 		", voidFlag = " + butil.getString(voidFlag) , false);

			/*if(butil.voidCount == 9){
				sutil.errorVoid = "NOTHING ASSIGNED YET!!";				
				voidFlag = -1000;
			 	butil.voidCount = -1000;
			}*/
			bool shouldPrintError = true;
			if((butil.voidCount == 9) && (voidFlag == 14)){
				shouldPrintError = false;
				voidFlag = -1000; butil.voidCount = -1000;
			}
		
		 	butil.printSemantic("----------->>>>HERE, butil.voidCount = " + butil.getString(butil.voidCount) + 
		 		", voidFlag = " + butil.getString(voidFlag) + ", shouldPrintError = " + butil.getString(shouldPrintError)
		 		+ " , and sutil.error = " + sutil.errorVoid);
 
			if(sutil.errorVoid != "NOTHING ASSIGNED YET!!"){
				butil.printSemantic(">>>>>>>>>>>>>>>>>>>>> INSIDE 390 line sutil.errorVoid = " + sutil.errorVoid
				+ " , and voidFlag = " + butil.getString(voidFlag));
				
				if(shouldPrintError == true){
					butil.printLog("--->>>SHOULD PRINT ERRRO0R HERE ", false);
					butil.printError(sutil.errorVoid);
				}else{
					butil.printLog("--===->>>NOT PRINT ERRRO0R HERE ", false);
				}
			}
			 
			voidFlag = -1000;
			butil.voidCount = -1000;
			sutil.errorVoid = "NOTHING ASSIGNED YET!!";
		}
	  | compound_statement 	{
	  		$$->setSymbol($1);
			butil.printLog("statement : compound_statement", $$);
	  }
	  | FOR LPAREN expression_statement expression_statement expression RPAREN statement 		{
	  	SymbolInfoNode *n = iutil.ForLoop($$, $3, $4, $5, $7);
	  	iutil.setSymbol($$, n);
	  	butil.printLog("statement : FOR LPAREN expression_statement expression_statement expression RPAREN statement", $$);
	  }
	  | IF LPAREN expression RPAREN statement %prec Lower_Than_Else 	{

	  	SymbolInfoNode *n = iutil.if_op($$,$3, $5);
	  	iutil.setSymbol($$, n);

	  	butil.printLog("statement : IF LPAREN expression RPAREN statement", $$);
	  }
	  | IF LPAREN expression RPAREN statement ELSE statement  {
 

	  	SymbolInfoNode *n = iutil.ifElse_op($$,$3, $5, $7);
	  	iutil.setSymbol($$, n);
	  	
	  	butil.printLog("statement : IF LPAREN expression RPAREN statement ELSE statement", $$);
	  }
	  | WHILE LPAREN expression RPAREN statement 	{
		
		
	  	SymbolInfoNode *n = iutil.whileLoop($$, $3, $5);
		iutil.setSymbol($$, n);

		butil.printLog("statement : WHILE LPAREN expression RPAREN statement", $$);
	  }
	  | PRINTLN LPAREN ID RPAREN SEMICOLON 	{
  		$$->setSymbol($1);$$->setUnion($2);$$->setUnion($3);$$->setUnion($4);$$->setUnion($5);
  		$$->setUnion("\n");
  		butil.printLog("statement : PRINTLN LPAREN ID RPAREN SEMICOLON", $$);

  		SymbolInfoNode *res = butil.tab->LookUp($3->getName());
  		if(res){
  			iutil.printID($$, res, butil.currentFunctionName);
  		}else{
  			butil.printError("SEMANTIC ERROR at line. " + butil.getString($3->getLineBegin()) + 
  			", Trying to use an undeclared variable " + $3->getName());
  		}
	  }
	  | PRINTLN LPAREN ID RPAREN error 	{
		butil.printError("MISSING SEMICOLON at Line. " + butil.getString($3->getLineBegin()));

  		$$->setSymbol($1);$$->setUnion($2);$$->setUnion($3);$$->setUnion($4);$$->setUnion(";");
  		$$->setUnion("\n");
  		butil.printLog("statement : PRINTLN LPAREN ID RPAREN ERROR", $$);
	  }
	  | RETURN expression SEMICOLON 	{
	  		int line = $1->getLineBegin();
	  		SymbolInfoNode *n = iutil.returnFun($$, $2, butil);
	  		iutil.setSymbol($$, n);
	  		$$->setLineBegin(line);
		  	butil.printLog("statement : RETURN expression SEMICOLON", $$);
	  }
	  | RETURN expression error 	{
			butil.printError("MISSING SEMICOLON at Line. " + butil.getString($1->getLineBegin()));

		  	$$->setSymbol($1); $$->setUnion(" ") ;$$->setUnion($2);$$->setUnion(";");
		  	butil.printLog("statement : RETURN expression ERROR", $$);
	  }
	  ;

expression_statement : SEMICOLON		{
	$$->setSymbol($1);
	$$->code = "";
	butil.printLog("expression_statement : SEMICOLON", $$);
	
}
			| expression SEMICOLON  {
				//$$->setSymbol($1); $$->setUnion($2);
				
				$$->makeNode($1); $$->setUnion(";");
				butil.printLog("expression_statement : expression SEMICOLON", $$);
				voidFlag += 2;
				
			}
			| expression error  {
				butil.printError("MISSING SEMICOLON at Line. " + butil.getString($1->getLineBegin()));
				$$->setSymbol($1); $$->setUnion(";");
				butil.printLog("expression_statement : expression ERROR", $$);
			}
			;

variable : ID 		{
 
 		SymbolInfoNode *node = sutil.findFromST($1, butil);
		$$->setSymbol(node);
		

 		
 		$$->setDataType($1->getDataType());
 		$$->code = "";

 		$$->isArray = false; 
 
 		butil.pushUndeclared($$);

		butil.printLog("variable : ID", $$); 		

	}
	 | ID LTHIRD expression RTHIRD {
	 	sutil.checkForArrayExpression($1, $3, butil);

	 	
		$1->isArray = true ;
	 	$$->isArray = true;
	 	$$->setDataType($1->dataType);

	 	$$->setSymbol($1);$$->setUnion($2);$$->setUnion($3);$$->setUnion($4);
	 	
	 	
	 	sutil.makeArrayExpression($1, $3, butil);
		sutil.makeGoodType($1, butil);
	 	
	 	//$$->code=$3->code+"mov bx, " +$3->getSymbol() +"\nadd bx, bx\n";

	 	iutil.arrayMaking($$, $1, $3, butil);
		$$->isArray = true; 
	 	iutil.printDeb("==>Line 571 Bisno.y, After var -> ID[ exp ], src = ", $$);
	
		butil.printLog("variable : ID LTHIRD expression RTHIRD", $$);

	 }
	 ;

 expression : logic_expression	{
 	$$->setSymbol($1);
 	voidFlag += 2;
 	butil.printLog("expression : logic_expression", $$);
 	
 }
	   | variable ASSIGNOP logic_expression 	{
	   		SymbolInfoNode *var = new SymbolInfoNode();
	   		SymbolInfoNode *logicExp = new SymbolInfoNode();
	   		var->makeNode($1);	logicExp->makeNode($3);

		 	sutil.printSemantic("==>>Line 405. (exp -> var = logic_exp) Printing variable $1 and logic_expression $3 .. \n", 
		 	butil, $1, $3);

		 	butil.printSemantic("===============>> Line 436.. Before assignOp printing the var.\n $1=" , $1);
		 	sutil.makeGoodType($1, butil);
		 	

		 	sutil.assignOp($$, $1, $3, butil);
			iutil.assignOp($$, var, logicExp, butil);

		 	butil.printLog("expression : variable ASSIGNOP logic_expression", $$);
 			
 			iutil.printDeb("LINE 597 Bison.y => AFTER exp -> var ASSIGNOP logic_exp, printing var", $$);
	   }
	   ;

logic_expression : rel_expression 	{
	$$->setSymbol($1);
	butil.printLog("logic_expression : rel_expression", $$);
	voidFlag += 2;
	
}
		 | rel_expression LOGICOP rel_expression 	{
				iutil.relAndLogicOp($$, $1, $3, $2, butil);

				sutil.relAndLogicOp($$, $1, $3, $2, butil);

				butil.printLog("logic_expression : rel_expression LOGICOP rel_expression", $$);

				iutil.printDeb("Line 615 Bison.y => After logic->rel LOGICOP rel, src = ", $$);
		 }
		 ;

rel_expression	: simple_expression  	{
		$$->setSymbol($1);
		butil.printLog("rel_expression : simple_expression", $$);
		voidFlag += 2;
		
}
		| simple_expression RELOP simple_expression	{
 				
			iutil.relAndLogicOp($$, $1, $3, $2, butil);
			sutil.relAndLogicOp($$, $1, $3, $2, butil);
			iutil.printDeb("Line 630 Bison.y => After rel -> simple RELOP simple, src = ", $$);
			butil.printLog("rel_expression : simple_expression RELOP simple_expression", $$);
		}
		;

simple_expression : term 	{
		$$->setSymbol($1);
		butil.printLog("simple_expression : term", $$);
		voidFlag += 2;
		
}
		  | simple_expression ADDOP term {

		  	butil.printSemantic("======>>>>>>> Line 485. simple_exp -> simple_exp ADDOP term");
		  	

		  	SymbolInfoNode *n = iutil.addop($$, $1, $2, $3, butil);
		  	iutil.setSymbol($$, n);
		  	butil.printLog("simple_expression : simple_expression ADDOP term", $$);
		  	
		  }
		  ;

term :	unary_expression 	{
	$$->setSymbol($1);
	butil.printLog("term : unary_expression", $$);
	voidFlag += 2;
	
}
     |  term MULOP unary_expression 	{
		sutil.makeMulop($$, $1, $3, $2, butil);
		
		SymbolInfoNode *n = iutil.mulop($$, $1, $2, $3, butil);
		iutil.setSymbol($$, n);

    	butil.printLog("term : term MULOP unary_expression", $$);
 

     }
     ;

unary_expression : ADDOP unary_expression  {
	sutil.unaryOperation($$, $2, $1, butil);
 
	SymbolInfoNode *n = iutil.unary($$, $1, $2);
	iutil.setSymbol($$, n);

	butil.printLog("unary_expression : ADDOP unary_expression", $$);

}
		 | NOT unary_expression {
			sutil.unaryOperation($$, $2, $1, butil);
		 
			SymbolInfoNode *n = iutil.unary($$, $1, $2);
			iutil.setSymbol($$, n);

			butil.printLog("unary_expression : NOT unary_expression", $$);
		 }
		 | factor 	{
		 	$$->setSymbol($1);
		 	butil.printLog("unary_expression : factor", $$);
		 	voidFlag += 2;
		 	
		 }
		 ;

factor	: variable 	{
 
		$$->setSymbol($1);
		
 			
		iutil.setArraySymbol($$, $1, butil);

		iutil.printDeb("=>Line 691 Bison.y factor: variable, printing $$: " , $$);
		butil.printLog("factor : variable", $$);
	}
	| ID LPAREN argument_list RPAREN	{
		butil.printSemantic("==>>===>>>===>>> Line 504. THIS IS FUNCTION CALL.. printing func name/ID =\n", $1);
	
		voidFlag = 0;
		butil.voidCount = 0;
		string returnType = sutil.functionCall($$, $1, $3, butil);
  
		$$->dataType = returnType ;

		SymbolInfoNode *res = iutil.funcCall($$, $1, $3 ,butil);
		iutil.setSymbol($$, res);

		$$->setLineBegin($1->getLineBegin());
		butil.printLog("factor : ID LPAREN argument_list RPAREN", $$);
		butil.currentArgumentList.clear();


	}
	| LPAREN expression RPAREN	{
		SymbolInfoNode *node = $2;
		butil.printSemantic("PRINTING $2 at factor->(exp), $2=" + $2->getPrintForTable());
		
		$$->setName("(" + $2->getName() + ")");
		$$->code = $2->code;
		$$->symbol = $2->symbol;

		$$->setDataType(node->dataType);

		butil.printLog("factor : LPAREN expression RPAREN", $$);

	}
	| CONST_INT {
		$1->dataType = "INT";
		$1->symbol = $1->getName();
		$$->setSymbol($1);
		$$->setDataType("INT");
		$$->actualValue = $1->actualValue;
		butil.printDeb("Line 524. factor -> CONST_INT , printing $$", $$);
		butil.printLog("factor : CONST_INT", $$);
		
	}
	| CONST_FLOAT	{
		$1->dataType = "FLOAT";
		$$->setSymbol($1);
		$1->symbol = $1->getName();
		$$->setDataType("FLOAT");
		$$->actualValue = $1->actualValue;
		butil.printDeb("Line 531. factor -> CONST_FLOAT , printing $$", $$);
		butil.printLog("factor : CONST_FLOAT", $$);
		
	}
	| variable INCOP {
		SymbolInfoNode *n = iutil.incOrdec($$, $1, $2);
		iutil.setSymbol($$, n);

		sutil.operate($$, $1, $2, butil);		//src is not changed here 


		butil.printLog("factor : variable INCOP", $$);
			
	}
	| variable DECOP	{
		
		SymbolInfoNode *n = iutil.incOrdec($$, $1, $2);
		iutil.setSymbol($$, n);

		sutil.operate($$, $1, $2, butil);	//src is not changed here 



		butil.printLog("factor : variable DECOP", $$);
		
	}
	;

argument_list : arguments 	{
	
	$$->setSymbol($1);
	butil.printLog("argument_list : arguments", $$);

}
			  |		{
			  			//DO NOTHING
		  				SymbolInfoNode *node = new SymbolInfoNode(" HI ", " HI ");
		  				$$ = sutil.makeNewNode("", "");
	  					butil.printSemantic("=======================>>>>Line 670.Bison.y => $$ is ", $$);

			  }
			  ;

arguments : arguments COMMA logic_expression 	{

	sutil.makeSymbol($$, $1); sutil.makeUnion($$, $2); sutil.addSpace($$); sutil.makeUnion($$, $3);
	butil.printLog("arguments : arguments COMMA logic_expression", $$);
	//butil.printSemantic("Line 612. Bison.y ,, Before adding to currentArgumentList.. ");

	sutil.getArgument($3, butil);
	butil.currentArgumentList.addArgument($3->getName(), $3->dataType, $3->getLineBegin(), $3->actualValue, $3->isArray, 
											$3->symbol, $3->code);
	iutil.pushArgument($3);
	butil.printArrayDebug("\nArgument was added ", $3);
}
	      | logic_expression 	{
	      		$$->setSymbol($1);
	      		butil.printLog("arguments : logic_expression", $$);

  				sutil.getArgument($1, butil);
      			butil.currentArgumentList.addArgument($1->getName(), $1->dataType, $1->getLineBegin(), $1->actualValue, 
      													$1->isArray, $1->symbol, $1->code);

				iutil.pushArgument($1);
      			butil.printArrayDebug("\nArgument was added ", $1);
	      }
	      ;


%%
int main(int argc,char *argv[])
{
	printf("\n\n\n");

	sutil.checkFloatFlag = true ;
	if((fp=fopen(argv[1],"r"))==NULL)
	{
		printf("Cannot Open Input File.\n");
		exit(1);
	}

	yyin=fp;

	yyparse();


	iutil.printFinalCode();


	butil.printLog("\n\t\t\tSymbol Table: \n");
	butil.printST();

	butil.printError("\nTotal Errors: " + butil.getString(butil.numErrors), false);
	butil.printLog("\nTotal Lines: " + butil.getString(numLinesOfInput));
	butil.printLog("\nTotal Errors: " + butil.getString(butil.numErrors));

	butil.printSemantic("CURRENT FUNC NAME = " + butil.currentFunctionName + " , current ret type = " + butil.currentFunctionReturnType);

	printf("\n\n\nTotal Lines: %d,Total Errors : %d \n\n", numLinesOfInput ,butil.numErrors);


	butil.printZero("\nTotal Errors: " + butil.getString(butil.zeroError), false);

	butil.closeFiles();

 

	return 0;
}
/*

clear
clear
clear
clear
 
bison -d -y 1505022_Bison.y
echo '===>>> 1st step bison -d -y 1505022_Bison.y done\n'
g++ -fpermissive -w -c -o y.o y.tab.c
echo '===>>> 2nd step g++ -fpermissive -w -c -o y.o y.tab.c done \n'
flex 1505022_Flex.l
echo '====>> 3rd step flex 1505022_Flex.l done \n'
g++ -fpermissive -w -c -o l.o lex.yy.c
echo '====>> 4th step g++ -fpermissive -w -c -o l.o lex.yy.c done\n'
g++ -o a.out y.o l.o -lfl -ly
echo '====>> 5th step g++ -o a.out y.o l.o -lfl -ly done\n\n\n'
./a.out input1.txt 


*/