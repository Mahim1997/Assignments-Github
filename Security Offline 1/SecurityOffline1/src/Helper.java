

import java.util.List;

public class Helper {

    public static int SHORTENED_KEY_LEN = 56;
    public static int FULL_KEY_LEN = 64;
    public static char SPECIAL_CHARACTER = '@';
    
    
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
        for (int i = 0; i < arr.length; i++) {
            arr[i] = booleanValue;
        }
        return arr;
    }

    public static boolean[] getNumBits(boolean[] array, int leftZeroRightOne, int numBits) {
        boolean[] bArr = new boolean[numBits];
        if (leftZeroRightOne == 0) {
            //left 32 bits
            System.arraycopy(array, 0, bArr, 0, numBits);
        } else {
            //right 32 bits
            for (int i = numBits; i < array.length; i++) {
                bArr[i - numBits] = array[i];
            }
        }
        return bArr;
    }

    static void printStringArray(String[] s_arr) {
        for (String s : s_arr) {
            System.out.println(s);
        }
    }

    static void printIntegerArray(int[] asciiValues) {
        for (int x : asciiValues) {
            System.out.print(x + " ");
        }
        System.out.println("");
    }

    private static String padZeroBefore(String s, int howManyBits) {
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

    public static boolean[] getBits_InBooleanArray(String str) {
        boolean[] b_arr = new boolean[str.length()];
        String resultantBinString = "";
        for (int i = 0; i < str.length(); i++) {
            char c = str.charAt(i);
            int ascii = (int) c;
            String binString = Integer.toBinaryString(ascii);
            //Each character 1 byte i.e. 8 bits so pad pervious with 0's
            binString = Helper.padZeroBefore(binString, 8);

            //Append to String
            resultantBinString += binString;
        }
        b_arr = Helper.convertByOneFromString(resultantBinString);

        return b_arr;
    }

    public static String padString(String s) {
        String str = s;
        int len = s.length();
        if (len % 8 != 0) {
            int remaining = 8 - (len % 8);
            for (int i = 0; i < remaining; i++) {
                str += SPECIAL_CHARACTER;
            }
        }
        return str;

    }

    public static String getString_AsAWhole(boolean[] booleanArray) {
        //64 bits 
        String s = getStringFromBoolArray(booleanArray);
        String[] s_arr = getStringArray(s);

//        System.out.println("\n\n\n\nBLA BLA BLA string array is : ");
//        Helper.printStringArray(s_arr);
        int[] asciiValues = new int[8];
        for (int i = 0; i < asciiValues.length; i++) {
            asciiValues[i] = Integer.parseInt(s_arr[i], 2);
        }

//        System.out.println("Now printing int array: ");
//        Helper.printIntegerArray(asciiValues);
        char[] asciiChar = new char[8];
        for (int i = 0; i < 8; i++) {
            asciiChar[i] = (char) asciiValues[i];
        }

        String str = "";
        for (int i = 0; i < 8; i++) {
            str += asciiChar[i];
        }
//        System.out.println("DECODED TEXT IS :\n" + str);
        return str;
    }

    private static String getStringFromBoolArray(boolean[] booleanArray) {
        String s = "";
        for (int i = 0; i < booleanArray.length; i++) {
            boolean b = booleanArray[i];
            if (b == false) {
                s += '0';
            } else {
                s += '1';
            }
        }
        return s;
    }

    private static String[] getStringArray(String s) {
        String[] arr = new String[s.length() / 8];
        for (int i = 0; i < arr.length; i++) {
            String str = "";
            for (int j = 0; j < 8; j++) {
                str += s.charAt(i * 8 + j);
            }
            arr[i] = str;
        }
        return arr;
    }

    public static void printListString(List<String> list) {
        list.forEach((x) -> {
            System.out.println(x);
        });
    }

    public static String removePadding(String plainTextBack) {
        String str = "";
        for(int i=0; i<plainTextBack.length(); i++){
            if(plainTextBack.charAt(i) == SPECIAL_CHARACTER){
                break;
            }
            str += plainTextBack.charAt(i);
        }
        return str;
    }
}
