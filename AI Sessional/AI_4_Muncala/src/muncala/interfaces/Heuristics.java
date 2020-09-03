package muncala.interfaces;

import muncala.GameState;
import static muncala.interfaces.Util.*;

public class Heuristics {

    public static int HEURISTIC_CHOOSER_BOT_1 = 2;
    public static int HEURISTIC_CHOOSER_BOT_2 = 4;

//-------------------------------------------------------------------------------------
    private static final int NULL_VAL = -9999;

//------------------------------------------------------------------------------------
    public static int heuristic_MaxPlayer(GameState state) {
        switch (HEURISTIC_CHOOSER_BOT_1) {
            case 1:
//                System.out.println("H1 Max");
                return h1(state, PLAYER_1);
            case 2:
//                System.out.println("H2 Max");
                return h2(state, PLAYER_1);
            case 3:
//                System.out.println("H3 Max");
                return h3(state, PLAYER_1);
            case 4:
//                System.out.println("H4 Max");
                return h4(state, PLAYER_1);
            default:
                return -1;
        }
    }

    public static int heuristic_MinPlayer(GameState state) {
        switch (HEURISTIC_CHOOSER_BOT_2) {
            case 1:
//                System.out.println("H1 Min");
                return h1(state, PLAYER_2);
            case 2:
//                System.out.println("H2 Min");
                return h2(state, PLAYER_2);
            case 3:
//                System.out.println("H3 Min");
                return h3(state, PLAYER_2);
            case 4:
//                System.out.println("H4 Min");
                return h4(state, PLAYER_2);
            default:
                return -1;
        }
    }
//------------------------------------------------------------------------------------

    private static int h1(GameState state, String whichPlayer) {
        int[] board = state.getBoard();
        int val_max = board[IDX_PLAYER1_MUNCALA];
        int val_min = board[IDX_PLAYER2_MUNCALA];
        int h = -1;
        if (whichPlayer.equals(PLAYER_1)) {
            h = (val_max - val_min);
        } else if (whichPlayer.equals(PLAYER_2)) {
            h = (val_min - val_max);
        }
        return (int) h;
    }

    private static int h2(GameState state, String whichPlayer) {
        int[] board = state.getBoard();

        int val_player1_muncala = board[IDX_PLAYER1_MUNCALA];
        int val_player2_muncala = board[IDX_PLAYER2_MUNCALA];

        int val_player1_side = Helper.numberOfStonesInPlayer1Side(state);
        int val_player2_side = Helper.numberOfStonesInPlayer2Side(state);

        int h = -1;
        if (whichPlayer.equals(PLAYER_1)) {
            h = Player1_Weights.W1_h2 * (val_player1_muncala - val_player2_muncala) + Player1_Weights.W2_h2 * (val_player1_side - val_player2_side);
        } else if (whichPlayer.equals(PLAYER_2)) {
            h = Player2_Weights.W1_h2 * (val_player2_muncala - val_player1_muncala) + Player2_Weights.W2_h2 * (val_player2_side - val_player1_side);
        }
        return h;
    }

    private static int h3(GameState state, String whichPlayer) {
        int h = -1;
        //For calculation
        int stones_my_storage = -1, stones_opp_storage = -1, stones_my_side = -1, stones_opp_side = -1, additional_move = -1;

        if (whichPlayer.equals(PLAYER_1)) {
            stones_my_storage = Helper.stonesStoragePlayer1(state);
            stones_opp_storage = Helper.stonesStoragePlayer2(state);
            stones_my_side = Helper.numberOfStonesInPlayer1Side(state);
            stones_opp_side = Helper.numberOfStonesInPlayer2Side(state);
            additional_move = state.PLAYER1_ADDITIONAL_MOVES_EARNED;
            h = Player1_Weights.W1_h3 * (stones_my_storage - stones_opp_storage) + Player1_Weights.W2_h3 * (stones_my_side - stones_opp_side) + Player1_Weights.W3_h3 * (additional_move);
        } else if (whichPlayer.equals(PLAYER_2)) {
            stones_my_storage = Helper.stonesStoragePlayer2(state);
            stones_opp_storage = Helper.stonesStoragePlayer1(state);
            stones_my_side = Helper.numberOfStonesInPlayer2Side(state);
            stones_opp_side = Helper.numberOfStonesInPlayer1Side(state);
            additional_move = state.PLAYER2_ADDITIONAL_MOVES_EARNED;
            h = Player2_Weights.W1_h3 * (stones_my_storage - stones_opp_storage) + Player2_Weights.W2_h3 * (stones_my_side - stones_opp_side) + Player2_Weights.W3_h3 * (additional_move);
        }

        return h;
    }

    private static int h4(GameState state, String whichPlayer) {
        int h = -1;
        //For calculation
        int stones_my_storage = -1, stones_opp_storage = -1, stones_my_side = -1, stones_opp_side = -1, additional_move = -1, stones_captured = -1;

        if (whichPlayer.equals(PLAYER_1)) {
            stones_my_storage = Helper.stonesStoragePlayer1(state);
            stones_opp_storage = Helper.stonesStoragePlayer2(state);
            stones_my_side = Helper.numberOfStonesInPlayer1Side(state);
            stones_opp_side = Helper.numberOfStonesInPlayer2Side(state);
            additional_move = state.PLAYER1_ADDITIONAL_MOVES_EARNED;
            stones_captured = state.PLAYER1_STONES_CAPTURED;
            h = Player1_Weights.W1_h4 * (stones_my_storage - stones_opp_storage) + Player1_Weights.W2_h4 * (stones_my_side - stones_opp_side) + Player1_Weights.W3_h4 * (additional_move) + Player1_Weights.W4_h4 * (stones_captured);

        } else if (whichPlayer.equals(PLAYER_2)) {
            stones_my_storage = Helper.stonesStoragePlayer2(state);
            stones_opp_storage = Helper.stonesStoragePlayer1(state);
            stones_my_side = Helper.numberOfStonesInPlayer2Side(state);
            stones_opp_side = Helper.numberOfStonesInPlayer1Side(state);
            additional_move = state.PLAYER2_ADDITIONAL_MOVES_EARNED;
            stones_captured = state.PLAYER2_STONES_CAPTURED;
            h = Player2_Weights.W1_h4 * (stones_my_storage - stones_opp_storage) + Player2_Weights.W2_h4 * (stones_my_side - stones_opp_side) + Player2_Weights.W3_h4 * (additional_move) + Player2_Weights.W4_h4 * (stones_captured);

        }

        return h;
    }
}
