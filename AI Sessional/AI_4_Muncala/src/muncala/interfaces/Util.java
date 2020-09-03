package muncala.interfaces;

import muncala.GameState;

public interface Util {


    
    public static int MAX_DEPTH = 20;
    public static final int DEBUG_MODE = 1;
    public static final int PRINT_STATES_INTERMEDIATE = 1;
    public static final int PRINT_PARENT_STATE = 0;
    public static final int INFINITY = 9999999;
//---------------------------------------------------------------------------

    public static final int IDX_PLAYER1_MUNCALA = 6;
    public static final int IDX_PLAYER2_MUNCALA = 13;
    public static final int IDX_PLAYER1_LEFT = 0;
    public static final int IDX_PLAYER1_RIGHT = 5;
    public static final int IDX_PLAYER2_RIGHT = 7;
    public static final int IDX_PLAYER2_LEFT = 12;

//---------------------------------------------------------------------------
    public static final int TOTAL_BOARD_ARRAY_SIZE = 14;
    public static final int TOTAL_STONES = 6 * 2 * 4;
//---------------------------------------------------------------------------
    public static String PLAYER_1 = "Player1";
    public static String PLAYER_2 = "Player2";
    public static float TIMEOUT_SECONDS = (float) 0.1;
    public static long TIMEOUT_MILLI_SECONDS = (long) (TIMEOUT_SECONDS * 1000);
    public static int NULL_VALUE = -9999;

    public static int getUtility(GameState state)
    {
        int []board = state.getBoard();
        int num_stones_1 = board[IDX_PLAYER1_MUNCALA];
        int num_stones_2 = board[IDX_PLAYER2_MUNCALA];
        String turnNext = state.getWhoseTurn();
        if(turnNext.equals(PLAYER_1)){
            return (num_stones_1 - num_stones_2);
        }else{
            return (num_stones_2 - num_stones_1);
        }
//        return Math.abs((num_stones_1 - num_stones_2));
    }

}
