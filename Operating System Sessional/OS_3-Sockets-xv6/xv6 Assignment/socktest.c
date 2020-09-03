#include "types.h"
#include "user.h"

//int serverPort = 10;

int checkError(int givenInteger) {
    if (givenInteger == E_NOTFOUND) {
        printf(1, "--->> Error in Accessing a socket that is not in the stable\n");
    } else if (givenInteger == E_ACCESS_DENIED) {
        printf(1, "--->> Error in Accessing a socket from wrong process\n");
    } else if (givenInteger == E_WRONG_STATE) {
        printf(1, "--->> Error in Attempting to send or receive, when the socket is not connected\n");
    } else if (givenInteger == E_FAIL) {
        printf(1, "--->> Error in no more socket opening (limit exceeded)\n");
    } else if (givenInteger == E_INVALID_ARG) {
        printf(1, "--->> Error in Parameter issues\n");
    }
    return givenInteger;
}

void clientProc(int serverPort) {
    int clientPort;
    char buf[128];
    char host[16] = "localhost";

    // sleep for 100 clock ticks to ensure that the server process starts first.
    sleep(100);

    printf(1, "Client>> pid = %d, Attempting to connect to port %d, host %s ...\n", getpid(), serverPort, host);
    clientPort = connect(serverPort, host); // [ Here the socket of server oitao connected, client socket also connected ]

    sleep(20);
    printf(1, "Client>> pid = %d, connect() returned %d\n", getpid(), clientPort);
    clientPort = checkError(clientPort);

    while (1) {

#if ERROR_HANDLING == 1
        if (clientPort < 0) {
            break;
        }
#endif
        printf(1, "Client pid = %d>>  Enter text to send to server: ", getpid());
        gets(buf, sizeof (buf));
        buf[strlen(buf) - 1] = '\0'; // Eliminating the '\n'
        printf(1, "Client pid = %d, Message to send : %s\n", getpid(), buf);
        int x = send(clientPort, buf, strlen(buf) + 1);
        printf(1, "Client pid = %d, send() returned %d\n", getpid(), x);
        if (0 == strcmp(buf, "exit")) {
            printf(1, "Client pid = %d, exiting...\n\n", getpid());
#if ABRUPT_ENDING == 1
            break; //break without calling disconnect ... 
#endif
            disconnect(clientPort);
            break;
        }

        sleep(100 + uptime() % 100);

        recv(clientPort, buf, sizeof (buf));
        printf(1, "Client pid = %d>> Received: \"%s\"\n\n", getpid(), buf);
    }
}

void serverProc(int serverPort) {
    int status;
    char buf[128];

    printf(1, "Server pid = %d >> Starting to listen at port %d ...\n", getpid(), serverPort);
    status = listen(serverPort); // ke message ta pathaitese ami actually jantesi na .. [EIKHANE LISTENING]
    printf(1, "Server pid = %d >> listen() returned %d\n", getpid(), status);
    status = checkError(status);

    while (1) {
        sleep(100 + uptime() % 100);

        recv(serverPort, buf, sizeof (buf));
        printf(1, "Server pid = %d >> Received: \"%s\"\n\n", getpid(), buf);

        if (0 == strcmp(buf, "exit")) {
            printf(1, "Server pid = %d, exiting...\n", getpid());
#if ABRUPT_ENDING == 1
            break;
#endif
            disconnect(serverPort);
            break;
        }

        sleep(100 + uptime() % 100);

        strcpy(buf + strlen(buf), " <OK>");
        send(serverPort, buf, strlen(buf) + 1); // ??? should send to clientPort (by using stable) 
    }
}

void integer_to_string(char * string, int number) {

    if (number == 0) {
        string[0] = '0';
        return;
    };
    int divide = 0;
    int modResult;
    int length = 0;
    int isNegative = 0;
    int copyOfNumber;
    //    int offset = 0;
    copyOfNumber = number;
    if (number < 0) {
        isNegative = 1;
        number = 0 - number;
        length++;
    }
    while (copyOfNumber != 0) {
        length++;
        copyOfNumber /= 10;
    }

    for (divide = 0; divide < length; divide++) {
        modResult = number % 10;
        number = number / 10;
        string[length - (divide + 1)] = modResult + '0';
    }
    if (isNegative) {
        string[0] = '-';
    }
    string[length] = '\0';
    //    return string;
}

void clientProcTest(int serverPort) {
    int clientPort;
    char buf[128];
    char host[16] = "localhost";

    // sleep for 100 clock ticks to ensure that the server process starts first.
    sleep(100);

    printf(1, "Client>> pid = %d, Attempting to connect to port %d, host %s ...\n", getpid(), serverPort, host);
    clientPort = connect(serverPort, host); // [ Here the socket of server oitao connected, client socket also connected ]
    if (clientPort == 127) {
        sleep(10000); //Just for testing
    }
    sleep(20);
    printf(1, "Client>> pid = %d, connect() returned %d\n", getpid(), clientPort);
    int count = 0;
    while (1) {
        if (clientPort < 0) {
            //            break;
        }
        count++;
        //        sprintf(str_count, " %d", count);
        char str_count[10];
        integer_to_string(str_count, count);
        strcpy(buf + 0, "MessageSending_");
        strcpy(buf + strlen(buf), str_count);
        printf(1, "Client pid = %d >>, message to send = %s\n", getpid(), buf);
        //        gets(buf, sizeof (buf));
        buf[strlen(buf) - 1] = '\0'; // Eliminating the '\n'

        send(clientPort, buf, strlen(buf) + 1);

        if (0 == strcmp(buf, "exit")) {
            printf(1, "Client pid = %d, exiting...\n\n", getpid());
            disconnect(clientPort);
            break;
        }

        sleep(100 + uptime() % 100);
        char recvBuf[128];
        recv(clientPort, recvBuf, sizeof (recvBuf));
        printf(1, "Client pid = %d>> Received: \"%s\"\n\n", getpid(), recvBuf);

    }
}

