package a_star_search;

public class Initializer {

    public static int modeHammingDistance = 1;
    public static int modeManattanDistance = 2;
    public static int modeLinearConflict = 3;

    public static int mode_for_heuristicFunction;//= Initializer.modeLinearConflict;
    public static int MAX_VAL = 10000;
    public static int maxIter = 3;

    public static void run() {
        Writer.eraseOutput();
        Writer.eraseAnalysis();

        int[][] inputArr = Helper.takeInput();

        runHamilton(inputArr);
        runManhattan(inputArr);
        runLinearConflict(inputArr);

    }

    public static void runProcess(int[][] arr) {
        Node initialNode = new Node(arr);
        System.out.println("INITIAL NODE is : " + initialNode.toString());
        Node goalNode = Node.getGoalNode(initialNode);
        System.out.println("GOAL NODE IS " + goalNode.toString());

        Writer.writeOutput("Running for board size = " + Main.k_in_Main + " X " + Main.k_in_Main);
        if (Main.k_in_Main % 2 == 0) {
            //Even board...
            boolean flag = initialEvenErrorExists(initialNode);
            if (flag) {
                System.out.println("--->>>INITIAL ERROR IN EVEN BOARD EXISTS !!!");
                return;
            }

        } else {
            //Odd board...
            boolean flag = initialOddErrorExists(initialNode);
            if (flag) {
                System.out.println("--->>>INITIAL ERROR IN ODD BOARD EXISTS !!!");
                return;
            }
        }

        //Set initialNode and goalNode respectively ...
        Processor processor = new Processor(initialNode, Node.getGoalNode(initialNode));

        processor.process();
//        processor.process_with_HashMap();
    }

    private static void runLinearConflict(int[][] inputArr) {
        Writer.writeOutput("---------------========------ Writing output for Linear Conflict begins --------=========------------------");
        Writer.writeAnalysis("---------------========------ Writing analysis for Linear Conflict begins --------=========------------------");
        Initializer.mode_for_heuristicFunction = Initializer.modeLinearConflict;
        Initializer.runProcess(inputArr);
        Writer.writeAnalysis("---------------========------ Writing analysis for Linear Conflict ends --------=========------------------");
        Writer.writeOutput("---------------========------ Writing output for Linear Conflict ends --------=========------------------");
    }

    private static void runManhattan(int[][] inputArr) {
        Writer.writeOutput("---------------**********------ Writing output for Manhattan distance begins --------************------------------");
        Writer.writeAnalysis("---------------**********------ Writing analysis for Manhattan distance begins --------************------------------");
        Initializer.mode_for_heuristicFunction = Initializer.modeManattanDistance;
        Initializer.runProcess(inputArr);
        Writer.writeAnalysis("---------------**********------ Writing analysis for Manhattan distance ends --------************------------------");
        Writer.writeOutput("---------------**********------ Writing output for Manhattan distance ends --------************------------------");
    }

    private static void runHamilton(int[][] inputArr) {
        Writer.writeOutput("================== Writing Output for Hamming Distance Heuristic begins ===================================");
        Writer.writeAnalysis("================== Writing Analysis for Hamming Distance Heuristic begins ===================================");
        Initializer.mode_for_heuristicFunction = Initializer.modeHammingDistance;
        Initializer.runProcess(inputArr);
        Writer.writeAnalysis("================== Writing Analysis for Hamming Distance Heuristic ends ===================================");
        Writer.writeOutput("================== Writing Output for Hamming Distance Heuristic ends ===================================");
    }

//-------------------------------------------------------------INITIAL ERROR CHECKING-----------------------------------------------------------
    private static boolean initialErrorExists(Node initialNode) {

        if (Main.k_in_Main % 2 == 0) {
            //Even board...
            return initialEvenErrorExists(initialNode);
        } else {
            return initialOddErrorExists(initialNode);
        }

    }

