package muncala;

import muncala.interfaces.Helper;
import muncala.interfaces.Player1_Weights;
import muncala.interfaces.Player2_Weights;
import muncala.simulator.Simulator;
import muncala.strategies.Strategies;

public class Main {

    public static boolean DEBUG_MODE = false;
    public static boolean PRINT_THINGS = false;
    public static boolean PRINT_NEW = false;
    public static boolean PRINT_ONLY_IDX = true;
    public static boolean RUN_TEST = false;

    public static int TEST_CUT_DEPTH = 4;
    public static boolean DEPTH_RAND;
    public static int CUT_DEPTH = 12;
    
    
    public static void main(String[] args) {
//        rand();
        Simulator.NUMBER_OF_SIMULATIONS = 50;
        Simulator.TAKE_RAND_WEIGHTS = true;
        if (RUN_TEST) {
            Test.mainTestFunction();
        } else {
            fullRun();
        }
    }

    private static void fullRun() {
        
        Helper.eraseOutput();
        
        Main.DEPTH_RAND = false;
//        Simulator.chooseHeuristics(1, 1);
        Simulator.simulateFull(Strategies.ALPHA_BETA_PRUNING);
//        Simulator.simulateFull(Strategies.ALPHA_BETA_PRUNING);

        Helper.writeOutput("\n==========================================================\n");
    }

    static int getRandom(int min, int max) {
        int x;
        do {
            x = (int) (Math.random() * max);
        } while (x < min);
        return x;
    }

    private static void rand() {
        for (int i = 0; i < 1000; i++) {
            int x = getRandom(10, 20);
            System.out.println(x);
        }
    }

}
