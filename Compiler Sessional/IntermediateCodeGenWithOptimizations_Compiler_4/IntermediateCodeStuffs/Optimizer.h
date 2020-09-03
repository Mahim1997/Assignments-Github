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

#include"../Miscalenous/Converter.h"

using namespace std ;

class Optimizer{

public:	
	ofstream code;
	ofstream code2;
	ofstream deb ;
	ifstream input;
	Converter converter ;
	std::vector<string> finalLines;
	std::vector<string> lines;
	std::vector<string> ekdomFinalLines;
	int currentIndex; 
	int untilDotIndex;
	int line ;
	Optimizer(){
		code.open("IntermediateCodeStuffs/OptimizedCodes/OptimizedCode.asm");
		deb.open("IntermediateCodeStuffs/OptimizedCodes/OptimizedDeb.txt");
		code2.open("IntermediateCodeStuffs/OptimizedCodes/1505022.asm");
		input.open("IntermediateCodeStuffs/ICode.asm");
		currentIndex = -1;
		line = 0;
	}
	void optimize(){
		printf("=>Inside Optimze ...\n\n");
		retrieveInput();
		removeNewLinesExtra();
		removeComments();
		
	 
		 

		processCode();
		removeConsecutiveSameCode();
		printFullCode();

	}
	bool contains(string str, string str2){
		if (str.find(str2) != string::npos) {
		//.. found.
			return true;
		} 
		return false ;
	}
	std::vector<std::string> splitString(const std::string& a, char delim)
	{
	    std::vector<std::string> tokens;
	    std::string token;
	    std::istringstream tokenStream(a);
	    while (std::getline(tokenStream, token, delim))
	    {
	        tokens.push_back(token);
	    }
	    return tokens;
	}
	void retrieveInput(){
		std::string str((std::istreambuf_iterator<char>(input)), std::istreambuf_iterator<char>());
		//deb << str << endl << endl ;
		lines = splitString(str, '\n');
	}
	bool isValidForKeyWords(string line)
	{
		int size = 9;
		string keyWords[size] = {".DATA", ".CODE", ".MODEL", ".STACK", "RET", "PROC", "ENDP", "MAIN", ":"};
		for(int i=0; i<size; i++){
			if(contains(line, keyWords[i]) == true)
				return true;
		}
		return false ;
	}
	bool isValid(string line){
		bool flag = false ;
		if(line == "\n"){
			flag = true; 
		}
		else{
			//flag = isValidForKeyWords(line);
			if(isValidForKeyWords(line) == true){
				flag = true; 
			}else{
				//Check for comments in Icode.

			}
		}
		return flag ;
	}
	int actuallyPrintUntilDot(){
		printDeb("==================[[[[ Inside actuallyPrintUntilDot begins ]]]====================");
		int size = (int)lines.size();
		printDeb("Size of lines vector is " + converter.integerToString(size));
		for(int i=0; i<size; i++){
			string line = lines[i];
			printDeb("Line is " + line);
			if(line != ".CODE"){
				finalLines.push_back(lines[i]);
				ekdomFinalLines.push_back(lines[i]);
			}
			else if(line == ".CODE"){
				finalLines.push_back(lines[i]);
				ekdomFinalLines.push_back(lines[i]);
				return (i+1);
			}
		}
		printDeb("==================[[[[ Inside actuallyPrintUntilDot ends ]]]====================");
	}
	bool checkIfSameCode(string ins1, string ins2){
		ins1 = getTrimmedName(ins1);
		ins2 = getTrimmedName(ins2);
		ins1 = removeComment(ins1);
		ins2 = removeComment(ins2);

		printDeb("INSIDE checkIfSameCode ... ");
		printDeb("ins1 = |" + ins1 + "|");
		printDeb("ins2 = |" + ins2 + "|");

		if(ins1 == ins2){
			return true;
		}
		else{
			return false ;
		}
	}
	void removeConsecutiveSameCode(){
		int size = (int)finalLines.size();
	 	int i = untilDotIndex;
	 	while(i < size){
	 		string line = finalLines[i];
	 		printDeb("==>removeConsecutiveSameCode, Line got is |" + line + "|");
	 		if(isValid(line)){
	 			ekdomFinalLines.push_back(line);
	 			printDeb("-->>removeConsecutiveSameCode, Line is valid so i++ , line is |" + line + "|");
	 			i++;
	 		}
	 		else{
 				string thisInstruction = finalLines[i];
 				string nextInstruction = finalLines[i+1];
 				bool flag = checkIfSameCode(thisInstruction, nextInstruction);
 				if(flag == true){
 					//Peephole condition occured !!
 					//Get Rid of BOTH Conditions..
 					ekdomFinalLines.push_back(thisInstruction);
 					i += 2;

 				}	
 				else{
 					i++ ;
 					//Push THIS instruction
 					ekdomFinalLines.push_back(thisInstruction);
 				} 			
	 		}

	 		 
	 	}

	 	printDeb("AFTER removeConsecutiveSameCode , printing vector .. ekdomFinalLines ");
	 	printVector(ekdomFinalLines);
	}
	void processCode(){
		printDeb("====================processCode begins======================");
		int size = (int)lines.size();
	 	
	 	untilDotIndex = actuallyPrintUntilDot();

	 	int i = untilDotIndex ;

	 	printDeb("=============>>>>>Upto actuallyPrintUntilDot ... printing the lines .. untilDotIndex = "
	 		 + converter.integerToString(untilDotIndex));
	 	printVector();

	 	printDeb("Going into while loop, i = " + converter.integerToString(i) + " and size = " + converter.integerToString(size));

	 	while(i < size){
	 		string line = lines[i];
	 		printDeb("==>Line got is |" + line + "|");
		 	if(isValid(line)){
	 			finalLines.push_back(line);
	 			printDeb("-->>Line is valid so push and  i++ , line is |" + line + "|");
	 			i++;
	 		}
	 		else{
 				string thisInstruction = lines[i];
 				string nextInstruction = lines[i+1];
 				bool isPeepHole = checkIfPeepholeHasOccured(thisInstruction, nextInstruction);
 				if(isPeepHole == true){
 					//Peephole condition occured !!
 					//Get Rid of 2nd Condition..
 					printDeb("-->>Peephole occured for  |" + thisInstruction + "| and |" + nextInstruction + "|");
 					string inst_with_comment = thisInstruction + ("\t\t;Peephole occured with " + nextInstruction);
 					finalLines.push_back(inst_with_comment);	//Just push 1st instruction
 					i += 2;

 				}	
 				else if(isPeepHole == false){
 					i++ ;
 					//Push THIS instruction
 					printDeb("-->>Peephole not occured for |" + thisInstruction + "|");
 					finalLines.push_back(thisInstruction);
 				} 			
	 		}

	 		 
	 	}

	 	printDeb("AFTER PROCESS , printing vector .. ");
	 	printVector(finalLines);

	 
	}

