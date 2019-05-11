package des;

import static des.Helper.CP_2;
import static des.Helper.SHIFT;

public class IterationValues {

    public int iterationNumber;
    public boolean[] left32BitsPlainText = new boolean[32];
    public boolean[] right32BitsPlainText = new boolean[32];
    public boolean[] left28bitsKeys = new boolean[28];
    public boolean[] right28bitsKeys = new boolean[28];
    int shortenedKeyLength = 28;

    public IterationValues(int iterationNumber) {
        this.iterationNumber = iterationNumber;
    }
    public IterationValues(){}



    public void makeZeroIteration(boolean[] transposePaddedBits, boolean[] shortenedKeys) {
        boolean[] left32Bits = getNumBits(transposePaddedBits, 0, 32);
        boolean[] right32Bits = getNumBits(transposePaddedBits, 1, 32);
        
        this.left32BitsPlainText = right32Bits; //L_i = R_(i-1)
        
        //R_i = L_(i-1) XOR f(R_(i-1), K_i)
        
        //First we get  each 28 bits of key
        left28bitsKeys = getNumBits(shortenedKeys, 0, 28);
        right28bitsKeys = getNumBits(shortenedKeys, 1, 28);        
        
        //Now we rotate left according to SHIFT array
        int numRotation = SHIFT[iterationNumber];
        left28bitsKeys = Helper.leftRotate(left28bitsKeys, numRotation);
        right28bitsKeys = Helper.leftRotate(right28bitsKeys, numRotation);
        
        boolean [] mergedKeys = Helper.mergeBooleanArray(left28bitsKeys, right28bitsKeys);
        //Key_in_round_i[0] = modified_key[13], ....
        for(int i=0; i<shortenedKeyLength; i++){
            int pos = CP_2[i] - 1;
            shortenedKeys[i] = mergedKeys[pos];
        }
        
        
        System.out.println("<><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><>");
        System.out.println("--->>>>>>Inside makeZeroIteration .... printing things ... ");
        System.out.println("Printing left32Bits, then right32Bits, then left28Keys, then right28Keys");
        Helper.printBooleanArray(left32Bits);
        Helper.printBooleanArray(right32Bits);
        Helper.printBooleanArray(left28bitsKeys);
        Helper.printBooleanArray(right28bitsKeys);
        
        System.out.println("--->>>SHORTENED KEYS [48 bits] : ");
        Helper.printBooleanArray(shortenedKeys);
        
        System.out.println("<><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><>");
    }

    private boolean[] getNumBits(boolean[] array, int leftZeroRightOne, int numBits) {
        boolean[] bArr = new boolean[numBits];
        
        if(leftZeroRightOne == 0){
            //left 32 bits
            System.arraycopy(array, 0, bArr, 0, numBits);
        }
        else{
            //right 32 bits
            for(int i=numBits; i<array.length; i++){
                bArr[i - numBits] = array[i];
            }
        }
        
        return bArr;
    }

}
