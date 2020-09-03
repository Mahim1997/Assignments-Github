%{
#include<iostream>
#include<cstdlib>
#include<cstring>
#include<cmath>


 
#include "1505022_SemanticUtil.h"
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

int voidFlag = -1000;
int errorBison = 0;
int numLinesOfInput = 0 ;
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
		butil.printLog("start : program", $$);
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

								$$->setSymbol($1); $$->setUnion(" ");
								$$->setUnion($2); $$->setUnion($3); $$->setUnion($4);$$->setUnion($5); $$->setUnion($6);
								$$->setUnion("\n");
								butil.printLog("func_declaration : type_specifier ID LPAREN parameter_list RPAREN SEMICOLON", $$);

							 	$2->setDataType($1->getDataType());

		 						
		 						SymbolInfoNode *node = new SymbolInfoNode();
		 						node->setSymbol($3); node->setUnion($4); node->setUnion($5);
							 	butil.removeFromCurrentFunction(node);
								
							 	
							 	butil.declareFunction($$);
							}
		| type_specifier ID LPAREN RPAREN SEMICOLON		{
				$$->setSymbol($1); $$->setUnion(" ");
				$$->setUnion($2); $$->setUnion($3); $$->setUnion($4);$$->setUnion($5);
				$$->setUnion("\n");
				butil.printLog("func_declaration : type_specifier ID LPAREN RPAREN SEMICOLON", $$);

				$2->setDataType($1->getDataType());

				butil.declareFunction($$);
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
 
		butil.currentFunctionReturnType = $1->getName();
	 	butil.currentFunctionName = $2->getName();
	 	butil.currentFunctionLine = $2->getLineBegin();
		
	 	butil.printLog("Here, setting, currentFuncName = " + butil.currentFunctionName +
	 	 " and current return type = " + butil.currentFunctionReturnType, false);

	 	//Setting return types and naming the function [NO]
		$2->setDataType($1->getDataType());


		SymbolInfoNode *node = new SymbolInfoNode("", "");
		node->setSymbol($1); node->setUnion(" "); node->setUnion($2); node->setUnion($3); node->setUnion($4); node->setUnion($5);
		butil.defineFunction(node);

	}
	compound_statement 	{
	$$->setSymbol($1);$$->setUnion(" ");$$->setUnion($2);$$->setUnion($3);$$->setUnion($4);$$->setUnion($5); $$->setUnion($7);
	butil.printLog("func_definition : type_specifier ID LPAREN parameter_list RPAREN compound_statement", $$);

	butil.removeAllCurrentFunVar();	//Upore dileh error ashbe.

/*
 	//Setting return types and naming the function [NO]
	$2->setDataType($1->getDataType());

	butil.removeAllCurrentFunVar();
	butil.defineFunction($$);
*/
}
		| type_specifier ID LPAREN RPAREN	{
			

	 	  	butil.currentFunctionReturnType = $1->getName();
		 	butil.currentFunctionName = $2->getName();
		 	butil.currentFunctionLine = $2->getLineBegin();

 	 		butil.printLog("Here, setting, currentFuncName = " + butil.currentFunctionName +
	 	 	" and current return type = " + butil.currentFunctionReturnType, false);

	 	 	 	//Setting return types and naming the function [NO]
			$2->setDataType($1->getDataType());

			SymbolInfoNode *node = new SymbolInfoNode("", "");
			node->setSymbol($1); node->setUnion(" "); node->setUnion($2); node->setUnion($3); node->setUnion($4);
			butil.defineFunction(node);
		}
		 compound_statement 	{

	$$->setSymbol($1);$$->setUnion(" ");$$->setUnion($2);$$->setUnion($3);$$->setUnion($4);$$->setUnion($6);
	butil.printLog("func_definition : type_specifier ID LPAREN RPAREN compound_statement", $$);

	butil.removeAllCurrentFunVar();	//Upore dileh error ashbe.

/*	$2->setDataType($1->getDataType());

	butil.removeAllCurrentFunVar();
	butil.defineFunction($$);
*/
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
								sutil.pushCurrentParameters(butil);
							}
					statements RCURL 	{

		$$->setSymbol($1); $$->setUnion("\n");  $$->setUnion($3); $$->setUnion("\n"); $$->setUnion($4); $$->setUnion("\n");
		butil.printLog("compound_statement : LCURL statements RCURL", $$);

 		//butil.printLog("==--->>>NEED EXIT SCOPE HERE[1]\n");
 		butil.exitScope();
}
 		    | LCURL {
								butil.enterNewScope();
								butil.returnFlag = "false";
						butil.printSemantic("\n==================+>>>>> AT LINE 235 Bison.y ... LCURL er pore[2].");
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

				$$->setSymbol($1); $$->setUnion($2); $$->setUnion(" "); $$->setUnion($3);

				butil.printLog("declaration_list: declaration_list COMMA ID", $$);

				butil.printDebug("===>>PUSHED TO SYMBOL TABLE $3 at Line 235", $3);
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

					butil.printDebug("===>>>PUSHED TO SYMBOL TABLE LINE 281 $1", $1);
					butil.checkUniquenessAndPushST($1);
 
		}
 		  | ID LTHIRD CONST_INT RTHIRD 	{
 		  		$1->dataType = butil.currentDataType;
				bool flag = sutil.isDuplicate($1, butil);
				SymbolInfoNode *node = $1;
				butil.printSemantic("PRINTING $1 earlier=" + $1->getPrintForTable());

				if(flag == false){
	  				$1->setDataType(butil.currentDataType);
					$1->arrayItems.makeArray($1->getName(), butil.getSize($3), $1->getLineBegin());
					$1->isArray = true;
					sutil.pushArray($1, $3, butil);
				}

				butil.printSemantic("=>>>>===>>>. AFTER pushARRAY Line313.. printing ST");
				butil.printSemantic("PRINTING $1 later=" + node->getPrintForTable());

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
		//butil.printLog("statement : expression_statement and butil.voidCount = " + butil.getString(butil.voidCount), $$);
		
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
	  	$$->setSymbol($1);$$->setUnion($2);$$->setUnion($3);$$->setUnion($4);$$->setUnion($5);$$->setUnion($6);
	  	$$->setUnion("\n"); $$->setUnion($7);
	  	butil.printLog("statement : FOR LPAREN expression_statement expression_statement expression RPAREN statement", $$);
	  }
	  | IF LPAREN expression RPAREN statement %prec Lower_Than_Else 	{
	  	$$->setSymbol($1);$$->setUnion($2);$$->setUnion($3);$$->setUnion($4);$$->setUnion($5);
	  	butil.printLog("statement : IF LPAREN expression RPAREN statement", $$);
	  }
	  | IF LPAREN expression RPAREN statement ELSE statement  {
	  	$$->setSymbol($1);$$->setUnion($2);$$->setUnion($3);$$->setUnion($4);$$->setUnion($5);
	  	$$->setUnion($6); $$->setUnion($7);
	  	butil.printLog("statement : IF LPAREN expression RPAREN statement ELSE statement", $$);
	  }
	  | WHILE LPAREN expression RPAREN statement 	{
		$$->setSymbol($1);$$->setUnion($2);$$->setUnion($3);$$->setUnion($4);$$->setUnion("\n");$$->setUnion($5);
		butil.printLog("statement : WHILE LPAREN expression RPAREN statement", $$);
	  }
	  | PRINTLN LPAREN ID RPAREN SEMICOLON 	{
  		$$->setSymbol($1);$$->setUnion($2);$$->setUnion($3);$$->setUnion($4);$$->setUnion($5);
  		$$->setUnion("\n");
  		butil.printLog("statement : PRINTLN LPAREN ID RPAREN SEMICOLON", $$);
	  }
	  | PRINTLN LPAREN ID RPAREN error 	{
		butil.printError("MISSING SEMICOLON at Line. " + butil.getString($3->getLineBegin()));

  		$$->setSymbol($1);$$->setUnion($2);$$->setUnion($3);$$->setUnion($4);$$->setUnion(";");
  		$$->setUnion("\n");
  		butil.printLog("statement : PRINTLN LPAREN ID RPAREN ERROR", $$);
	  }
	  | RETURN expression SEMICOLON 	{
		  	$$->setSymbol($1); $$->setUnion(" ") ;$$->setUnion($2);$$->setUnion($3);
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

	butil.printLog("expression_statement : SEMICOLON", $$);
	
}
			| expression SEMICOLON  {
				$$->setSymbol($1); $$->setUnion($2);
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
		butil.printSemantic("Line 381 variable -> ID , printing ID now $1 =", node);
		$$->setSymbol(node);
		butil.printLog("variable : ID", $$);
 		$$->setDataType($1->getDataType());

 		butil.pushUndeclared($$);
	}
	 | ID LTHIRD expression RTHIRD {
	 	sutil.checkForArrayExpression($1, $3, butil);

	 	$$->setDataType($1->getDataType());
	 	$$->isArray = true;
	 	$1->isArray = true ;

	 	$$->setSymbol($1);$$->setUnion($2);$$->setUnion($3);$$->setUnion($4);
	 	butil.printLog("variable : ID LTHIRD expression RTHIRD", $$);

	 	sutil.makeArrayExpression($1, $3, butil);
		sutil.makeGoodType($1, butil);
	 	//butil.pushUndeclared($1);

	 }
	 ;

 expression : logic_expression	{
 	$$->setSymbol($1);
 	voidFlag += 2;
 	butil.printLog("expression : logic_expression", $$);
 	
 }
	   | variable ASSIGNOP logic_expression 	{

		 	sutil.printSemantic("==>>Line 405. (exp -> var = logic_exp) Printing variable $1 and logic_expression $3 .. \n", 
		 	butil, $1, $3);

		 	butil.printSemantic("===============>> Line 436.. Before assignOp printing the var.\n $1=" , $1);
		 	sutil.makeGoodType($1, butil);
		 	sutil.assignOp($$, $1, $3, butil);

		 	butil.printSemantic("==++==>>> Line 410. After variable ASSIGNOP logic_exp, printin $$", $$);
		 	sutil.makeSymbol($$, $1 ); sutil.addSpace($$);
 		 	sutil.makeUnion($$, $2 ); sutil.addSpace($$);
 		 	sutil.makeUnion($$, $3 ); 
		 	butil.printLog("expression : variable ASSIGNOP logic_expression", $$);
 
	   }
	   ;

