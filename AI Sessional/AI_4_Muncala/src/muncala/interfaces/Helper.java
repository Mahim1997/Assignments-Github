package muncala.interfaces;

import java.io.*;
import muncala.GameState;
import static muncala.interfaces.Util.*;

public interface Helper {

    public static String outputFile = "src/muncala/texts/Output.txt";

    public static boolean inRangeOfPlayer1(int idx) {
        return (inBetween(Util.IDX_PLAYER1_LEFT, Util.IDX_PLAYER1_RIGHT, idx));
    }

    public static boolean inRangeOfPlayer2(int idx) {
        return (inBetween(Util.IDX_PLAYER2_LEFT, Util.IDX_PLAYER2_RIGHT, idx));
    }

    public static boolean isEmpty(int[] board, int idx) {
        return (board[idx] == 0);
    }

    public static int getComplimentaryIndex(int idx) {
        return (12 - idx);
    }

    public static int numberOfStonesInPlayer1Side(GameState state) {
        int[] board = state.getBoard();
        int cnt = 0;
        for (int i = IDX_PLAYER1_LEFT; i <= IDX_PLAYER1_RIGHT; i++) {
            cnt += (board[i]);
        }
        return cnt;
    }

    public static int numberOfStonesInPlayer2Side(GameState state) {
        int[] board = state.getBoard();
        int cnt = 0;
        for (int i = IDX_PLAYER2_RIGHT; i <= IDX_PLAYER2_LEFT; i++) {
            cnt += (board[i]);
        }
        return cnt;
    }

    public static int stonesStoragePlayer1(GameState state) {
        return (state.getBoard()[IDX_PLAYER1_MUNCALA]);
    }

    public static int stonesStoragePlayer2(GameState state) {
        return (state.getBoard()[IDX_PLAYER2_MUNCALA]);
    }

    public static boolean isGameOver(GameState state) {
        int cnt1 = 0, cnt2 = 0;
        int[] board = state.getBoard();
        for (int i = IDX_PLAYER1_LEFT; i <= IDX_PLAYER1_RIGHT; i++) {
            cnt1 += board[i];
        }
        for (int i = IDX_PLAYER2_RIGHT; i <= IDX_PLAYER2_LEFT; i++) {
            cnt2 += board[i];
        }
        return ((cnt1 == 0) || (cnt2 == 0));
    }

    public static boolean inBetween(int leftIdx, int rightIdx, int idxGiven) {
//        System.out.print("leftIdx = " + leftIdx + " , rightIdx = " + rightIdx + " , idxToSearch = " + idxGiven + "    ");
        if (leftIdx == rightIdx) {
            return false;
        }
        if (leftIdx <= idxGiven && idxGiven <= rightIdx && leftIdx < rightIdx) {
            return true;
        }
        if (rightIdx < leftIdx) {
            int temp = rightIdx;
            rightIdx = leftIdx;
            leftIdx = temp;
            if (leftIdx <= idxGiven && idxGiven <= rightIdx && leftIdx < rightIdx) {
                return true;
            }
        }
        return false;
    }

    public static void eraseOutput() {
        try (FileWriter fw = new FileWriter(outputFile);
                BufferedWriter bw = new BufferedWriter(fw);
                PrintWriter out = new PrintWriter(bw)) {
            out.println("");

        } catch (IOException e) {
        }
    }

    public static void writeOutput(String s) {
        try (FileWriter fw = new FileWriter(outputFile, true);
                BufferedWriter bw = new BufferedWriter(fw);
                PrintWriter out = new PrintWriter(bw)) {
            out.println(s);
            System.out.println(s);
        } catch (IOException e) {
        }
    }

    public static void noNewLineWriteOutput(String s) {
        try (FileWriter fw = new FileWriter(outputFile, true);
                BufferedWriter bw = new BufferedWriter(fw);
                PrintWriter out = new PrintWriter(bw)) {
            out.print(s);

        } catch (IOException e) {
        }
    }
}
