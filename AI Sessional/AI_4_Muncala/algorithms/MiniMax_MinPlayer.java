package muncala.algorithms;

import muncala.GameState;
import muncala.interfaces.Heuristics;
import muncala.interfaces.Util;
import static muncala.interfaces.Util.PLAYER_1;
import static muncala.interfaces.Util.PLAYER_2;

public class MiniMax_MinPlayer {

    public int getBestMove(GameState state, int depth) {

        if (depth <= 0) {
            System.out.println("-->>In getBestMove() of MiniMax_MinPlayer ... depth given is <= 0 ... return -1");
            return -1;
        }

        long startTime = System.currentTimeMillis();

        int bestMove = -1;
        int highestSeenValue = Integer.MIN_VALUE;
        int lowestSeenValue = Integer.MAX_VALUE;
        int currentValue;
        String currentPlayer = state.getWhoseTurn();
//
        System.out.println(state.getWhoseTurn() + " is thinking ... "
                + " with depth = " + depth);
        
        for (Move move : ChildrenGenerator.getChildren(state, state.getWhoseTurn())) {

            currentValue = (currentPlayer.equals(PLAYER_1))
                    ? min(move.newState, depth - 1)
                    : max(move.newState, depth - 1);

            if (currentPlayer.equals(PLAYER_1) && (currentValue >= highestSeenValue)) {
                bestMove = move.idxMoveFromParent;
                highestSeenValue = currentValue;
            } else if ((currentPlayer.equals(PLAYER_2)) && (currentValue <= lowestSeenValue)) {
                bestMove = move.idxMoveFromParent;
                lowestSeenValue = currentValue;
            }
        }
        long execTime = System.currentTimeMillis() - startTime;
        System.out.println(" -->> Thinking time = " + execTime
                + " best Move idx = " + bestMove);
        return bestMove;
    }

    protected int heuristic_function(GameState state) {
        return Heuristics.heuristic_MinPlayer(state);
    }

    public int min(GameState state, int depth) {

        if (depth == 0 /*|| game over */) {
            return heuristic_function(state);
        }
        if(state.isGameOver()){
            ///RETURNING NUM_STONES difference in STORAGE
            return Util.getUtility(state);
        }
        
        int v = Integer.MAX_VALUE;
        String nowTurn = state.getWhoseTurn();
        for (Move move : ChildrenGenerator.getChildren(state, state.getWhoseTurn())) {
            if(move.newState.getWhoseTurn().equals(nowTurn)){   //SAME manush er TURN
                v = Math.min(v, max(move.newState, depth - 1));
            }else{
                v = Math.min(v, min(move.newState, depth - 1));
            }
        }

        return v;
    }

    public int max(GameState state, int depth) {
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
            if(move.newState.getWhoseTurn().equals(nowTurn)){   //Next state eo JODI same TURN hoy
                v = Math.max(v, max(move.newState, depth - 1));
            }
            else{
                v = Math.max(v, min(move.newState, depth - 1));
            }
        }

        return v;
    }

}
