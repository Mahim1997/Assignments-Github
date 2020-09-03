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
	string DUMMY = "dumVal";
	Converter(){

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
};