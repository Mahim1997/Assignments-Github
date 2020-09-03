package muncala.interfaces;

import java.util.Arrays;
import muncala.GameState;

public interface Padder {

    public static final int PAD_LENGTH = 10;
    public static int PAD_LENGTH_BEFORE = 10;
    public static int PAD_LENGTH_AFTER = 70;

    public static String getPadding(String s, int padLength) {
        String str = s;

        for (int i = s.length(); i < padLength; i++) {
            str += " ";
        }

        return str;
    }

    public static String getPadding(int x, int padLength) {
        String s = String.valueOf(x);
        return getPadding(s, padLength);
    }

    public static String getPadding(int x) {
        String s = String.valueOf(x);
        return getPadding(s, Padder.PAD_LENGTH);
    }

    public static String getPadding(String s) {
        return getPadding(s, Padder.PAD_LENGTH);
    }

    public static String beforePadOnce(String s, int len) {
         
        String str = "";
        for (int i = 0; i < len; i++) {
            str += " ";
        }
        if (str.length() == 1) {
            str += " ";
        }
        str += s;
        return str;
    }

    public static String getBeforePadding(String s, int len) {
        String[] arr = s.split("\n");
        if (arr.length == 1) {
            return beforePadOnce(s, len);
        }
        String str2 = "";
        for(String x: arr){
            str2 += (beforePadOnce(s, len));
        }
        return str2;
    }

    public static String getBeforePadding(String s) {
        return beforePadOnce(s, Padder.PAD_LENGTH_BEFORE);
    }
    
    
        public static String getOpponentIndices() {
        String s = "";
        for (int i = Util.IDX_PLAYER2_LEFT; i >= Util.IDX_PLAYER2_RIGHT; i--) {
            s += Padder.getPadding("/" + String.valueOf(i) + "/", Padder.PAD_LENGTH);
        }
        return s;
    }

    public static String getMyIndices() {
        String s = "";
        for (int i = Util.IDX_PLAYER1_LEFT; i <= Util.IDX_PLAYER1_RIGHT; i++) {
            s += Padder.getPadding("/" + String.valueOf(i) + "/", Padder.PAD_LENGTH);
        }
        return s;
    }

    public static String getMyValues(int[] board) {
        String s = "";
        for (int i = Util.IDX_PLAYER1_LEFT; i <= Util.IDX_PLAYER1_RIGHT; i++) {
            s += Padder.getPadding("#" + board[i]);
        }
        return s;
    }

    public static String getOpponentValues(int[] board) {
        String s = "";
        for (int i = Util.IDX_PLAYER2_LEFT; i >= Util.IDX_PLAYER2_RIGHT; i--) {
            s += Padder.getPadding("#" + board[i]);
        }
        return s;
    }

    public static String getBoardString(GameState state) {
        int[] gameBoard = state.getBoard();
        String s = "";
        s += "\n======================================================================================\n";
        s += "Player 2 things";
        s += "\n";
        s += ("idx:" + Padder.getBeforePadding(getOpponentIndices()));
        s += ("Next Move: " + state.getWhoseTurn() );
        s += "\n--------------------------------------------------------------------------------------\n";

        s += "val:" + Padder.getBeforePadding(getOpponentValues(Arrays.copyOf(gameBoard, gameBoard.length)));

        s += "\n\n";

        s += Padder.getBeforePadding("Player 2", 0);

        s += "\tStonesCap. P1: " + (state.PLAYER1_STONES_CAPTURED + ", P2: " + state.PLAYER2_STONES_CAPTURED) + "  AddMoves. P1: " +
                state.PLAYER1_ADDITIONAL_MOVES_EARNED + " , P2: " + state.PLAYER2_ADDITIONAL_MOVES_EARNED ;
        
        s += "\tPlayer 1";
//        s += Padder.getBeforePadding("Player 1", Padder.PAD_LENGTH_AFTER);

        
        s += "\n";
        s += ("Val=" + String.valueOf(gameBoard[Util.IDX_PLAYER2_MUNCALA]));
        s += Padder.getBeforePadding(("Val=" + String.valueOf(gameBoard[Util.IDX_PLAYER1_MUNCALA])), Padder.PAD_LENGTH_AFTER + ("Player 2".length() - "Val=0".length()));

        s += "\n";

        s += ("(Idx=" + String.valueOf(Util.IDX_PLAYER2_MUNCALA) + ")");
        s += Padder.getBeforePadding(("(Idx=" + String.valueOf(Util.IDX_PLAYER1_MUNCALA) + ")"), Padder.PAD_LENGTH_AFTER + ("Player 2".length() - "(Idx=13)".length()));

        s += "\n\n";

        s += "val:" + Padder.getBeforePadding(getMyValues(Arrays.copyOf(gameBoard, gameBoard.length)));

        s += "\n--------------------------------------------------------------------------------------\n";
        s += "idx:" + Padder.getBeforePadding(getMyIndices());
        s += "\n";
        s += "Player 1 things";
        s += ("  ...SCORES... Player 1 => " + String.valueOf(state.PLAYER1_SCORE) + " , Player 2 => " + String.valueOf(state.PLAYER2_SCORE));
        s += "\n======================================================================================\n";

        return s;
    }

}
