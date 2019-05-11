package des;

import static des.Helper.*;

public class DES_Runner {

    private static final char SPECIAL_CHARACTER = '@';
    public static String inputStringPlainText = "Hello wo";//"Hello world";
    public static String inputKey = "megabuck";

    public static String paddedInputPlainText;

    public static boolean[] paddedInputBits;
    public static boolean[] transposePaddedBits;
    public static boolean[] keysBits = new boolean[FULL_KEY_LEN];
    public static boolean[] shortenedKeys = new boolean[SHORTENED_KEY_LEN];

    //For DEBUG
    private static final boolean DEBUG_PRINT = true;

    //Iteration Values
    private IterationValues[] iterationValues = new IterationValues[16];
    
    public void run() {
        //1. Pad the input String to make 64bits multiple [8 bytes multiple]
        padInputStringPlainText();
        //2. Initialise array of booleans
        initialiseAndFillBooleanArrays();
        
        //2.5: Initialise the 0th iter and boolean arrays
        iterationValues[0] = new IterationValues(0);
        iterationValues[0].makeZeroIteration(transposePaddedBits, shortenedKeys);
        
        
        //3. Apply 15 iterations [first done beforehand]       
        for(int iterNum = 1; iterNum < 16; iterNum++){
            runIterations(iterNum);
        }
        
    }

    private void runIterations(int iterNum) {
        
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

    }

    private void initialiseAndFillBooleanArrays() {
        paddedInputBits = new boolean[paddedInputPlainText.length() * 8];
        transposePaddedBits = new boolean[paddedInputPlainText.length() * 8];

        //Inputs
        paddedInputBits = getBits(paddedInputPlainText);

        transposePaddedBits = transpose(paddedInputBits);

        //Keys
        keysBits = getBits(inputKey);
        shortenedKeys = shortenTheKeys(keysBits);
        
        if (DEBUG_PRINT) {
            System.out.println("---------------------------------------------------------------------------");
            System.out.println("Normal padded data [boolean arr/ bits]");

            Helper.printBooleanArray(paddedInputBits);

            System.out.println("Transposed data [bits]");

            Helper.printBooleanArray(transposePaddedBits);
            System.out.println("---------------------------------------------------------------------------");

            System.out.println("Normal Key (bits)");
            Helper.printBooleanArray(keysBits);
            System.out.println("Modified Key Shortened (56 bits)");
            Helper.printBooleanArray(shortenedKeys);

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
        for (int i = 0; i < PI.length; i++) {
            int whichPos = PI[i] - 1;   //eg. PI[0] - 1 = 58 - 1 = 57
            //transpose_data[0] = data[57];
            transposed_data[i] = data[whichPos];
        }

        return transposed_data;
    }

    private boolean[] shortenTheKeys(boolean[] key) {
        //modified_key[0] = key[56] = CP_1[0] - 1 ... Use the help of CP_1 matrix
        boolean[] modified_data = new boolean[SHORTENED_KEY_LEN];
        
        for(int i=0; i<CP_1.length; i++){
            int pos = CP_1[i] - 1;
            modified_data[i] = key[pos];
        }
        
        return modified_data;
    }

}
