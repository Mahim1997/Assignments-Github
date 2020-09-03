#define INFINITY 10000
#define PAD_LEN_NUM 10
#define PAD_LEN_IP 18
#include "RoutingEntry.h"

class RoutingTable{

public:
    vector<RoutingEntry> entries;
	RoutingTable(){}
	vector<RoutingEntry>::iterator getBeginOfVector(){
		return entries.begin();
	}
	void replace(int idx, RoutingEntry entry){
        if((idx >= 0) && (idx < entries.size())){
            entries[idx] = entry;
        }
	}
	void push_back(RoutingEntry entry){
		entries.push_back(entry);
	}
	void insert(int index, RoutingEntry entry){
		entries.insert(entries.begin() + index, entry);
	}
	RoutingEntry getElementAt(int idx){
		if((idx >= 0) || (idx < entries.size())){
			return entries[idx];
		}
	}
	int getCostOfRouter(string ip){
		int idx = findIndexOf_NextDestination(ip);
		if(idx == -1)
			return -1;
		return entries[idx].getCost();
	}

	
	int findIndexOf_NextDestination(string destinationIP){
		for(int i=0; i<entries.size(); i++){
			if(entries[i].getDestinationIP() == destinationIP){
				return i;
			}
		}
		return -1;
	}
	int findIndexOf_NextHop(string nextHop){
		for(int i=0; i<entries.size(); i++){
			if(entries[i].getNextHop() == nextHop){
				return i;
			}
		}
		return -1;
	}
	string getRoutingTable(){
		string s = "";
		for(int i=0; i<entries.size(); i++){
			s += entries[i].getEntry();
			s += "\n";
		}
		return s;
	}
	string getNextHopUsingDestination(string srcIP, string destIP)
	{
		for(int i=0; i<entries.size(); i++){
			if(entries[i].getDestinationIP() == destIP){
				//destination is found... return the next hop 
				return entries[i].getNextHop();
			}
		}
		return "NULL";
	}

};