logic_expression : rel_expression 	{
	$$->setSymbol($1);
	butil.printLog("logic_expression : rel_expression", $$);
	voidFlag += 2;
	
}
		 | rel_expression LOGICOP rel_expression 	{
				sutil.makeSymbol($$, $1);	sutil.addSpace($$); 
				sutil.makeUnion($$, $2); 	sutil.addSpace($$);
				sutil.makeUnion($$, $3); 

				sutil.relAndLogicOp($$, $1, $3, $2, butil);

				butil.printLog("logic_expression : rel_expression LOGICOP rel_expression", $$);
		 }
		 ;

rel_expression	: simple_expression  	{
		$$->setSymbol($1);
		butil.printLog("rel_expression : simple_expression", $$);
		voidFlag += 2;
		
}
		| simple_expression RELOP simple_expression	{

			sutil.makeSymbol($$, $1);	sutil.addSpace($$); 
			sutil.makeUnion($$, $2); 	sutil.addSpace($$);
			sutil.makeUnion($$, $3); 	 
 				
			sutil.relAndLogicOp($$, $1, $3, $2, butil);

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
		  	sutil.addOp($$, $1, $3, $2, butil);

		  	sutil.makeSymbol($$, $1); sutil.makeUnion($$, $2); sutil.makeUnion($$, $3);
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
		sutil.makeSymbol($$, $1); sutil.makeUnion($$, $2); sutil.makeUnion($$, $3);
		

    	butil.printLog("term : term MULOP unary_expression", $$);

    	butil.printSemantic("LINE 503 Bison.y ,, after term -> term MULOP unaryExp , printing $$", $$);

     }
     ;

