package problem2;

import java.io.*;
import java.net.*;

public class HTTP_Server {

    public static final int serverPortNumber = 6789;
    public static final int maxLimit = 7000;
    public static String serverName = "1505022_1505021_Server";

    public static void main(String[] args) {
        try {
            LogGenerator.writeInitial();
            runServer(serverPortNumber);
            
//            Post_Handler handler = new Post_Handler(testString);
//            byte[] data = handler.writeHTMLFileChanged("Mahim Mahbub ALALALALLALALAAA", "form_submited.html", "POST");
//            String s = new String(data);
//            System.out.println(s);
//            Helper helper = new Helper();
//            System.out.println(helper.getUserName("user=Mahim+Mahbub"));
//            runTest();
//            runMimeTest();
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    private static void runServer(int serverPortNumber) throws IOException {
        ServerSocket serverSocket = new ServerSocket(serverPortNumber);
        System.out.println("Server started.\nListening for connections on port : " + serverPortNumber + " ...\n");
        int id = 0;
        while (true) {
            Socket socket = serverSocket.accept();
            String name = "Thread_" + id;
            Thread t = new Thread(new Worker_Thread(socket, (name)));
            t.start();
            id++;
            System.out.println("\n\n====+++=======>>>>>INSIDE runServer()... The Thread with name " + name + " dies ... \n\n");
        }
    }

    private static void runTest() {
        String s1 = "GET / HTTP/1.1";
        s1 = "GET /web HTTP/1.1";
//        s1 = "GET /folder1/log.html HTTP/1.1";
//        s1 = "GET /folder1/ HTTP/1.1";
//        s1 = "GET /folder1/text.txt HTTP/1.1";
//        s1 = "GET / HTTP/1.1";
//        s1 = "GET/f o   lder1/ ha   ss.html HTTP/1.1";
        String[] arr = s1.split("/");
        Handler.trimArray(arr);
//        Handler.printArray(arr);

        System.out.println(searchFileAndRespond(runTestFileName(s1)));
    }

    private static void runTest2() {
        String firstLine = "GET /folder1/ HTTP/1.1";
        firstLine = "GET /folder1/inde  x.html HTTP/1.1";
        firstLine = "GET /folder1/fold ear2/ fi l e  x.css HTTP/1.1";
        firstLine = "GET/ HTTP/1.1";

        System.out.println("==>>Command is:" + firstLine);
        String[] firstLineSeparated = firstLine.split("/");
        Handler.trimArray(firstLineSeparated);
        Handler.printArray(firstLineSeparated);

        String fileNameWithHTTP = firstLineSeparated[firstLineSeparated.length - 2];
        String str = "/";
        String fileName = "";
        if (fileNameWithHTTP.length() == ("HTTP".length())) {
            fileName = Handler.defaultFileName;

        } else {
            fileName = fileNameWithHTTP.substring(0, fileNameWithHTTP.length() - 5);

        }
        for (int i = 1; i < (firstLineSeparated.length - 2); i++) {
            str = str + firstLineSeparated[i] + "/";
        }
        str += fileName;
//        str = str.substring(0, str.length() - 2);
        System.out.println("Final str: <" + str + ">");
    }

    static String runTestFileName(String firstLine) {
        Info info = new Info();
//        String firstLine = "GET /folder1/ HTTP/1.1";
//        firstLine = "GET /folder1/inde  x.html HTTP/1.1";
//        firstLine = "GET /folder1/fold ear2/ fi l e  x.css HTTP/1.1";
//        firstLine = "GET/ HTTP/1.1";
        info.isEmptyFileName = false;
        System.out.println("==>>Command is:" + firstLine);
        String[] firstLineSeparated = firstLine.split("/");
        Handler.trimArray(firstLineSeparated);
        Handler.printArray(firstLineSeparated);
        String fileNameWithHTTP = firstLineSeparated[firstLineSeparated.length - 2];
        if ((firstLineSeparated.length == 3)) {
            if (fileNameWithHTTP.length() == ("HTTP".length())) {   //check length 3 otherwise .. GET /folder1/ HTTP/1.1 for a folder
                info.fileName = Handler.defaultFileName;    //use default index.html if no file is mentioned
                info.isEmptyFileName = true;
            } else {
                info.fileName = fileNameWithHTTP.substring(0, fileNameWithHTTP.length() - 5);
                info.isEmptyFileName = false;
                /*
                    eg=>text.txt HTTP                  P is len - 1 , T is len - 2, T is len - 3, H is len - 4, SPACE is len - 5, 
                                                        so choose from start to len - 5 (start due to trimming)
                                        
                 */
            }
        } else {
            //Form a directory..
            String str = "";
            String fileName = "";
            if (fileNameWithHTTP.length() == ("HTTP".length())) {
                fileName = Handler.defaultFileName;
                info.isEmptyFileName = true;
            } else {
                fileName = fileNameWithHTTP.substring(0, fileNameWithHTTP.length() - 5);

            }
            for (int i = 1; i < (firstLineSeparated.length - 2); i++) {
                str = str + firstLineSeparated[i] + "/";
            }
            str += fileName;
            info.fileName = str;
        }
        if (!(info.fileName.contains("."))) {
            info.fileName += ".html";
        }
        System.out.println("RETURNING fileName : " + info.fileName);
        return info.fileName;
    }

    public static String searchFileAndRespond(String fileNameWithExtension) {
        File file;
        String status = null;
        try {
//            fileNameWithExtension = "folder1/text.txt";
            file = new File(fileNameWithExtension);
            System.out.println("SEARCHING FOR FILE NAME: <" + fileNameWithExtension + ">");
            if (file.exists() == true) {
                //File does exist, response 200 OK
                status = Handler.getResponse(Handler.validResponseID);

            } else {
                // File does not exist, response 404 Not Found
                status = Handler.getResponse(Handler.notFoundID);
            }
        } catch (Exception e) {
            e.printStackTrace();
        }
        return status;
    }

    private static void runMimeTest() {
        Helper helper = new Helper();
        String s2 = "text.txt";
        String[] arr = {"t ex t.txt", "i nd ex.html", "fol  d er1/id x.css", "readMe .pdf"};

        for (String s1 : arr) {
            System.out.println(helper.getMimeType(s1));
        }

    }
    public static String testString = "POST /form_submited HTTP/1.1\n"
            + "Host: localhost:6789\n"
            + "Connection: keep-alive\n"
            + "Content-Length: 17\n"
            + "Cache-Control: max-age=0\n"
            + "Origin: http://localhost:6789\n"
            + "Upgrade-Insecure-Requests: 1\n"
            + "Content-Type: application/x-www-form-urlencoded\n"
            + "User-Agent: Mozilla/5.0 (Windows NT 6.3; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/70.0.3538.77 Safari/537.36\n"
            + "Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,image/apng,*/*;q=0.8\n"
            + "Referer: http://localhost:6789/\n"
            + "Accept-Encoding: gzip, deflate, br\n"
            + "Accept-Language: en-US,en;q=0.9\n"
            + "\n"
            + "user=Mahim+Mahbub";
}
