
import java.util.Arrays;

public class DES_Decryptor {
    public String inputKey ;
    private final char SPECIAL_CHARACTER = '@';
    public String cipheredText;
    public String plainTextDecrypted;
    public boolean[] shortenedKeysForDecryption = new boolean[Helper.SHORTENED_KEY_LEN];

    public DES_Decryptor(String key){
//        this.cipheredText = inputCipher;
        this.inputKey = key;
    }
    
    private void runDecryptIterations(IterationValues[] decryptors, int iterNum, IterationValues[] keyCalculators) {
        decryptors[iterNum] = new IterationValues(iterNum);
        decryptors[iterNum].encryptMode = false;
        decryptors[iterNum].keys_48bits_ThisIteration = Arrays.copyOf(keyCalculators[15 - iterNum].keys_48bits_ThisIteration,
                keyCalculators[15 - iterNum].keys_48bits_ThisIteration.length);
//        decryptors[iterNum].keys_48bits_ThisIteration = Arrays.copyOf(iterationValues[15 - iterNum].keys_48bits_ThisIteration,
//                iterationValues[15 - iterNum].keys_48bits_ThisIteration.length);
        decryptors[iterNum].completeThisIteration(decryptors[iterNum - 1].fullDataPlainText_BooleanArray, this.shortenedKeysForDecryption);
    }

    private void calculateKeys(IterationValues[] iters, String inputKey) {
        //0th
        boolean[] keysBits_ForAll = Helper.getBits_InBooleanArray(inputKey);
        boolean[] shortenedKeysForDecryption = Helper.shortenTheKeys(keysBits_ForAll);

        for (int i = 0; i < iters.length; i++) {
            iters[i].obtainThisIterationKey(shortenedKeysForDecryption);
        }

    }

    public void runDecryption(String inputString_CipherText) {
        //1. Pad the input String to make 64bits multiple [8 bytes multiple]
        String paddedInput = Helper.padString(inputString_CipherText);
        //2. Initialise array of booleans
        boolean[] paddedInputBits_Dec = new boolean[paddedInput.length() * 8];
        boolean[] transposePaddedBits_Dec = new boolean[paddedInput.length() * 8];

        //Inputs
        paddedInputBits_Dec = Helper.getBits_InBooleanArray(paddedInput);

        transposePaddedBits_Dec = Helper.transpose(paddedInputBits_Dec);

        IterationValues[] decryptors = new IterationValues[16];

        IterationValues[] keyCalculators = new IterationValues[16];
        for (int i = 0; i < keyCalculators.length; i++) {
            keyCalculators[i] = new IterationValues(i);
        }
        calculateKeys(keyCalculators, this.inputKey);

//        System.out.println("--->>>>>>>>>>> PRINTING KEYS >>>> ");
//        
//        for(int i=0; i<keyCalculators.length; i++){
//            Helper.printBooleanArray(keyCalculators[i].keys_48bits_ThisIteration);
//        }
//        
//        System.out.println("\n\n\n---------=======-----===---------========-------\n\n\n");
        //2.5: Initialise the 0th iter and boolean arrays
        decryptors[0] = new IterationValues(0);
        decryptors[0].encryptMode = false;
//        decryptors[0].keys_48bits_ThisIteration = Arrays.copyOf(this.iterationValues[15].keys_48bits_ThisIteration,
//                this.iterationValues[15].keys_48bits_ThisIteration.length);
        decryptors[0].keys_48bits_ThisIteration = Arrays.copyOf(keyCalculators[15].keys_48bits_ThisIteration,
                keyCalculators[15].keys_48bits_ThisIteration.length);
        decryptors[0].completeThisIteration(transposePaddedBits_Dec, this.shortenedKeysForDecryption);

        //3. Apply 15 iterations [first done beforehand]       
        for (int iterNum = 1; iterNum < 16; iterNum++) {
            runDecryptIterations(decryptors, iterNum, keyCalculators);
        }

        boolean[] finalDataAfter16Iterations = decryptors[15].fullDataPlainText_BooleanArray;
        //4. Make Left<- Right and Right<- Left
        boolean[] left = Helper.getNumBits(finalDataAfter16Iterations, 0, 32);
        boolean[] right = Helper.getNumBits(finalDataAfter16Iterations, 1, 32);

        boolean[] newLeft = new boolean[right.length];
        System.arraycopy(right, 0, newLeft, 0, right.length);

        boolean[] newRight = new boolean[left.length];
        System.arraycopy(left, 0, newRight, 0, left.length);

        boolean[] ultimateData = Helper.mergeBooleanArray(newLeft, newRight);

        System.out.println("=================================== After Decryption ======================================================");
        System.out.println("Left<-Right and Right<-Left, Final Data is: ");
        Helper.printBooleanArray(ultimateData);
        System.out.println("After transposing finally with PI_1 to get: ");
        boolean[] finalData = new boolean[64];
        for (int i = 0; i < Helper.PI_1.length; i++) {
            int pos = Helper.PI_1[i] - 1;
            finalData[i] = ultimateData[pos];
        }
        System.arraycopy(finalData, 0, ultimateData, 0, finalData.length);
        Helper.printBooleanArray(ultimateData);

        this.plainTextDecrypted = Helper.getString_AsAWhole(ultimateData);

//        System.out.println("\nCiphered Text is: ");
//        System.out.println(this.cipheredText);
        System.out.println("\nAfter Decryption, Plain Text is: ");
        System.out.println(this.plainTextDecrypted);
    }
}
