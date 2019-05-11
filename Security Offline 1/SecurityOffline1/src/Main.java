
import des.DES_Runner;
 
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
        DES_Runner runner = new DES_Runner();
        runner.encrypt();
        runner.runDecryption();
    }



}
