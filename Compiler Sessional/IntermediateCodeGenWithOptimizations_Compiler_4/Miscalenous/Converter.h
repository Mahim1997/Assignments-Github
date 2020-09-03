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


class Converter{

public:
	string DUMMY ;
	Converter(){
		DUMMY = "dumVal";
	}
	string integerToString(int x){
		int a = x;
		stringstream ss;
		ss << a;
		string str = ss.str();
		return str ;
	}
	string floatToString(float x){
		float a = x;
		stringstream ss;
		ss << a;
		string str = ss.str();
		return str ;
	}
	int stringToInteger(string s){
		if(s == "NOT_GIVEN_YET"){
			return -1 ;
		}
		stringstream geek(s);
		int x = 0;
		geek >> x;
		return x;
	}
	float stringToFloat(string s){
		stringstream geek(s);
		float x = 0;
		geek >> x;
		return x;
	}

	string boolToString(bool x){
		bool a = x;
		stringstream ss;
		ss << a;
		string str = ss.str();
		return str ;
	}
	bool stringToBool(string s){
		stringstream geek(s);
		bool x = 0;
		geek >> x;
		return x;
	}
	string makeName(string name1, string name2){
		return (name1 + "_" + name2);
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
};

class FunResult{

public:
	string funName;
	string resName;
	FunResult(){

	}
	FunResult(string f, string r){
		funName = f;
		resName = r;
	}
	FunResult(const FunResult &f){
		funName = f.funName;
		resName = f.resName;
	}
	string getFunResult(){
		return ("funName : " + funName + " , resName : " + resName + "\n");
	}
};