    private static boolean initialOddErrorExists(Node initialNode) {
        int cnt = countInversions(initialNode.board);

        boolean flag = (cnt % 2 != 0);
        System.out.print("<<Inside initialOdd Inversion checking>> ... countInversion = " + cnt);

        if (flag) {
            Writer.writeOutput("ODD BOARD ERROR, number of inversions = " + cnt + " is odd.");
        }
        return flag;
    }

    private static boolean initialEvenErrorExists(Node initialNode) {
        int cnt = countInversions(initialNode.getBoard());
        initialNode.setZeroPosition();
        int blankRowIdx = initialNode.rowOfZero;
        System.out.println("<<Even Board Check>> number inversions : " + cnt + " , blank's row idx = " + blankRowIdx);
        boolean flag = ((blankRowIdx + cnt) % 2 == 0);

        if (flag) {
            Writer.writeOutput("EVEN BOARD ERROR, number of inversions: " + cnt + " , blank's row idx = " + blankRowIdx + " , sum = " + (cnt + blankRowIdx)
                    + " is even.");
        }
        return flag;

    }

//------------------------------------------------------HEURISTIC FUNCTIONS-----------------------------------------------------------
    public static String getCurrentMethod() {
        if (Initializer.mode_for_heuristicFunction == Initializer.modeHammingDistance) {
            return "Hamming Distance";
        } else if (Initializer.mode_for_heuristicFunction == Initializer.modeLinearConflict) {
            return "Linear Conflict";
        } else if (Initializer.mode_for_heuristicFunction == Initializer.modeManattanDistance) {
            return "Manhattan Distance";
        }
        return "NULL_MODE";
    }

    public static int h_function(Node initialNode, Node goalNode) {
        //The final heuristic function ... 
        if (Initializer.mode_for_heuristicFunction == Initializer.modeHammingDistance) {
            return h_hammingDistance(initialNode, goalNode);
        } else if (Initializer.mode_for_heuristicFunction == Initializer.modeLinearConflict) {
            return h_LinearConflict(initialNode, goalNode);
        } else if (Initializer.mode_for_heuristicFunction == Initializer.modeManattanDistance) {
            return h_manhattanDistance(initialNode, goalNode);
        }
        return -1;
    }

    private static int h_hammingDistance(Node initialNode, Node goalNode) {
        //The number of blocks in the wrong position, IGNORE the blank
        int cnt = 0;
        for (int i = 0; i < initialNode.board.length; i++) {
            for (int j = 0; j < initialNode.board[i].length; j++) {
                if (initialNode.board[i][j] != 0) {
                    if (initialNode.board[i][j] != goalNode.board[i][j]) {
                        cnt++;
                    }
                }
            }
        }
        return cnt;
    }

    private static int h_manhattanDistance(Node initialNode, Node goalNode) {
//        int[][] board = initialNode.getBoard();
        int[][] board = initialNode.board;
        int elementToCompare;
        int sum = 0;
        for (int i = 0; i < board.length; i++) {
            for (int j = 0; j < board[i].length; j++) {
                elementToCompare = board[i][j];
                if (elementToCompare != 0) {
                    sum += manhattan(elementToCompare, i, j, goalNode);
                }
            }
        }
        return sum;
    }

    private static int manhattan(int element, int row, int col, Node finalNode) {

        int[][] finalBoard = finalNode.board;
//        int[][] finalBoard = finalNode.getBoard();
        int rowFinal = row, colFinal = col;
        for (int i = 0; i < finalBoard.length; i++) {
            for (int j = 0; j < finalBoard[i].length; j++) {
                if (finalBoard[i][j] == element) {
                    rowFinal = i;
                    colFinal = j;
                    break;
                }
            }
        }
        int distance = Math.abs(rowFinal - row) + Math.abs(colFinal - col);
//        System.out.println("Element to compare : " + element + " , and returning dist: " + distance) ;
        return distance;
    }

    private static int h_LinearConflict(Node initialNode, Node goalNode) {
//HOY NAAI KORA !!! RIGHT CHECK LAAGBE NOT EXACTLY RIGHT BLOCK TA !!!
        int numConflicts = Helper.getNumLinearConflicts(initialNode, goalNode);
        int manHattanDistance = h_manhattanDistance(initialNode, goalNode);
        int x = manHattanDistance + (2 * numConflicts);
        return x;
    }

