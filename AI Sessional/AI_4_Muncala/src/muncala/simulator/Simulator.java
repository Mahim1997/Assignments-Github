package muncala.simulator;

import muncala.*;
import static muncala.Main.DEPTH_RAND;

import static muncala.interfaces.Heuristics.*;
import muncala.interfaces.*;
import muncala.interfaces.Util;
import static muncala.interfaces.Util.*;

import muncala.strategies.A_B_IterativeDeepening;
import muncala.strategies.AlphaBeta;
import muncala.strategies.MiniMax;
import muncala.strategies.Strategies;

public class Simulator {

    static int min = 10;
    static int max = 20;
    static String firstRun = PLAYER_1;
    public static int NUMBER_OF_SIMULATIONS = 10;
    public static boolean TAKE_RAND_WEIGHTS = true;

    private static int getRandom() {
        int x;
        do {
            x = (int) (Math.random() * max);
        } while (x < min);
        return x;
    }

    private static int getRandom(int min, int max) {
        int x;
        do {
            x = (int) (Math.random() * max);
        } while (x < min);
        return x;
    }

    static void randPlayer1() {
        Player1_Weights.W1_h2 = getRandom(10, 20);
        Player1_Weights.W2_h2 = getRandom(10, 20);

        Player1_Weights.W1_h3 = getRandom();
        Player1_Weights.W2_h3 = getRandom();
        Player1_Weights.W3_h3 = getRandom();

        Player1_Weights.W1_h4 = getRandom();
        Player1_Weights.W2_h4 = getRandom();
        Player1_Weights.W3_h4 = getRandom();
        Player1_Weights.W4_h4 = getRandom();
    }

    static void randPlayer2() {
        Player2_Weights.W1_h2 = getRandom();
        Player2_Weights.W2_h2 = getRandom();

        Player2_Weights.W1_h3 = getRandom();
        Player2_Weights.W2_h3 = getRandom();
        Player2_Weights.W3_h3 = getRandom();

        Player2_Weights.W1_h4 = getRandom(20, 40);
        Player2_Weights.W2_h4 = getRandom(20, 40);
        Player2_Weights.W3_h4 = getRandom(20, 40);
        Player2_Weights.W4_h4 = getRandom(20, 40);
    }

    static void randomWeights() {
        randPlayer1();
        randPlayer2();
    }

    static void printWeights() {
        Helper.noNewLineWriteOutput("Player 1 Heuristic Weights: ");
        switch (HEURISTIC_CHOOSER_BOT_1) {
            case 2:
                Helper.writeOutput("W1 = " + Player1_Weights.W1_h2 + " W2 = " + Player1_Weights.W2_h2);
                break;
            case 3:
                Helper.writeOutput("W1 = " + Player1_Weights.W1_h3 + " W2 = " + Player1_Weights.W2_h3 + " W3 = " + Player1_Weights.W3_h3);
                break;
            case 4:
                Helper.writeOutput("W1 = " + Player1_Weights.W1_h4 + " W2 = " + Player1_Weights.W2_h4 + " W3 = " + Player1_Weights.W3_h4 + " W4 = " + Player1_Weights.W4_h4);
                break;
            default:
                Helper.writeOutput("");
                break;
        }
        Helper.noNewLineWriteOutput("Player 2 Heuristic Weights: ");
        switch (HEURISTIC_CHOOSER_BOT_2) {
            case 2:
                Helper.writeOutput("W1 = " + Player2_Weights.W1_h2 + " W2 = " + Player2_Weights.W2_h2);
                break;
            case 3:
                Helper.writeOutput("W1 = " + Player2_Weights.W1_h3 + " W2 = " + Player2_Weights.W2_h3 + " W3 = " + Player2_Weights.W3_h3);
                break;
            case 4:
                Helper.writeOutput("W1 = " + Player2_Weights.W1_h4 + " W2 = " + Player2_Weights.W2_h4 + " W3 = " + Player2_Weights.W3_h4 + " W4 = " + Player2_Weights.W4_h4);
                break;
            default:
                Helper.writeOutput("");
                break;
        }
    }

    public static void simulateFull(String whichStrategy) {

        Strategies st = null;
        String firstTurn = PLAYER_2;
        if (whichStrategy.equals(Strategies.ALPHA_BETA_ITERATIVE_DEEPENING)) {
            st = new A_B_IterativeDeepening(firstTurn);
            System.out.println("Option 1");
            simulate(st);
        } else if (whichStrategy.equals(Strategies.ALPHA_BETA_PRUNING)) {
            st = new AlphaBeta(firstTurn);
            System.out.println("Option 2");
            simulate(st);
        } else if (whichStrategy.equals(Strategies.MINI_MAX)) {
            st = new MiniMax(firstTurn);
            System.out.println("Option 3");
            simulate(st);
        }
    }
//    public static void simulateABIterative() {

    public static void chooseHeuristics(int mode1, int mode2) {
        HEURISTIC_CHOOSER_BOT_1 = mode1;
        HEURISTIC_CHOOSER_BOT_2 = mode2;
    }

