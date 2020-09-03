package problem2;

import java.io.*;
import java.util.Date;

public class Post_Handler implements Handler {

    private String inputString;
    private Helper helper;

    Post_Handler(String inputString) {
        this.inputString = inputString;
        this.helper = new Helper();
    }

    @Override
    public Info handle() {
        Info info = new Info();
        System.out.println("-->>>>POST: inputString is <" + this.inputString + ">");
        String[] separateLines = inputString.split("\n");

        String firstLine = separateLines[0];

        String[] firstLineSeparated = firstLine.split("/");

        if (firstLineSeparated.length < 3) {
            System.out.println("++!!!!====+++===--->>>>>Line 21 Get_Handler().... First Line is invalid .. -> " + firstLine);
            return new ErrorHandler().handle();
//            return null;
        }

        String correctFileName = helper.getCorrectFileName(firstLineSeparated);

        String lastLine = separateLines[separateLines.length - 1];
//        System.out.println("==>Last line :" + lastLine);
        String userName = helper.getUserName(lastLine);

        byte[] data = writeHTMLFileChanged(userName, correctFileName, "POST");

        info.inputLine = firstLine;

        info.dataInBytes = new byte[data.length];
        System.arraycopy(data, 0, info.dataInBytes, 0, data.length);

        info.method = "POST";

        int id;
        File file = new File(correctFileName);
        info.isEmptyFileName = false;

        boolean error = false;

        if (file.exists() == false) {
            id = Handler.notFoundID;
            file = new File(Handler.errorPageName);
            error = true;
            info.isEmptyFileName = true;
        } else {
            id = Handler.validResponseID;
        }
        info.file = file;
        info.fileName = correctFileName;

        if (error == true) {
            info.mimeType = "text/html";
        } else {
            info.mimeType = helper.getMimeType(correctFileName);
        }

        info.version = firstLineSeparated[firstLineSeparated.length - 1];   //Last er String ta 1.1 thake

        info.request = firstLine;
        info.status = this.helper.statusCode(info.fileName);

        //insert headers..
        info.headers.add("HTTP/" + info.version + " " + Handler.getResponse(id));
        info.headers.add("Server: Java HTTP " + HTTP_Server.serverName + ": " + info.version);
        info.headers.add("Date " + new Date());
        info.headers.add("Content-type: " + info.mimeType);
        info.headers.add("Content-Length: " + info.file.length());

        //headers done... add a blank line in Working_Thread.
//        System.out.println("===>>>>Inside handle(). correctFileName is :" + correctFileName);
        return info;
    }

    public byte[] writeHTMLFileChanged(String userName, String fileNameWithExtension, String methodType) {
        String htmlDoc = "<html>\n"
                + "	<head>\n"
                + "		<meta http-equiv=\"Content-Type\" content=\"text/html; charset=UTF-8\">\n"
                + "	</head>\n"
                + "	<body>\n"
                + "		<h1> Welcome to CSE 322 Offline 1</h1>\n"
                + "		<h2> HTTP REQUEST TYPE->" + methodType + " </h2>\n"
                + "		<h2> Post->" + userName + " </h2>\n"
                + "		<form name=\"input\" action=\"http://localhost:6789/form_submited\" method=\"post\">\n"
                + "			Your Name: <input type=\"text\" name=\"user\">\n"
                + "		<input type=\"submit\" value=\"Submit\">\n"
                + "		</form>\n"
                + "	</body>\n"
                + "</html>";
        byte[] data = new byte[htmlDoc.length()];
        data = htmlDoc.getBytes();

        try {
            File file = new File(fileNameWithExtension);
            if (file.exists()) {
                System.out.println("FILE EXISTS!!!");
            } else {
                System.out.println("FILE DOESNOT EXIST!!!");
            }
//            buffer_input = new BufferedReader(new InputStreamReader(socket.getInputStream()));
            PrintWriter printWriter = new PrintWriter(new FileOutputStream(file));
            printWriter.write(htmlDoc);
            printWriter.flush();
        } catch (Exception e) {
            e.printStackTrace();
        }

        return data;
    }

}
