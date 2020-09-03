
#include "RoutingTable.h"
#include "Neighbor.h"

#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>

#define INFINITY 10000
#define PAD_LEN_NUM 10
#define PAD_LEN_IP 18
#define MAX_CLOCK_MISSED 4

class Router{
private:
	string thisIpAddress;
	RoutingTable routingTable;
	vector<string> otherRoutersIP;
	int sockid;
public:
	void setSocketID(int s){sockid = s;}
    vector<Neighbor> neighbours;
	Router(string s){thisIpAddress = s;}
    string getIPAddress(){return thisIpAddress;}
	string getRoutingTable(){
		string s = "";
		s += "\n------------------- Showing Routing Table for Router IP: " + thisIpAddress + " -----------------------\n\n";
		s += ( pad("Destination IP", PAD_LEN_IP) + ", " + pad("Next Hop", PAD_LEN_IP) + ", " + pad("Cost ", PAD_LEN_NUM) + "\n\n");
		s += routingTable.getRoutingTable();
		s += "\n\n=================== Done Showing Routing Table for Router IP: " + thisIpAddress + " ========================\n";
		return s;
	}

	void showRouter(){
        string s = getRoutingTable();
		cout << s << endl ;
	}



	void setUpInitialRoutingTable(vector<string> sourceIps, vector<string> destIps, vector<int> costs
		, set<string> allIpAddresses){
		//cout << "Inside Router.h setUpInitialRoutingTable () , printing the vectors .... \n";
		//cout << "Size: " << sourceIps.size() << endl ;
	    std::set<std::string>::iterator it = allIpAddresses.begin();

	    // Iterate till the end of set
	    while (it != allIpAddresses.end())
	    {
	        // Use the element
	        if(*it != thisIpAddress){
	        	otherRoutersIP.push_back(*it);
	        }
	        //Increment the iterator
	        it++;
	    }

//First make all routers INIFNINITY ....
	    for(int i=0; i<otherRoutersIP.size(); i++){
	    	addRoutingEntry(otherRoutersIP[i],  INFINITY, "-");
	    }

//NOW Override them ...
		for(int i=0; i<sourceIps.size(); i++){
			//cout << sourceIps[i] << " ||| " << destIps[i] << " |||| " << costs[i] << endl ;
			if(sourceIps[i] == thisIpAddress){
				//LINK FROM THIS IP AS SOURCE EXITS ...
				addRoutingEntry(destIps[i], costs[i] , destIps[i]);
				Neighbor neb(destIps[i], costs[i]);
				neighbours.push_back(neb);

			}
			if(destIps[i] == thisIpAddress){
                addRoutingEntry(sourceIps[i], costs[i] , sourceIps[i]);
                Neighbor neb(sourceIps[i], costs[i]);
				neighbours.push_back(neb);
			}
		}

	}

	void processMessage(char* buffer, int sockfid){
		string message = buffer;
	    if(message.find("send") != string::npos){           
            string src = decodeIP(buffer+4, "INT");
            string dest = decodeIP(buffer+8, "INT");
            string shortLen_string = decodeIP(buffer+12, "SHORT");
            int len = stringToInteger(shortLen_string);

            string msg = "";
            for(int i=14; i<(14+len); i++){
            	msg += buffer[i];
            }

            string nextHop = routingTable.getNextHopUsingDestination(thisIpAddress, dest);
            string toSendMsg = formForwardMessage(dest, len, msg);
            string str = msg + " packet forwarded to " + nextHop + " (printed by " + thisIpAddress + ")";
			cout << str << endl ;
            if(nextHop != "NULL"){
            	sendMessageToIP(nextHop, toSendMsg);
        	}
        }
	    else if(message.find("show") != string::npos){
            //Need to show message
            showRouter();
	    }
	    else if(message.find("frwd") != string::npos){
	    	//message was forwarded to here...
	    	//cout << "FRWD MESSAGE RECEIVED !! " << message << endl;
	    	process_forwardMessage(message);
	    }
        else if(message.find("Neighbour") != string::npos){
            //This message is sent by neighbour so update routing table accordingly ...
            updateRoutingTable(message);

        }
        
        else if(message.find("down")  != string::npos){
            //....
        }
        else if(message.find("up") != string::npos){
            //...
        }
        else if(message.find("clk") != string::npos){
            //send to all neighbors .. also increase count for each neighbor ...
            for(int i=0; i<neighbours.size(); i++){
                neighbours[i].increaseClockCount();
            }
            sendToAllNeighbors(sockfid);
            checkDownLink();
            //for(int i=0; i<neighbours.size(); i++){
            	//neighbours[i].printNeighbour();
            //}
        }
        else if(message.find("cost") != string::npos){
        	string src = decodeIP(buffer+4, "INT");
            string dest = decodeIP(buffer+8, "INT");
            //string cost_str = decodeIP(buffer + 12, "INT_NORMAL");
            string cost2 = decodeIP(buffer+12, "SHORT");

            //cout << "cost_string is <" << cost_str << "> and cost2 is <" << cost2 << ">\n";
            
            int cost = stringToInteger(cost2);
        	checkAndUpdateCost(src, dest, cost);
        }

	}

