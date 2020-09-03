package problem2;

import java.io.*;
import java.util.*;

class Info {
    
    String status;
    String request;
    
    String fileName;
    String method;
    boolean isEmptyFileName;
    String inputLine;
    String version;
    String mimeType;
    List<String> headers;
    byte[] dataInBytes;
    File file;

    public Info() {
        headers = new ArrayList<>();
    }

    @Override
    public String toString() {
        return "Info{" + "fileName=" + fileName + ", method=" + method + ", isEmptyFileName=" + isEmptyFileName + ", inputLine=" + inputLine + ", version=" + version + ", mimeType=" + mimeType + '}';
    }
    public String getData(){
        return new String(dataInBytes);
    }
    public String getHeaders(){
        String str = "";
        for(String s: headers){
            str += (s + "\n");
        }
        return str;
    }
}