void test1() {
    int p1 = 10;
    int x; // y; // z;
    x = fork();
    //    y = fork();
    //    z = fork();
    if (x == 0) {

        clientProc(p1);
        exit();
    } else {
        int f = send(p1, "Hi", 2);
        f = checkError(f);
        serverProc(p1);
        wait();
        exit();
    }
}

void test2() {
    int x, y, z;
    x = fork();
    y = fork();
    z = fork();
    int p1 = 2;
    int p2 = 10;
    int p3 = 15;
    if (z == 0 && x == 0 && y == 0) {
        printf(1, "Inside Cond 1st, pid = %d\n", getpid());
        clientProc(p1);
        exit();
    } else if (z == 0 && x == 0 && y != 0) {
        printf(1, "Inside Cond 2nd, pid = %d\n", getpid());
        clientProc(p1);
        exit();
    } else if (z == 0 && x != 0 && y == 0) {
        printf(1, "Inside Cond 3rd, pid = %d\n", getpid());
        clientProc(p2);
        exit();
    } else if (z == 0 && x != 0 && y != 0) {
        printf(1, "Inside Cond 4th, pid = %d\n", getpid());
        serverProc(p2);
        wait(); // ??
        exit();
    } else if (z != 0 && x == 0 && y == 0) {
        printf(1, "Inside Cond 5th, pid = %d\n", getpid());
        clientProc(p3);
        exit();
    } else if (z != 0 && x == 0 && y != 0) {
        printf(1, "Inside Cond 6th, pid = %d\n", getpid());
        serverProc(p3);
        wait();
        exit();
    } else if (z != 0 && x != 0 && y == 0) {
        printf(1, "Inside Cond 7th, pid = %d\n", getpid());
        exit();
    } else if (z != 0 && x != 0 && y != 0) {
        printf(1, "Inside Cond 8th, pid = %d\n", getpid());
        serverProc(p1);
        wait();
        exit();
    }
}

int main(int argc, char *argv[]) {
    printf(1, "In Main function ... \n");
    //    test2();
    test1();

}
/*
     Send test 1
      int main(int argc, char *argv[]) {
        int x, y, z;
        z = fork();
        x = fork();
        y = fork();

        if (z == 0 && x == 0 && y == 0) {
            //        printf(1, "\n\n");
            listen_test(22);
            while (1);
        } else if (z == 0 && x == 0 && y != 0) {
            while (1);
        } else if (z == 0 && x != 0 && y == 0) {
            sleep(150);
            int clientPort = connect_test(22);
            char buf[128] = "Hello first try\0";
            printf(1, "\nSend test first one...\n\n");
            send(clientPort, buf, strlen(buf) + 1);
            //Trying to send again ...
            sleep(400);
        
            while (1);
        } else if (z == 0 && x != 0 && y != 0) {
            sleep(300);
            connect_test(22);
            while (1);
        } else if (z != 0 && x == 0 && y == 0) {
            sleep(450);
            char buf[128] = "Hello SECOND try\0";
            printf(1, "\nSend test second one...\n\n");
            int clientPort = 150;
            int x = send(clientPort, buf, strlen(buf) + 1);
            printf(1, "User prog, returned x from send ( ) is %d\n", x);
            while (1);
        } else if (z != 0 && x == 0 && y != 0) {
            sleep(550);
            char buf[128] = "Hello THURD try\0";
            printf(1, "\nSend test third one with port = 127 [Should give ACCESS_DENIED]...\n\n");
            int clientPort = 127;
            int x = send(clientPort, buf, strlen(buf) + 1);
            printf(1, "User prog, returned x from send ( ) is %d\n", x);
            while (1);
        }        //    else if (z != 0 && x != 0 && y == 0) {

            //    }
        else {
            while (1);
        }

    }
 
 */
/*
 //Connect_test
 * 
     int x, y, z;
    z = fork();
    x = fork();
    y = fork();

    if (z == 0 && x == 0 && y == 0) {
        printf(1, "\n\n");
        listen_test(22);
        while (1);
    } else if (z == 0 && x == 0 && y != 0) {
        sleep(100);
        listen_test(21);
        while (1);
    } else if (z == 0 && x != 0 && y == 0) {
        sleep(150);
        connect_test(22);
        while (1);
    } else if (z == 0 && x != 0 && y != 0) {
        sleep(250);
        connect_test(21);
        while (1);
    } else if (z != 0 && x == 0 && y == 0) {
        sleep(300);
        connect_test(21);
    } else if(z != 0 && x == 0 && y != 0){
        sleep(350);
        connect_test(50);
    }
    else {
        while (1);
    }

 */

/*
 * LISTEN Test
 * int main(int argc, char *argv[]) {
    int x, y;
    x = fork();
    y = fork();
    if (x == 0 && y == 0) {
        printf(1, "\n\n");
        listen_test(22);
        while (1);

    } else if (x == 0 && y != 0) {
        sleep(500);
        listen_test(21);
        while (1);
    } else if (x != 0 && y == 0) {
        sleep(900);
        listen_test(22);
        while (1);
    } else {
        while (1);
    }

} 
 */