	void checkDownLink(){
		string str;
		str = "[On getting clk]Inside checkDownLink() ... printing for each neighbour their numberOfClocksMissed ... \n";
		//cout << str;
		int idx = -1;
		int backUpCost;
		string ipOfNeighborGone = "NULL";	//eg.B 
		for(int i=0; i<neighbours.size(); i++){
			if(neighbours[i].numberOfClocksMissed == MAX_CLOCK_MISSED){
				//This neighbours is gone... update routing table accordingly
				string strToPrint = "The link between " + thisIpAddress + " and " + neighbours[i].getIPAddress() + " is down\n";
				cout << strToPrint;
				idx = i;
				ipOfNeighborGone = neighbours[idx].getIPAddress();
				backUpCost = neighbours[idx].getCost();
				neighbours[idx].removeNeighbour();
		//Updating routing table accordingly...
				makeDownLink(ipOfNeighborGone);
			}
			string s = neighbours[i].getIPAddress() + " <" + integerToString(neighbours[i].numberOfClocksMissed) + ">, ";
			str += s;
		}
		//cout << str << endl;

		


	}

	void makeDownLink(string ipOfNeighborGone){
		//Put these lines as function param (ipOfNeighborGone) ...
		for(int i=0; i<routingTable.entries.size(); i++){
			if((routingTable.entries[i].getDestinationIP() == ipOfNeighborGone)&&(routingTable.entries[i].getNextHop() == ipOfNeighborGone)){
				routingTable.entries[i].setNextHop("-");
				routingTable.entries[i].setCost(INFINITY);
			}
			if((routingTable.entries[i].getDestinationIP() != ipOfNeighborGone) && (routingTable.entries[i].getNextHop() == ipOfNeighborGone)){
				routingTable.entries[i].setNextHop("-");
				routingTable.entries[i].setCost(INFINITY);

				//if destination is my neighbour THEN nextHop = dest AND cost = link cost
				for(int i=0; i<neighbours.size(); i++){
					if((routingTable.entries[i].getDestinationIP() == neighbours[i].getIPAddress()) && (neighbours[i].getIPAddress() != ipOfNeighborGone)){
						//if(neighbours[i].isNeighbor == true){
						if(neighbours[i].getCost() != INFINITY){	
							routingTable.entries[i].setNextHop(neighbours[i].getIPAddress());
							routingTable.entries[i].setCost(neighbours[i].getCost());
							break;
						}
					}
				}
			}
		}
	}
	bool isNeighbor(string neighborIP){
		for(int i=0; i<neighbours.size(); i++){
			if(neighbours[i].getIPAddress() == neighborIP){
				return true;
			}
		}
		return false;
	}
	void checkAndUpdateCost(string src, string dest, int cost ){
		//cout << "Inside checkAndUpdateCost() , src: <" << src << "> dest: <" << dest << "> cost: <" << cost << ">\n";  
	//Obtain neighbour's IP address ...
		string neighborIP;
		if(src == thisIpAddress){
			neighborIP = dest;
		}else{
			neighborIP = src;
		}

	//Check if this is actually neighbor or not [DORKAR NAI, cost dite parbe na unless neighbour] ..???
		//if(isNeighbor(neighborIP) == false){
		//	return;
		//}

		int oldCost = -1;
//UPDATE the cost ... in neighbours vector (link) ...
		for(int i=0; i<neighbours.size(); i++){
			if(neighbours[i].getIPAddress() == neighborIP){
				oldCost = neighbours[i].getCost();
				neighbours[i].setCost(cost);
				break;	
			}
		}


		for(int i=0; i<routingTable.entries.size(); i++){
	//Case 1 , destination == neighbour and nextHop == neighbour , then cost = THIS_NEW_COST, no need to update nextHop  
			if((routingTable.entries[i].getDestinationIP() == neighborIP) && (routingTable.entries[i].getNextHop() == neighborIP)){
				routingTable.entries[i].setCost(cost);

			}
	//Case 2, destination != neighbour AND nextHop == neighbour
			if((routingTable.entries[i].getDestinationIP() != neighborIP) && (routingTable.entries[i].getNextHop() == neighborIP) ){
				//oldCost != -1 in this case ..
				int routingTableEntryCost = routingTable.entries[i].getCost();
				routingTableEntryCost = routingTableEntryCost - oldCost + cost ;
				routingTable.entries[i].setCost(routingTableEntryCost);
				routingTable.entries[i].setNextHop(neighborIP);
			//update cost and nextHop (going through neighbourIP)

			}
	//Case 3, destination == neighbour(eg.B) and nextHop != neighbour(eg.B) ..
	// Check if A-B yields less cost than the cost that is present in the routing table 
			if((routingTable.entries[i].getDestinationIP() == neighborIP) && (routingTable.entries[i].getNextHop() != neighborIP)){
				if(cost < routingTable.entries[i].getCost()){	//A-B cost is cost
					routingTable.entries[i].setCost(cost);
					routingTable.entries[i].setNextHop(neighborIP);
				}
			}
		}

		cout << "\nCost is updated..\n";
		//showRouter();
	}

