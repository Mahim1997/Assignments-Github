#include "types.h" 
#include "defs.h"
#include "param.h"
#include "spinlock.h"
#include "memlayout.h"
#include "mmu.h"
#include "x86.h"  
#include "sock.h"
#include "proc.h"


// A structure to maintain a list of sockets
// Should it have locking? [Yes?? To access while ** connect, listen, recv, send **  are being called ... ]

struct {
    struct spinlock lock;
    struct socket sock[NSOCK];
    int local_ports_used[NPORT]; //0->available, 1->used ... no need for double loop
    //print the whole socket table ...
} stable;


// ----------------------------------------------------- Util functions begin --------------------------------------------------
// Util functions to return socket

static struct socket*
getSocket_LocalPort(int lport) {
    struct socket *s;
    for (s = stable.sock; s < &stable.sock[NSOCK]; s++) {
        if (s->localPort == lport) {
            return s;
        }
    }
    return 0;
}

static struct socket*
getSocket_RemotePort(int port) {
    struct socket *s;
    for (s = stable.sock; s < &stable.sock[NSOCK]; s++) {
        if (s->remotePort == port) {
            return s;
        }
    }
    return 0;
}
// Using functions to return socket

char*
mystrcopy(char *s, const char *t, int n) { //taken from safestrcpy from string.c
    char *os;

    os = s;
    if (n <= 0)
        return os;
    while (--n > 0 && (*s++ = *t++) != 0)
        ;
    *s = 0;
    return os;
}

void printSocket(struct socket* s) {
    cprintf("Socket's localPort = %d, remotePort = %d, state = %d, owner = %d, has_data = %d\n",
            s->localPort, s->remotePort, s->socket_state, s->owner_process_ID, s->has_data);
}

// ----------------------------------------------------- Util functions end --------------------------------------------------

//Initialisation

void
sinit(void) {


#if DEBUG_SOCKET == 1
    cprintf("\n\n\n--->> sinit() function is called .... initlock(sTable) will be called ... \n");
    cprintf("Printing initial stable ... \n");
#endif

    initlock(&stable.lock, "stable"); //Initialise lock 
    struct socket *sIter;

    //Acquire Lock
    acquire(&stable.lock);
    for (sIter = stable.sock; sIter < &stable.sock[NSOCK]; sIter++) {
        sIter->socket_state = CLOSED;
    }
    int x;
    for (x = 0; x < NPORT; x++) {
        stable.local_ports_used[x] = 0; //0 -> available , 1 -> used 
    }
    release(&stable.lock);
    //Release Lock

    cprintf("\n\n");
}

/*
    struct socket *sIter;
    acquire(&stable.lock);
    for (sIter = stable.sock; sIter < &stable.sock[NSOCK]; sIter++)
    {
        
    }
    release(&stable.lock);
 */

//Listen function ... 

int
listen(int lPort) {

    int status = 0;

#if DEBUG_SOCKET == 1
    cprintf("Inside listen system call on lport = %d\n", lPort);
#endif

    //Error Checking ... Check if lPort already exists in stable or not ... 
    struct socket *sIter;
    acquire(&stable.lock);


    for (sIter = stable.sock; sIter < &stable.sock[NSOCK]; sIter++) {
        //If table has an entry with localPort == lPort , then report an error [ Sir er kon error ?? ] 
        if (sIter->localPort == lPort) {
            status = E_ACCESS_DENIED; //Since some other process had opened as local port so THIS process is not the owner
        }
    }
    struct socket *unusedSocket = 0;
    if (status == 0) {
        //Socket table does NOT have this lPort as local port so ... we assign any unused/CLOSED socket with this config ...
        for (unusedSocket = stable.sock; unusedSocket < &stable.sock[NSOCK]; unusedSocket++) {
            if (unusedSocket->socket_state == CLOSED) {
                unusedSocket->localPort = lPort; //Make localPort = parameter's lPort
                unusedSocket->remotePort = lPort; //For simplicity, make remote port also same as localPort
                unusedSocket->socket_state = LISTENING; //Make state as listening.
                unusedSocket->owner_process_ID = myproc()->pid; //sys_getpid() ?? Access paay na //Assign as the calling process's pid ??
                unusedSocket->buffer[0] = '\0';
                stable.local_ports_used[lPort] = 1; //Make that local port as 1 ...
#if DEBUG_SOCKET == 1
                cprintf("UNUSED SOCKET FOUND , Printing that socket ... !!!\n");
                printSocket(unusedSocket);
#endif
                break;
            }
        }
    }
    release(&stable.lock);


#if DEBUG_SOCKET == 1
    cprintf("Returning the status = %d \n\n\n", status);
#endif

    return status;
}

