package des;

import java.util.Arrays;
import java.util.BitSet;

public class Helper {

    public static int SHORTENED_KEY_LEN = 56;
    public static int FULL_KEY_LEN = 64;

    public static int[] PI = {58, 50, 42, 34, 26, 18, 10, 2,
        60, 52, 44, 36, 28, 20, 12, 4,
        62, 54, 46, 38, 30, 22, 14, 6,
        64, 56, 48, 40, 32, 24, 16, 8,
        57, 49, 41, 33, 25, 17, 9, 1,
        59, 51, 43, 35, 27, 19, 11, 3,
        61, 53, 45, 37, 29, 21, 13, 5,
        63, 55, 47, 39, 31, 23, 15, 7};

    public static int[] CP_1 = {57, 49, 41, 33, 25, 17, 9,
        1, 58, 50, 42, 34, 26, 18,
        10, 2, 59, 51, 43, 35, 27,
        19, 11, 3, 60, 52, 44, 36,
        63, 55, 47, 39, 31, 23, 15,
        7, 62, 54, 46, 38, 30, 22,
        14, 6, 61, 53, 45, 37, 29,
        21, 13, 5, 28, 20, 12, 4};

    public static int[] CP_2 = {14, 17, 11, 24, 1, 5, 3, 28,
        15, 6, 21, 10, 23, 19, 12, 4,
        26, 8, 16, 7, 27, 20, 13, 2,
        41, 52, 31, 37, 47, 55, 30, 40,
        51, 45, 33, 48, 44, 49, 39, 56,
        34, 53, 46, 42, 50, 36, 29, 32};

    public static int[] E = {32, 1, 2, 3, 4, 5,
        4, 5, 6, 7, 8, 9,
        8, 9, 10, 11, 12, 13,
        12, 13, 14, 15, 16, 17,
        16, 17, 18, 19, 20, 21,
        20, 21, 22, 23, 24, 25,
        24, 25, 26, 27, 28, 29,
        28, 29, 30, 31, 32, 1};

    public static int[] PI_2 = {35, 38, 46, 6, 43, 40, 14, 45,
        33, 19, 26, 15, 23, 8, 22, 10,
        12, 11, 5, 25, 27, 21, 16, 31,
        28, 32, 34, 24, 9, 37, 2, 1};

    public static int[] P = {16, 7, 20, 21, 29, 12, 28, 17,
        1, 15, 23, 26, 5, 18, 31, 10,
        2, 8, 24, 14, 32, 27, 3, 9,
        19, 13, 30, 6, 22, 11, 4, 25};

    public static int[] PI_1 = {40, 8, 48, 16, 56, 24, 64, 32,
        39, 7, 47, 15, 55, 23, 63, 31,
        38, 6, 46, 14, 54, 22, 62, 30,
        37, 5, 45, 13, 53, 21, 61, 29,
        36, 4, 44, 12, 52, 20, 60, 28,
        35, 3, 43, 11, 51, 19, 59, 27,
        34, 2, 42, 10, 50, 18, 58, 26,
        33, 1, 41, 9, 49, 17, 57, 25};

    public static int[] SHIFT = {1, 1, 2, 2, 2, 2, 2, 2, 1, 2, 2, 2, 2, 2, 2, 1};

    //------------------------------------- Functions --------------------------------------
    public static boolean[] toBooleanArray(byte[] bytes) {
        BitSet bits = BitSet.valueOf(bytes);
        boolean[] bools = new boolean[bytes.length * 8];
        for (int i = bits.nextSetBit(0); i != -1; i = bits.nextSetBit(i + 1)) {
            bools[i] = true;
        }
        return bools;
    }

    public static byte[] toByteArray(boolean[] bools) {
        BitSet bits = new BitSet(bools.length);
        for (int i = 0; i < bools.length; i++) {
            if (bools[i]) {
                bits.set(i);
            }
        }

        byte[] bytes = bits.toByteArray();
        if (bytes.length * 8 >= bools.length) {
            return bytes;
        } else {
            return Arrays.copyOf(bytes, bools.length / 8 + (bools.length % 8 == 0 ? 0 : 1));
        }
    }

    public static void printBooleanArray(boolean[] arr, boolean flag) {
        for (boolean b : arr) {
            if (b) {
                System.out.print('1');
            } else {
                System.out.print('0');
            }
        }

        if (flag == true) {
            System.out.println("");
        }

    }

    public static void printBooleanArray(boolean[] arr) {
        for (boolean b : arr) {
            if (b) {
                System.out.print('1');
            } else {
                System.out.print('0');
            }
        }

        System.out.println("");
    }

    static boolean[] convertByOneFromString(String s) {
        boolean[] bArr = new boolean[s.length()];

        for (int i = 0; i < s.length(); i++) {
            char c = s.charAt(i);
            if (c == '0') {
                bArr[i] = false;
            } else {
                bArr[i] = true;
            }
        }

        return bArr;
    }

    public static boolean[] leftRotate(boolean arr[], int d) {
        /*Function to left rotate arr[] of size n by d*/
        int n = arr.length;
        for (int i = 0; i < d; i++) {
            arr = leftRotatebyOne(arr, n);
        }

        return arr;
    }

    private static boolean[] leftRotatebyOne(boolean arr[], int n) {
        int i;
        boolean temp;
        temp = arr[0];
        for (i = 0; i < n - 1; i++) {
            arr[i] = arr[i + 1];
        }
        arr[i] = temp;

        return arr;
    }

    public static boolean[] mergeBooleanArray(boolean[] left28bitsKeys, boolean[] right28bitsKeys) {
        int len = left28bitsKeys.length + right28bitsKeys.length;
        boolean[] arr = new boolean[len];
        System.arraycopy(left28bitsKeys, 0, arr, 0, left28bitsKeys.length);
        System.arraycopy(right28bitsKeys, 0, arr, left28bitsKeys.length, right28bitsKeys.length);
        return arr;
    }

    public static boolean XOR(boolean a, boolean b) {
        boolean ans;

        if (a == false && b == false) {
            ans = false;
        } else if (a == false && b == true) {
            ans = true;
        } else if (a == true && b == false) {
            ans = true;
        } else {
            ans = false;
        }

        return ans;
    }

    static boolean[] getBoolean(boolean booleanValue, int len) {
        boolean[] arr = new boolean[len];
        for(int i=0; i<arr.length; i++){
            arr[i] = booleanValue;
        }
        return arr;
    }
}
