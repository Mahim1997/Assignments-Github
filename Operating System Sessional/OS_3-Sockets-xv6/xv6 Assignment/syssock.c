#include "types.h"
#include "defs.h"
#include "param.h"
#include "memlayout.h"
#include "mmu.h"
#include "x86.h"
#include "proc.h"
//#include "user.h"   //To include the error codes ... ?? for now they in param.h

int
sys_listen(void) {
    int port = 0;
    argint(0, &port); //First argument is the port number

    if (port < 0 || port >= NPORT) {
        return E_INVALID_ARG; //Invalid arguments
    }

    return listen(port);
}

int
sys_connect(void) {
    int port = 0;
    char *host = 0;

    // Allow connection to "localhost" or "127.0.0.1" host only


    argint(0, &port); //First argument is the port number
    argstr(1, &host); //Second arg is the host

    if (!((strncmp(host, "localhost", 9) == 0) || (strncmp(host, "127.0.0.1", 9) == 0))) {
        //Invalid Argument of host
        return E_INVALID_ARG;
    }
    if (port < 0 || port >= NPORT) {
        //Invalid argument of port number
        return E_INVALID_ARG;
    }

    return connect(port, host);
}

int
sys_send(void) {
    int port = 0;
    char* buf = 0;
    int n = 0;


    // Code to get and validate port no., buffer and buffer size

    argint(0, &port); //First argument is the port number
    int ret = argstr(1, &buf); //Second arg is the buffer
    argint(2, &n); //Third arg is the buffer size

    if(ret < 0){    //
        return E_INVALID_ARG ;
    }
    if (n <= 0 || n > MAX_BUFFER_SIZE_SOCKET) {
        return E_INVALID_ARG;
    }
    if (port < 0 || port >= NPORT) {
        return E_INVALID_ARG;
    }
    //Buffer validation ?? 

    return send(port, buf, n);
}

int
sys_recv(void) {
    int port = 0;
    char* buf = 0;
    int n = 0;

    // Code to get and validate port no., buffer and buffer size

    argint(0, &port); //First argument is the port number
    int returnedVal = argstr(1, &buf); //Second arg is the buffer
    argint(2, &n); //Third arg is the buffer size

    if(returnedVal < 0){
        return E_INVALID_ARG ;  //Problem has occured with argstr .... so error
    }
    if (n <= 0 || n > MAX_BUFFER_SIZE_SOCKET) {
        return E_INVALID_ARG;
    }
    if (port < 0 || port >= NPORT) {
        return E_INVALID_ARG;
    }

    return recv(port, buf, n);
}

int
sys_disconnect(void) {
    int port = 0;

    argint(0, &port); //First argument is the port number
    // Code to get and validate port no.
    if (port < 0 || port >= NPORT) {
        return E_INVALID_ARG;
    }


    return disconnect(port);
}