int
connect(int rport, const char* host) {
    //host check was already resolved in sys_connect 

    //Search the stable for local port available and then connect as remote port <-> rport and return that local port ??

    int iter = 0, lPortFound = -1;
    struct socket *sIter;
    acquire(&stable.lock);
    //Initial checking


    int willUse = 0;
    for (sIter = stable.sock; sIter<&stable.sock[NSOCK]; sIter++) {
        if (sIter->localPort == rport) {
            if ((sIter->remotePort == rport) && (sIter->socket_state == LISTENING)) {
                willUse = 1;
                break;
            } else {
                //Already before a connection was established so ... this should be a access_denied error
#if DEBUG_SOCKET == 1
                cprintf("The remote socket with rport = %d was not in listening state, release and return E_ACCESS_DENIED\n\n", rport);
#endif
                release(&stable.lock);
                return E_ACCESS_DENIED;

            }
        }

    }
    if (willUse == 0) {
#if DEBUG_SOCKET == 1
        cprintf("Socket with rport = %d does not exist in sTable\n\n", rport);
#endif  
        release(&stable.lock);
        return E_NOTFOUND;
    }
    for (iter = NPORT - 1; iter >= 0; iter--) {
        if (stable.local_ports_used[iter] == 0) {
            lPortFound = iter; //Local port unused is found 
            break;
        }
    }

    int newSocketFound = -1;
#if DEBUG_SOCKET == 1
    cprintf("In connect(serverPort = %d), localPort found = %d\n", rport, lPortFound);
#endif
    for (sIter = stable.sock; sIter < &stable.sock[NSOCK]; sIter++) {
        //Now lets try to find an UNUSED Socket ... otherwise return E_FAIL
        if (sIter->socket_state == CLOSED && lPortFound != -1) {
            //Found a CLOSED Socket , assign local port and remote port accordingly .. 

            stable.local_ports_used[lPortFound] = 1; //Make local port array of that idx as 1 .. since it will be in use 
            sIter->socket_state = CONNECTED;
            sIter->localPort = lPortFound;
            sIter->remotePort = rport;
            sIter->owner_process_ID = myproc()->pid;
#if DEBUG_SOCKET == 1
            cprintf("In connect(serverPort = %d), CLOSED socket found, printing new assigned socket...\n", rport);
            printSocket(sIter);
#endif            
            newSocketFound = 1;
            break;
        }
    }
    if (newSocketFound == 1) {
        //Fix the local port of the server side
        for (sIter = stable.sock; sIter < &stable.sock[NSOCK]; sIter++) {
            if (sIter->localPort == rport) {
                sIter->remotePort = lPortFound;
                sIter->socket_state = CONNECTED;
#if DEBUG_SOCKET == 1
                cprintf("Inside connect(serverPort = %d), After changing server socket, printing it\n", rport);
                printSocket(sIter);
#endif                
                release(&stable.lock);
                cprintf("\n\n");
                return lPortFound;
            }
        }
    }
    cprintf("\n\n");
    release(&stable.lock);
    return E_FAIL; //Cannot find a CLOSED socket since all are booked !!
}

