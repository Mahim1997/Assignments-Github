

import java.io.RandomAccessFile;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.Collections;
import java.util.Comparator;
import java.util.HashSet;
import java.util.List;
import java.util.Set;

public class SubstitutionRunner {

    private final String fileName;

    private String INITIAL_CIPHER_TEXT;

    private String cipherText;
    private List<String> cipherText_Phases = new ArrayList<>();
    //mostFreqThreeChars[0] has the highest frequency, mostFreqThreeChars[1] has 2nd highest and so on
    private char[] mostFreqThreeChars = new char[3];
    private char ALPHA_CHAR = '@';
    private char[] mostFreqThreeCharsCipher = new char[3];

    private List<String> presentWords = new ArrayList<>();

    private CipherTextInfo[] cipherLettersInfo = new CipherTextInfo[26];

    private List<Mapping> mapping = new ArrayList<>();

    private String plainTextDecoded;

    public SubstitutionRunner(String file) {
//        this.fileName = "src/substitution/" + file;
        this.fileName = file;
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
//        Util.printMapping(this.mapping);
    }

    //------------------------------------------------------------------------------------------------------------------------
    private void runDecryption() {
        System.out.println("Inside decryption .... ");

//        printThings();
        gatherCipherTextInfo();

        this.cipherText_Phases.add(this.cipherText);

        //Phase 1 [Three most frequent letters map]
        changeCipherText_ThreeMostFreqLetters_Phase1();
        this.cipherText_Phases.add(this.cipherText);

        //Phase 2 [Pass 1 using letter distances]
        List<StringMap> givenLettersWithAlpha = tryLetterDistances();  //eg. atlantis experiment laboratory timeline
        //for at@a@t@@ e@@e@@@e@t @a@@@at@@@ t@@e@@@e 

        //Phase 3. Check for these @ symbols in the '1st phase decoded' cipher text
        changeCipherText_Phase2(givenLettersWithAlpha);

        //Print Solution Thus Far
        this.plainTextDecoded = this.cipherText;    //After iterations
        System.out.println("Plain Text is: \n" + this.plainTextDecoded);
        System.out.println("\nMapping is: ");
        Util.printMapping(mapping);

        //Phase 4. Encode
        encodeText();
        System.out.println("Encoded Text is: \n" + this.cipherText);

        //Accuracy
        int numMatchedChars = this.mapping.size();
        double acc = (numMatchedChars / 26.0) * 100;
        System.out.println("Accuracy (Number of Matched Chars)  = " + numMatchedChars + ", in Percent (Number of Matched Chars out of 26) = " + acc + "%");
    }

    private void encodeText() {
        String s = "";
        for (int i = 0; i < this.plainTextDecoded.length(); i++) {
            char c = this.plainTextDecoded.charAt(i);
            char mappedChar = c;
            for (int k = 0; k < this.mapping.size(); k++) {
                if (this.mapping.get(k).plainTextChar == c) {
                    mappedChar = this.mapping.get(k).cipherTextChar;
                }
            }
            s += mappedChar;
        }
        this.cipherText = s;
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

//        System.out.println(this.cipherText);
    }

    private int posOf_UsingPlainText(char c, List<Mapping> map) {
        for (int i = 0; i < map.size(); i++) {
            if (map.get(i).plainTextChar == c) {
                return i;
            }
        }
        Set<String> set = new HashSet<>();

        return -1;
    }

    private List<StringMap> tryLetterDistances() {
        List<StringMap> strMap = new ArrayList<>();
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
                    s += (ALPHA_CHAR);  //ALPHA_CHAR = '@'
                }
            }
            words_given_withMappedChars.add(s);

        }

        for (int i = 0; i < this.presentWords.size(); i++) {
            strMap.add(new StringMap(this.presentWords.get(i), words_given_withMappedChars.get(i)));
        }
        return strMap;
    }

    private void changeCipherText_Phase2(List<StringMap> givenLettersWithAlpha) {
        //eg. atlantis experiment laboratory timeline
        //for at@a@t@@ e@@e@@@e@t @a@@@at@@@ t@@e@@@e 
        List<StringMap> toDoString = sortWithRespectToFrequency(givenLettersWithAlpha);

//        System.out.println("\nAfter Sorting ..... ");
//        Util.printListStringMap(givenLettersWithAlpha);
        System.out.println("");

        checkCipherTextsAndMap(givenLettersWithAlpha);

    }

    private boolean isIn(char c) {
        for (int k = 0; k < mostFreqThreeChars.length; k++) {
            if (c == mostFreqThreeChars[k]) {
                return true;
            }
        }
        return false;
    }

    private List<StringMap> sortWithRespectToFrequency(List<StringMap> givenLettersWithAlpha) {

        Collections.sort(givenLettersWithAlpha, new Comparator<StringMap>() {
            @Override
            public int compare(StringMap o1, StringMap o2) {
                int cnt1 = 0, cnt2 = 0;
                for (int i = 0; i < o1.getStr1().length(); i++) {
                    char c = o1.getStr1().charAt(i);
                    if (isIn(c)) {
                        cnt1++;
                    }
                }
                for (int i = 0; i < o2.getStr1().length(); i++) {
                    char c = o2.getStr1().charAt(i);
                    if (isIn(c)) {
                        cnt2++;
                    }
                }
                if (cnt1 == cnt2) {
                    return (o2.getStr1().length() - o1.getStr1().length());
                } else {
                    return (cnt2 - cnt1);
                }
            }
        });

        return givenLettersWithAlpha;

    }

    private void addToMapping(char c_cipher, char c_word) {
        this.mapping.add(new Mapping(c_word, c_cipher));    //Aage plainText then CipherText

        Set<Mapping> targetSet = new HashSet<>(this.mapping);
        this.mapping = new ArrayList<>(targetSet);
    }

    private void replaceCipherText(int basePointer, StringMap wordWithAlpha) {
//        System.out.println("==>>Inside replaceCipherText , basePointer = " + basePointer + " , word = " + wordWithAlpha.getStr1());
        //First do the Mapping
//        if(basePointer == this.cipherText.length() - 1){
//            return ;
//        }
        for (int i = 0; i < wordWithAlpha.getStr1().length(); i++) {
            int pointer = basePointer + i;
            char c_cipher = this.cipherText.charAt(pointer);
            char c_word = wordWithAlpha.getStr1().charAt(i);
            if (c_cipher != c_word) {
                //Get the Mapping
                addToMapping(c_cipher, c_word);
//                System.out.println("After add to mapping, print mapping ... "); Util.printMapping(mapping);
            }

        }
        int len = wordWithAlpha.getStr1().length();
        //Now replace cipher text
//        System.out.println("Before replaceCipherWRTMapping ... printing Mapping ");
//        Util.printMapping(mapping);
        replaceCipherTextWithRespectToMapping();
//        System.out.println("After changin cipher, printin cipher ... ");
//        System.out.println(this.cipherText);
    }

    private Mapping findMapping(char capitalLetter) {
        for (int i = 0; i < this.mapping.size(); i++) {
            if (this.mapping.get(i).cipherTextChar == capitalLetter) {
                return this.mapping.get(i);
            }
        }
        return null;
    }

    private void replaceCipherTextWithRespectToMapping() {
        String str = "";
        for (int i = 0; i < this.cipherText.length(); i++) {
            char c = this.cipherText.charAt(i);
            if (isCapitalLetter(c)) {
                //Check if mapping exists
                Mapping m = findMapping(c);
                if (m != null) {
                    str += m.getPlainTextChar();

                } else {
                    str += c;
                }
            } else {
                str += c;
            }

        }

        this.cipherText_Phases.add(str);
        this.cipherText = str;
    }
