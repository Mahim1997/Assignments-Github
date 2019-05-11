package des;

import java.io.UnsupportedEncodingException;
import java.util.logging.Level;
import java.util.logging.Logger;

public class DES_Runner {

    private static final char SPECIAL_CHARACTER = '@';
    public static String inputStringPlainText = "Hello wo";//"Hello world";
    public static String paddedInputPlainText;

    public static boolean[] paddedInputBits;
    public static boolean[] transposePaddedBits;

    //For DEBUG
    private static final boolean DEBUG_PRINT = true;

    public void run() {
        //1. Pad the input String to make 64bits multiple [8 bytes multiple]
        padInputStringPlainText();
        //2. Initialise array of booleans
        initialiseAndFillBooleanArrays();
    }

    private void padInputStringPlainText() {
        paddedInputPlainText = inputStringPlainText;
        int len = inputStringPlainText.length();
        if (len % 8 != 0) {
            int remaining = 8 - (len % 8);
            for (int i = 0; i < remaining; i++) {
                paddedInputPlainText += SPECIAL_CHARACTER;
            }
        }
//        printAll();

    }

    private void printAll() {
        if (DEBUG_PRINT) {
            System.out.println("Padded String: <" + paddedInputPlainText + ">");
        }

    }

    private void initialiseAndFillBooleanArrays() {
        paddedInputBits = new boolean[paddedInputPlainText.length() * 8];
        transposePaddedBits = new boolean[paddedInputPlainText.length() * 8];

        paddedInputBits = getBits(paddedInputPlainText);

        if (DEBUG_PRINT) {
            Helper.printBooleanArray(paddedInputBits);
        }

    }

    private boolean[] getBits(String str) {
        byte[] bytes = null;
        try {
            bytes = str.getBytes("US-ASCII");
        } catch (UnsupportedEncodingException ex) {
            ex.printStackTrace();
        }
        boolean[] barr = Helper.toBooleanArray(bytes);
        return barr;
    }
}
