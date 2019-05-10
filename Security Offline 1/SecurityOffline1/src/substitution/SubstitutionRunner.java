package substitution;

import java.io.RandomAccessFile;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.Collections;
import java.util.Comparator;
import java.util.List;

public class SubstitutionRunner {

    private final String fileName;

    private String INITIAL_CIPHER_TEXT;

    private String cipherText;
    private List<String> cipherText_Phases = new ArrayList<>();
    //mostFreqThreeChars[0] has the highest frequency, mostFreqThreeChars[1] has 2nd highest and so on
    private char[] mostFreqThreeChars = new char[3];

    private char[] mostFreqThreeCharsCipher = new char[3];

    private List<String> presentWords = new ArrayList<>();

    private CipherTextInfo[] cipherLettersInfo = new CipherTextInfo[26];
    private List<Mapping> mapping = new ArrayList<>();

    private String plainTextDecoded;

    public SubstitutionRunner(String file) {
        this.fileName = "src/substitution/" + file;
    }

    public void run() {
        readInputs();
        runDecryption();

        System.out.println("\n\n");

    }

    private void readInputs() {
        String[] arrInputs = new String[3];
        try {

//            FileOutputStream fout = new FileOutputStream("HALALALO.txt");
            RandomAccessFile file = new RandomAccessFile(fileName, "r");
            String str;
            int cnt = 1;
            while ((str = file.readLine()) != null) {
                if ("\n".equals(str) || str.equals("")) {
                    continue;
                }
//                System.out.println(cnt + "-->>  " + "<" + str + ">");
                arrInputs[cnt - 1] = str;
                cnt++;
            }

            retrieveInputs(arrInputs);

            file.close();
        } catch (Exception ex) {
            ex.printStackTrace();
        }
    }

    private void retrieveInputs(String[] arrInputs) {
//        for (int i = 0; i < arrInputs.length; i++) {
//            System.out.println(i + " --> " + arrInputs[i]);
//        }
        this.cipherText = arrInputs[0];
        this.INITIAL_CIPHER_TEXT = this.cipherText; //for backup

        retrieveFrequentCharacters(arrInputs[1]);
        retrieveMostFrequentWords(arrInputs[2]);
    }

    private String[] splitAndTrim(String str, String regex) {
        String[] splitStrings = str.split(regex);
        for (int i = 0; i < splitStrings.length; i++) {
            splitStrings[i] = splitStrings[i].trim();
        }
        return splitStrings;
    }

    private void retrieveFrequentCharacters(String str) {
//        System.out.println("Most Freq Chars string is <" + str + ">");

        String[] splitStrings = splitAndTrim(str, ",");
        int cnt = 0;
        for (String x : splitStrings) {
            this.mostFreqThreeChars[cnt++] = x.charAt(0);
        }
//        for(char c: this.mostFreqThreeChars){
//            System.out.println(c);
//        }

    }

    private void retrieveMostFrequentWords(String str) {
//        System.out.println("Most Freq Words string is <" + str + ">");
        String[] splitStrings = splitAndTrim(str, ",");

        for (String x : splitStrings) {
            this.presentWords.add(x);
        }

//        for(String x: this.presentWords){
//            System.out.println("<" + x + ">");
//        }
    }

    private void printThings() {
        System.out.println("Cipher Text is:\n" + this.cipherText + "\n");
        System.out.print("Three most frequence chars are: ");
        Util.printArray(this.mostFreqThreeChars);
        System.out.print("\nPresent words are: ");
        Util.printList(this.presentWords);
        System.out.println("");

    }

    private void gatherCipherTextInfo() {
        int[] cipherLetterFreq = new int[26];
        for (int i = 0; i < this.cipherText.length(); i++) {
            char c = this.cipherText.charAt(i);

            int pos = (int) c - 'A';
            cipherLetterFreq[pos]++;

        }
//        printArray(this.cipherLetterFreq);

        for (int i = 0; i < cipherLetterFreq.length; i++) {
            char c = (char) ('A' + i);
            this.cipherLettersInfo[i] = new CipherTextInfo(c, cipherLetterFreq[i]);
        }

        Arrays.sort(cipherLettersInfo, new Comparator<CipherTextInfo>() {

            @Override
            public int compare(CipherTextInfo o1, CipherTextInfo o2) {
                return (o2.getFrequency() - o1.getFrequency());   //o2.f - o1.f is the descending order
            }
        });

//        Util.printArray(this.cipherLettersInfo);    //descending order DONE
        for (int i = 0; i < 3; i++) {
            this.mostFreqThreeCharsCipher[i] = this.cipherLettersInfo[i].getCipheredLetter();
        }

        //First Three Characters Mapping 
        for (int i = 0; i < 3; i++) {
            this.mapping.add(new Mapping(this.mostFreqThreeChars[i], this.mostFreqThreeCharsCipher[i]));
        }
        Util.printMapping(this.mapping);
    }