    public static int f_function(Node node, Node goalNode) {
        int h = Initializer.h_function(node, goalNode);
        int g = node.g_value;

        return (h + g);
    }

//===================================================================UTILITY FUNCTIONS=========================================================
    public static int[][] getCopy(int[][] arr) {
        int[][] newArr = new int[arr.length][arr.length];
        for (int i = 0; i < newArr.length; i++) {
            System.arraycopy(arr[i], 0, newArr[i], 0, newArr[i].length);
        }
        return newArr;
    }

    private static int countInversions(int[][] doubleArray) {
        int[] arr = Initializer.getSingleArray(doubleArray);

        int cnt = 0;
        for (int i = 0; i < arr.length - 1; i++) {
            for (int j = i + 1; j < arr.length; j++) {
                if ((arr[i] > arr[j]) && (arr[i] != 0) && (arr[j] != 0)) {

                    cnt++;
                }
            }
        }

        return cnt;

    }

    public static int[] getSingleArray(int[][] doubleArr) {
        int[] arr = new int[doubleArr.length * doubleArr.length];
        for (int i = 0; i < doubleArr.length; i++) {
            System.arraycopy(doubleArr[i], 0, arr, i * (doubleArr.length), doubleArr[i].length);
        }
        return arr;
    }

    public static void printSingleArray(int[] arr) {
        for (int x : arr) {
            System.out.print(x + ", ");
        }
        System.out.println("");
    }

    public static void printDoubleArray(int[][] doubleArr) {
        for (int i = 0; i < doubleArr.length; i++) {
            for (int j = 0; j < doubleArr[i].length; j++) {
                System.out.print(doubleArr[i][j] + ", ");
            }
            System.out.println("");
        }
    }

    public static String getDoubleArray(int[][] arr) {
        String s = "";
        for (int[] arr1 : arr) {
            for (int j = 0; j < arr1.length; j++) {
                s += (arr1[j] + " ");
            }
            s += "\n";
        }
        return s;
    }
}
/*
    public static int[][] takeInput() {
//        n_in_Main = 16;

        //File Input of initial matrix ....
        Scanner sc = null;
        boolean fromFile = true;
        int[][] arr = null;

        try {
            sc = new Scanner(new File(Writer.inputFile));
            if (sc.hasNext()) {
                Main.n_in_Main = sc.nextInt();
                Main.k_in_Main = (int) Math.sqrt(Main.n_in_Main + 1);
                System.out.println("In Main, n = " + Main.n_in_Main + " , k = " + Main.k_in_Main);
                arr = new int[Main.k_in_Main][Main.k_in_Main];
            }
            int rowIdx = 0, colIdx = 0;
            while (sc.hasNext()) {
                int nextNum = sc.nextInt();
                arr[rowIdx][colIdx] = nextNum;
                if (colIdx >= (Main.k_in_Main - 1)) {
                    rowIdx++;
                    colIdx = 0;
                } else {
                    colIdx++;
                }

            }
        } catch (FileNotFoundException e) {

            fromFile = false;
            System.out.println("FILE Problems ... Enter in Console .. Enter n (9 or 16) then array.... ");
            sc = new Scanner(System.in);

        }

        if (fromFile == false) {
            Main.n_in_Main = sc.nextInt();
            Main.k_in_Main = (int) Math.sqrt(Main.n_in_Main + 1);
            System.out.println("In Main, n = " + Main.n_in_Main + " , k = " + Main.k_in_Main);
            arr = new int[Main.k_in_Main][Main.k_in_Main];
            for (int i = 0; i < Main.k_in_Main; i++) {
                for (int j = 0; j < Main.k_in_Main; j++) {
                    arr[i][j] = sc.nextInt();
                }
            }
        }
        System.out.println("Printing double array .... \n" + Initializer.getDoubleArray(arr));

        return arr;
    }
 */
