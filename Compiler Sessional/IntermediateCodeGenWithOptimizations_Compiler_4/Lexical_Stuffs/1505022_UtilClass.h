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

using namespace std ;
std::string numToString ( int  Number )
{
 std::ostringstream ss;
 ss << Number;
 return ss.str();
}
class Utility
{
    //SymbolTable *tab ;
    ofstream tokenout;
    ofstream logout ;
    int beginCommentLine ;
    int beginSlashCommentLine ;
    string commentString ;
    string slashComment;
    int unfinishedStringLine;
    string unfinishedString;

    int unfinishedCommentLine;
    string unfinishedComment;

    int increaseInLineCount ;

public:
    Utility( )
    {
        commentString = "";
        slashComment = "";
        unfinishedString = "" ;
        unfinishedComment = "";
//        tokenout= fopen("token.txt","w");
        tokenout.open("Lexical_Stuffs/1505022_token.txt"); //		../doc/my_file.txt
        logout.open("Lexical_Stuffs/1505022_log.txt");
        //tab = new SymbolTable(50);
        //tab->enterScope();
	   increaseInLineCount = 0 ;
    }
/*    ~Utility()
    {
        if(tab)
            delete tab ;
        tab = 0 ;
    }
*/
    void closeFiles()
    {
//        fclose(tokenout);
        tokenout.close();
        logout.close();
    }
    string getStringFromChar(char *ptr)
    {
        string s = "";
        if(ptr != 0)
            s.push_back(ptr[1]);

        return s;
    }