    //------------------------------------------------------------------------------------------------------------------------
    private void runDecryption() {
        System.out.println("Inside decryption .... ");

        printThings();

        gatherCipherTextInfo();

        this.cipherText_Phases.add(this.cipherText);

        //Phase 1 [Three most frequent letters map]
        changeCipherText_ThreeMostFreqLetters_Phase1();
        this.cipherText_Phases.add(this.cipherText);

        //Phase 2 [Pass 1 using letter distances]
        List<String> givenLettersWithAlpha = tryLetterDistances();  //eg. atlantis experiment laboratory timeline
        //for at@a@t@@ e@@e@@@e@t @a@@@at@@@ t@@e@@@e 

        //Phase 3. Check for these @ symbols in the '1st phase decoded' cipher text
        changeCipherText_Phase2(givenLettersWithAlpha);
    }

    //------------------------------------------------------------------------------------------------------------------------
    private int whereIsIn(char c, List<Mapping> map) {
        for (int i = 0; i < map.size(); i++) {
            char cT = map.get(i).cipherTextChar;
            if (c == cT) {
                return i;
            }
        }
        return -1;
    }

    private void changeCipherText_ThreeMostFreqLetters_Phase1() {
        String str = "";
        for (int i = 0; i < this.cipherText.length(); i++) {
            char c = this.cipherText.charAt(i);
            int indexInMap = whereIsIn(c, this.mapping);

            if (indexInMap != -1) {
                char newChar = this.mapping.get(indexInMap).plainTextChar;
                str += newChar;
            } else {
                str += c;
            }
        }
        this.cipherText = str;

        System.out.println(this.cipherText);
    }

    private int posOf_UsingPlainText(char c, List<Mapping> map) {
        for (int i = 0; i < map.size(); i++) {
            if (map.get(i).plainTextChar == c) {
                return i;
            }
        }
        return -1;
    }

    private List<String> tryLetterDistances() {
        List<String> words_given_withMappedChars = new ArrayList<>();
        for (int i = 0; i < this.presentWords.size(); i++) {
            String word = this.presentWords.get(i);

            String s = "";
            for (int j = 0; j < word.length(); j++) {
                char c = word.charAt(j);
                int pos = posOf_UsingPlainText(c, mapping);
                if (pos != -1) {
                    //EXISTS
                    s += c;
                } else {
                    s += "@";
                }
            }
            words_given_withMappedChars.add(s);

        }
        Util.printList(words_given_withMappedChars);
        return words_given_withMappedChars;
    }

    private void changeCipherText_Phase2(List<String> givenLettersWithAlpha) {
        //eg. atlantis experiment laboratory timeline
        //for at@a@t@@ e@@e@@@e@t @a@@@at@@@ t@@e@@@e 
        List<String> toDoString = sortWithRespectToFrequency(givenLettersWithAlpha);

    }

    private boolean isIn(char c) {
        for (int k = 0; k < mostFreqThreeChars.length; k++) {
            if (c == mostFreqThreeChars[k]) {
                return true;
            }
        }
        return false;
    }

    private List<String> sortWithRespectToFrequency(List<String> givenLettersWithAlpha) {
        System.out.println("\nInside bla bla bla");

        //Sort wrt NUMBER OF FREQ LETTERS
        /*Collections.sort(givenLettersWithAlpha, new Comparator<String>() {
            @Override
            public int compare(String o1, String o2) {
                int cnt1 = 0, cnt2 = 0;
                for (int i = 0; i < o1.length(); i++) {
                    char c = o1.charAt(i);
                    if (isIn(c)) {
                        cnt1++;
                    }
                }
                for (int i = 0; i < o2.length(); i++) {
                    char c = o2.charAt(i);
                    if (isIn(c)) {
                        cnt2++;
                    }
                }
//                System.out.println("Cnt1 = " + cnt1 + " for str1 = " + o1 + ", AND Cnt2 = " + cnt2 + " for str2 = " + o2);
                return (cnt2 - cnt1);
            }
        });
        */
        Util.printList(givenLettersWithAlpha);
        System.out.println("");
        //Sort wrt Length
        Collections.sort(givenLettersWithAlpha, new Comparator<String>() {
            @Override
            public int compare(String o1, String o2) {
                int cnt1 = 0, cnt2 = 0;
                for (int i = 0; i < o1.length(); i++) {
                    char c = o1.charAt(i);
                    if (isIn(c)) {
                        cnt1++;
                    }
                }
                for (int i = 0; i < o2.length(); i++) {
                    char c = o2.charAt(i);
                    if (isIn(c)) {
                        cnt2++;
                    }
                }
//                System.out.println("Cnt1 = " + cnt1 + " for str1 = " + o1 + ", AND Cnt2 = " + cnt2 + " for str2 = " + o2);
                if (cnt1 == cnt2) {
                    return (o2.length() - o1.length());
                } else {
                    return (cnt2 - cnt1);
                }
            }
        });
        Util.printList(givenLettersWithAlpha);

        return null;

    }

}
