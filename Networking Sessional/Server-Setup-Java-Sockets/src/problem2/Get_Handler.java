package problem2;

import java.io.*;
import java.util.Date;

public class Get_Handler implements Handler {

    private String inputString;
    private boolean isEmptyFile;
    private String fileName;
    private Helper helper;

    Get_Handler(String inputString) {
        this.inputString = inputString;
        this.helper = new Helper();

    }

    @Override
    public Info handle() {
        Info info = new Info();
        //Null Checking done before
        String[] separateLines = inputString.split("\n");

        String firstLine = separateLines[0];

        String[] firstLineSeparated = firstLine.split("/");

        if (firstLineSeparated.length < 3) {
            System.out.println("++!!!!====+++===--->>>>>Line 21 Get_Handler().... First Line is invalid .. -> " + firstLine);
            return new ErrorHandler().handle();
//            return null;
        }

        info.inputLine = firstLine;

        //Get file name
        info.fileName = this.helper.getCorrectFileName(firstLineSeparated); //Obtain File's Name..
        this.fileName = info.fileName;  //For safety ..(redundant)

        boolean error = false;

        File file = new File(info.fileName);        //Open a file object for future file.length()  in Content-Length
        if (!(file.exists())) {
            file = new File(Handler.errorPageName);
            error = true;
        }
        info.file = file;

        String status = this.helper.searchFileAndRespond(info.fileName);    //200 OK  or  404 Not Found

        //Get info.dataInBytes
        info.dataInBytes = new byte[this.helper.loadHTMLPageIntoByteArray(info.fileName).length];
        System.arraycopy(this.helper.loadHTMLPageIntoByteArray(info.fileName), 0, info.dataInBytes, 0, this.helper.loadHTMLPageIntoByteArray(info.fileName).length);

        info.version = firstLineSeparated[firstLineSeparated.length - 1]; //Last er string ta  te ja thaake eg. GET / index.html HTTP/1.1

        if (error == true) {
            info.mimeType = "text/html";
        } else {
            info.mimeType = helper.getMimeType(info.fileName);

        }
        info.method = "GET";

        info.request = firstLine;
        info.status = this.helper.statusCode(info.fileName);

        //insert headers..
        info.headers.add("HTTP/" + info.version + " " + status);
        info.headers.add("Server: Java HTTP " + HTTP_Server.serverName + ": " + info.version);
        info.headers.add("Date " + new Date());
        info.headers.add("Content-type: " + info.mimeType);
        info.headers.add("Content-Length: " + info.file.length());

        //headers done... add a blank line in Working_Thread.
        return info;
    }

    public String getCorrectFileName(String[] firstLineSeparated) {

        return this.helper.getCorrectFileName(firstLineSeparated);
    }

    public String searchFileAndRespond(String fileNameWithExtension) {
        return this.helper.searchFileAndRespond(fileNameWithExtension);
    }

}
