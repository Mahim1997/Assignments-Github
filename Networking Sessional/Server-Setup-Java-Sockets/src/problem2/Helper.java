package problem2;

import java.io.File;
import java.io.FileInputStream;
import java.util.StringTokenizer;

public class Helper {

    private String fileName;
    private boolean isEmptyFile;
    public String htmlFileMime;// = "text/html";

    public Helper() {
        htmlFileMime = "text/html";
    }

    public String statusCode(String fileNameWithExtension) {
        File file;
        String status = "504";
        try {
            file = new File(fileNameWithExtension);
            if (file.exists() == true) {
                //File does exist, response 200 OK
                status = String.valueOf(Handler.validResponseID);

            } else {
                // File does not exist, response 404 Not Found
                status = String.valueOf(Handler.notFoundID) ;
            }
        } catch (Exception e) {
            e.printStackTrace();
        }
        return status;
    }

    public String searchFileAndRespond(String fileNameWithExtension) {
        File file;
        String status = Handler.defaultInvalid;
        try {
            file = new File(fileNameWithExtension);
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

    public String getCorrectFileName(String[] firstLineSeparated) {

        Handler.trimArray(firstLineSeparated);  //Trim to adjust spaces..
        Handler.replacePercentagesWithSpaces(firstLineSeparated);
        String fileNameWithHTTP = firstLineSeparated[firstLineSeparated.length - 2];    //Get second last line that has file name with HTTP in space

        isEmptyFile = false;
        if ((firstLineSeparated.length == 3)) {
            if (fileNameWithHTTP.length() == ("HTTP".length())) {   //check length 3 otherwise .. GET /folder1/ HTTP/1.1 for a folder
                this.fileName = Handler.defaultFileName;    //use default index.html if no file is mentioned
                this.isEmptyFile = true;
            } else {
                this.fileName = fileNameWithHTTP.substring(0, fileNameWithHTTP.length() - 5);
                this.isEmptyFile = false;
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
                this.isEmptyFile = true;
            } else {
                fileName = fileNameWithHTTP.substring(0, fileNameWithHTTP.length() - 5);
                this.isEmptyFile = false;

            }
            for (int i = 1; i < (firstLineSeparated.length - 2); i++) {
                str = str + firstLineSeparated[i] + "/";
            }
            str += fileName;
            this.fileName = str;
        }
        if (!(this.fileName.contains("."))) {
//            this.fileName += ".html";
            this.fileName += Handler.defaultHTMLFormat;
        }
        return this.fileName;
    }

    byte[] loadHTMLPageIntoByteArray(String fileName) {
        byte[] data = null;

        try {
            File file = new File(fileName);
            if (!file.exists()) {
                file = new File(Handler.errorPageName);     //To Display Error 404 page.
            }
            FileInputStream fin = new FileInputStream(file);
            data = new byte[(int) file.length()];
            fin.read(data);

        } catch (Exception e) {
            e.printStackTrace();
        }

        return data;
    }

    public String getMimeType(String fileNameWithExtension) {
        StringTokenizer token = new StringTokenizer(fileNameWithExtension, ".");
        String fileName;
        String format = "INVALID_MIME_TYPE";
        if (token.hasMoreTokens()) {
            fileName = token.nextToken();
            if (token.hasMoreTokens()) {
                format = token.nextToken();
            } else {
                format = null;
            }
        } else {
            return null;
        }

        String s = getMime(format);
        if (s == null) {
            s = "text/plain";
        }
        return s;
    }

    private String getMime(String formatOfFile) {
        String mime = null;

        mime = getMime_Text(formatOfFile);      //Chain of responsibility informally.
        if (mime != null) {
            return mime;
        }

        mime = getMime_Image(formatOfFile);
        if (mime != null) {
            return mime;
        }

        mime = getMime_Documents_Pdf(formatOfFile);
        if (mime != null) {
            return mime;
        }

        mime = getMime_Audio(formatOfFile);
        if (mime != null) {
            return mime;
        }

        mime = getMime_Video(formatOfFile);
        if (mime != null) {
            return mime;
        }

        return mime;
    }

    private String getMime_Text(String formatOfFile) {
        String mime = null;
        if (formatOfFile.equals("txt")) {
            mime = "text/plain";
        } else if (formatOfFile.equals("html")) {
            mime = "text/html";
        } else if (formatOfFile.equals("css")) {
            mime = "text/css";
        } else if (formatOfFile.equals("javascript")) {
            mime = "text/javascript";
        }

        return mime;
    }

    private String getMime_Image(String formatOfFile) {
        /*
            image/gif, image/png, image/jpeg, image/bmp, image/webp
         */
        String mime = null;

        if (formatOfFile.equals("gif")) {
            mime = "image/gif";
        } else if (formatOfFile.equals("png")) {
            mime = "image/png";
        } else if (formatOfFile.equals("bmp")) {
            mime = "image/bmp";
        } else if (formatOfFile.equals("webp")) {
            mime = "image/webp";
        }

        return mime;
    }

    private String getMime_Documents_Pdf(String s) {
        /*
        s -> formatOfFile
        
        application/javascript, application/octet-stream, 
        application/pkcs12, application/vnd.mspowerpoint, 
        application/xhtml+xml, application/xml, application/pdf
         */
        String mime = null;

        if (s.equals("javascript") || s.equals("octet-stream") || s.equals("pkcs12") || s.equals("vnd.mspowerpoint")
                || s.equals("xhtml+xml") || s.equals("xml") || s.equals("pdf") || s.equals("doc") || s.equals("docx")) {
            mime = "application/" + s;
        }

        return mime;
    }

    private String getMime_Audio(String s) {
        /*
             audio/midi, audio/mpeg, audio/webm, audio/ogg, audio/wav
         */
        String mime = null;
        if (s.equals("midi") || s.equals("mpeg") || s.equals("webm") || s.equals("ogg") || s.equals("wav")) {
            mime = "audio/" + s;
        }

        //vlc , km4 ???
        return mime;
    }

    private String getMime_Video(String formatOfFile) {
        /*
        video/webm, video/ogg
         */
        String mime = null;
        if (formatOfFile.equals("webm") || formatOfFile.equals("ogg")) {
            mime = "video/" + formatOfFile;
        } else if (formatOfFile.equals("vlc")) {
            mime = "video/" + formatOfFile;
        } else if (formatOfFile.equals("mp4")) {
            mime = "video/" + formatOfFile;
        }
        return mime;
    }

    public String getUserName(String lastLine) {
        //user=Mahim+Mahbub
        String user = "";

        if (lastLine.length() == 0) {
            return user;
        }

        user = lastLine.substring(5, lastLine.length());

        user = convertPlusToSpace(user);

        return user;
    }

    private String convertPlusToSpace(String str) {
        String s = "";
        for (int i = 0; i < str.length(); i++) {
            if (str.charAt(i) == '+') {
                s += " ";
            } else {
                s += str.charAt(i);
            }
        }
        return s;
    }

}
