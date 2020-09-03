package assignment1_networking;

import java.io.*;
import java.net.*;
import java.nio.charset.*;
import java.nio.file.*;
import java.util.ArrayList;
import java.util.List;
import java.util.StringTokenizer;
import java.util.logging.*;
import java.util.concurrent.*;

public class HTTP_Server {

    static final int PORT = 6789;

    public static void main(String[] args) throws IOException {

        String input = "GET /idx.html HTTP/1.1";
        input = "GET /folder1/idx.html HTTP/1.1";
        input = "GET / HTTP/1.1";
        StringParser parser = new StringParser(input);
        parser.getInfoOfRequest();
        System.out.println("In main ... " + parser.info);
//        parser.formHeaders();
        Info.printArray(parser.headers);

        /*
        System.out.println("======================*****===============");
        ServerSocket serverSocket = new ServerSocket(PORT);
        System.out.println("Server started.\nListening for connections on port : " + PORT + " ...\n");
        //System.out.println(str);

        while (true) {
            Socket s = serverSocket.accept();
            BufferedReader in = new BufferedReader(new InputStreamReader(s.getInputStream()));
            PrintWriter pr = new PrintWriter(s.getOutputStream());
            input = in.readLine();
            System.out.println("Here Input : " + input);
        }
         */
    }

}

/*

        localhost:6789/post.html
        GET /idx.html HTTP/1.1

 */
class Worker_Thread implements Runnable {

    Thread t;
    StringParser parser;
    String input;

    public Worker_Thread(String input) {
        t = new Thread();
        t.start();
        this.input = input;
        this.parser = new StringParser(input);
    }

    @Override
    public void run() {

    }

}

class StringParser {

    String input;
    Info info;
    List<String> headers;
    ///   Info outputInfo;

    StringParser(String input) {
        this.input = input;
        this.info = new Info(input);
        this.headers = new ArrayList<>();
    }

    public void getInfoOfRequest() {
        info.processTheInput();
        this.formHeaders();
        System.out.println("===>> INSIDE getInfoOfRequest()");
    }

    public void formHeaders() {
        System.out.println("====>>>> INSIDE formHeader1() method...printing the info.");
        System.out.println(this.info.toString());
//        if (this.info.method == null) {
//            System.err.print("=====>>>> METHOD IS NULL");
//            return;
//        }
        
        if (this.info.method.trim().equals("GET") == true) {
            File file = new File("text.html");
            
            System.out.println("file name :  " + file.getName() + " exists?? == " + file.exists());
            System.out.println("====>>>> INSIDE formHeader2() method");

        } else if (this.info.method.equalsIgnoreCase("POST")) {

        }
    }
}

class Info {

    String fileName;
    String method;
//    String directory;
    boolean isEmptyFileName;
    String inputLine;
    String version;
    String mimeType;

    public Info(String s) {
        this.inputLine = s;
    }

    public void processTheInput() {
        ///get rid of %20 signs for extra spaces in http input ??
        String[] arr = inputLine.split("/");
        printArray(arr);
        this.method = arr[0];
        if (arr.length < 3) {
            System.err.print("INVALID HTTP INPUT => " + inputLine);
            return;
        }

        this.version = arr[arr.length - 1];

        if (arr.length == 3) {
            //no file name ... access index.html
            this.isEmptyFileName = true;
            this.fileName = "index.html";
            this.mimeType = "index/html";
        } else {
            this.isEmptyFileName = false;
            findFileName(arr[arr.length - 2]);
        }
    }

    private void findFileName(String intermediateInput) {
        
        String[] arr2 = intermediateInput.split(" ");
        //trimming array
        //trimArray(arr2);
        printArray(arr2);
        this.fileName = arr2[0]; //eg=>   index.html
        //arr2[1] is HTTP
        this.mimeType = arr2[0] + "/" + arr2[1];
    }

    @Override
    public String toString() {
        return "Info{" + "fileName=" + fileName + ", method=" + method + ", isEmptyFileName=" + isEmptyFileName + ", inputLine=" + inputLine + ", version=" + version + ", mimeType=" + mimeType + '}';
    }

    public static void printArray(String[] arr) {
        System.out.println("-----------------------******---------------------");
        for (String s : arr) {
            System.out.println(s);
        }
        System.out.println("-----------------------******---------------------");
    }

    public static void printArray(List<String> arr) {
        System.out.println("-----------------------******---------------------");
        arr.forEach((s) -> {
            System.out.println(s);
        });
        System.out.println("-----------------------******---------------------");
    }

    private void trimArray(String[] arr2) {
        for(int i=0; i<arr2.length; i++){
            arr2[i].trim();
        }
    }
}
