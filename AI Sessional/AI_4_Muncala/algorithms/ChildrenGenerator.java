package muncala.algorithms;

import java.util.ArrayList;
import java.util.List;
import muncala.GameState;
import static muncala.interfaces.Util.IDX_PLAYER1_LEFT;
import static muncala.interfaces.Util.IDX_PLAYER1_RIGHT;
import static muncala.interfaces.Util.IDX_PLAYER2_LEFT;
import static muncala.interfaces.Util.IDX_PLAYER2_RIGHT;
import static muncala.interfaces.Util.PLAYER_1;
import static muncala.interfaces.Util.PLAYER_2;

public class ChildrenGenerator {

    public static List<Move> getChildren(GameState currentState, String whoMoves) {
//        if (Helper.isGameOver(currentState) == true) {
//            return null;
//        }
        if (whoMoves.equals(PLAYER_1)) {
            return getChildrenPlayer1(currentState);
        } else if (whoMoves.equals(PLAYER_2)) {
            return getChildrenPlayer2(currentState);
        }

        return null;
    }

    private static List<Move> getChildrenPlayer1(GameState currentState) {
        int[] nowBoard = currentState.getBoard();
        List<Move> list = new ArrayList<>();

        for (int idxMover = IDX_PLAYER1_LEFT; idxMover <= IDX_PLAYER1_RIGHT ; idxMover++) {

            if (nowBoard[idxMover] != 0) {
                GameState childState = currentState.makeMove(idxMover);
                list.add(new Move(childState, idxMover));
            }

        }
        return list;
    }

    private static List<Move> getChildrenPlayer2(GameState currentState) {
        int[] nowBoard = currentState.getBoard();
        List<Move> list = new ArrayList<>();
 
        for (int idxMover = IDX_PLAYER2_LEFT; idxMover >= IDX_PLAYER2_RIGHT; idxMover--) {
//            System.out.println("Checking for idxMover = " + idxMover + " val[ " + idxMover + " ] = " + nowBoard[idxMover] );
            if (nowBoard[idxMover] != 0) {
                GameState childState = currentState.makeMove(idxMover);
                list.add(new Move(childState, idxMover));
            }

        }

        return list;
    }

}
