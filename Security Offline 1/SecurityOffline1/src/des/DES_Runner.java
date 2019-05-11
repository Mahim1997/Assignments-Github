package des;

import static des.Helper.*;

public class DES_Runner {

    private final char SPECIAL_CHARACTER = '@';
    public String inputStringPlainText = "Hello wo";//"Hello world";
    public String inputKey = "megabuck";

    public String paddedInputPlainText;

    public boolean[] paddedInputBits;
    public boolean[] transposePaddedBits;
    public boolean[] keysBits = new boolean[FULL_KEY_LEN];
    public boolean[] shortenedKeys = new boolean[SHORTENED_KEY_LEN];

    //For DEBUG
    private final boolean DEBUG_PRINT = true;

    //Iteration Values
    private IterationValues[] iterationValues = new IterationValues[16];

    public boolean[] ultimateFinalData = new boolean[64];
    public String cipheredText;

    public String plainTextDecrypted;

    public void initialiseParameters(String inputPlainText, String keyInput) {

    }

    public String getCipheredTextFinalOutput() {
        return this.cipheredText;
    }

    public String getDecryptedPlainText() {
        return this.plainTextDecrypted;
    }

    public void run() {
        //1. Pad the input String to make 64bits multiple [8 bytes multiple]
        padInputStringPlainText();
        //2. Initialise array of booleans
        initialiseAndFillBooleanArrays();

        //2.5: Initialise the 0th iter and boolean arrays
        iterationValues[0] = new IterationValues(0);
        iterationValues[0].completeThisIteration(transposePaddedBits, shortenedKeys);

        //3. Apply 15 iterations [first done beforehand]       
        for (int iterNum = 1; iterNum < 16; iterNum++) {
            runIterations(iterNum);
        }

        boolean[] finalDataAfter16Iterations = iterationValues[15].fullDataPlainText_BooleanArray;
        //4. Make Left<- Right and Right<- Left
        boolean[] left = Helper.getNumBits(finalDataAfter16Iterations, 0, 32);
        boolean[] right = Helper.getNumBits(finalDataAfter16Iterations, 1, 32);

        boolean[] newLeft = new boolean[right.length];
        System.arraycopy(right, 0, newLeft, 0, right.length);

        boolean[] newRight = new boolean[left.length];
        System.arraycopy(left, 0, newRight, 0, left.length);

        this.ultimateFinalData = Helper.mergeBooleanArray(newLeft, newRight);

        System.out.println("================================================================================");
        System.out.println("Left<-Right and Right<-Left, Final Data is: ");
        Helper.printBooleanArray(this.ultimateFinalData);
        System.out.println("After transposing finally with PI_1 to get: ");
        boolean[] finalData = new boolean[64];
        for (int i = 0; i < PI_1.length; i++) {
            int pos = PI_1[i] - 1;
            finalData[i] = this.ultimateFinalData[pos];
        }
        System.arraycopy(finalData, 0, this.ultimateFinalData, 0, finalData.length);
        Helper.printBooleanArray(this.ultimateFinalData);

        this.cipheredText = Helper.getString_AsAWhole(this.ultimateFinalData);

        System.out.println("\nCiphered Text is: ");
        System.out.println(this.cipheredText);

        decrypt();

        System.out.println("\nAfter Decryption, Plain Text is: ");
        System.out.println(this.plainTextDecrypted);
    }

    private void runIterations(int iterNum) {
        iterationValues[iterNum] = new IterationValues(iterNum);
        boolean[] text = iterationValues[iterNum - 1].fullDataPlainText_BooleanArray;
        boolean[] keys = shortenedKeys;

        iterationValues[iterNum].completeThisIteration(text, shortenedKeys);
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
        paddedInputBits = getBits_InBooleanArray(paddedInputPlainText);

        transposePaddedBits = transpose(paddedInputBits);

        //Keys
        keysBits = getBits_InBooleanArray(inputKey);
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

        for (int i = 0; i < CP_1.length; i++) {
            int pos = CP_1[i] - 1;
            modified_data[i] = key[pos];
        }

        return modified_data;
    }



    private void decrypt() {
        IterationValues[] decryptThings = new IterationValues[16];

        System.out.println("\n\n\n");
        System.out.println("------------------------------------------------ DECRYPTING ------------------------------------------------");

        for (int i = 0; i < 16; i++) {
            decryptThings[i] = new IterationValues(i);  //Emni !! dorkar nai
            decryptThings[i].setKeyForThisIteration(this.iterationValues[16 - 1 - i].keys_48bits_ThisIteration);
            decryptThings[i].encryptMode = false;   //Decrypt mode
            if (i == 0) {
                decryptThings[i].completeThisIteration(Helper.getBits_InBooleanArray(this.cipheredText), shortenedKeys);
            }else{
                decryptThings[i].completeThisIteration(decryptThings[i-1].fullDataPlainText_BooleanArray, shortenedKeys);
            }

        }
        boolean[] finalData = decryptThings[15].fullDataPlainText_BooleanArray;
        this.plainTextDecrypted = Helper.getString_AsAWhole(finalData);
    }

}