	void process_forwardMessage(string frdMsg){
		vector<string> tokens = splitString(frdMsg, 'D');
		string destinationIP = tokens[1];
		vector<string> tokens2 = splitString(tokens[2], '$');
		string len_string = tokens2[0];
		int len_obtained = stringToInteger(len_string);
		string actualMessage = tokens2[1];


		if(destinationIP == thisIpAddress){
			string str = actualMessage + " packet has reached destination (printed by " + thisIpAddress + ")";
			cout << str << endl;
		}
		else{
			string nextHop = routingTable.getNextHopUsingDestination(thisIpAddress, destinationIP);
			if(nextHop != "NULL"){
				string str = actualMessage + " packet forwarded to " + nextHop + " (printed by " + thisIpAddress + ")";
				cout << str << endl ;
				sendMessageToIP(nextHop, frdMsg);
			}
		} 
	}

	void sendMessageToIP(string ip, string toSendMsg){
		struct sockaddr_in router_address;
        char buffer[2048];

		memset(buffer, 0, sizeof(buffer));
		router_address.sin_family = AF_INET;
		router_address.sin_port = htons(4747);
		router_address.sin_addr.s_addr = inet_addr(ip.c_str());

		strcpy(buffer, toSendMsg.c_str());
		sendto(sockid, buffer, 2048, 0, (struct sockaddr*)&router_address, sizeof(sockaddr_in));
		//cout << "Msg : " << toSendMsg << " sent to <" << ip << ">\n";
	}

	string formForwardMessage(string destIP, int len ,string message)
	{
		string msg = "frwd D" + destIP + "D" + integerToString(len) + "$" + message;
		return msg;
	}

