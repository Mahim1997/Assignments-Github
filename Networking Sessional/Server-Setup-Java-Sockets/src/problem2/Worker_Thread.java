package problem2;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.OutputStream;
import java.io.PrintWriter;
import java.net.Socket;
import java.util.logging.Level;
import java.util.logging.Logger;

public class Worker_Thread implements Runnable {

    private String name;

    private BufferedReader buffer_input;
    private PrintWriter printWriter;
    private OutputStream outputStream;
    Socket socket;

    StringProcessor processor;

    Worker_Thread(Socket socket, String name) {
        this.name = name;
        this.socket = socket;
        this.processor = new StringProcessor();
        try {
            buffer_input = new BufferedReader(new InputStreamReader(socket.getInputStream()));
            printWriter = new PrintWriter(socket.getOutputStream());
            outputStream = socket.getOutputStream();
//            System.out.println("=>Worker_Thread Constructor: New thread of name " + this.name + " is formed.");
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    @Override
    public void run() {
        //Process for each request and then thread will die..
        if (true) {
            try {
                //Everything inside this try-catch block .. '\u0000'
                char[] charArray = new char[HTTP_Server.maxLimit];   //5000bytes

                int numBytes = buffer_input.read(charArray);

                String input = new String(charArray);

                if (input.trim().length() == 0) {
                    return;
                }
//                System.out.println("=-=>Inside run(). Num Bytes : " + numBytes + ",  input is \n--------*****-------\n" + input + "\n--------*****-------\n");
                numBytes++;
                char[] arr = new char[numBytes];
                input.getChars(0, numBytes - 1, arr, 0);
                String actualInput = new String(arr);

//                System.out.println("=-=>Inside run(). Num Bytes : " + numBytes + ",  actualInput is \n--------*****-------\n" + actualInput + "\n--------*****-------\n");
                this.processor.setInput(actualInput);
                this.processor.setIpAddress(this.socket.getInetAddress().getHostAddress());
                Info info = this.processor.getResponse();

//                test(info, actualInput);
                //Write actually now...
                printHeaders(info);
                for (String s : info.headers) {
//                    System.out.println("Wrinting <" + s + ">");
                    printWriter.println(s);     //Send headers as output 
                }
                printWriter.println();  //A new line then send data
                printWriter.flush();    //Flush
                //Now write the data
                System.out.println("Writing data now. ..");
                if (info.dataInBytes != null) {
                    outputStream.write(info.dataInBytes);
                    outputStream.flush();
                }
//                System.out.println("Writing data is DONE. ..");
                //Writing is done..
                
            } catch (Exception e) {
                e.printStackTrace();
            }
        }
        System.out.println("\n===>>>>Inside run() .. The thread <"+this.name+"> dies now . CLOSING Socket connection ..");
        try {
            this.socket.close();
            System.out.println("===>>>>>Socket connection for thread <"+this.name+"> is closed.\n");
        } catch (IOException ex) {
            ex.printStackTrace();
        }
    }

    private void test(Info info, String actualInput) {
        System.out.println("----*****-----\n=--->>Inside Worker_Thread. , actualInput is :\n" + actualInput + "\n------*******------------");

        if (info == null) {
            return;
        }
        System.out.println("=========**Printing info now.****========\n" + info.toString() + "\n----------------\n" + info.getData()
                + "\n------------\n"
                + info.getHeaders() + "\n=======***Printing the whole info is over*****=======\n");

    }

    private void printHeaders(Info info) {
        System.out.println("=========================<<<<<Printing headers begin for Thread : " + this.name + ">>>>>>>>=========================");

        for(String s:info.headers){
            System.out.println(s);
        }
        
        System.out.println("=========================<<<<<Printing headers END>>>>>>>>=========================");
    }

}
