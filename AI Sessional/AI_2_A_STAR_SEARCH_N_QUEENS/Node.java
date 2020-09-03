package a_star_search;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;

public class Node {

    public int[][] board;
    public String moveFromParent;
    public Node parentNode;

    public int g_value;
    public int f_value;

    public int rowOfZero;
    public int colOfZero;

    public Node(int[][] board) {
        this.board = board;
        this.setZeroPosition();
        this.parentNode = null;
        this.g_value = Initializer.MAX_VAL;
    }

    public Node() {
        this.board = new int[Main.k_in_Main][Main.k_in_Main];
        for (int[] b1 : this.board) {
            Arrays.fill(b1, 0, b1.length - 1, 0); //Fill With zeroes initially...
        }
        this.setZeroPosition();
        this.parentNode = null;
        this.g_value = Initializer.MAX_VAL;
    }

    public int getG_value() {
        return g_value;
    }

    public void setG_value(int g_value) {
        this.g_value = g_value;
    }

    public int getF_value() {
        return f_value;
    }

    public void setF_value(int f_value) {
        this.f_value = f_value;
    }

    public int[][] getBoard() {
        int[][] newArr = new int[this.board.length][this.board.length];
        for (int i = 0; i < this.board.length; i++) {
            System.arraycopy(this.board[i], 0, newArr[i], 0, this.board[i].length);
        }
        return newArr;
    }

    public void setBoard(int[][] board) {
        this.board = board;
        this.setZeroPosition();
    }

    public String getMoveFromParent() {
        return moveFromParent;
    }

    public void setMoveFromParent(String moveFromParent) {
        this.moveFromParent = moveFromParent;
    }

    public Node getParentNode() {
        return parentNode;
    }

    public void setParentNode(Node parentNode) {
        this.parentNode = parentNode;
    }

    @Override
    public int hashCode() {
        int hash = 7;
        hash = 83 * hash + Arrays.deepHashCode(this.board);
        return hash;
    }

    @Override
    public boolean equals(Object obj) {
        if (this == obj) {
            return true;
        }
        Node node = (Node) obj;
        for (int i = 0; i < this.board.length; i++) {
            for (int j = 0; j < this.board[i].length; j++) {
                if (this.board[i][j] != node.board[i][j]) {
                    return false;
                }
            }
        }
        return true;
    }

    public int get_h() {
        return Initializer.h_function(this, Node.getGoalNode(this));
    }

    @Override
    public String toString() {
        String s = "\n----------Node " + " f = " + Initializer.f_function(this, Node.getGoalNode(this)) + (" , h = " + get_h()) + (", g = " + g_value) + "  begins---------\n";
        s += " Move is : " + moveFromParent + "\n";
        s += Initializer.getDoubleArray(this.board) + "------------Node ends-------------\n";

        return s;
    }

    public static Node getGoalNode(Node initialNode) {
        Node newNode = new Node();

        int cnt = 1;
        for (int i = 0; i < Main.k_in_Main; i++) {
            for (int j = 0; j < Main.k_in_Main; j++) {
                if (cnt < Main.n_in_Main) {
                    newNode.board[i][j] = cnt;
                    cnt++;
                }
            }
        }
        newNode.g_value = 0;
        newNode.board[Main.k_in_Main - 1][Main.k_in_Main - 1] = 0;

        return newNode;
    }

    public void setZeroPosition() {
        for (int i = 0; i < this.board.length; i++) {
            for (int j = 0; j < this.board[i].length; j++) {
                if (this.board[i][j] == 0) {
                    this.rowOfZero = i;
                    this.colOfZero = j;
                    return;
                }
            }
        }
    }

    public boolean isValidCoordinate(int x, int y, int boardSize) {
        return x >= 0 && y >= 0 && x < boardSize && y < boardSize;
    }

    public List<Node> getNeighbours() {
        List<Node> list = new ArrayList<>();

        this.setZeroPosition();
        int rowZero = this.rowOfZero;
        int colZero = this.colOfZero;

        int[][] thisBoard = this.board;
        int boardSize = thisBoard.length;
        int[][] newBoard = new int[boardSize][boardSize];

        int maxIdx = thisBoard.length - 1;
        int colLeft = colZero - 1;
        int colRight = colZero + 1;
        int rowDown = rowZero + 1;
        int rowUp = rowZero - 1;
        Node node = null;

        if (isValidCoordinate(rowZero, colLeft, boardSize)) {
            //can move left...
            newBoard = Initializer.getCopy(thisBoard);
            newBoard[rowZero][colLeft] = 0;  //EMPTY
            newBoard[rowZero][colZero] = thisBoard[rowZero][colLeft];
            node = new Node(newBoard);
            node.parentNode = this;
            node.moveFromParent = "<MOVE EMPTY TO LEFT> Swap " + newBoard[rowZero][colZero] + " with EMPTY";
            node.g_value = this.g_value + 1;
            list.add(node);
        }
        if (isValidCoordinate(rowZero, colRight, boardSize)) {
            //can move right ....
            newBoard = Initializer.getCopy(thisBoard);
            newBoard[rowZero][colRight] = 0;  //EMPTY
            newBoard[rowZero][colZero] = thisBoard[rowZero][colRight];
            node = new Node(newBoard);
            node.parentNode = this;
            node.moveFromParent = "<MOVE EMPTY TO RIGHT> Swap " + newBoard[rowZero][colZero] + " with EMPTY";
            node.g_value = this.g_value + 1;
            list.add(node);
        }
        if (isValidCoordinate(rowUp, colZero, boardSize)) {
            //can move Upwards
            newBoard = Initializer.getCopy(thisBoard);
            newBoard[rowUp][colZero] = 0;  //EMPTY
            newBoard[rowZero][colZero] = thisBoard[rowUp][colZero];
            node = new Node(newBoard);
            node.parentNode = this;
            node.moveFromParent = "<MOVE EMPTY UPWARDS> Swap " + newBoard[rowZero][colZero] + " with EMPTY";
            node.g_value = this.g_value + 1;
            list.add(node);
        }
        if (isValidCoordinate(rowDown, colZero, boardSize)) {
            //can move Downwards
            newBoard = Initializer.getCopy(thisBoard);
            newBoard[rowDown][colZero] = 0;  //EMPTY
            newBoard[rowZero][colZero] = thisBoard[rowDown][colZero];
            node = new Node(newBoard);
            node.parentNode = this;
            node.moveFromParent = "<MOVE EMPTY DOWNWARDS> Swap " + newBoard[rowZero][colZero] + " with EMPTY";
            node.g_value = this.g_value + 1;
            list.add(node);
        }

//        System.out.println("=------->>>>RETURNING LIST of size = " + list.size());
        return list;
    }
}
