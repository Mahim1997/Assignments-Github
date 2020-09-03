%option noyywrap
%x COMMENT_STATE
%x SLASH_COMMENT_STATE
%x UNFINISHED_STRING
%{
#include<cstdio>
#include<cstdlib>
#include "1505022_SymbolTable.cpp"
#include "1505022_UtilClass.cpp"
int line_cnt = 1;
int comment_len = 0;
int error_no = 0;
Utility util ;

%}

integer [0-9]
digits {integer}+
letter [a-zA-Z]
extremeSpecialSymbol (\\'')
stringSymbols  (\\n|\\t|\\\|\\a|\\f|\\r|\\b|\\v|\\0|\\'|\\")
specialSymbols (\\n|\\t|\\\|\\a|\\f|\\r|\\b|\\v|\\0|\\')
commentSymbols	(\\t|\\\|\\a|\\f|\\r|\\b|\\v|\\0|\\')
backslash [\\]
%%
[ \t\v]+			{}/*DO NOTHING*/



				/*Keyword begins*/
"if"			{
					// printf("\n<IF> at line %d\n", line_cnt);
					util.printForKeywords(yytext, line_cnt, "<IF>", yytext);
				}		
"for"			{
					// printf("<FOR> at line %d\n", line_cnt);
					util.printForKeywords(yytext, line_cnt, "<FOR>", yytext);
				}
"do"			{
					// printf("<DO> at line %d\n", line_cnt);
					util.printForKeywords(yytext, line_cnt, "<DO>", yytext);
				}
"int"			{
					// printf("<INT> at line %d\n", line_cnt);
					util.printForKeywords(yytext, line_cnt, "<INT>", yytext);
				}
"float"			{
					// printf("<FLOAT> at line %d\n", line_cnt);
					util.printForKeywords(yytext, line_cnt, "<FLOAT>", yytext);
				}
"void"			{
					// printf("<VOID> at line %d\n", line_cnt);
					util.printForKeywords(yytext, line_cnt, "<VOID>", yytext);
				}
"switch"		{
					// printf("<SWITCH> at line %d\n", line_cnt);
					util.printForKeywords(yytext, line_cnt, "<SWITCH>", yytext);
				}
"default"		{
					// printf("<DEFAULT> at line %d\n", line_cnt);
					util.printForKeywords(yytext, line_cnt, "<DEFAULT>", yytext);
				}
"else"			{
					// printf("<ELSE> at line %d\n", line_cnt);
					util.printForKeywords(yytext, line_cnt, "<ELSE>", yytext);
				}
"while"			{
					// printf("<WHILE> at line %d\n", line_cnt);
					util.printForKeywords(yytext, line_cnt, "<WHILE>", yytext);
				}
"break"			{
					// printf("<BREAK> at line %d\n", line_cnt);
					util.printForKeywords(yytext, line_cnt, "<BREAK>", yytext);
				}
"char"			{
					// printf("<CHAR> at line %d\n", line_cnt);
					util.printForKeywords(yytext, line_cnt, "<CHAR>", yytext);
				}
"double"		{
					// printf("<DOUBLE> at line %d\n", line_cnt);
					util.printForKeywords(yytext, line_cnt, "<DOUBLE>", yytext);
				}
"return"		{
					// printf("<RETURN> at line %d\n", line_cnt);
					util.printForKeywords(yytext, line_cnt, "<RETURN>", yytext);
				}
"case"			{
					// printf("<CASE> at line %d\n", line_cnt);
					util.printForKeywords(yytext, line_cnt, "<CASE>", yytext);
				}
"continue"		{
					// printf("<CONTINUE> at line %d\n", line_cnt);
					util.printForKeywords(yytext, line_cnt, "<CONTINUE>", yytext);
				}

				/*Keyword ends*/
				
				/*Integer Begins..*/
[0-9]+			{
 
					util.printLiteral(yytext, line_cnt, "CONST_INT", yytext, yytext);
				}
				/*Integer ends*/

				
				/*Floating point literals begin*/
[0-9]*(\.[0-9]+)?(E[+-]?[0-9]+)?	{
 
					util.printLiteral(yytext, line_cnt, "CONST_FLOAT", yytext, yytext);
				}
				
				/*Floating point literals end*/
				
				/*Character literal begins*/
"'"[^(\\\n\t )]"'"		|
"'"{extremeSpecialSymbol}"'"	|
"'"{specialSymbols}"'"		{
					//char ch = yytext[1];
					//printf("\n+==>>Char literal pattern matching, string is %s at line %d, string len = %d AND ",yytext, line_cnt, strlen(yytext));
					//util.print(yytext);
					util.printForCharacter(yytext, line_cnt);
				}	
 
				/*Character literal ends*/ 
 
			/*Punctuation begins*/
 
				/*ADDOP Begins*/
"+"	|
"-"				{
					util.printForOp(yytext, line_cnt, "ADDOP", yytext);
				}
				/*ADDOP Ends*/
				
				/*MULOP begins*/				
"*"	|
"/"	|
"%"				{
					util.printForOp(yytext, line_cnt, "MULOP", yytext);
				}
				/*MULOP ends*/				
				
				/*INCOP begins*/
"++"	|
"--"			{
					util.printForOp(yytext, line_cnt, "INCOP", yytext);
				}
				/*INCOP ends*/				
				
				/*RELOP begins*/
"<"	|
"<="	|
">"	|
">="	|
"=="	|
"!="			{
					util.printForOp(yytext, line_cnt, "RELOP", yytext);

				}		
				/*RELOP ends*/
				
				/*ASSIGNOP begins*/
"="				{
					util.printForOp(yytext, line_cnt, "ASSIGNOP", yytext);
				}
				/*ASSIGNOP ends*/
				
				/*LOGICOP ends*/				
"&&"	|
"||"			{
					util.printForOp(yytext, line_cnt, "LOGICOP", yytext);
				}
				/*LOGICOP ends*/
				
				/*BITOP begins*/
"&"	|
"|"	|
"^"	|
"<<"	|
">>"			{
					util.printForOp(yytext, line_cnt, "BITOP", yytext);
				}
				/*BITOP ends*/
				
				/*NOT begins*/
"!"				{
					util.printForOp(yytext, line_cnt, "NOT", yytext);
				}
				/*NOT end*/				
				
				/*LPAREN begins*/
"("				{
					util.printForOp(yytext, line_cnt, "LPAREN", yytext);
				}
				/*LPAREN ends*/
				
				/*RPAREN begins*/
")"				{
					util.printForOp(yytext, line_cnt, "RPAREN", yytext);
				}
				/*RPAREN ends*/
				
				/*LCURL begins*/
"{"				{
					util.printForOp(yytext, line_cnt, "LCURL", yytext);
				}
				/*LCURL ends*/				
				
				/*RCURL begins*/
"}"				{
					util.printForOp(yytext, line_cnt, "RCURL", yytext);
				}
				/*RCURL ends*/	
				
				/*LTHIRD begins*/
"["				{
					util.printForOp(yytext, line_cnt, "LTHIRD", yytext);
				}
				/*LTHIRD ends*/	

				/*RTHIRD begins*/
"]"				{
					util.printForOp(yytext, line_cnt, "RTHIRD", yytext);
				}
				/*RTHIRD ends*/	
				
				/*COMMA begins*/
","				{
					util.printForOp(yytext, line_cnt, "COMMA", yytext);
				}
				/*COMMA ends*/	
				
				/*SEMICOLON begins*/
";"				{
					util.printForOp(yytext, line_cnt, "SEMICOLON", yytext);
				}
				/*SEMICOLON ends*/	
				
			/*Punctuation ends*/	
				
				
				/*Identifiers begins*/
[a-zA-Z_][a-zA-Z0-9_]*	{
					util.printIdentifier(yytext, line_cnt, "ID" , yytext);
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
					printf("\n--->>>NEWLINE... line_cnt = %d\n", line_cnt);
					line_cnt++;
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
					line_cnt-- ;
					printf("<><><>EOF IS REACHED Finally line_cnt = %d and error_cnt = %d\n", line_cnt, error_no);
					util.printEOF(line_cnt, error_no);
					return 0;
				}
				/*EOF ends*/
%%


int main(int argc,char *argv[]){

	if(argc!=2){
		printf("Please provide input file name and try again\n");
		return 0;
	}
	
	FILE *fin=fopen(argv[1],"r");
	if(fin==NULL){
		printf("Cannot open specified file\n");
		return 0;
	}
	


	yyin= fin;
	yylex();
	fclose(yyin);
	util.closeFiles();
	return 0;
}