int
send(int lport, const char* data, int n) {
    //Find the corresponding remote port of localport = lport 
    //Call sleep(localSocket) for itself, and wakeUp(remoteSocket) for the counterpart recv() 
    //1. Find the socket -> localPort == lport
    struct socket* localSocket = 0;
    struct socket* remoteSocket = 0;

    acquire(&stable.lock);
    localSocket = getSocket_LocalPort(lport);
    remoteSocket = getSocket_RemotePort(lport);

#if DEBUG_SOCKET == 1
    //    cprintf("Inside send() function  of lport = %d, pid = %d\n", lport, myproc()->pid);
#endif
    //Either Sockets do not exist, return E_NOTFOUND error
    if (!localSocket) {
#if DEBUG_SOCKET == 1
        cprintf("Local Socket does not exist in the stable, return E_NOTFOUND error\n\n");
#endif
        release(&stable.lock);
        return E_NOTFOUND;
    }
    if (!remoteSocket) {
#if DEBUG_SOCKET == 1
        cprintf("Remote Socket does not exist in the stable, return E_NOTFOUND error\n\n");
#endif
        release(&stable.lock);
        return E_NOTFOUND;
    }

    //Checking owner process ID MISMATCH occurs ...
    if (localSocket->owner_process_ID != myproc()->pid) {
#if DEBUG_SOCKET == 1
        cprintf("Process ID Mismatch occures, owner id of local sock = %d, and myPID = %d, release and return E_ACCESS_DENIED error.\n\n",
                localSocket->owner_process_ID, myproc()->pid);
#endif
        release(&stable.lock);
        return E_ACCESS_DENIED;
    }
#if DEBUG_SOCKET == 1
    cprintf("In send() , Both sockets found ... printing local and then remote socket\n");
    printSocket(localSocket);
    printSocket(remoteSocket);
#endif

    //Checking if any of the sockets is not in connected state
    if (localSocket->socket_state != CONNECTED) {
#if DEBUG_SOCKET == 1
        cprintf("Local Socket is not in CONNECTED State, return E_WRONG_STATE error\n\n");
#endif
        release(&stable.lock);
        return E_WRONG_STATE;
    }
    if (remoteSocket->socket_state != CONNECTED) {
#if DEBUG_SOCKET == 1
        cprintf("Remote Socket is not in CONNECTED State, return E_WRONG_STATE error\n\n");
#endif
        release(&stable.lock);
        return E_WRONG_STATE;
    }
    int flag = 0;
    //Everything looks OK up until now , check for whether the remote socket has data or not
    while (remoteSocket->has_data == 1) {
        if (flag == 0) {
#if DEBUG_SOCKET == 1
            cprintf("Remote Socket has data == 1, so sleep ... on the channel localSocket\n");
#endif
            flag = 1;
        }
        //remote socket has data, so should sleep until that data is CLEARED 
        sleep(localSocket, &stable.lock); //Channel1 is the local socket for send
    }
    //Sleeping/Polling is over ...
    mystrcopy(remoteSocket->buffer, data, n); //Copy the data , memcpy teh header include e problem ase
    remoteSocket->has_data = 1; //Set has_data

#if DEBUG_SOCKET == 1
    cprintf("[Polling over] Data is written to remoteSocket [local = %d, remote = %d, data = %s] and calling wakeup\n\n",
            remoteSocket->localPort, remoteSocket->remotePort, remoteSocket->buffer);
#endif
    //Wake up the remoteSocket [Channel2 for recv]
    wakeup(remoteSocket);
    release(&stable.lock);

    return 0;
}

int
recv(int lport, char* data, int n) {
    //Counterpart send -> sleep(localSocket) so in here should wakeUp(remoteSocket)
    //Counterpart send -> wakeup(remoteSocket) so in here should sleep(localSocket)
    struct socket* localSocket = 0;
    struct socket* remoteSocket = 0;
    acquire(&stable.lock);
    localSocket = getSocket_LocalPort(lport);
    remoteSocket = getSocket_RemotePort(lport); //Check using if remote port of [server]Socket == [this/local] port

#if DEBUG_SOCKET == 1
    //    cprintf("Inside recv() of lport = %d, pid = %d\n", lport, myproc()->pid);
#endif

    //Check 1. If either sockets do not exist
    if ((!localSocket) || (!remoteSocket)) {
#if DEBUG_SOCKET == 1
        cprintf("Either of the Local or Remote Socket does not exist in the sTable, release and return E_NOTFOUND error\n\n");
#endif
        release(&stable.lock);
        return E_NOTFOUND;
    }

    //Check 2. If owner of LOCAL SOCKET mismatch
    if (localSocket->owner_process_ID != myproc()->pid) {
#if DEBUG_SOCKET == 1
        cprintf("Local socket owner id = %d and myproc->id = %d, MISMATCH occurs, so release and return E_ACCESS_DENIED error\n\n",
                localSocket->owner_process_ID, myproc()->pid);
#endif
        release(&stable.lock);
        return E_ACCESS_DENIED;
    }

#if DEBUG_SOCKET == 1
    cprintf("Both sockets found ... further checking needed, printing local and remote\n");
    printSocket(localSocket);
    printSocket(remoteSocket);
#endif

    //Check 3. Not in CONNECTED state either of them ... return E_WRONG_STATE error
    if ((localSocket->socket_state != CONNECTED) || (remoteSocket->socket_state != CONNECTED)) {
#if DEBUG_SOCKET == 1
        cprintf("Either of local or remote socket is not in the connected state so release and return E_WRONG_STATE error\n\n");
#endif
        release(&stable.lock);
        return E_WRONG_STATE;
    }

    //So far everything OK, now check if there is NO data present in LOCAL_SOCKET , if no data , go to sleep [Server Side]
    int flag = 0;
    while (localSocket->has_data == 0) {
        //No data in local socket, go to sleep
        if (flag == 0) {
#if DEBUG_SOCKET == 1
            cprintf("Local Socket has_data == 0 so sleeping on the channel localSocket [Server Side] \n\n");
            flag = 1;
#endif
        }
        sleep(localSocket, &stable.lock);
    }


    //Now has_data = 1 
    //This is where we have data [in server side] , retrieve the data to this char * in the parameter and make has_data = 0.
#if DEBUG_SOCKET == 1
    cprintf("Copying data to buf from localSocket->buffer, making local->hasData = 0 ,and calling wakeUp(remoteSocket) in recv \n\n");
#endif 

    mystrcopy(data, localSocket->buffer, n);
    localSocket->has_data = 0;
    wakeup(remoteSocket); //Counter part remote socket i.e. client's local socket is being woken up since has_data (local side) = 0

    release(&stable.lock);

    return 0;
}

