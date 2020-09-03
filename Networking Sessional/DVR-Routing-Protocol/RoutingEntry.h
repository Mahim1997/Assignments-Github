#include<iostream>
#include<cstdio>
#include<cstdlib>
#include<cstring>
#include<list>
#include <vector>
#include<sstream>

#include "util.h"

using namespace std ;

class RoutingEntry{
	string destinationIP;	//Destination IP Address
	string nextHop;	//Next Hop IP Address
	int cost;	//estimated distance to destination router
public:
//Constructor begins
	RoutingEntry(){cost = INFINITY; nextHop = "-";}
	RoutingEntry(string dest, int c, string nextH){nextHop = nextH; cost = c; destinationIP = dest;}
//Constructor ends

//Getter and Setter functions begin
	string getDestinationIP(){return destinationIP;}
	string getNextHop(){return nextHop;}
	int getCost(){return cost;}

	void setDestinationIP(string s){destinationIP = s;}
	void setCost(int c){cost = c;}
	void setNextHop(string ip){nextHop = ip;}
//Getter and Setter functions end


	string getEntry(){
		string s = pad(destinationIP, PAD_LEN_IP)  + ", " + pad(nextHop, PAD_LEN_IP) + ", " + pad(integerToString(cost), PAD_LEN_NUM);
		return s;
	}
};

