package muncala.strategies;

import java.util.List;
import muncala.GameState;
import static muncala.Main.PRINT_NEW;

import muncala.interfaces.Util;
import static muncala.interfaces.Util.PLAYER_1;
import static muncala.interfaces.Util.PLAYER_2;
import static muncala.Main.PRINT_THINGS;

public class AlphaBeta extends Strategies {

    boolean DEBUG_GetBestMove = true;

    public AlphaBeta(String playerType) {
        super.setPlayerType(playerType);
        super.setStrategyName(Strategies.ALPHA_BETA_PRUNING);
    }

    @Override
    public int getBestMove(GameState state, int depth) {
        if (depth <= 0) {
            ///INVALID DEPTH
            return Util.NULL_VALUE;
        }

        int bestMove = -1;
        int highestSeenValue = Integer.MIN_VALUE;
        int lowestSeenValue = Integer.MAX_VALUE;
        int currentValue;

        String thisPlayerType = this.playerType;
        String whoseTurnOnBoard = state.getWhoseTurn();
    
        if (thisPlayerType.equals(PLAYER_1)) {
            //This player is player 1
            for (Action actionWithState : ChildrenProducer.getChildren(state, state.getWhoseTurn())) {
                String nextTurn = actionWithState.newState.getWhoseTurn();
                if (nextTurn.equals(thisPlayerType)) {
                    currentValue = max(actionWithState.newState, highestSeenValue, lowestSeenValue, depth - 1);
//                    System.out.println("\nIn getBestMove ( ) , currentVal = " + currentValue + "\n");
                } else {
                    currentValue = min(actionWithState.newState, highestSeenValue, lowestSeenValue, depth - 1);
                }
                if (currentValue <= lowestSeenValue) {
                    lowestSeenValue = currentValue;
                }
                if ((currentValue >= highestSeenValue)) {
                    bestMove = actionWithState.idxMoveFromParent;
                    highestSeenValue = currentValue;
                }
            }
            return bestMove;
        } else if (thisPlayerType.equals(PLAYER_2)) {
            //This player is player 2
            for (Action actionWithState : ChildrenProducer.getChildren(state, state.getWhoseTurn())) {
                String nextTurn = actionWithState.newState.getWhoseTurn();
                if (nextTurn.equals(thisPlayerType)) {
                    currentValue = max(actionWithState.newState, highestSeenValue, lowestSeenValue, depth - 1);
                } else {   //next Turn NOT equals to THIS PLAYER
                    currentValue = min(actionWithState.newState, highestSeenValue, lowestSeenValue, depth - 1);
                }
                if (currentValue <= lowestSeenValue) {
                    lowestSeenValue = currentValue;
                }
                if ((currentValue >= highestSeenValue)) {
                    bestMove = actionWithState.idxMoveFromParent;
                    highestSeenValue = currentValue;
                }

            }
            return bestMove;
        }
        System.out.println("-->>>In getBestMove_new .. MinMax... returning null value");
        return Util.NULL_VALUE;
    }

    @Override
    protected int max(GameState state, int alpha, int beta, int depth) {
        if (depth == 0) {
            return heuristic_function(state);
        }
        if (terminalTest(state)) {
            ///RETURNING NUM_STONES difference in STORAGE
            return utilityValue(state);
        }
        int v = Integer.MIN_VALUE;
        String nowTurn = state.getWhoseTurn();
        for (Action actionWithState : ChildrenProducer.getChildren(state, state.getWhoseTurn())) {
            if (actionWithState.getNewState().getWhoseTurn().equals(nowTurn)) {   //Next state eo JODI same TURN hoy
//                int temp = min(actionWithState.getNewState(), depth - 1);
                int temp = max(actionWithState.getNewState(), alpha, beta, depth - 1);
//                System.out.println("-->>>In MAX() , after calling min(), returned val = " + temp + " , for depth = " + depth + " , v = " + v);
                v = Math.max(v, temp);
            } else {
                int temp = min(actionWithState.getNewState(), alpha, beta, depth - 1);
//                System.out.println("-->>>In MAX() , after calling min(), returned val = " + temp + " , for depth = " + depth + " , v = " + v);

                v = Math.max(v, temp);
            }

            if (v > beta) {
                return v;
            }
            alpha = Math.max(alpha, v);
        }

        return v;
    }

    @Override
    protected int min(GameState state, int alpha, int beta, int depth) {

        if (depth == 0) {
            return heuristic_function(state);
        }
        if (terminalTest(state)) {
            ///RETURNING NUM_STONES difference in STORAGE
            return utilityValue(state);
        }

        int v = Integer.MAX_VALUE;
        String nowTurn = state.getWhoseTurn();
        for (Action actionWithState : ChildrenProducer.getChildren(state, state.getWhoseTurn())) {
            if (actionWithState.getNewState().getWhoseTurn().equals(nowTurn)) {   //SAME manush er TURN
                int temp = min(actionWithState.getNewState(), alpha, beta, depth - 1);
//                System.out.println("-->>>In MIN() , after calling min(), returned val = " + temp + " , for depth = " + depth + " , v = " + v);
                v = Math.min(v, temp);
            } else {
                int temp = max(actionWithState.getNewState(), alpha, beta, depth - 1);
//                System.out.println("-->>>In MIN() , after calling max(), returned val = " + temp + " , for depth = " + depth + " , v = " + v);

                v = Math.min(v, temp);
            }
            if (v < alpha) {
                return v;
            }
            beta = Math.min(beta, v);

        }

        return v;
    }

    @Override
    protected int min(GameState state, int depth) {
        throw new UnsupportedOperationException("Not supported yet."); //To change body of generated methods, choose Tools | Templates.   
    }

    @Override
    protected int max(GameState state, int depth) {
        throw new UnsupportedOperationException("Not supported yet."); //To change body of generated methods, choose Tools | Templates.
    }

}
