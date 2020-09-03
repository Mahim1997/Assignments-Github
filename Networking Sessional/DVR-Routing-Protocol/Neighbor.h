#include<sstream>
#define INFINITY 10000
class Neighbor{
public:
	string ipAddress;
	int numberOfClocksMissed;
	int lastCost;
	bool isNeighbor;
	int backupCost ;

	int getBackupCost(){
		return backupCost;
	}
	string getIPAddress(){
		return ipAddress;
	}
	Neighbor(string i, int l){
        ipAddress = i;
        lastCost = l;
        numberOfClocksMissed = 0;
        isNeighbor = true;
	}
	Neighbor(string i){
		ipAddress = i;
		numberOfClocksMissed = 0;
		isNeighbor = true;
	}
	void increaseClockCount(){
		numberOfClocksMissed++;
	}
	void hasMissedClock(){
        increaseClockCount();
	}
	void messageReceived(){
        numberOfClocksMissed = 0;
        isNeighbor = true;
	}
	int getNumberOfClocksMissed(){
        return numberOfClocksMissed;
	}

	void removeNeighbour(){
		isNeighbor = false;
		backupCost = lastCost;
		lastCost = INFINITY;
		
	}
	void makeNeighbour(){
		isNeighbor = true;
		lastCost = backupCost;
	}
	void setLastCost(int c){
		lastCost = c;
	}
	int getLastCost(){
		return lastCost;
	}
	int getCost(){
		return lastCost;
	}
	void setCost(int c){
		lastCost = c;
	}
	string integerToString(int x){
        stringstream ss;
        ss << x;
        string s = ss.str();
        return s;
    }
    string booleanToString(bool x){
    	stringstream ss;
        ss << x;
        string s = ss.str();
        return s;	
    }
    string getNeighbor(){
        string s =  "IP : " + ipAddress + " , Num Clocks missed: " + integerToString(numberOfClocksMissed) + " , last Cost : " + integerToString(lastCost);
        return s;
    }

    void printNeighbour(){
    	string s ;
    	s = "Neighbour: IP <" + ipAddress + "> lastCost = " + integerToString(lastCost);
    	s += (" , backupCost = " + integerToString(backupCost) + " , isNeighbor = " + booleanToString(isNeighbor));  
    	cout << s << endl;
    }
};
