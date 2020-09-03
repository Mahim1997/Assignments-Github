package muncala.algorithms;

import java.util.logging.Level;
import java.util.logging.Logger;
import muncala.GameState;

import muncala.interfaces.Heuristics;
import muncala.interfaces.Util;
import static muncala.interfaces.Util.PLAYER_1;
import static muncala.interfaces.Util.PLAYER_2;

public class AlphaBeta_Max extends MiniMax_PlayerMax {

    @Override
    public int getBestMove(GameState state, int depth) {
//        System.out.println("-> ... AlphaBeta_Max running for depth = " + depth);
        if (depth <= 0) {
            //System.out.println("-->>In getBestMove Alpha Beta() of AlphaBeta_PlayerMax ... depth given is <= 0 ... return -1");
            return -1;
        }

        long startTime = System.currentTimeMillis();

        int bestMove = -1;
        int highestSeenValue = Integer.MIN_VALUE;
        int lowestSeenValue = Integer.MAX_VALUE;
        int currentValue;
        String currentPlayer = state.getWhoseTurn();

        for (Move move : ChildrenGenerator.getChildren(state, state.getWhoseTurn())) {

            currentValue = (currentPlayer.equals(PLAYER_1))
                    ? Min(move.newState, Integer.MIN_VALUE, Integer.MAX_VALUE, depth - 1)
                    : Max(move.newState, Integer.MIN_VALUE, Integer.MAX_VALUE, depth - 1);

            if (currentPlayer.equals(PLAYER_1) && (currentValue >= highestSeenValue)) {
                bestMove = move.idxMoveFromParent;
                highestSeenValue = currentValue;
            } else if ((currentPlayer.equals(PLAYER_2)) && (currentValue <= lowestSeenValue)) {
                bestMove = move.idxMoveFromParent;
                lowestSeenValue = currentValue;
            }
        }
        long execTime = System.currentTimeMillis() - startTime;
        //System.out.println(" -->> Thinking time = " + execTime
//                + " best Move idx = " + bestMove + " for alphaBetaMax depth = " + depth);
        return bestMove;
    }

    private int Max(GameState state, int alpha, int beta, int depth) {
        if (depth == 0 /*|| game over */) {
            return heuristic_function(state);
        }
        if(state.isGameOver()){
            ///RETURNING NUM_STONES difference in STORAGE
            return Util.getUtility(state);
        }
        
        int v = Integer.MIN_VALUE;
        String nowTurn = state.getWhoseTurn();
        for (Move move : ChildrenGenerator.getChildren(state, state.getWhoseTurn())) {
            if (move.newState.getWhoseTurn().equals(nowTurn)) {   //Next state eo JODI same TURN hoy
                v = Math.max(v, Max(move.newState, alpha, beta, depth - 1));

            } else {
                v = Math.max(v, Min(move.newState, alpha, beta, depth - 1));

            }
            //Cut off
            if (v >= beta) {
                return v;
            }
            alpha = Math.max(alpha, v);
        }

        return v;
    }

    private int Min(GameState state, int alpha, int beta, int depth) {
        if (depth == 0 /*|| game over */ || state.isGameOver()) {
            return heuristic_function(state);
        }
        if(state.isGameOver()){
            ///RETURNING NUM_STONES difference in STORAGE
            return Util.getUtility(state);
        }
        int v = Integer.MAX_VALUE;
        String nowTurn = state.getWhoseTurn();
        for (Move move : ChildrenGenerator.getChildren(state, state.getWhoseTurn())) {
            if (move.newState.getWhoseTurn().equals(nowTurn)) {   //SAME manush er TURN
                v = Math.min(v, Max(move.newState, alpha, beta, depth - 1));
            } else {
                v = Math.min(v, Min(move.newState, alpha, beta, depth - 1));
            }
            //Cutoff
            if (v <= alpha) {
                return v;
            }
            beta = Math.min(beta, v);
        }

        return v;
    }

}
