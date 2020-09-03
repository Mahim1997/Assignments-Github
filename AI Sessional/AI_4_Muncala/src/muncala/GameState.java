package muncala;

import java.util.Arrays;
import muncala.interfaces.Helper;
import static muncala.interfaces.Padder.*;
import static muncala.interfaces.Util.*;

public class GameState implements Cloneable {
    //idx: 0 to 5 are mine left to right
    //idx: 6 is my muncala
    //idx: 7 to 12 are opponent's from right to left
    //idx: 13 is opponent's muncala

    int[] board = new int[TOTAL_BOARD_ARRAY_SIZE];

    private String whoseTurn;   //Player1, Player2
    public int PLAYER1_SCORE;
    public int PLAYER2_SCORE;

    ////For heuristics 3 and 4
    public int PLAYER1_ADDITIONAL_MOVES_EARNED;
    public int PLAYER2_ADDITIONAL_MOVES_EARNED;
    public int PLAYER1_STONES_CAPTURED;
    public int PLAYER2_STONES_CAPTURED;
    ////For heuristics 3 and 4 done

    public String winner, loser;

    public GameState() {
        PLAYER1_SCORE = 0;
        PLAYER1_ADDITIONAL_MOVES_EARNED = 0;
        PLAYER1_STONES_CAPTURED = 0;
        PLAYER2_ADDITIONAL_MOVES_EARNED = 0;
        PLAYER2_SCORE = 0;
        PLAYER2_STONES_CAPTURED = 0;
    }

    public GameState(GameState newState) {
        this.copyState(newState);
    }

    public void printBoard() {
//        Helper.writeOutput(this.toString());
        System.out.println(this.toString());
    }

    @Override
    public String toString() {
        String s = "";
        String onlyBoardValues = getBoardString(this);
//        s = onlyBoardValues;

//        s = getBeforePadding(onlyBoardValues);
//        s += "\n";
        return onlyBoardValues;
    }

    public GameState(int[] gameBoard) {
        this.board = gameBoard;
        this.whoseTurn = PLAYER_1;
    }

    public void setBoard(int[] board) {
        this.board = board;
    }

    public void setTurn(String turn) {
        this.whoseTurn = turn;
    }

    public void flipTurn() {
        if (this.whoseTurn.equals(PLAYER_1)) {
            this.whoseTurn = PLAYER_2;
        } else {
            this.whoseTurn = PLAYER_1;
        }
    }

    public boolean isGameOver() {
        int cnt1 = 0, cnt2 = 0;
        for (int i = IDX_PLAYER1_LEFT; i <= IDX_PLAYER1_RIGHT; i++) {
            cnt1 += this.board[i];
        }
        for (int i = IDX_PLAYER2_RIGHT; i <= IDX_PLAYER2_LEFT; i++) {
            cnt2 += this.board[i];
        }
        return ((cnt1 == 0) || (cnt2 == 0));
    }

    public void calculateScore() {
        this.PLAYER1_SCORE = this.board[IDX_PLAYER1_MUNCALA];
        this.PLAYER2_SCORE = this.board[IDX_PLAYER2_MUNCALA];
//        if (isGameOver() == false) {
//            return;
//        }

        int cnt1 = 0, cnt2 = 0;
        
        for (int i = IDX_PLAYER1_LEFT; i <= IDX_PLAYER1_RIGHT; i++) {
            cnt1 += this.board[i];
        }
        for (int i = IDX_PLAYER2_LEFT; i >= IDX_PLAYER2_RIGHT; i--) {
            cnt2 += this.board[i];
        }

        this.PLAYER1_SCORE += cnt1;
        this.PLAYER2_SCORE += cnt2;

        if (this.PLAYER1_SCORE > this.PLAYER2_SCORE) {
            winner = PLAYER_1;
            loser = PLAYER_2;
        } else if (this.PLAYER2_SCORE > this.PLAYER1_SCORE) {
            winner = PLAYER_2;
            loser = PLAYER_1;
        } else {
            winner = loser = "TIE";
        }
    }