unary_expression : ADDOP unary_expression  {
	sutil.unaryOperation($$, $2, $1, butil);
	sutil.makeSymbol($$, $1); sutil.makeUnion($$, $2);

	butil.printLog("unary_expression : ADDOP unary_expression", $$);

}
		 | NOT unary_expression {
			sutil.unaryOperation($$, $2, $1, butil);
			sutil.makeSymbol($$, $1); sutil.makeUnion($$, $2);

			butil.printLog("unary_expression : NOT unary_expression", $$);
		 }
		 | factor 	{
		 	$$->setSymbol($1);
		 	butil.printLog("unary_expression : factor", $$);
		 	voidFlag += 2;
		 	
		 }
		 ;

factor	: variable 	{
		butil.printDeb("===>Line 498. factor->variable printing $1 now.", $1);
		$$->setSymbol($1);
		butil.printLog("factor : variable", $$);
		butil.printDeb("===>>>Line 501. factor->variable.. printing $$ now.", $$);
	}
	| ID LPAREN argument_list RPAREN	{
		butil.printSemantic("==>>===>>>===>>> Line 504. THIS IS FUNCTION CALL.. printing func name/ID =\n", $1);
		
		voidFlag = 0;
		butil.voidCount = 0;
		string returnType = sutil.functionCall($$, $1, $3, butil);
		butil.printSemantic("Inside Line 580 ... Bison.y");

		 
		butil.currentArgumentList.clear();
 		butil.printSemantic("Inside Line 584 ... Bison.y");

		sutil.makeSymbol($$, $1); sutil.makeUnion($$, $2); sutil.makeUnion($$, $3); sutil.makeUnion($$, $4);
		$$->dataType = returnType ;

		butil.printSemantic("Inside Line 589 ... Bison.y");

		butil.printLog("factor : ID LPAREN argument_list RPAREN", $$);
		butil.printSemantic("Inside Line 592 ... Bison.y ==> ", $$);
		


	}
	| LPAREN expression RPAREN	{
		SymbolInfoNode *node = $2;
		butil.printSemantic("PRINTING $2 at factor->(exp), $2=" + $2->getPrintForTable());
		
		sutil.makeSymbol($$, $1 ); sutil.makeUnion($$, $2 ); sutil.makeUnion($$, $3 ); 
		$$->setDataType(node->dataType);
		$$->actualValue = node->actualValue;
		butil.printSemantic("Line 516. After factor-> ( exp ) .. printing $$=", $$);
		

		butil.printLog("factor : LPAREN expression RPAREN", $$);

	}
	| CONST_INT {
		$1->dataType = "INT";
		$$->setSymbol($1);
		$$->setDataType("INT");
		$$->actualValue = $1->actualValue;
		butil.printDeb("Line 524. factor -> CONST_INT , printing $$", $$);
		butil.printLog("factor : CONST_INT", $$);
		
	}
	| CONST_FLOAT	{
		$1->dataType = "FLOAT";
		$$->setSymbol($1);

		$$->setDataType("FLOAT");
		$$->actualValue = $1->actualValue;
		butil.printDeb("Line 531. factor -> CONST_FLOAT , printing $$", $$);
		butil.printLog("factor : CONST_FLOAT", $$);
		
	}
	| variable INCOP {
		sutil.operate($$, $1, $2, butil);

		butil.printLog("factor : variable INCOP", $$);
		butil.printSemantic("Line 513. After factor -> variable INCOP  printing $$", $$);
	
	}
	| variable DECOP	{
		sutil.operate($$, $1, $2, butil);

		butil.printLog("factor : variable DECOP", $$);
		butil.printSemantic("Line 519. After factor -> variable DECOP  printing $$", $$);
	
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
	butil.currentArgumentList.addArgument($3->getName(), $3->dataType, $3->getLineBegin(), $3->actualValue, $3->isArray);
	butil.printArrayDebug("\nArgument was added ", $3);
}
	      | logic_expression 	{
	      		$$->setSymbol($1);
	      		butil.printLog("arguments : logic_expression", $$);

  				sutil.getArgument($1, butil);
      			butil.currentArgumentList.addArgument($1->getName(), $1->dataType, $1->getLineBegin(), $1->actualValue);
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


	butil.printLog("\n\t\t\tSymbol Table: \n");
	butil.printST();

	butil.printError("\nTotal Errors: " + butil.getString(butil.numErrors), false);
	butil.printLog("\nTotal Lines: " + butil.getString(numLinesOfInput));
	butil.printLog("\nTotal Errors: " + butil.getString(butil.numErrors));

	butil.printSemantic("CURRENT FUNC NAME = " + butil.currentFunctionName + " , current ret type = " + butil.currentFunctionReturnType);



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