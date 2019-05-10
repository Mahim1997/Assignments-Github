package substitution;

import java.util.List;

public class Util {

    public static void printArray(int[] mostFreqThreeChars) {
        for (int i = 0; i < mostFreqThreeChars.length; i++) {
            System.out.print(mostFreqThreeChars[i] + " ");
        }
    }

    public static void printArray(char[] mostFreqThreeChars) {
        for (int i = 0; i < mostFreqThreeChars.length; i++) {
            System.out.print(mostFreqThreeChars[i] + " ");
        }
    }

    public static void printList(List<String> list) {
        for (String x : list) {
            System.out.print(x + " ");
        }
    }

    public static void printArray(CipherTextInfo[] cipherLettersInfo) {
        for (CipherTextInfo x : cipherLettersInfo) {
            System.out.print(x.getCipheredLetter() + "," + x.getFrequency() + " ");
        }
        System.out.println("");
    }

    public static void printMapping(List<Mapping> list) {
        for(Mapping m: list){
            System.out.print(m.plainTextChar + "<->" + m.cipherTextChar + "  ");
        }
        System.out.println("");
    }
}