	bool isComment(string s){
		s = getTrimmedName(s);
		if(s == ""){
			return true;
		}
		if(s[0] == ';'){
			return true;
		}
		return false ;
	}

	string getNextInstruction(int currentIdx){
		for(int i=currentIdx+1; i<(int)lines.size(); i++){
			if(lines[i] == "\n"){
				//Do Nothing
			}else{
				return lines[i];
			}
		}
		return "\nNOTHING_DONE\n";
	}
    string getTrimmedName(string s)
    {
    	string str = "";
    	int idx = 0; 
    	for(int i=0; i<s.length(); i++){
			if((s[i]!=' ') && (s[i] != '\t')){
				idx = i;
				break ;
			}
    	}
    	for(int i=idx; i<s.length(); i++){
    		str += s[i]; 
    	}
    	return str ;
    }
	bool isMove(string s){
		s = getTrimmedName(s);
		if(s == "\n"){
			return false ;
		}
		if(s == ""){
			return false ;
		}
		if(s == "\t"){
			return false ;
		}
		if(contains(s, "MOV") == true){
			return true ;
		}

		return false ;
	}
	string removeComment(string s){
		string str = "";
		for(int i=0; i<s.length(); i++){
			if(s[i] == ';'){
				break;
			}else{
				str += s[i];
			}
		}
		return str ;
	}
	bool checkForConditionConsecInstr(string ins1, string ins2){
		std::vector<string> arr1 = splitString(ins1, ' ');
		std::vector<string> arr2 = splitString(ins2, ' ');

		printDeb("---->>>Inside checkForConditionConsecInstr ... printing arr1 and arr2");

		printDeb("arr1 size is " + converter.integerToString((int)arr1.size()));
		for(int i=0; i<(int)arr1.size(); i++){
			arr1[i] = removeComma(arr1[i]);
			printDeb(arr1[i]);
		}
		printDeb("arr2 size is " + converter.integerToString((int)arr2.size()));
		for(int i=0; i<(int)arr2.size(); i++){
			arr2[i] = removeComma(arr2[i]);
			printDeb(arr2[i]);
		}


		string leftReg1, rightReg1, leftReg2, rightReg2 ;
		leftReg1 = arr1[1];		rightReg1 = arr1[2];
		leftReg2 = arr2[1];		rightReg2 = arr2[2];
		printDeb("++>>PRINTING leftReg1 = |" + leftReg1 + "| equal to rightReg2 = |" + rightReg2 + "|");
		printDeb("and leftReg2 =|" + leftReg2 + "| equal to rightReg1 = |" + rightReg1 + "|");

		if((leftReg1 == rightReg2) && (leftReg2 == rightReg1)){
			//printDeb("return true since leftReg1 = " + leftReg1 + " equal to rightReg2 = " + rightReg2);
			//printDeb("and leftReg2 = " + leftReg2 + " equal to rightReg1 = " + rightReg1);
			printDeb("Return true from checkForConditionConsecInstr");
			return true ;
		}
		printDeb("Return FALSE from checkForConditionConsecInstr");
		return false;
	}
	bool checkPeeophole(string thisInstruction, string nextInstruction){
		printDeb("===>>> Inside checkPeeophole for two instructions ... ");
		if(thisInstruction == ""){
			return false ;
		}
		if(nextInstruction == ""){
			return false;
		}

		string trim1 = getTrimmedName(thisInstruction);
		string trim2 = getTrimmedName(nextInstruction);
		trim1 = removeComment(trim1);
		trim2 = removeComment(trim2);
		//trim1 = removeTrails(trim1);
		//trim2 = removeTrails(trim2);
		printDeb("1st :" + trim1);
		printDeb("2nd :" + trim2);

		if(contains(trim1, "MOV") && contains(trim2, "MOV")){
			return checkForConditionConsecInstr(trim1, trim2);
		}



		return false ;
	}
	bool checkIfPeepholeHasOccured(string thisInstruction, string nextInstruction){
		if((isMove(thisInstruction)==false) || (isMove(thisInstruction)==false)){
			return false ;
		}
		printDeb("===================================================================================");
		printDeb("First Inst : " + thisInstruction);
		printDeb("Second Inst: " + nextInstruction);
		bool checker = checkPeeophole(thisInstruction, nextInstruction);
		if(checker){
			printDeb("Checker is True => Eliminate Both ");
		}else{
			printDeb("Checker is False => Keep both ");
		}
		printDeb("===================================================================================");


		return checker ;
	}








