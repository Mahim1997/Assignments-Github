#include<iostream>
#include<cstdio>
#include<cstdlib>
#include<cstring>
#include<list>
#include<sstream>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#define INFINITY 10000
#define PAD_LEN_NUM 10
#define PAD_LEN_IP 18

using namespace std ;
std::string trim(std::string const& str)
{
    std::string word;
    std::stringstream stream(str);
    stream >> word;

    return word;
}
vector<string> splitString(const string &a, char delim){
    vector<string>tokens;
    string token;
    std::istringstream tokenStream(a);

    while(std::getline(tokenStream, token, delim)){
        tokens.push_back(token);
    }
    return tokens;

}
string integerToString(int x){
	stringstream ss;
	ss << x;
	string s = ss.str();
	return s;
}

string pad(string s, int len){
	string str = s;
	for(int i=s.length(); i<len; i++){
		str += " ";
	}
	return str ;
}

int stringToInteger(string s){
    stringstream geek(s);
    int x = 0;
    geek >> x;
    return x;
}
string decodeIP(char *buffer, string option){
    in_addr temp_addr;
    string str ;
    if(option == "INT"){
        int *buff = static_cast<int*>(static_cast<void*>(buffer));
        temp_addr.s_addr = *buff;
        str = inet_ntoa(temp_addr);
    }
    else if(option == "SHORT"){
        short *buff = static_cast<short*>(static_cast<void*>(buffer));
        short len = *buff;
        stringstream ss;
        //cout << "INSIDE util.h printing short <" << len << ">\n";
        ss << len;
        str = ss.str();
        //str = inet_ntoa(temp_addr);    
    }
    else if(option == "INT_NORMAL"){
        int *buff = static_cast<int*>(static_cast<void*>(buffer));
        int len = *buff;
        //cout << "INSIDE util.h printing int <" << len << ">\n";
        stringstream ss;
        ss << len;
        str = ss.str();
    }

    return str;
}