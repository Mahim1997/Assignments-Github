package muncala.strategies;

import muncala.GameState;

public class MiniMax extends Strategies {

    public MiniMax(String playerType) {
        super.setPlayerType(playerType);
        super.setStrategyName(Strategies.MINI_MAX);
    }

    @Override
    protected int max(GameState state, int alpha, int beta, int depth) {
        return max(state, depth);
    }

    @Override
    protected int min(GameState state, int alpha, int beta, int depth) {
        return min(state, depth);
    }

    @Override
    protected int max(GameState state, int depth) {
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
                int temp = max(actionWithState.getNewState(), depth - 1);
//                System.out.println("-->>>In MAX() , after calling min(), returned val = " + temp + " , for depth = " + depth + " , v = " + v);
                v = Math.max(v, temp);
            } else {
                int temp = min(actionWithState.getNewState(), depth - 1);
//                System.out.println("-->>>In MAX() , after calling min(), returned val = " + temp + " , for depth = " + depth + " , v = " + v);

                v = Math.max(v, temp);
            }
        }

        return v;
    }

    @Override
    protected int min(GameState state, int depth) {

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
            if (actionWithState.getNewState().getWhoseTurn().equals(nowTurn) ) {   //SAME manush er TURN
                int temp = min(actionWithState.getNewState(), depth - 1);
//                System.out.println("-->>>In MIN() , after calling min(), returned val = " + temp + " , for depth = " + depth + " , v = " + v);
                v = Math.min(v, temp);
            } else {
                int temp = max(actionWithState.getNewState(), depth - 1);
//                System.out.println("-->>>In MIN() , after calling max(), returned val = " + temp + " , for depth = " + depth + " , v = " + v);

                v = Math.min(v, temp);
            }
        }

        return v;
    }

}
