package muncala.simulator;

import java.util.Scanner;
import muncala.*;

import muncala.interfaces.Helper;
import muncala.interfaces.Heuristics;
import muncala.interfaces.Util;
import static muncala.interfaces.Util.*;
import muncala.strategies.AlphaBeta;
import muncala.strategies.Strategies;

public class Simulator {

    private GameState state;

    public Simulator() {
        this.state = GameState.getInitialState();
    }

    public void runSimulator_AB_IterativeDeepening() {
        Strategies maxPlayer = new AlphaBeta(PLAYER_1);
        Strategies minPlayer = new AlphaBeta(PLAYER_2);
        state = GameState.getInitialState();

        AB_ID_MAX abMax = new AB_ID_MAX(state, maxPlayer);
        AB_ID_MIN abMin = new AB_ID_MIN(state, minPlayer);
        int bestIdx = -1;
        System.out.println("Playing with player 1 heurisitic " + Heuristics.HEURISTIC_CHOOSER_BOT_1 +
                " , player 2 h = " + Heuristics.HEURISTIC_CHOOSER_BOT_2 + " timeout(s) = " + Util.TIMEOUT_SECONDS + " ... ");
        while (true) {
            if (state.isGameOver()) {
                state.calculateScore();
                System.out.println("->Game OVER!! Winner : " + state.winner);
                break;
            }
            
//            System.out.println("Inside while loop ... ");
            if (state.getWhoseTurn().equals(PLAYER_1)) {
                long startTime = System.currentTimeMillis();
                
                abMax.setState(state);
                bestIdx = abMax.runIterativeDeepening();

                long execTime = System.currentTimeMillis() - startTime;
                System.out.println("-> Best idx of MaxPlayer = " + bestIdx + " , time to think = " + execTime);
                state.move_By_Changing_State_with_idx(bestIdx);
                state.printBoard();
            } else if (state.getWhoseTurn().equals(PLAYER_2)) {
                long startTime = System.currentTimeMillis();

                abMin.setState(state);
                bestIdx = abMin.runIterativeDeepening();
                
                long execTime = System.currentTimeMillis() - startTime;
                System.out.println("==>> Best idx of MinPlayer = " + bestIdx + " , time to think = " + execTime);
                state.move_By_Changing_State_with_idx(bestIdx);
                state.printBoard();
            }
        }

    }

    public void runSimulator_AlphaBeta() {
        System.out.println("-->>> Running runSimulator_AlphaBeta(),, simulator with depth cut off = " + Util.CUT_OFF_DEPTH);
//        MiniMax_PlayerMax 
//        MiniMax_MinPlayers
        AlphaBeta_Max maxPlayer = new AlphaBeta_Max();
        AlphaBeta_Min minPlayer = new AlphaBeta_Min();

        state = GameState.getInitialState();
        int bestIdx = -1;
        int test_depth = Util.CUT_OFF_DEPTH;
        while (true) {
            if (state.isGameOver()) {
                System.out.println("->Game OVER!! Winner : " + state.winner);
                break;
            }
            System.out.println("Inside while loop ... ");
            if (state.getWhoseTurn().equals(PLAYER_1)) {
                long startTime = System.currentTimeMillis();
                bestIdx = maxPlayer.getBestMove(state, test_depth);
                long execTime = System.currentTimeMillis() - startTime;
                System.out.println("Best idx of MaxPlayer = " + bestIdx + " , time to think = " + execTime);
                state.move_By_Changing_State_with_idx(bestIdx);
                state.printBoard();
            } else if (state.getWhoseTurn().equals(PLAYER_2)) {
                long startTime = System.currentTimeMillis();
                bestIdx = minPlayer.getBestMove(state, test_depth);
                long execTime = System.currentTimeMillis() - startTime;
                System.out.println("Best idx of MinPlayer = " + bestIdx + " , time to think = " + execTime);
                state.move_By_Changing_State_with_idx(bestIdx);
                state.printBoard();
            }
        }

    }

    public void runSimulator_MinMax() {
        System.out.println("-->>> Running min max simulator with depth cut off = " + Util.CUT_OFF_DEPTH);
//        MiniMax_PlayerMax 
//        MiniMax_MinPlayers
        MiniMax_PlayerMax maxPlayer = new MiniMax_PlayerMax();
        MiniMax_MinPlayer minPlayer = new MiniMax_MinPlayer();

        state = GameState.getInitialState();
        int bestIdx = -1;
        int test_depth = Util.CUT_OFF_DEPTH;
        while (true) {
            if (state.isGameOver()) {
                System.out.println("->Game OVER!! Winner : " + state.winner);
                break;
            }
            System.out.println("Inside while loop ... ");
            if (state.getWhoseTurn().equals(PLAYER_1)) {
                long startTime = System.currentTimeMillis();
                bestIdx = maxPlayer.getBestMove(state, test_depth);
                long execTime = System.currentTimeMillis() - startTime;
                System.out.println("Best idx of MaxPlayer = " + bestIdx + " , time to think = " + execTime);
                state.move_By_Changing_State_with_idx(bestIdx);
                state.printBoard();
            } else if (state.getWhoseTurn().equals(PLAYER_2)) {
                long startTime = System.currentTimeMillis();
                bestIdx = minPlayer.getBestMove(state, test_depth);
                long execTime = System.currentTimeMillis() - startTime;
                System.out.println("Best idx of MinPlayer = " + bestIdx + " , time to think = " + execTime);
                state.move_By_Changing_State_with_idx(bestIdx);
                state.printBoard();
            }
        }

    }
///-------------------------------------------------------------------------------------------------------------------------
    private void runConsoleSimulator() {
        System.out.println("-->>Inside Run Console Simulator");
        Helper.writeOutput("Printing initial Board State");
        state.printBoard();
        state.setTurn(PLAYER_1);
        Scanner sc = new Scanner(System.in);
        int x = -1;
        do {
            System.out.println("Current turn of: " + state.getWhoseTurn() + " , Enter idx to move: ");
            x = sc.nextInt();
            state.move_By_Changing_State_with_idx(x);
        } while (x != -1);

    }
}
