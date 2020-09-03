package muncala.strategies;

import muncala.GameState;
import static muncala.Main.PRINT_THINGS;

import muncala.interfaces.Heuristics;
import muncala.interfaces.Player1_Weights;
import muncala.interfaces.Player2_Weights;
import muncala.interfaces.Util;
import static muncala.interfaces.Util.NULL_VALUE;
import static muncala.interfaces.Util.PLAYER_1;
import static muncala.interfaces.Util.PLAYER_2;

public abstract class Strategies {

    public static String MINI_MAX = "MiniMax";
    public static String ALPHA_BETA_PRUNING = "AlphaBetaPruning";
    public static String ALPHA_BETA_ITERATIVE_DEEPENING = "AlphaBeta_IterativeDeepening";

    protected String playerType;
    protected String strategyName;
    protected GameState currentGameState;

    public void setGameState(GameState state) {
        this.currentGameState = state;
    }

    public GameState getGameState() {
        return this.currentGameState;
    }

    public String getStrategyName() {
        return this.strategyName;
    }

    public void setPlayerType(String playerType) {
        this.playerType = playerType;
    }

    public void setStrategyName(String s) {
        this.strategyName = s;
    }

    public String getPlayerType() {
        return this.playerType;
    }

    protected int heuristic_function(GameState state) {
        switch (this.playerType) {
            case PLAYER_1:
//                System.out.println("-->>Returning for max player heuristic = " + Heuristics.heuristic_MaxPlayer(state));
                return Heuristics.heuristic_MaxPlayer(state);
            case PLAYER_2:
//                System.out.println("===>>Returning for min player heuristic = " + Heuristics.heuristic_MinPlayer(state));
                return Heuristics.heuristic_MinPlayer(state);
            default:
                return Util.NULL_VALUE;
        }
    }

    protected boolean terminalTest(GameState state) {
        return state.isGameOver();
    }

    protected int utilityValue(GameState state) {
        return Util.getUtility(state);
    }

    public String getMaxOrMinPlayer() {
        if (this.playerType.equals(PLAYER_1)) {
            return "MaxPlayer";
        } else {
            return "MinPlayer";
        }
    }

    public int getBestMove() {
        if (this.playerType.equals(PLAYER_1)) {
            return getBestMove(currentGameState, Player1_Weights.CUT_OFF_DEPTH_Player_1);
        } else if (this.playerType.equals(PLAYER_2)) {
            return getBestMove(currentGameState, Player2_Weights.CUT_OFF_DEPTH_Player_2);
        } else {
            System.out.println("===>>> In Strategies.getBestMove() .... playerType mismatch !!! playerType = " + this.playerType);
            return -1;
//           return getBestMove(currentGameState, Util.CUT_OFF_DEPTH);            
        }

    }

    private int getBestMove_2(GameState state, int depth) {
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
                    currentValue = max(actionWithState.newState, depth - 1);
//                    System.out.println("\nIn getBestMove ( ) , currentVal = " + currentValue + "\n");
                } else {
                    currentValue = min(actionWithState.newState, depth - 1);
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
                    currentValue = max(actionWithState.newState, depth - 1);
                } else {   //next Turn NOT equals to THIS PLAYER
                    currentValue = min(actionWithState.newState, depth - 1);
                }
                if (currentValue <= lowestSeenValue) {
                    lowestSeenValue = currentValue;
                }
                if ((currentValue >= highestSeenValue)) {
                    bestMove = actionWithState.idxMoveFromParent;
                    highestSeenValue = currentValue;
                }
//                if (currentValue >= highestSeenValue) {
//                    highestSeenValue = currentValue;
//                }
//                if ((currentValue <= lowestSeenValue)) {
//                    bestMove = actionWithState.idxMoveFromParent;
//                    lowestSeenValue = currentValue;
//                }
            }
            return bestMove;
        }
        System.out.println("-->>>In getBestMove_new .. MinMax... returning null value");
        return Util.NULL_VALUE;

    }
    boolean flag = true;

    public int getBestMove(GameState state, int depth) {
        if (flag) {
            return getBestMove_2(state, depth);
        }
        if (depth <= 0) {
            System.out.println("-->>In getBestMove() of " + getStrategyName() + " Player Type" + getMaxOrMinPlayer() + " ... depth given is <= 0 ... return -1");
            return NULL_VALUE;
        }

        long startTime = System.currentTimeMillis();
        int bestMove = NULL_VALUE, bestMove_max = -1, bestMove_min = -1;
        int highestSeenValue = Integer.MIN_VALUE;
        int lowestSeenValue = Integer.MAX_VALUE;
        int currentValue;
        String currentPlayer = state.getWhoseTurn();
//
//        System.out.println("In Strategies.getBestMove(state, depth) " + state.getWhoseTurn() + " is thinking ... "
//                + " with depth = " + depth);

        for (Action move : ChildrenProducer.getChildren(state, state.getWhoseTurn())) {

            currentValue = (move.newState.getWhoseTurn().equals(currentPlayer))
                    ? max(move.getNewState(), depth - 1)
                    : min(move.getNewState(), depth - 1);

            if (currentPlayer.equals(PLAYER_1) && (currentValue >= highestSeenValue)) {
                bestMove = move.getIdxMoveFromParent();
                bestMove_min = move.getIdxMoveFromParent();
                highestSeenValue = currentValue;
            } else if ((currentPlayer.equals(PLAYER_2)) && (currentValue <= lowestSeenValue)) {
                bestMove = move.getIdxMoveFromParent();
                bestMove_max = move.getIdxMoveFromParent();
                lowestSeenValue = currentValue;
            }
        }
        return bestMove;
    }

    protected abstract int max(GameState state, int alpha, int beta, int depth);

    protected abstract int min(GameState state, int alpha, int beta, int depth);

    protected abstract int max(GameState state, int depth);

    protected abstract int min(GameState state, int depth);
}