//a = 97 , A = 65

    private boolean isInMap(char c) {
        for (int i = 0; i < this.mapping.size(); i++) {
            if (this.mapping.get(i).plainTextChar == c) {
                return true;
            }
        }
        return false;
    }

    private void checkCipherTextsAndMap(List<StringMap> givenLettersWithAlpha) {
        for (int iter = 0; iter < givenLettersWithAlpha.size(); iter++) {

            StringMap wordWithAlpha = givenLettersWithAlpha.get(iter); //eg.e@@e@@@e@
            int pos = replaceCipherPhase2PerWord(wordWithAlpha, cipherText);

            //Replace the cipher Text
            replaceCipherText(pos, wordWithAlpha);

            //update the list
            updateList(givenLettersWithAlpha);
        }
    }

    private void updateList(List<StringMap> givenLettersWithAlpha) {
        for (int iter = 0; iter < givenLettersWithAlpha.size(); iter++) {
            StringMap wa = givenLettersWithAlpha.get(iter);
            String s = "";
            String s1 = wa.getStr1();
            for (int i = 0; i < wa.getStr2().length(); i++) {
                char c1 = s1.charAt(i);
                if (isInMap(c1)) {
                    s += c1;
                } else {
                    s += ALPHA_CHAR;
                }
            }
            wa.setStr2(s);
        }
    }

    private int replaceCipherPhase2PerWord(StringMap wordWithAlpha, String cipherText) {
        //str1 is normal text, str2 is @ wala text

        int basePointerCipher = 0, nextCipher = 0, basePointerWord = 0;

        while (true) {
//            System.out.println("=>> Inside while loop condition , baseCipher = " + basePointerCipher + " , baseWord = " + basePointerWord + " , nextCipher = " + nextCipher);
            char cip = cipherText.charAt(nextCipher);
            char cword = wordWithAlpha.getStr2().charAt(basePointerWord);

            if (basePointerCipher == (this.cipherText.length() - 1)) {
                break;
            }
            if (nextCipher == (this.cipherText.length() - 1)) {
                break;
            }

            if (basePointerWord == (wordWithAlpha.getStr2().length() - 1)) {
                //Found the pattern matching
//                System.out.println("-->>>PATTERN MATCHING FOUND BASE Pointer = " + basePointerCipher + " , for word = " + wordWithAlpha.getStr2());
                break;
            } else {
                //Not found pattern matching still searching ..... 
//                System.out.println("Inside else condition , baseCipher = " + basePointerCipher + " , baseWord = " + basePointerWord + " , nextCipher = " + nextCipher
//                        + " , comparing cipherLetter = " + cip + " , plain word = " + cword + ", word = " + wordWithAlpha.getStr2());
                if (isSmallLetter(cip) && isSmallLetter(cword)) {
                    if (cip == cword) {
                        nextCipher++;
//                    wordPointerFirst++;
                        basePointerWord++;
                    } else {
                        basePointerWord = 0;
                        basePointerCipher++;
                        nextCipher = basePointerCipher;
                    }
                } else if (isCapitalLetter(cip) && isAlpha(cword)) {
                    nextCipher++;
                    basePointerWord++;
                } else {
                    //Restart
                    basePointerWord = 0;
                    basePointerCipher++;
                    nextCipher = basePointerCipher;
                }
            }

        }

        return basePointerCipher;
    }

    private boolean isSmallLetter(char c) {
        if (c >= 'a' && c <= 'z') {
            return true;
        }
        return false;
    }

    private boolean isCapitalLetter(char c) {
        if (c >= 'A' && c <= 'Z') {
            return true;
        }
        return false;
    }

    private boolean isAlpha(char cword) {
        return (cword == ALPHA_CHAR);
    }

}