	string getMessageForSendToNeighbor(string ip){
        string s = "";
//        s += ("neighbour_Router <src = " + thisIpAddress + "> sending routing table to " + ip + "\n");
        s += ("Neighbour src=S" + thisIpAddress + "S, dest:D" + ip + "D\n");
        s += (routingTable.getRoutingTable());
        s += "\nEnd\n";
        return s;
	}


	void sendToAllNeighbors(int sockfid){
        struct sockaddr_in router_address;
        char buffer[2048];  



        for(int i=0; i<neighbours.size(); i++){

			//if(neighbours[i].isNeighbor == false){
			//	return ; //Not a neighbor so don't send ... SOCKET HANDLE KORE NA ???
			//}


			string neighborIP = neighbours[i].ipAddress;

			memset(buffer, 0, sizeof(buffer));
			router_address.sin_family = AF_INET;
			router_address.sin_port = htons(4747);
			router_address.sin_addr.s_addr = inet_addr(neighborIP.c_str());

			

			string toSendMsg = getMessageForSendToNeighbor(neighborIP);
			strcpy(buffer, toSendMsg.c_str());

			//Send using socket..
			sendto(sockfid, buffer, 2048, 0, (struct sockaddr*)&router_address, sizeof(sockaddr_in));
		}
		sleep(1);	//To smooth things ...
	}

	void upLink(string neighborIP){

		neighborIP = trim(neighborIP);
		
		string strToPrint = "Inside upLink() <" + neighborIP + "> ";
		//cout << strToPrint << endl;
		int idx = -1;
		int linkCostWithNeighbour = INFINITY;
		for(int i=0; i<neighbours.size(); i++){
			if(neighbours[i].getIPAddress() == neighborIP){
				if(neighbours[i].isNeighbor == true){
					strToPrint += " neighbour is already a neighbour so just return ;\n";
					//cout << strToPrint;
					return;
				}
				idx = i;
				neighbours[i].makeNeighbour();
				linkCostWithNeighbour = neighbours[i].getBackupCost();
				break;
			}
		}
		if(idx != -1){
			string str2 = "Link between " + thisIpAddress + " and " + neighborIP + " was up.\n";
			cout << str2;
			//link was actually done 'up'... update routing table accordingly
			for(int i=0; i<routingTable.entries.size(); i++){
				if(routingTable.entries[i].getDestinationIP() == neighborIP){
					if(routingTable.entries[i].getCost() > linkCostWithNeighbour){
						routingTable.entries[i].setCost(linkCostWithNeighbour);
						routingTable.entries[i].setNextHop(neighborIP);
						strToPrint += ("Changing cost of <" + neighborIP + "> with cost = <" + integerToString(linkCostWithNeighbour) + "> and nextHop = <" + neighborIP + "> , ");
						//cout << strToPrint;
					}	
				}
			}
			return ;
		}
	
		strToPrint += (" EKDOM SHESH e for neighborIP = <" + neighborIP + "> returning ...\n");
		//cout << strToPrint;
	}