    string getStringFromPointer(char *ptr)
    {
        char ch ;
        bool flag = false ;
        int id = strlen(ptr);
//        printf("\nInside getCharFromPointer function ... strlen = %d\n", id);
        string x = "";
        if(strlen(ptr) == 3)
        {
            ch = ptr[1];
            flag = true ;
        }
        else if(strlen(ptr) == 4)
        {
            char c = ptr[2] ;
            if(c == 'n')
            {
                ch = '\n';
                flag = true;
            }
            else if(c == 't')
            {
                ch = '\t';
                flag = true;
            }
            else if (c == '\\')
            {
                ch = '\\';
                flag = true;
            }
            else if(c == 'a')
            {
                ch = '\a';
                flag = true;
            }
            else if(c == 'f')
            {
                ch = '\f';
                flag = true;
            }
            else if(c == 'r')
            {
                ch = '\r';
                flag = true;
            }
            else if(c == 'b')
            {
                ch = '\b';
                flag = true;
            }
            else if(c == 'v')
            {
                ch = '\v';
                flag = true;
            }
            else if(c == '0')
            {
                ch = '\0';
                flag = true;
            }
            else if(c == '\'')
            {
                ch = '\'';
                flag = true ;
            }
        }
        else if(strlen(ptr) == 5)
        {
            x.push_back(ptr[2]);
            x.push_back(ptr[3]);
        }
        if(flag == false){
//            printf("FLAG = FALSE .. ptr[0] = %c, ptr[1] = %c, ptr[2] = %c, ptr[3] = %c, ptr[4] = %c\n", ptr[0],
//                ptr[1], ptr[2], ptr[3], ptr[4]);
            return x ;
        }
        else {
//            printf("FLAG = TRUE .. ptr[0] = %c, ptr[1] = %c, ptr[2] = %c, ptr[3] = %c\n", ptr[0],
//                ptr[1], ptr[2]);
            string s(1, ch);
            return s ;
        }
    }
    char getCharacter(char *ptr)
    {
        bool flag = false ;
        /// FOR '\n' type
        char ch ;
        char *singleChar = 0;
        char *doubleChar = 0;
//        cout << "\nINSIDE getCharacter(" << ptr << ")" << endl ;
        if(strlen(ptr) <= 3)
        {
            char c = ptr[1] ;
            if(c == 'n')
            {
                ch = '\n';
                flag = true;
            }
            else if(c == 't')
            {
                ch = '\t';
                flag = true;
            }
            else if (c == '\\')
            {
                ch = '\\';
                flag = true;
            }
            else if(c == 'a')
            {
                ch = '\a';
                flag = true;
            }
            else if(c == 'f')
            {
                ch = '\f';
                flag = true;
            }
            else if(c == 'r')
            {
                ch = '\r';
                flag = true;
            }
            else if(c == 'b')
            {
                ch = '\b';
                flag = true;
            }
            else if(c == 'v')
            {
                ch = '\v';
                flag = true;
            }
            else if(c == '0')
            {
                ch = '\0';
                flag = true;
            }
            else if(c == '\'')
            {
                ch = '\'';
                flag = true ;
            }
            else if(c == '\"')
            {
                ch = '\"';
                flag = true ;
            }
        }
        return ch ;
    }
    void print(char *ptr)
    {
        string s = getStringFromPointer(ptr);
//        cout << endl << "+==>> INSIDE TryTests.cpp , STR LEN = " << strlen(ptr) << " Util ... util.print(ptr) ... s = " << s << "|||DONE" << endl;
    }
    string getFullTokenNormal(string tokenname, string lexemename)
    {
        string fullTokenName = "";
        fullTokenName += "<";
        fullTokenName += tokenname;
        fullTokenName += ", ";
        fullTokenName += lexemename;
        fullTokenName += ">";
        return fullTokenName ;
    }
    string getFullTokenForOp(string tokenname, string lexemename)
    {
        string fullTokenName = "";
        fullTokenName += "<";
        fullTokenName += tokenname;
        fullTokenName += ", ";
        fullTokenName += lexemename;
        fullTokenName += ">";
        return fullTokenName ;
    }
    void printSingleCharToFile(char *ptr, int lineNumber, string tokenName, string lexemeName)
    {
        char ch = ptr[1];
        string s(1, ch);
    }
    string getKeyWordLog(int lineNumber, string token)
    {
        string s = "";
        s += "Line No. ";
        s += numToString(lineNumber);
        s += ": Token ";
        s += token ;
        s += "\n";
//        s += " found.\n";
        return s ;
    }
    string getLogString(int lineNumber, string token, string lexeme)
    {
        string s = "" ;
        s += "Line No. ";
        s += numToString(lineNumber);
        s += ": Token <";
        s += token;
        s += ">";
        s += " Lexeme: ";
        s += lexeme ;
//        s += "\n";
        s += " found\n";
        return s ;
    }
    void printForOp(char *ytext, int lineNumber, string tokenname, string lexemename)
    {
        string fullTokenName = getFullTokenForOp(tokenname, lexemename);
//        cout << "\n<><><>PRINTINGI TO FILE ... " << ytext << " at lien = " << lineNumber << " , tokenNAME is " << fullTokenName << " , lex is " << lexemename << endl ;
        tokenout << fullTokenName << ' ';
        string s = getLogString(lineNumber, tokenname, lexemename);
        logout << s ;
    }
    void printForKeywords(char *ytext, int lineNumber, string tokenname, string lexemname)
    {
//        cout << "\n==>>PRINTING KEYWORD TO FILE ..Token =  " << tokenname << " , For lexeme is " << lexemname << " , at line = " << lineNumber << endl ;
        string s = getKeyWordLog(lineNumber, tokenname);
        tokenout << tokenname << ' ';
        logout << s ;
//        tokenout << "\n==>>PRINTING KEYWORD TO FILE ..Token =  " << tokenname << " , For lexeme = " << lexemname << " , at line = " << lineNumber << endl ;
    }
    void printLiteral(char *ytext, int lineNumber, string tokenname, string tokenValue,string lexemname)
    {
        string x = getFullTokenNormal(tokenname, tokenValue);
        //tab->Insert(tokenValue,tokenname);
        //string s = tab->printCurrentNonEmpty();
        //cout << s << endl ;

//        cout << "\n==>>>> PRINTING Literal to file .. Token =  " << x << " , For lexeme is " << lexemname << " , at line = " << lineNumber << endl ;
        tokenout << x << ' ';
        string str = getLogString(lineNumber, tokenname, lexemname);
        logout << str ;
//        tokenout << "\n==>>>> PRINTING Literal to file .. Token =  " << x << " , For lexeme = " << lexemname << " , at line = " << lineNumber << endl ;
    }
    void printIdentifier(char *ytext, int lineNumber, string tokenName, string lexemname)
    {
        string fullTokenName = getFullTokenNormal(tokenName, lexemname);
        string tokenValue = lexemname;
//        cout << "\n<><><>PRINTINGI TO FILE ... " << ytext << " at lien = " << lineNumber << " , tokenNAME is " << fullTokenName << " , lex is " << lexemname << endl ;
       // tab->Insert(tokenValue, tokenName);
        //string x = tab->printCurrentNonEmpty();

        string s = getLogString(lineNumber, tokenName, tokenValue);
//        string s = "Line No. "; s += numToString(lineNumber); s += " : Token <" ; s += tokenName ; s += "> Lexeme "; s += tokenValue; s += " found.\n";
        logout << s ;
        //logout << x ;
        tokenout << fullTokenName << ' ';//        tokenout << "\n<><><>PRINTINGI TO FILE ... " << ytext << " at lien = " << lineNumber << " , tokenNAME = " << fullTokenName << " , lex = " << lexemname << endl ;
    }
    void printForCharacter(char *ytext, int lineNumber)
    {
        string tokenValue = getStringFromPointer(ytext);
        string tokenName = "CONST_CHAR";
//        cout << "\n =====---++++>>> Printin to File ... <" << tokenName << " , " << tokenValue << ">" << endl ;
        string fullToken = "<";
        for(int i=0; i<tokenName.size(); i++){
            fullToken.push_back(tokenName[i]);
        }
        fullToken.push_back(',');
        fullToken.push_back(' ');
        for(int i=0; i<tokenValue.size(); i++){
            fullToken.push_back(tokenValue[i]);
        }
        fullToken.push_back('>');
//        cout << "Token name : " << tokenName <<" , TOKEN VALUE = " << tokenValue << endl ;
        //tab->Insert(tokenValue, tokenName);
        //string x = tab->printCurrentNonEmpty();

        string s = getLogString(lineNumber, tokenName, tokenValue);
//        string s = "Line No. "; s += numToString(lineNumber); s += " : Token <" ; s += tokenName ; s += "> Lexeme "; s += tokenValue; s += " found.\n";
        logout << s ;
        //logout << x ;
        tokenout << fullToken << ' ';
//        tokenout << "\n =====---++++>>> Printin to File ... <" << tokenName << " , " << tokenValue << ">" << endl ;
    }
    string convertedSpecialCharToken(string lexemname)
    {
        string temp = lexemname;
        char *ptr = new char[3];
        string x = "";
//        printf("><><><><><>>< INSIDE convertedSpecialCharToken... \n");
        for(int i=0; i<(temp.size() - 1); i++)
        {
            if(temp[i] == '\\')
            {
//                cout << "temp[i] = \\ found ... temp[i] = " << temp[i] << " AND " ;
                ptr[0] = temp[i]; ptr[1] = temp[i+1];	ptr[2] = '\0';
                char ch = getCharacter(ptr);
//                cout << " AND ch = " << ch ;
                i++;
                x.push_back(ch);
            }
            else{
                x.push_back(temp[i]);
            }
        }
        return x ;
    }

