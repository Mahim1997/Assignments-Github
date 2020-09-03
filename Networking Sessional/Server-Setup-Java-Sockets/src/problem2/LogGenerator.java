package problem2;

import java.io.*;

public class LogGenerator {

    public static String logFileName = "server_log.txt";

    public static void writeInitial() {
        //Try With Resources
        try (PrintWriter printWriter = new PrintWriter(new FileOutputStream(new File(logFileName)))) {
            printWriter.write("");
            printWriter.flush();
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    public static void writeLog(Log toWriteLog) {
        try (FileWriter fw = new FileWriter(logFileName, true);
                BufferedWriter bw = new BufferedWriter(fw);
                PrintWriter out = new PrintWriter(bw)) {
                out.println(toWriteLog.toString());
                //more code
//                out.println("more text");
            //more code
        } catch (IOException e) {
            //exception handling left as an exercise for the reader
        }
    }

}
