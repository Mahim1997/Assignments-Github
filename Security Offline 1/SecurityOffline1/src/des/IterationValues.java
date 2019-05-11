package des;

import static des.Helper.*;

public class IterationValues {


    private boolean[] left32BitsPlainText = new boolean[32];
    private boolean[] right32BitsPlainText = new boolean[32];
    private boolean[] left28bitsKeys = new boolean[28];
    private boolean[] right28bitsKeys = new boolean[28];

    int shortenedKeyLength = 28;
    
    //PUBLIC VARIABLES FOR NEXT ITERATIONS ... 
    public boolean[] fullDataPlainText = new boolean[64];
    public boolean[] keys_48bits_ThisIteration = new boolean[48];    
    public int iterationNumber;    

    public IterationValues(int iterationNumber) {
        this.iterationNumber = iterationNumber;
    }

    public IterationValues() {
    }

    public void completeThisIteration(boolean[] transposePaddedBits, boolean[] shortenedKeys) {
        boolean[] left32BitsPreviousPaddedBits = getNumBits(transposePaddedBits, 0, 32);
        boolean[] right32BitsPreviousPaddedBits = getNumBits(transposePaddedBits, 1, 32);

        this.left32BitsPlainText = right32BitsPreviousPaddedBits; //L_i = R_(i-1)

        //R_i = L_(i-1) XOR f(R_(i-1), K_i)
        //First we get  each 28 bits of key
        left28bitsKeys = Helper.getNumBits(shortenedKeys, 0, 28);
        right28bitsKeys = Helper.getNumBits(shortenedKeys, 1, 28);

        //Now we rotate left according to SHIFT array
        int numRotation = SHIFT[iterationNumber];
        left28bitsKeys = Helper.leftRotate(left28bitsKeys, numRotation);
        right28bitsKeys = Helper.leftRotate(right28bitsKeys, numRotation);

        boolean[] mergedKeys = Helper.mergeBooleanArray(left28bitsKeys, right28bitsKeys);   //MERGING OKAY

        //Key_in_round_i[0] = modified_key[13], ....
        /*
        Ki is derived from this rotated key by applying yet another 56-bit transposition to it according to CP_2 array.
        A different 48-bit subset of the 56 bits is extracted and permuted on each round
         */
        for (int i = 0; i < 48; i++) {
            int pos = CP_2[i] - 1;
            this.keys_48bits_ThisIteration[i] = mergedKeys[pos];
        }

        //------------------------------------------- FUNCTIONS begin --------------------------------------------------------
        //Expand the R_(i-1) using E matrix
        /*(a) You need to expand your leftmost 32 bits to 48 bits. Use E array.
        expanded_bits [0] = leftmost_bit[31], expanded_bits[1] = leftmost_bit[0]*/
        boolean[] e_48_bit_number = new boolean[48];    //First, a 48-bit number, e, is constructed by expanding the 32-bit R(i − 1) = L(i) according to E array
        for (int i = 0; i < E.length; i++) {
            int pos = E[i] - 1;
            e_48_bit_number[i] = this.left32BitsPlainText[pos]; //same as R(i-1)
        }

        //(b) e and K_i are XORED Together
        boolean[] result_xor = new boolean[48];
        for (int i = 0; i < result_xor.length; i++) {
            result_xor[i] = Helper.XOR(e_48_bit_number[i], this.keys_48bits_ThisIteration[i]);
        }

        //(c) Sample 32 bits from the result according to PI_2 arrays. i.e. shorten_result [0] = result_xor [34], ......
        boolean[] shorten_result_FromPart_C = new boolean[32];
        for (int i = 0; i < shorten_result_FromPart_C.length; i++) {
            int pos = PI_2[i] - 1;
            shorten_result_FromPart_C[i] = result_xor[pos];
        }

        //(d) Finally, these 32 bits are passed through a P-box. P box simulated as P array. 
        boolean[] PBox_Input = shorten_result_FromPart_C;
        boolean[] PBox_Output = new boolean[32];
        for (int i = 0; i < 32; i++) {
            int pos = P[i] - 1;
            PBox_Output[pos] = PBox_Input[i];
        }

        //Obtain R_i using L_(i-1) XOR PBox_Output [32 bits]
        for (int i = 0; i < 32; i++) {
            this.right32BitsPlainText[i] = Helper.XOR(PBox_Output[i], left32BitsPreviousPaddedBits[i]);
        }

        //Obtain full data of this iteration
        this.fullDataPlainText = Helper.mergeBooleanArray(this.left32BitsPlainText, this.right32BitsPlainText);
                
        
        // ------------------------------------------------------------------------------------------------------------------
        //------------------------------------------ PRINTING THINGS ------------------------------------------------------
//        System.out.println("<><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><>");
//        System.out.println("--->>>>>>Inside makeZeroIteration .... printing things ... ");
//        System.out.println("Printing prev left32Bits, then prev right32Bits, then left28Keys, then right28Keys");
//        Helper.printBooleanArray(left32BitsPreviousPaddedBits);
//        Helper.printBooleanArray(right32BitsPreviousPaddedBits);
//        Helper.printBooleanArray(left28bitsKeys);
//        Helper.printBooleanArray(right28bitsKeys);
//
//        System.out.println("--->>>SHORTENED KEYS [48 bits] i.e. K_i = keys_48bits_ThisIteration : ");
//        Helper.printBooleanArray(keys_48bits_ThisIteration);
//
//        System.out.println("<><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><>");

        System.out.println("-------------------------------------------Iteration " + this.iterationNumber + " -----------------------------------------------------------------------");

        System.out.println("Just Copying prev right to current left, i.e. L(i) = R(i-1) [32 bits] we get [64 BITS now]");
        Helper.printBooleanArray(this.left32BitsPlainText, false);
        Helper.printBooleanArray(Helper.getBoolean(false, 32), true);

        System.out.println("KEY_i is [48 bits] : ");
        Helper.printBooleanArray(this.keys_48bits_ThisIteration);

        System.out.println("Expanded 48 bit number of R(i-1) (Using E matrix) is :");
        Helper.printBooleanArray(e_48_bit_number);  //Correct

        System.out.println("After XORING expandedNumber and KEY [48 bits] we get :");
        Helper.printBooleanArray(result_xor);

        System.out.println("PBOX Input (32 bits): ");
        Helper.printBooleanArray(PBox_Input);

        System.out.println("PBox Output is [32 bits] : ");
        Helper.printBooleanArray(PBox_Output);

        System.out.println("Right 32 bits [PBOX_Output XOR L(i-1)] : ");
        Helper.printBooleanArray(this.right32BitsPlainText);

        System.out.println("Full data after Iteration " + this.iterationNumber + " : ");
        Helper.printBooleanArray(this.fullDataPlainText);
        
        System.out.println("------------------------------------------------------------------------------------------------------------------");

    }

 

}
