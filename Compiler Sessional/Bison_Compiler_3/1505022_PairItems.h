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


class PairItems{
public:
    int index;
    string value;
    PairItems(){
    	index = -1;
    	value = "NOT_YET_ASSIGNED";
    }
    std::string getString ( int  Number )
    {
     std::ostringstream ss;
     ss << Number;
     return ss.str();
    }
    PairItems(int i, string v){
        index = i;
        value = v;
    }
    string getString(){
        string s = "";
        s += "<<< Index = " ;
        s += getString(index) ;
        s += " , value = ";
        s += value;
        s += " >>>";
        s += "  ";  
        return s;  
    }
};