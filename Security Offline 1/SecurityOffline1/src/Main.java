
import des.DES_Runner;
import des.Helper;
import java.io.RandomAccessFile;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;

import substitution.SubstitutionRunner;

public class Main {

    static int SUBSTITUTION = 0;
    static int DES = 1;

    static int mode = DES;

    public static void main(String[] args) {

        if (mode == SUBSTITUTION) {
            runSubstitution();
        } else {
            runDES();
        }

    }

    public static void runSubstitution() {
        SubstitutionRunner runner = new SubstitutionRunner("substitution-22.txt");
        runner.run();
    }

    public static void runDES() {

        String[] ins = Main.readInputs("src/des/input_des.txt");
        String inputWholePlainText = ins[0];
        String inputKey = ins[1];


        List<String> characterStrings64bits = divideBy64bits(inputWholePlainText);
        int howMany = characterStrings64bits.size();
        DES_Runner[] runners = new DES_Runner[howMany]; //Each working on 64bit block/chunk

        //Initialise each
        for(int i=0; i<runners.length; i++){
            runners[i] = new DES_Runner();
        }
        
        
        String cipheredText = "";
        for (int i = 0; i < characterStrings64bits.size(); i++) {
            String input = characterStrings64bits.get(i);
            runners[i].setParameters(input, inputKey);
            runners[i].encrypt();
            cipheredText += runners[i].getCipheredTextFinalOutput();
        }

//        DES_Runner runner = new DES_Runner();
//        runner.encrypt();
//        runner.runDecryption();


        //DECIPHERING or DECRYPTING
        List<String> ciphered64bitsSeparated = divideBy64bits(cipheredText);
        DES_Runner decryptor = new DES_Runner();
        
        String plainTextBack = "";
        
        for(int i=0; i<ciphered64bitsSeparated.size(); i++){
            String input = ciphered64bitsSeparated.get(i);
            runners[i].runDecryption();
            plainTextBack += runners[i].plainTextDecrypted;
        }
        
        System.out.println("\n\n\nIn Main, After Encryption, Ciphered Text is: ");
        System.out.println(cipheredText);
        System.out.println("\nIn Main, After Decryption, Plain Text is: ");
        plainTextBack = Helper.removePadding(plainTextBack);
        System.out.println(plainTextBack);
    }

    private static String[] readInputs(String fileName) {
        String[] arrInputs = new String[2];
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

            file.close();
        } catch (Exception ex) {
            ex.printStackTrace();
        }
        return arrInputs;
    }

    private static List<String> divideBy64bits(String inputPlainText) {
        List<String> list = new ArrayList<>();

        String paddedInput = Helper.padString(inputPlainText);

        int cnt = 0;
        String str = "";
        for (int i = 0; i < paddedInput.length(); i++) {
            cnt++;
            str += paddedInput.charAt(i);
            if (cnt == 8) {
                list.add(str);
                str = "";
                cnt = 0;
            }
        }
        return list;
    }

}
