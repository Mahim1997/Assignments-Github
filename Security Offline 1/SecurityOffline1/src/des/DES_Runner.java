package des;

import static des.Helper.*;

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
        
        transposePaddedBits = transpose(paddedInputBits);

        if (DEBUG_PRINT) {
            System.out.println("---------------------------------------------------------------------------");
            System.out.println("Normal padded data [boolean arr/ bits]");

            Helper.printBooleanArray(paddedInputBits);

            System.out.println("Transposed data [bits]");

            Helper.printBooleanArray(transposePaddedBits);
            System.out.println("---------------------------------------------------------------------------");
        }

    }

    private boolean[] getBits(String str) {
        boolean[] b_arr = new boolean[str.length()];
        String resultantBinString = "";
        for (int i = 0; i < str.length(); i++) {
            char c = str.charAt(i);
            int ascii = (int) c;
            String binString = Integer.toBinaryString(ascii);
            //Each character 1 byte i.e. 8 bits so pad pervious with 0's
            binString = padZeroBefore(binString, 8);

            //Append to String
            resultantBinString += binString;
        }
        b_arr = Helper.convertByOneFromString(resultantBinString);

        return b_arr;
    }

    private String padZeroBefore(String s, int howManyBits) {
        if (s.length() >= howManyBits) {
            return s;
        }
        int rem = howManyBits - s.length();
        String str = "";
        for (int i = 0; i < rem; i++) {
            str += '0';
        }
        str += s;
        return str;
    }

    private boolean[] transpose(boolean[] data) {
        boolean[] transposed_data = new boolean[data.length];
        
        //Take Help From PI matrix
        for(int i=0; i<PI.length; i++){
            int whichPos = PI[i] - 1;   //eg. PI[0] - 1 = 58 - 1 = 57
            //transpose_data[0] = data[57];
            transposed_data[i] = data[whichPos];
        }
        
        return transposed_data;
    }
}