    private GameState makeMovePlayer1_new(int idx) {
        GameState newState = copyState(this);
        int chipsToGive = newState.board[idx];
        int[] newBoard = newState.board.clone();
        if (Helper.inRangeOfPlayer1(idx) == false) {
            System.out.println("Inside makeMovePlayer 1 " + idx + " not in range of player 1 .. ");
            return newState;
        }
        if (Helper.isEmpty(newBoard, idx) == true) {
            System.out.println("Cannot move an empty board !! of idx = " + idx);
            return newState;
        }

        int newIdx = idx;

        newState.flipTurn();

        while (chipsToGive >= 1) {
            newIdx = getNext(newIdx);
            if (newIdx == IDX_PLAYER2_MUNCALA) {
                continue;
            }
            newBoard[idx]--;
//            newBoard[newIdx]++;

            if (chipsToGive == 1) {   //Final chips checking
                //1. IF final chips is in muncala
                if (newIdx == IDX_PLAYER1_MUNCALA) {
                    //Flip turn again
                    newBoard[IDX_PLAYER1_MUNCALA]++;
                    newState.PLAYER1_ADDITIONAL_MOVES_EARNED++;
                    newState.flipTurn();
                } else if (Helper.isEmpty(newBoard, newIdx) && !Helper.isEmpty(newBoard, 12 - newIdx)) {
                    //If empty peg of ##MINE##
                    int complimentaryIDX = 12 - newIdx;
                    if ((Helper.inRangeOfPlayer1(newIdx)) && (Helper.inRangeOfPlayer2(complimentaryIDX))) {
                        int complimentaryValue = newBoard[complimentaryIDX];

                        newBoard[newIdx] = 0;
                        newBoard[complimentaryIDX] = 0;
                        newBoard[IDX_PLAYER1_MUNCALA] += (complimentaryValue + 1);
                        newState.PLAYER1_STONES_CAPTURED += complimentaryValue;
                    } else {
                        newBoard[newIdx]++;
                    }
                } else {
                    newBoard[newIdx]++;
                }
                break;
            }

            chipsToGive--;
            newBoard[newIdx]++;

        }
        newState.board = newBoard;
        newState.calculateScore();
        return newState;
    }

    private GameState makeMovePlayer2_new(int idx) {
        GameState newState = copyState(this);

        int chipsToGive = newState.board[idx];
        int[] newBoard = newState.board.clone();

        if (Helper.inRangeOfPlayer2(idx) == false) {
            System.out.println("Inside makeMovePlayer2 " + idx + " not in range of player 2 .. ");
            return newState;
        }
        if (Helper.isEmpty(newBoard, idx) == true) {
            System.out.println("Cannot move an empty board !! of idx = " + idx);
            return newState;
        }
        int newIdx = idx;
        newState.flipTurn();

        while (chipsToGive >= 1) {
            newIdx = getNext(newIdx);
            if (newIdx == IDX_PLAYER1_MUNCALA) {
                continue;
            }
            newBoard[idx]--;

            if (chipsToGive == 1) {   //Final chips checking
                //1. IF final chips is in muncala
                if (newIdx == IDX_PLAYER2_MUNCALA) {
                    //Flip turn again
                    newState.PLAYER2_ADDITIONAL_MOVES_EARNED++;
                    newBoard[IDX_PLAYER2_MUNCALA]++;
                    newState.flipTurn();
                } else if (Helper.isEmpty(newBoard, newIdx) && !Helper.isEmpty(newBoard, 12 - newIdx)) {
                    //If empty peg of ##MINE##
                    int complimentaryIDX = 12 - newIdx;
                    if ((Helper.inRangeOfPlayer2(newIdx)) && (Helper.inRangeOfPlayer1(complimentaryIDX))) {
                        int complimentaryValue = newBoard[complimentaryIDX];

                        newBoard[newIdx] = 0;
                        newBoard[complimentaryIDX] = 0;
                        newBoard[IDX_PLAYER2_MUNCALA] += (complimentaryValue + 1);
                        newState.PLAYER2_STONES_CAPTURED += complimentaryValue;
                    } else {
                        newBoard[newIdx]++;
                    }
                } else {
                    newBoard[newIdx]++;
                }
                break;
            }

            chipsToGive--;
            newBoard[newIdx]++;
        }
        newState.board = newBoard;
        newState.calculateScore();
        return newState;
    }