    int getLength(char *ptr)
    {
        int len = 0;
        char *newPtr = new char[strlen(ptr) + 1];
        int i = 0;
        i++ ; ///TO DISCARD FIRST " mark
        while(i < (strlen(ptr)-2))
        {
            if(ptr[i] == '\\')
            {
                if(ptr[i+1] == '\n')
                {
                    ///MULTI LINE STRING case ...
                    i = i + 2 ;
                }
                else{
                    ///SINGLE LINE STRING BUT \n or \t i.e. special chars type things used .
                    char *pointer = new char[3];
                    pointer[0] = '\\';
                    pointer[1] = ptr[i+1];	pointer[2] = '\0';
                    char ch = getCharacter(pointer);
                    newPtr[len] = ch ;
                    i+=2 ;
                    len++ ;
                }
            }
            else {
                newPtr[len] = ptr[i];
                len++; i++ ;
            }
        }
        newPtr[len++] = ptr[i++]; ///TO COPY THE LAST CHARACTER AND NOT THE " mark
        newPtr[len] = '\0';
//        cout << "\n==>>> RETUURNING newPtr = " << newPtr << endl ;
        return len ;
    }
    char *getNewLine(char *ptr)
    {
        int len = 0;
        char *newPtr = new char[strlen(ptr) + 2];
        int i = 0;
//        printf("\n===>>>Inside getNewline() , string is %s...\n", ptr);

        i++ ; ///TO DISCARD FIRST " mark
        while(i < (strlen(ptr)-2))
        {
            if(ptr[i] == '\\')
            {
                if(ptr[i+1] == '\n')
                {
                    ///MULTI LINE STRING case ...
                    i = i + 2 ;
//                    printf("A NEW LINE IS ENCOUNTERED...\n");
                    increaseInLineCount++ ;
                }
                else{
                    ///SINGLE LINE STRING BUT \n or \t i.e. special chars type things used .
                    char *pointer = new char[3];
                    pointer[0] = '\\';
                    pointer[1] = ptr[i+1];
                    pointer[3] = '\0' ;
                    char ch = getCharacter(pointer);
                    newPtr[len] = ch ;
                    i+=2 ;
                    len++ ;
                }
            }
            else {
                newPtr[len] = ptr[i];
                len++; i++ ;
            }
        }
        newPtr[len++] = ptr[i++]; ///TO COPY THE LAST CHARACTER AND NOT THE " mark
        newPtr[len] = '\0';
//        cout << "\n==>>> RETUURNING newPtr = " << newPtr << endl ;
        return newPtr ;
    }
    string getString(char *ptr, int len)
    {
        string s = "";
        for(int i=0; i<len; i++){
            s.push_back(ptr[i]);
        }
        return s ;
    }
    void printLineString(char *ytext, int lineNumber, string tokenname, string lexemname)
    {
        if(lexemname == "\"\"")
        {
            string s = getLogString(lineNumber, tokenname, lexemname);
            logout << s ;
            string fullToken = getFullTokenForOp(tokenname, lexemname);
            tokenout << fullToken << ' ' ;
            return ;
        }
//        lexemname = convertedSpecialCharToken(lexemname);
//        lexemname.clear();
        char *temp = getNewLine(ytext);
        int len = getLength(ytext);
        lexemname = getString(temp, len);
        string s = getLogString(lineNumber, tokenname, lexemname);
        logout << s ;
        string fullToken = getFullTokenForOp(tokenname, lexemname);
        tokenout << fullToken << ' ' ;
    }
    void printComment(char *ytext, int lineCount)
    {
        int len = strlen(ytext);
//        ytext[len - 1] = '\0';  ///To get rid of the last new line char
        //cout << "Token <COMMENT> Lexeme : " << ytext << " |||at line = " << lineCount << endl ;
        string s = getLogString(lineCount, "<COMMENT>", ytext);
        logout << s ;
//        cout << "\n=> Comment: \"" << ytext << "\" at line = " << lineCount << endl ;
    }
    void printCharArray(int len, char *ptr)
    {
        printf("\n");
        for(int i=0; i<len; i++){
            printf("%c<||||||>", ptr[i]);
        }
        printf("\n");
    }