int
disconnect(int lport) {
    //Check 1. if socket with this Local Port does exist or not
    struct socket *localSocket;

    acquire(&stable.lock);

    localSocket = getSocket_LocalPort(lport);
    if (!localSocket) {
#if DEBUG_SOCKET == 1
        cprintf("Local Socket with lport %d does not exist in table so release table and return E_NOTFOUND\n", lport);
#endif
        release(&stable.lock);
        return E_NOTFOUND;
    }
    if (localSocket->owner_process_ID != myproc()->pid) {
#if DEBUG_SOCKET == 1
        cprintf("PROCESS MISMATCH in disconnect () , Local Socket process id = %d, and myprocess pid = %d, mismatch, release and return E_ACCESS_DENIED\n",
                localSocket->owner_process_ID, myproc()->pid);
#endif
        release(&stable.lock);
        return E_ACCESS_DENIED;
    }
    struct socket* remoteSocket = getSocket_RemotePort(localSocket->localPort);

    if (remoteSocket) {
        //This local socket is connected to a remote socket  
#if DEBUG_SOCKET == 1
        cprintf("In disconnect() , Local Socket IS connected to a remote socket, printing them (l , r) and changing to CLOSED in local\n");
        printSocket(localSocket);
        printSocket(remoteSocket);
#endif
        localSocket->socket_state = CLOSED;
        localSocket->remotePort = 0;
        localSocket->localPort = 0;

        localSocket->has_data = 0; // ?? Closed means that data is now useless so should be has_data = 0
        localSocket->owner_process_ID = 0; ///Owner process also doesn't matter now
        stable.local_ports_used[lport] = 0; //Make it available now ...
        remoteSocket->remotePort = remoteSocket->localPort; //Make remote socket as server ... i.e. make remote socket's local port the same as remote port
        wakeup(remoteSocket); //To make sure if other side's socket which went into sleep eg. due to recv() then it should wake up


    } else {
        //Local Socket is NOT connected to a remote socket
#if DEBUG_SOCKET == 1
        cprintf("Local Socket is NOT connected to a remote socket ... just change its state to CLOSED and make port available\n");
#endif
        localSocket->socket_state = CLOSED;
        localSocket->localPort = 0;
        localSocket->remotePort = 0;
        localSocket->has_data = 0;

        stable.local_ports_used[lport] = 0;
        localSocket->owner_process_ID = 0;
    }


    release(&stable.lock);

    return 0;
}

//Additional function ... closeSocket for process abrupt crashing

void closeSockets(int processID) {
    //TODO
#if DEBUG_SOCKET == 1
    cprintf("==>>>Calling closeSockets ... for processID = %d\n", processID);
#endif
    //Check all sockets in stable with owner = process ID and make them CLOSED ...
    struct socket *s;
    acquire(&stable.lock);
    for (s = stable.sock; s < &stable.sock[NSOCK]; s++) {
        if (s->owner_process_ID == processID && s->socket_state != CLOSED) {
#if DEBUG_SOCKET == 1
            cprintf("Printing socket to close ... \n");
            printSocket(s);
#endif
            stable.local_ports_used[s->localPort] = 0;
            struct socket *remote = getSocket_RemotePort(s->localPort);
            s->has_data = 0;
            s->localPort = 0;
            s->remotePort = 0;
            s->owner_process_ID = 0;
            s->socket_state = CLOSED;
            if (remote) {
                remote->remotePort = remote->localPort;
                //                wakeup(s);   //wake up dile trap khay for some reason ..
                //        remote->socket_state = CLOSED ; // ???
            }
        }
    }
#if PRINT_FINALLY_ALL_SOCKET == 1
    cprintf("-->>In closeSockets() .. Printing all sockets ... \n");
    for (s = stable.sock; s < &stable.sock[NSOCK]; s++) {
        printSocket(s);
    }
    cprintf("\n\n");
#endif
    release(&stable.lock);
}