	int getIndexOfDot(){
		int size = (int)lines.size();
		for(int i=0; i<size; i++){
			string line = lines[i];
			if(line != ".CODE"){
 				
			}
			else if(line == ".CODE"){
 
				return (i+1);
			}
		}
	}

	string get(string s){
		return ("|" + s + "|");
	}
	void removeComments(){
		std::vector<string> v;
		printDeb("=================<<<removeComments Begin>>>>============================");
		for(int i=0; i<(int)lines.size(); i++){
			//lines[i] = getTrimmedName(lines[i]);
			string line = lines[i];
			string s = "Line Found is " + get(line);
			
			if(isComment(lines[i])){
				//printDeb("-->>Line is a comment so i++ , line is |" + lines[i] + "|");
				s += (" ,,, Line is a comment so simply do nothin \n");
			
			}else{
				//printDeb("===>>Pushing back line: " + get(lines[i]));
				s += (" ,,, Push back the line \n");
				v.push_back(lines[i]);
			}
			s += "\n";
			//printDeb(s);
		}
		//printDeb("-->>>Size of v: " + converter.integerToString((int)v.size()));
		for(int i=0; i<(int)v.size(); i++){
			//printDeb(v[i]);
		}
		lines.clear();
		for(int i=0; i<(int)v.size(); i++){
			lines.push_back(v[i]);
		}

		printDeb("AFTER removeComments .. printing vectror");
		printVector(lines);
		printDeb("=================<<<removeComments ENDS>>>>============================");
 
	}