    public GameState makeMove(int idx) {
//        Helper.writeOutput("===>>> Moving idx = " + idx + " of player " + this.whoseTurn);
        if (whoseTurn.equals(PLAYER_1)) {
            return makeMovePlayer1_new(idx);
        } else if (whoseTurn.equals(PLAYER_2)) {
            return makeMovePlayer2_new(idx);
        }
        return null;
    }

    private int getNext(int val) {
        return ((val + 1) % TOTAL_BOARD_ARRAY_SIZE);
    }

    public static GameState getInitialState() {
        GameState state = new GameState();
        Arrays.fill(state.board, 4);
        state.board[IDX_PLAYER1_MUNCALA] = 0;
        state.board[IDX_PLAYER2_MUNCALA] = 0;
        state.whoseTurn = PLAYER_1;
        state.PLAYER1_SCORE = 0;
        state.PLAYER2_SCORE = 0;
        return state;
    }

    public int[] getBoard() {
        return this.board;
    }

    public String getWhoseTurn() {
        return this.whoseTurn;
    }

    public void move_By_Changing_State(GameState newState) {
        this.changeState(newState);
        if (PRINT_STATES_INTERMEDIATE == 1) {
//            this.printBoard();
        }
    }

    public void move_By_Changing_State_with_idx(int idx) {
        GameState newState = this.makeMove(idx);
//        System.out.println("-->>Got newState ... printing it " + newState.toString());
        this.changeState(newState);
//        this.printBoard();
    }

    public void changeState(GameState source) {
        this.PLAYER1_ADDITIONAL_MOVES_EARNED = source.PLAYER1_ADDITIONAL_MOVES_EARNED;
        this.PLAYER1_SCORE = source.PLAYER1_SCORE;
        this.PLAYER1_STONES_CAPTURED = source.PLAYER1_STONES_CAPTURED;
        this.PLAYER2_ADDITIONAL_MOVES_EARNED = source.PLAYER2_ADDITIONAL_MOVES_EARNED;
        this.PLAYER2_SCORE = source.PLAYER2_SCORE;
        this.PLAYER2_STONES_CAPTURED = source.PLAYER2_STONES_CAPTURED;

        this.winner = source.winner;
        this.loser = source.loser;
        this.board = (source.board).clone();
        this.whoseTurn = source.whoseTurn;
    }

    private GameState copyState(GameState sourceState) {
        GameState state = new GameState();

        state.PLAYER1_ADDITIONAL_MOVES_EARNED = sourceState.PLAYER1_ADDITIONAL_MOVES_EARNED;
        state.PLAYER1_SCORE = sourceState.PLAYER1_SCORE;
        state.PLAYER1_STONES_CAPTURED = sourceState.PLAYER1_STONES_CAPTURED;
        state.PLAYER2_ADDITIONAL_MOVES_EARNED = sourceState.PLAYER2_ADDITIONAL_MOVES_EARNED;
        state.PLAYER2_SCORE = sourceState.PLAYER2_SCORE;
        state.PLAYER2_STONES_CAPTURED = sourceState.PLAYER2_STONES_CAPTURED;
        state.winner = sourceState.winner;
        state.loser = sourceState.loser;
        state.board = (sourceState.board).clone();
        state.whoseTurn = sourceState.whoseTurn;

        return state;
    }

//    public boolean getGetAnotherTurn() {
//        throw new UnsupportedOperationException("Not supported yet."); //To change body of generated methods, choose Tools | Templates.
//    }
}
