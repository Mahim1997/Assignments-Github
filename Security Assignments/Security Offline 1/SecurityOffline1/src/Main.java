 
import java.io.RandomAccessFile;
import java.util.ArrayList;
import java.util.List;

 

public class Main {

    public static String input_DES = "src/input_des.txt";
    public static String input_Substitution = "src/substitution-22.txt";
    
    static int SUBSTITUTION = 0;
    static int DES = 1;

    static int mode = DES;
//    static int mode = SUBSTITUTION;
    
    public static void main(String[] args) {

        if (mode == SUBSTITUTION) {
            runSubstitution();
        } else {
            runDES();
        }

    }

    public static void runSubstitution() {
        SubstitutionRunner runner = new SubstitutionRunner(Main.input_Substitution);
        runner.run();
    }

    public static void runDES() {

//        String[] ins = Main.readInputs("src/des/input_des.txt");
        String[] ins = Main.readInputs(Main.input_DES);
        String inputWholePlainText = ins[0];
        String inputKey = ins[1];


        List<String> characterStrings64bits = divideBy64bits(inputWholePlainText);
        int howMany = characterStrings64bits.size();
        DES_Encryptor[] runners = new DES_Encryptor[howMany]; //Each working on 64bit block/chunk

        //Initialise each
        for(int i=0; i<runners.length; i++){
            runners[i] = new DES_Encryptor();
        }
        
        
        String cipheredText = "";
        for (int i = 0; i < characterStrings64bits.size(); i++) {
            String input = characterStrings64bits.get(i);
            runners[i].setParameters(input, inputKey);
            runners[i].encrypt();
            cipheredText += runners[i].getCipheredTextFinalOutput();
        }

//        DES_Encryptor runner = new DES_Encryptor();
//        runner.encrypt();
//        runner.runDecryption();


        //DECIPHERING or DECRYPTING
        List<String> ciphered64bitsSeparated = divideBy64bits(cipheredText);
        DES_Decryptor [] decryptors = new DES_Decryptor[howMany];
        for(int i=0; i<decryptors.length; i++){
            decryptors[i] = new DES_Decryptor(inputKey);
        }
        String plainTextBack = "";
        
        for(int i=0; i<ciphered64bitsSeparated.size(); i++){
            String input = ciphered64bitsSeparated.get(i);
            decryptors[i].runDecryption(input);
            plainTextBack += decryptors[i].plainTextDecrypted;
        }
        
        System.out.println("\n\n\n");
        System.out.println("-----------------------------------------------------------------\n");
        System.out.println("<In Main, After Encryption, Ciphered Text is:>");
        System.out.println(cipheredText);
        System.out.println("\n<In Main, After Decryption, Plain Text is:>");
        plainTextBack = Helper.removePadding(plainTextBack);
        System.out.println(plainTextBack);
        System.out.println("\n\n");
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