	string removeExtraLine(string s){
		string str = "";
		for(int i=0; i<s.length(); i++){
			if(s[i] == '\n'){
				break;
			}
			else{
				str += s[i];
			}
		}
		return str ;
	}

	void printVector(std::vector<string> v){
		printDeb("-----------------------------------------------------------------------------------------");
		printDeb("Printing vector  v. size = " + converter.integerToString((int)v.size()));
		for(int i=0; i<(int)v.size(); i++){
			printDeb(v[i]);
		}
		printDeb("-----------------------------------------------------------------------------------------");
	}
	void printVector(){
		printDeb("-----------------------------------------------------------------------------------------");
		printDeb("Printing finalLines . size = " + converter.integerToString((int)finalLines.size()));
		for(int i=0; i<(int)finalLines.size(); i++){
			printDeb(finalLines[i]);
		}
		printDeb("-----------------------------------------------------------------------------------------");
	} 
	void printFullCode(){
		for(int i=0; i<(int)finalLines.size(); i++){
			string line = indent(finalLines[i]);
			line = removeExtraLine(line);
			//line = removeTrails(line);
			code << line << endl;
		}
		for(int i=0; i<(int)ekdomFinalLines.size(); i++){
			string line = indent(ekdomFinalLines[i]);
			line = removeExtraLine(line);
			//line = removeTrails(line);
			code2 << line << endl;
		}
	}

	bool isWhiteSpace(string s){
		for(int i=0; i<s.length(); i++){
			if((s[i] != ' ')){
				if(s[i] != '\n'){
					if(s[i] != '\t'){
						return false ;
					}
				}
			}
		}
		return true; 
	}

	void removeNewLinesExtra(){
		std::vector<string> v;
		for(int i=0; i<(int)lines.size(); i++){
			//printDeb("===>>>Line is |" + lines[i] + "|\n\n");
			lines[i] = getTrimmedName(lines[i]);
			//lines[i] = removeTrails(lines[i]);
			if((lines[i] == "\n") || (lines[i] == "") || (isWhiteSpace(lines[i]))){
				//Do Nothing
			}
			else{
				v.push_back(lines[i]);
			}
		}
		printDeb("-->>>Size of v: " + converter.integerToString((int)v.size()));
		for(int i=0; i<(int)v.size(); i++){
			printDeb(v[i]);
		}
		lines.clear();
		for(int i=0; i<(int)v.size(); i++){
			lines.push_back(v[i]);
		}
	}

	string removeTrails(string s){
		string str = "";
		s = getTrimmedName(s);
		for(int i=0; i<s.length(); i++){
			if((s[i] == ' ') || (s[i] == '\t') || (s[i] == '\n')){
				//Normal letters...
				break;
			}
			else{
				//White space encountered
				str += s[i];
			}
		}
		return str ; 
	}
	string removeComma(string s){
		string str = getTrimmedName(s);
		string ans = "";
		for(int i=0; i<str.length(); i++){
			if(str[i] == ','){
				break;
			}
			else{
				ans += str[i];
			}
		}
		ans = removeTrails(ans);
		return ans ;
	}
	string indent(string s){
		if(isValid(s) == true){
			return s ;
		}
		vector<string> line = converter.splitString(s, '\n');
		string res = "";
		for(int i=0; i<(int)line.size(); i++){
			string str = line[i];
			string s1 = "\t" + str + "\n";
			res += s1; 
		}
		return res; 
	}
 
	void printDeb(string s){
		deb << s << endl ;
		//cout << s << endl; 
	}

 
};	