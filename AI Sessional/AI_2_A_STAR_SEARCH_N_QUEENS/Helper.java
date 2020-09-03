package a_star_search;

import java.io.*;
import java.util.*;

public class Helper {

    public static int getLinearConflictsOfThisRow(int[] rowStart, int[] rowEnd) {
        int cnt = 0;
        for (int i = 0; i < (rowStart.length - 1); i++) {
            for (int j = (i + 1); j < rowStart.length; j++) {
                int leftStartElement = rowStart[i];
                int rightStartElement = rowStart[j];
                int leftEndIdx = getIndexOf(rowEnd, leftStartElement);
                int rightEndIdx = getIndexOf(rowEnd, rightStartElement);
                if ((leftEndIdx != -1) && (rightEndIdx != -1) && (rightStartElement != 0) && (leftStartElement != 0)) {    //BOTH ELEMENTS ARE IN "THIS" ROW....
                    int leftStartIdx = getIndexOf(rowStart, leftStartElement);
                    int rightStartIdx = getIndexOf(rowStart, rightStartElement);
                    cnt += (getConflictOf(leftStartIdx, rightStartIdx, leftEndIdx, rightEndIdx));
                }
            }
        }
        return cnt;
    }

    private static int getConflictOf(int leftStartIdx, int rightStartIdx, int leftEndIdx, int rightEndIdx) {

        if ((leftStartIdx < leftEndIdx) && (rightEndIdx < rightStartIdx)) {
            return 1;
        }

        return 0;
    }

    public static int getNumLinearConflicts(Node initialNode, Node goalNode) {
//        int[][] startBoard = initialNode.getBoard();
//        int[][] endBoard = goalNode.getBoard();
        int[][] startBoard = initialNode.board;
        int[][] endBoard = goalNode.board;
        //row check
        int rowCnt = 0;
        int colCnt = 0;
        int cnt = 0;
        for (int i = 0; i < startBoard.length; i++) {
            int[] rowStart = startBoard[i];
            int[] rowEnd = endBoard[i];
            rowCnt += (Helper.getLinearConflictsOfThisRow(rowStart, rowEnd));
        }
        //column check ... same as row check but of the TRANSPOSE
        int[][] startTranspose = transposeMatrix(startBoard);
        int[][] endTranspose = transposeMatrix(endBoard);
        for (int i = 0; i < startBoard.length; i++) {
            int[] rowStart = startTranspose[i];
            int[] rowEnd = endTranspose[i];
            colCnt += (Helper.getLinearConflictsOfThisRow(rowStart, rowEnd));
        }

        cnt = rowCnt + colCnt;
        return cnt;
    }

    public static int[][] takeInput() {
        File file = null;
        Scanner sc = null;

        file = new File(Writer.inputFile);
        try {
            sc = new Scanner(file);
            System.out.println("Taking input from file .... ");
            List<Integer> list = new ArrayList<>();
            int maxElement = -1000;
            while (sc.hasNext()) {
                int num = sc.nextInt();
                if (num > maxElement) {
                    maxElement = num;
                }
                list.add(num);
            }
            Main.n_in_Main = maxElement;
            Main.n_in_Main++;
            Main.k_in_Main = (int) Math.sqrt(Main.n_in_Main + 1);
            int[][] arr = new int[Main.k_in_Main][Main.k_in_Main];
            int cnt = 0;
            for (int i = 0; i < Main.k_in_Main; i++) {
                for (int j = 0; j < Main.k_in_Main; j++) {
                    arr[i][j] = list.get(cnt);
                    cnt++;
                }
            }
            System.out.println("Here, n = " + Main.n_in_Main + " , k = " + Main.k_in_Main);
            return arr;
        } catch (FileNotFoundException ex) {
            sc = new Scanner(System.in);
            System.out.println("<FILE ERROR> use console .... JUST WRITE THE INPUTS max number followed by array elements .... ");
            Main.n_in_Main = sc.nextInt();
            Main.k_in_Main = (int) Math.sqrt(Main.n_in_Main + 1);
            System.out.println("In Main, n = " + Main.n_in_Main + " , k = " + Main.k_in_Main);
            int[][] arr = new int[Main.k_in_Main][Main.k_in_Main];
            for (int i = 0; i < Main.k_in_Main; i++) {
                for (int j = 0; j < Main.k_in_Main; j++) {
                    arr[i][j] = sc.nextInt();
                }
            }
            return arr;
        }

    }

//-------------------------------------------------------------UTILITY FUNCTIONS---------------------------------------------------------------------------
    private static int getIndexOf(int[] singleArr, int key) {
        int idx = -1;
        for (int i = 0; i < singleArr.length; i++) {
            if (singleArr[i] == key) {
                idx = i;
                break;
            }
        }

        return idx;
    }

    public static int[][] transposeMatrix(int[][] m) {
        int[][] temp = new int[m[0].length][m.length];
        for (int i = 0; i < m.length; i++) {
            for (int j = 0; j < m[0].length; j++) {
                temp[j][i] = m[i][j];
            }
        }
        return temp;
    }

    public static void printList(List<Node> list) {
        list.forEach(System.out::println);
    }

}