	void updateRoutingTable(string msg){
        vector<string> tokens = splitString(msg, 'S');  //Source hoche je pathache


        vector<string>tokens2 = splitString(tokens[2], 'D');    //Destination ami nije

        string neighboursIP = tokens[1];
        string routingTab_unsplit = tokens[2];

        vector<string> tokens3 = splitString(routingTab_unsplit, '\n');

        vector<string> receivedFrom;

//Check for uplink begin..
        upLink(neighboursIP);

//Check for uplink done...

//INITIAL UPDATE...
        for(int i=1; i<(tokens3.size()-2); i++){

            vector<string>tokens4 = splitString(tokens3[i], ',');

 			initialUpdateEntry(neighboursIP, tokens4[0], tokens4[1], tokens4[2]);
        }
//INITIAL UPDATE DONE...

        for(int i=1; i<(tokens3.size()-2); i++){

            vector<string>tokens4 = splitString(tokens3[i], ',');
            updateEntry(neighboursIP, tokens4[0], tokens4[1], tokens4[2]);

            receivedFrom.push_back(neighboursIP);
        }


        for(int i=0; i<receivedFrom.size(); i++){
            checkZeroClocksMissedForThisNeighbor(receivedFrom[i]);
        }

        //cout << "\nInside updateRoutingTable() .. after updating ... printing table ...\n";
        //showRouter();

	}
	void initialUpdateEntry(string neighboursIP, string destIP, string nextHop, string cost_string){
		neighboursIP = trim(neighboursIP);
        destIP = trim(destIP);
        nextHop = trim(nextHop);
		string cost_string_temp = "";
	    for(int i=1; i<cost_string.size(); i++){
            cost_string_temp += cost_string[i];
	    }
	    int cost_B_dest = atoi(cost_string_temp.c_str());

	    string nextHopThis = routingTable.getNextHopUsingDestination(thisIpAddress, destIP);
	    if(nextHopThis == neighboursIP){
	    	int idx = routingTable.findIndexOf_NextDestination(destIP);
	    	int cost_A_B = getCostOfNeighbour(neighboursIP);
	    	if(idx != -1){
    			int cost_A_dest = cost_A_B + cost_B_dest;
			 	addRoutingEntry(destIP, cost_A_dest ,nextHopThis);
 				string str = "Inside initialUpdateEntry () ... updating for neighboursIP = <" + neighboursIP + "> ";
		    	str += ("destIP = <" + destIP + "> nextHop = <" + nextHop + "> ... updating to nextHop = <" + nextHopThis + "> and cost = <" + integerToString(cost_A_dest) + ">\n");
				//cout << str ;
	    	}

	    }	   

	}
	int getCostOfNeighbour(string neighboursIP){
		for(int i=0; i<neighbours.size(); i++){
			if(neighbours[i].getIPAddress() == neighboursIP){
				return neighbours[i].getCost();
			}
		}
		return -1 ;
	}
	void updateEntry(string neighboursIP, string destIP, string nextHop, string cost_string){
		neighboursIP = trim(neighboursIP);
        destIP = trim(destIP);
        nextHop = trim(nextHop);

        //cout << "To update entry for neighboursIP : " << neighboursIP << " , destIP = " << destIP << " , nextHop " << nextHop << " , cost = " + cost_string << endl;	
	    string cost_string_temp = "";
	    for(int i=1; i<cost_string.size(); i++){
            cost_string_temp += cost_string[i];
	    }
	    int cost = atoi(cost_string_temp.c_str());

	    //Get cost of neighbour using link's cost ...
	    int costOfNeighbour = -1;
	    for(int i=0; i<neighbours.size(); i++){
	    	if(neighbours[i].getIPAddress() == neighboursIP){
	    		costOfNeighbour = neighbours[i].getLastCost();
	    	}
	    }
	    //cout << " Cost of neighbor " << neighboursIP << " (LINK) = " << costOfNeighbour << endl;

	    if((costOfNeighbour != -1) && (nextHop != thisIpAddress) ){
    		int costToDest = routingTable.getCostOfRouter(destIP);
			int tempCost = costOfNeighbour + cost;
			//cout << "CostToDest : " << costToDest << " , tempCost = " << tempCost << endl ;
			if(tempCost < costToDest){
				//cout << "Updating routing entry ..." << endl; 
				//Update cost...NEED TO GO THROUGH Neighbour as next hop			
				string nextHopToAdd = neighboursIP;
				addRoutingEntry(destIP, tempCost, nextHopToAdd);
			}
	    }

	}
	void addRoutingEntry(string destinationIP, int cost, string nextHop){
		RoutingEntry entry(destinationIP, cost, nextHop);
		int idx = routingTable.findIndexOf_NextDestination(destinationIP);

		if(idx == -1){  //Does not exist ....
			routingTable.push_back(entry);
		}else{
			routingTable.replace(idx, entry);
		}
		//routingTable.push_back(entry);
	}
    void checkZeroClocksMissedForThisNeighbor(string ipAdd)
    {
        string ip = trim(ipAdd);
        for(int i=0; i<neighbours.size(); i++){
            if(neighbours[i].ipAddress == ip){
                neighbours[i].messageReceived();	//numClocksMissed = 0;
            }
        }
    }
};