    ///Handle Comments...
    void beginComment(char *ptr, int lineNum)
    {
        commentString.clear();
        for(int i=0; i<strlen(ptr); i++){
            commentString.push_back(ptr[i]);
        }
        this->beginCommentLine = lineNum;
    }
    void addComment(char *yytext)
    {
        for(int i=0; i<strlen(yytext); i++){
            commentString.push_back(yytext[i]);
        }
    }
    void printComment()
    {
        //cout << "Token <COMMENT> , Lexeme: " << commentString << " |||Found\n";
        string s = getLogString(beginCommentLine, "<COMMENT>", commentString);
        logout << s ;
        commentString.clear();
    }
    void beginSlashComment(char *yytext, int lineNumber)
    {
        slashComment.clear();
        beginSlashCommentLine = lineNumber ;
        for(int i=0; i<strlen(yytext); i++){
            slashComment.push_back(yytext[i]);
        }
    }
    void addSlashComment(char *yytext)
    {
        for(int i=0; i<strlen(yytext); i++){
            slashComment.push_back(yytext[i]);
        }
    }
    void printSlashComment()
    {
        //cout << "Token <SLASH_COMMENT> , Lexeme: " << slashComment << " |||Found\n";
        string s = getLogString(this->beginSlashCommentLine, "<COMMENT>", slashComment);
        logout << s ;
        slashComment.clear();
    }
    void beginUnfinishedString(int lineNumber)
    {
        unfinishedStringLine = lineNumber ;
        unfinishedString.clear();
    }
    void addUnfinishedString(char *yytext)
    {
        for(int i=0; i<strlen(yytext); i++){
            unfinishedString.push_back(yytext[i]);
        }
    }
    string getErrorLog(int line, string errorMessage, string toPrint)
    {
        string s = "ERROR AT ";
        s += "Line No. ";
        s += numToString(line);
        s += " ";
        s += errorMessage;
        s += " ";
        s += toPrint ;
        s += "\n";
        return s ;
    }
    void printUnfinishedString()
    {
        string s = getErrorLog(unfinishedStringLine,"Unfinished String",unfinishedString);
        logout << s ;
        unfinishedString.clear();
    }
    void beginUnfinishedComment(int line)
    {
        this->unfinishedCommentLine = line ;
        unfinishedComment.clear();
    }
    void addUnfinishedComment(char *yytext)
    {
        for(int i=0; i<strlen(yytext); i++){
            unfinishedComment.push_back(yytext[i]);
        }
    }
    void printUnfinishedComment()
    {
        string s = getErrorLog(unfinishedCommentLine,"Unfinished COMMENT",unfinishedComment);
        logout << s ;
        unfinishedComment.clear();
    }
    void destroyUnfinishedComment()
    {
        unfinishedComment.clear();
    }
    void printError(int line, string errorType, char *ytext)
    {
        string x = "";
        for(int i=0; i<strlen(ytext); i++){
            x.push_back(ytext[i]);
        }
        string s = getErrorLog(line, errorType, x);
        logout << s ;
    }
    int printCharError(int line, string errorType, char *ytext)
    {
        int inc_line = 0;
        for(int i=0; i<strlen(ytext)-1; i++){
            if(ytext[i] == '\\')
            {
                if(ytext[i+1] == '\n')
                {
                    inc_line ++ ;
                }
            }
        }
        printError(line, errorType, ytext);
        return inc_line ;
    }
    void printEOF(int line_cnt, int error)
    {
        string s = "";
        s += "TOTAL LINE NUMBER : " ;
        s += numToString(line_cnt);
        s += " , TOTAL ERRORS : ";
        s += numToString(error);
        s += "\n";
        logout << s << endl ;
    }
    int getIncreaseInLineCount()
    {
        return increaseInLineCount ;
    }
    void makeIncreaseInLine(int val)
    {
        increaseInLineCount = val ;
    }

};

/*
FOR WINDOWS.. 

flex 1505022.l
g++ lex.yy.c -L"C:\Program Files (x86)\GnuWin32\lib" -lfl -o test.exe
test.exe 1505022_Input.txt
pause
*/
/*
FOR LINUX .. 

flex -o output.cpp 1505022.l
g++ output.cpp -lfl -o out.out
./out.out 1505022_Input.txt
*/