    public static void simulate(Strategies strategy) {

//        Strategies maxPlayer = new A_B_IterativeDeepening(PLAYER_1);
        Helper.writeOutput("\n==========================================================\n");

        int winsPlayer1 = 0, winsPlayer2 = 0, tied = 0;

        for (int i = 1; i <= NUMBER_OF_SIMULATIONS; i++) {
            Helper.writeOutput("\n---------------------Iter = " + i + " begins --------------------------\n");
            if (i <= 10) {
                if (TAKE_RAND_WEIGHTS == true && DEPTH_RAND == false) {
                    randomWeights();
                } else if (DEPTH_RAND == true) {
                    //random depth
                    Main.TEST_CUT_DEPTH = i;
                    Player1_Weights.CUT_OFF_DEPTH_Player_1 = Main.TEST_CUT_DEPTH;
                    Player2_Weights.CUT_OFF_DEPTH_Player_2 = Main.TEST_CUT_DEPTH;
                }
            }else{
                if(TAKE_RAND_WEIGHTS){
                    randomWeights();
                }
            }
            printWeights();
            Helper.writeOutput("Running with Strategy: " + strategy.getStrategyName() + " Time Limit(s) = " + Util.TIMEOUT_SECONDS
                    + " ,(without Iterative) Depth limit for player 1: " + Player1_Weights.CUT_OFF_DEPTH_Player_1 + " , For Player 2: " + Player2_Weights.CUT_OFF_DEPTH_Player_2 + "\n");
            Helper.writeOutput("Player 1 heuristic " + HEURISTIC_CHOOSER_BOT_1 + " and Player 2 heuristic " + HEURISTIC_CHOOSER_BOT_2);

            String winner = null;
            Strategies maxPlayer = null, minPlayer = null;
            if (strategy.getStrategyName().equals(Strategies.ALPHA_BETA_ITERATIVE_DEEPENING)) {
                maxPlayer = new A_B_IterativeDeepening(PLAYER_1);
                minPlayer = new A_B_IterativeDeepening(PLAYER_2);
            } else if (strategy.getStrategyName().equals(Strategies.ALPHA_BETA_PRUNING)) {
                maxPlayer = new AlphaBeta(PLAYER_1);
                minPlayer = new AlphaBeta(PLAYER_2);
            } else if (strategy.getStrategyName().equals(Strategies.MINI_MAX)) {
                maxPlayer = new MiniMax(PLAYER_1);
                minPlayer = new MiniMax(PLAYER_2);
            }
            winner = runOnce(maxPlayer, minPlayer);
//            runOnce();
            switch (winner) {
                case PLAYER_1:
                    winsPlayer1++;
                    break;
                case PLAYER_2:
                    winsPlayer2++;
                    break;
                default:
                    tied++;
                    break;
            }
            Helper.writeOutput("Winner: " + winner);
        }
        Helper.writeOutput("\n\n--------------------------------------------------------\n");
        Helper.writeOutput("Finally , number of wins for player 1 = " + winsPlayer1 + " , for player 2 = " + winsPlayer2 + " tied = " + tied);
        int total = winsPlayer1 + winsPlayer2 + tied;
        float percent1 = ((float) winsPlayer1 / (float) total) * 100;
        float percent2 = ((float) winsPlayer2 / (float) total) * 100;
        float percentTie = ((float) tied / (float) total) * 100;
        Helper.writeOutput("Percentage Winnings : Player 1 =  " + percent1 + "% , Player 2 = " + percent2 + "% , Tie = " + percentTie + " %");

    }

    public static String runOnce(Strategies strategyMax, Strategies strategyMin) {
//        Strategies maxPlayer = new A_B_IterativeDeepening(PLAYER_1);
//        Strategies minPlayer = new A_B_IterativeDeepening(PLAYER_2);
        int numMovesDone = 0;
        Strategies maxPlayer = strategyMax;
        Strategies minPlayer = strategyMin;
        GameState state = GameState.getInitialState();
        state.setTurn(Simulator.firstRun);
        int bestIdx = -1;
        do {
            if (numMovesDone == Main.TEST_CUT_DEPTH && Main.DEBUG_MODE) {
                break;
            }
            numMovesDone++;
            if (state.getWhoseTurn().equals(PLAYER_1)) {
//                System.out.println("-->>NOW TURN OF PLAYER 1");
                maxPlayer.setGameState(state);
                bestIdx = maxPlayer.getBestMove();
//                System.out.println("==>> Best idx of MaxPlayer = " + bestIdx);
            } else if (state.getWhoseTurn().equals(PLAYER_2)) {
//                System.out.println("-->>NOW TURN OF PLAYER 2");
                minPlayer.setGameState(state);
                bestIdx = minPlayer.getBestMove();
//                System.out.println("==>> Best idx of MinPlayer = " + bestIdx);
            }
//            System.out.println("\n==================CHANGING BOARD===============\n");
            state.move_By_Changing_State_with_idx(bestIdx);
//            state.printBoard();
            state.calculateScore();
//            System.out.println("\n======================================================\n");
//            GameState newState = state.makeMove(bestIdx);
//            newState.printBoard();
//            state.changeState(state);
        } while (state.isGameOver() == false);
        state.calculateScore();
        Helper.writeOutput("Player 1 Score : " + state.PLAYER1_SCORE + " , Player 2 Score: " + state.PLAYER2_SCORE);
        return state.winner;
    }

    public static void simulateDepth(String ALPHA_BETA_PRUNING) {

    }

}
