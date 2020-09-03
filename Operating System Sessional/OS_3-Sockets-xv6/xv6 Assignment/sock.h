#include "param.h"  //For MAX_BUFFER_SIZE_SOCKET
//
// DONE: Define an enumeration to represent socket state.
//
enum sockstate {CLOSED, LISTENING, CONNECTED}; 

//
// DONE: Define a structure to represent a socket.
//
struct socket{
    int localPort;  //Local port
    int remotePort; // Remote Port 
    char buffer[MAX_BUFFER_SIZE_SOCKET];   //This is in param.h
    int owner_process_ID ; // the process's pid 
    enum sockstate socket_state;   //The state of the socket ... 
    int has_data ;  // 1 -> means it has data ... , 0 -> means it does not have data 
};