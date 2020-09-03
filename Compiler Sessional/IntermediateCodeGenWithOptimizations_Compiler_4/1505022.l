%option noyywrap 
%x COMMENT_STATE
%x SLASH_COMMENT_STATE
%x UNFINISHED_STRING
%option yylineno

%{
#include<cstdio>
#include<cstdlib>
#include<iostream>
#include"Lexical_Stuffs/1505022_UtilClass.h"

#include"SymbolTable/1505022_SymbolTable.h"

#include "y.tab.h"

using namespace std ;

extern YYSTYPE yylval;
extern SymbolInfoNode* node; 
extern SymbolTable *table; 
int comment_len = 0;
int error_no = 0;
int line_cnt = 1;
void yyerror(char *s);
Utility util ;

%}



integer [0-9]
digits {integer}+
letter [a-zA-Z]
stringSymbols  (\\n|\\t|\\\|\\a|\\f|\\r|\\b|\\v|\\0|\\'|\\")
specialSymbols (\\n|\\t|\\\|\\a|\\f|\\r|\\b|\\v|\\0|\\')

commentSymbols	(\\t|\\\|\\a|\\f|\\r|\\b|\\v|\\0|\\')
backslash [\\]
%%
[ \t\v]+			{}/*DO NOTHING*/

"println"		{
					/*println special word*/

					SymbolInfoNode* node = new SymbolInfoNode(yytext, "PRINTLN", line_cnt);
					yylval = node ;
					return PRINTLN ;

				}
				/*Keyword begins*/
"if"			{
					// printf("\n<IF> at line %d\n", line_cnt);
					util.printForKeywords(yytext, line_cnt, "<IF>", yytext);
					SymbolInfoNode* node = new SymbolInfoNode(yytext, "IF", line_cnt);
					yylval = node ;
					return IF ;
				}		
"for"			{
					// printf("<FOR> at line %d\n", line_cnt);
					util.printForKeywords(yytext, line_cnt, "<FOR>", yytext);
					SymbolInfoNode* node = new SymbolInfoNode(yytext, "FOR", line_cnt);
					yylval = node ;
					return FOR ;
				}
"do"			{
					// printf("<DO> at line %d\n", line_cnt);
					util.printForKeywords(yytext, line_cnt, "<DO>", yytext);
					SymbolInfoNode* node = new SymbolInfoNode(yytext, "DO", line_cnt);
					yylval = node ;
					return DO ;
				}
"int"			{
					// printf("<INT> at line %d\n", line_cnt);
					util.printForKeywords(yytext, line_cnt, "<INT>", yytext);
					SymbolInfoNode* node = new SymbolInfoNode(yytext, "INT", line_cnt);
					yylval = node ;
					return INT ;
				}
"float"			{
					// printf("<FLOAT> at line %d\n", line_cnt);
					util.printForKeywords(yytext, line_cnt, "<FLOAT>", yytext);
					SymbolInfoNode* node = new SymbolInfoNode(yytext, "FLOAT", line_cnt);
					yylval = node ;
					return FLOAT ;
				}
"void"			{
					// printf("<VOID> at line %d\n", line_cnt);
					util.printForKeywords(yytext, line_cnt, "<VOID>", yytext);
					SymbolInfoNode* node = new SymbolInfoNode(yytext, "VOID", line_cnt);
					yylval = node ;
					return VOID ;
				}
"switch"		{
					// printf("<SWITCH> at line %d\n", line_cnt);
					util.printForKeywords(yytext, line_cnt, "<SWITCH>", yytext);
					SymbolInfoNode* node = new SymbolInfoNode(yytext, "SWITCH", line_cnt);
					yylval = node ;
					return SWITCH ;
				}
"default"		{
					// printf("<DEFAULT> at line %d\n", line_cnt);
					util.printForKeywords(yytext, line_cnt, "<DEFAULT>", yytext);
					SymbolInfoNode* node = new SymbolInfoNode(yytext, "DEFAULT", line_cnt);
					yylval = node ;
					return DEFAULT ;
				}
"else"			{
					// printf("<ELSE> at line %d\n", line_cnt);
					util.printForKeywords(yytext, line_cnt, "<ELSE>", yytext);
					SymbolInfoNode* node = new SymbolInfoNode(yytext, "ELSE", line_cnt);
					yylval = node ;
					return ELSE ;
				}
"while"			{
					// printf("<WHILE> at line %d\n", line_cnt);
					util.printForKeywords(yytext, line_cnt, "<WHILE>", yytext);
					SymbolInfoNode* node = new SymbolInfoNode(yytext, "WHILE", line_cnt);
					yylval = node ;
					return WHILE ;
				}
"break"			{
					// printf("<BREAK> at line %d\n", line_cnt);
					util.printForKeywords(yytext, line_cnt, "<BREAK>", yytext);
					SymbolInfoNode* node = new SymbolInfoNode(yytext, "BREAK", line_cnt);
					yylval = node ;
					return BREAK ;
				}
"char"			{
					// printf("<CHAR> at line %d\n", line_cnt);
					util.printForKeywords(yytext, line_cnt, "<CHAR>", yytext);
					SymbolInfoNode* node = new SymbolInfoNode(yytext, "CHAR", line_cnt);
					yylval = node ;
					return CHAR ;
				}
"double"		{
					// printf("<DOUBLE> at line %d\n", line_cnt);
					util.printForKeywords(yytext, line_cnt, "<DOUBLE>", yytext);
					SymbolInfoNode* node = new SymbolInfoNode(yytext, "DOUBLE", line_cnt);
					yylval = node ;
					return DOUBLE ;
				}
"return"		{
					// printf("<RETURN> at line %d\n", line_cnt);
					util.printForKeywords(yytext, line_cnt, "<RETURN>", yytext);
					SymbolInfoNode* node = new SymbolInfoNode(yytext, "RETURN", line_cnt);
					yylval = node ;
					return RETURN ;
				}
"case"			{
					// printf("<CASE> at line %d\n", line_cnt);
					util.printForKeywords(yytext, line_cnt, "<CASE>", yytext);
					SymbolInfoNode* node = new SymbolInfoNode(yytext, "CASE", line_cnt);
					yylval = node ;
					return CASE ;
				}
"continue"		{
					// printf("<CONTINUE> at line %d\n", line_cnt);
					util.printForKeywords(yytext, line_cnt, "<CONTINUE>", yytext);
					SymbolInfoNode* node = new SymbolInfoNode(yytext, "CONTINUE", line_cnt);
					yylval = node ;
					return CONTINUE ;
				}

				/*Keyword ends*/
				
				/*Integer Begins..*/
[0-9]+			{
 
					util.printLiteral(yytext, line_cnt, "CONST_INT", yytext, yytext);
					SymbolInfoNode* node = new SymbolInfoNode(yytext, "CONST_INT", line_cnt);
					node->actualValue = yytext;
					yylval = node ;
					return CONST_INT ;
				}
				/*Integer ends*/

				
				/*Floating point literals begin*/
[0-9]*(\.[0-9]+)?(E[+-]?[0-9]+)?	{
 
					util.printLiteral(yytext, line_cnt, "CONST_FLOAT", yytext, yytext);
					SymbolInfoNode* node = new SymbolInfoNode(yytext, "CONST_FLOAT", line_cnt);
					node->actualValue = yytext;
					yylval = node ;
					return CONST_FLOAT ;
				}
				
				/*Floating point literals end*/
				
				/*Character literal begins*/
"'"[^(\\\n\t )]"'"		|
"'"{specialSymbols}"'"		{
					//printf("\n+==>>Char literal pattern matching, string is %s at line %d, string len = %d AND ",yytext, line_cnt, strlen(yytext));
					//util.print(yytext);
					util.printForCharacter(yytext, line_cnt);
					SymbolInfoNode* node = new SymbolInfoNode(yytext, "CONST_CHAR", line_cnt);
					yylval = node ;
					return CONST_CHAR ;
				}	
 
				/*Character literal ends*/ 
 
			/*Punctuation begins*/
 
				/*ADDOP Begins*/
"+"	|
"-"				{
					util.printForOp(yytext, line_cnt, "ADDOP", yytext);
					SymbolInfoNode* node = new SymbolInfoNode(yytext, "ADDOP", line_cnt);
					yylval = node ;
					return ADDOP ;
				}
				/*ADDOP Ends*/
				
				/*MULOP begins*/				
"*"	|
"/"	|
"%"				{
					util.printForOp(yytext, line_cnt, "MULOP", yytext);
					SymbolInfoNode* node = new SymbolInfoNode(yytext, "MULOP", line_cnt);
					yylval = node ;
					return MULOP ;
				}
				/*MULOP ends*/				
				
				/*INCOP begins*/
"++"			{
					util.printForOp(yytext, line_cnt, "INCOP", yytext);
					SymbolInfoNode* node = new SymbolInfoNode(yytext, "INCOP", line_cnt);
					yylval = node ;
					return INCOP ;
				}
				/*INCOP ends*/	

				/*DECOP begins*/
"--"			{
					util.printForOp(yytext, line_cnt, "DECOP", yytext);
					SymbolInfoNode* node = new SymbolInfoNode(yytext, "DECOP", line_cnt);
					yylval = node ;
					return DECOP ;
				}
				/*DECOP ends*/			
				
				/*RELOP begins*/
"<"	|
"<="	|
">"	|
">="	|
"=="	|
"!="			{
					util.printForOp(yytext, line_cnt, "RELOP", yytext);
					SymbolInfoNode* node = new SymbolInfoNode(yytext, "RELOP", line_cnt);
					yylval = node ;
					return RELOP ;
				}		
				/*RELOP ends*/
				
				/*ASSIGNOP begins*/
"="				{
					util.printForOp(yytext, line_cnt, "ASSIGNOP", yytext);
					SymbolInfoNode* node = new SymbolInfoNode(yytext, "ASSIGNOP", line_cnt);
					yylval = node ;
					return ASSIGNOP ;
				}
				/*ASSIGNOP ends*/
				
				/*LOGICOP ends*/				
"&&"	|
"||"			{
					util.printForOp(yytext, line_cnt, "LOGICOP", yytext);
					SymbolInfoNode* node = new SymbolInfoNode(yytext, "LOGICOP", line_cnt);
					yylval = node ;
					return LOGICOP ;
				}
				/*LOGICOP ends*/
				
				/*BITOP begins*/
"&"	|
"|"	|
"^"	|
"<<"	|
">>"			{
					util.printForOp(yytext, line_cnt, "BITOP", yytext);
					SymbolInfoNode* node = new SymbolInfoNode(yytext, "BITOP", line_cnt);
					yylval = node ;
					return BITOP ;
				}
				/*BITOP ends*/
				
				/*NOT begins*/
"!"				{
					util.printForOp(yytext, line_cnt, "NOT", yytext);
					SymbolInfoNode* node = new SymbolInfoNode(yytext, "NOT", line_cnt);
					yylval = node ;
					return NOT ;
				}
				/*NOT end*/				
				
				/*LPAREN begins*/
"("				{
					util.printForOp(yytext, line_cnt, "LPAREN", yytext);
					SymbolInfoNode* node = new SymbolInfoNode(yytext, "LPAREN", line_cnt);
					yylval = node ;
					return LPAREN ;
				}
				/*LPAREN ends*/
				
				/*RPAREN begins*/
")"				{
					util.printForOp(yytext, line_cnt, "RPAREN", yytext);
					SymbolInfoNode* node = new SymbolInfoNode(yytext, "RPAREN", line_cnt);
					yylval = node ;
					return RPAREN ;
				}
				/*RPAREN ends*/
				
				/*LCURL begins*/
"{"				{
					util.printForOp(yytext, line_cnt, "LCURL", yytext);
					SymbolInfoNode* node = new SymbolInfoNode(yytext, "LCURL", line_cnt);
					yylval = node ;
					return LCURL ;
				}
				/*LCURL ends*/				
				
				/*RCURL begins*/
"}"				{
					util.printForOp(yytext, line_cnt, "RCURL", yytext);
					SymbolInfoNode* node = new SymbolInfoNode(yytext, "RCURL", line_cnt);
					yylval = node ;
					return RCURL ;
				}
				/*RCURL ends*/	
				
				/*LTHIRD begins*/
"["				{
					util.printForOp(yytext, line_cnt, "LTHIRD", yytext);
					SymbolInfoNode* node = new SymbolInfoNode(yytext, "LTHIRD", line_cnt);
					yylval = node ;
					return LTHIRD ;
				}
				/*LTHIRD ends*/	

				/*RTHIRD begins*/
"]"				{
					util.printForOp(yytext, line_cnt, "RTHIRD", yytext);
					SymbolInfoNode* node = new SymbolInfoNode(yytext, "RTHIRD", line_cnt);
					yylval = node ;
					return RTHIRD ;
				}
				/*RTHIRD ends*/	
				
				/*COMMA begins*/
","				{
					util.printForOp(yytext, line_cnt, "COMMA", yytext);
					SymbolInfoNode* node = new SymbolInfoNode(yytext, "COMMA", line_cnt);
					yylval = node ;
					return COMMA ;
				}
				/*COMMA ends*/	
				
				/*SEMICOLON begins*/
";"				{
					util.printForOp(yytext, line_cnt, "SEMICOLON", yytext);
					SymbolInfoNode* node = new SymbolInfoNode(yytext, "SEMICOLON", line_cnt);
					yylval = node ;
					return SEMICOLON ;
				}
				/*SEMICOLON ends*/	
				
			/*Punctuation ends*/	
				
				
				/*Identifiers begins*/
[a-zA-Z_][a-zA-Z0-9_]*	{
					util.printIdentifier(yytext, line_cnt, "ID" , yytext);
					SymbolInfoNode* node = new SymbolInfoNode(yytext, "ID", line_cnt);
					yylval = node ;
					return ID ;
				}
	
				/*Identifiers ends*/				
				
				/*All string begins*/
				
["]([^"\\\n]|\\.|\\\n)*["]   {
					//printf("\n==>>INSIDE FLEX .. STRING is %s\n", yytext);
					//printf("INSIDE FLEX... STRING IS %s\n", yytext);
					util.printLineString(yytext, line_cnt, "STRING", yytext);
					int increaseInLine = util.getIncreaseInLineCount();
					util.makeIncreaseInLine(0);
					line_cnt += increaseInLine ;
					int l2 = line_cnt - increaseInLine ;
					SymbolInfoNode* node = new SymbolInfoNode(yytext, "STRING", l2, line_cnt);
					yylval = node ;

					return STRING ;
				}				
				/*All string ends*/				
				
				/*Comment Begins*/
				
"//"			{
					//printf("BEGIN COMMENT_STATE at line %d\n", line_cnt);
					util.beginComment(yytext, line_cnt);
					BEGIN COMMENT_STATE ;
				}
<COMMENT_STATE>[ \t\v]+	{/*DO NOTHING*/
					//printf("TAB/SPACE etc is found at lin = %d\n", line_cnt);
					util.addComment(yytext);
				}

<COMMENT_STATE>(\\\n)	{
					//printf("\nA BACKSLASH IS FOUND Simply going to next line so do nothing\n");
					/*Simply going to next line so do nothing..*/
					line_cnt++;
				}


<COMMENT_STATE>({letter}|{specialSymbols}|{digits})*		{
					//printf("<ACTUAL COMMENT> : %s\n", yytext);
					util.addComment(yytext);
				}
				
<COMMENT_STATE>(\n)		{
					//printf("ENDING COMMENT_STATE , starting INITIAL\n");
					//util.endComment(line_cnt);
					util.printComment();
					line_cnt++;
					BEGIN INITIAL ;
				}
				
				/*Comment ends*/	
					
					
					
				/*Multiline Comments begin*/
				
"/*"			{
					BEGIN SLASH_COMMENT_STATE;
					util.beginUnfinishedComment(line_cnt);
					util.addUnfinishedComment(yytext);
					util.beginSlashComment(yytext, line_cnt);
				}
<SLASH_COMMENT_STATE>"*/"	{
					util.addSlashComment(yytext);
					util.printSlashComment();
					util.destroyUnfinishedComment();
					BEGIN INITIAL ;
				}
<SLASH_COMMENT_STATE>[\n]	{
					util.addSlashComment(yytext);
					util.addUnfinishedComment(yytext);
					line_cnt++; 
					printf("INSIDE SLASH_COMMENT_STATE new line is found at line = %d and s = %s\n", (line_cnt - 1), yytext);
				}
<SLASH_COMMENT_STATE>(.)	{
					//printf("INSIDE SLASH_COMMENT_STATE s = %s\n", yytext);
					util.addUnfinishedComment(yytext);
					util.addSlashComment(yytext);					
				}
				
				/*Multiline Comments end*/
				
				
				
				/*newline begins*/
[\n]			{
					//printf("\n--->>>NEWLINE... line_cnt = %d\n", line_cnt);
					line_cnt++;
					//yylineno-- ;
				}
				/*newline ends*/
				
			/*Lexical Errors Begin*/
			
				/*Too many decimal point errors begin*/
({integer})?"."{integer}"."({integer}|".")*	{
					error_no++;
					printf("\n<LEX ERROR>TOO MANY DECIMAL POINT ERRORS at line = %d and string is %s\n", line_cnt, yytext);
					util.printError(line_cnt, "TOO MANY DECIMAL POINT ERRORS", yytext);
				}
			
				/*Too many decimal point errors end*/		
			
				
				/*Ill Formed Number begins*/
[0-9]*(\.[0-9]+)?((E[+-]?)+(\.)*[0-9]+(\.)([0-9]+(\.)*)+)+	{
					error_no++;
					printf("<LEX ERROR>Ill Formed Number at line %d and string = %s\n", line_cnt, yytext);
					util.printError(line_cnt, "Ill Formed Number", yytext);
				}
			
				/*Ill Formed Number ends*/			

				/*Invalid Suffix on numeric constant or invalid prefix on identifier/character sequence begin*/
{integer}+[a-zA-Z]+	|
{integer}+({letter}|{specialSymbols})+	{
					error_no++;
					printf("<LEX ERROR>Invalid Suffix on numeric constant or invalid prefix on identifier/character sequence at line %d and string is %s\n", line_cnt, yytext);
					util.printError(line_cnt, "Invalid Suffix on numeric constant or invalid prefix on identifier/character sequence", yytext);
				}

				/*Invalid Suffix on numeric constant or invalid prefix on identifier/character sequence ends*/

				
				/*Multichar const error begin*/
"'"[^'\n][^'\n]+"'"		{
					printf("<LEX ERROR> Multichar const error at line %d string %s\n", line_cnt, yytext);
					error_no++;
					util.printError(line_cnt, "Multichar const error", yytext);
				}
				/*Multichar const error end*/
				
				/*Unfinished char error begin*/
"'"[^'\n][^'\n]*	|
"'""'"					{
					error_no++;
					printf("<LEX ERROR> Unfinished char at line %d and string %s\n", line_cnt, yytext);
					util.printError(line_cnt, "Unfinished character", yytext);
					//int increase_line = util.printCharError(line_cnt, "Unfinished character", yytext);
					//line_cnt += increase_line ;
					BEGIN INITIAL;
				}	
"'"{backslash}"'"		{
					error_no++;
					printf("<LEX ERROR> Unfinished char at line %d and string %s\n", line_cnt, yytext);
					util.printError(line_cnt, "Unfinished character", yytext);
					BEGIN INITIAL;
				}				
				/*Unfinished char error end*/
				
				/*Unfinished string begin*/
["]				{
					BEGIN UNFINISHED_STRING ;
					util.beginUnfinishedString(line_cnt);
				}
<UNFINISHED_STRING>({integer}|{letter}|{commentSymbols}|\\\"|[ \t\v]|\.)*	{
					util.addUnfinishedString(yytext);
				}
<UNFINISHED_STRING>(\\)	{
					util.addUnfinishedString(yytext);
				}
<UNFINISHED_STRING>(\\\n)	{
					printf("\nBACKSLASH NEWLINE is found at line %d\n", line_cnt);
					line_cnt++ ;
					BEGIN UNFINISHED_STRING ;
				}
<UNFINISHED_STRING>[\n]	{
					util.printUnfinishedString();
					line_cnt++ ;
					error_no++ ;
					BEGIN INITIAL ;
				}
<UNFINISHED_STRING><<EOF>>	{
					util.printUnfinishedString();
					error_no++ ;
					BEGIN INITIAL ;
					printf("UNFINISHED_STRING e EOF reach korse at line %d\n", line_cnt);
				}
			
				/*Unfinished string end*/

				/*Unfinished Comment begin*/

<SLASH_COMMENT_STATE><<EOF>>	{
					error_no++;
					printf("<LEX ERROR>Unfinished Comment EOF REACHED at line %d\n", line_cnt);
					util.printUnfinishedComment();
					BEGIN INITIAL ;
				}				
				
				/*Unfinished Comment end*/
				
				/*Unrecognized char begin*/
(.)				{
					error_no++;
					printf("<LEX ERROR>Unrecognized char at line %d and lexeme is %s\n", line_cnt, yytext);
					util.printError(line_cnt, "Unrecognized character", yytext);
				}				
				/*Unrecognized char end*/
				
				
			/*Lexical Errors End*/
			
			
			
			
			
				/*EOF begins*/
<<EOF>>			{
					//line_cnt-- ;
					//printf("\n\n\n<><Flex><>EOF IS REACHED Finally line_cnt = %d and error_cnt = %d\n", line_cnt, error_no);
					util.printEOF(line_cnt, error_no);
					return 0;
				}
				/*EOF ends*/
%%

