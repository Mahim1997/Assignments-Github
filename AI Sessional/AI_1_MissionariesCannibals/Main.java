package problem1;

import java.io.*;
import java.util.*;
import java.util.logging.Level;
import java.util.logging.Logger;

public class Main {

    public static void main(String[] args) {

        Writer.eraseComparison();
        Writer.eraseOutput("");
        File file = new File(Initializer.inputFile);
        Scanner sc;

        try {
            sc = new Scanner(file);
            while (true) {
                if (sc.hasNext() == false) {
                    break;
                }
                initial(sc);

                System.out.println("<<From File>>Getting parameters...");

                System.out.println("Parameters are, Num Cannibals: " + Initializer.numberOfCannibalsInitial
                        + " , Num Missionaries: " + Initializer.numberOfMissionariesInitial
                        + ", Max num people in a boat: " + Initializer.maxNumberOfPeopleInBoat
                        + ", Max num of expanded nodes(limit): " + Initializer.maxNumberOfExpandedNodes
                        + ", Max Time in sec : " + (Initializer.maxTimeInMs/1000));

                System.out.println("Running BFS and DFS ... ");
                runProgram();
            }
        } catch (FileNotFoundException ex) {
            System.out.println(".... Enter from console .... file Input.txt doesn't exist ... \n"
                    + "Enter number of cannibals, number of missionaries , "
                    + "max number of people in boats, "
                    + "max number of nodes can be expanded(limit) "
                    + "and maximum time limit in seconds.");
            sc = new Scanner(System.in);
            initial(sc);
            System.out.println("Running BFS and DFS ... ");
            runProgram();
        }

    }
/*
169
198
13
10000000
31
297
329
17
1000000
22
*/
    private static void initial(Scanner sc) {
//        Scanner sc = null;

        try {
            int c = sc.nextInt();
            Initializer.numberOfCannibalsInitial = c;
            int m = sc.nextInt();
            Initializer.numberOfMissionariesInitial = m;
            int k = sc.nextInt();
            Initializer.maxNumberOfPeopleInBoat = k;
            int maxLimit = sc.nextInt();
            Initializer.maxNumberOfExpandedNodes = maxLimit;
            long maxTimeInSec = sc.nextLong();
            Initializer.maxTimeInMs = maxTimeInSec*1000;
        } catch (Exception e) {
            e.printStackTrace();
            return;
        }
    }


    private static void runProgram() {
//        initial();
        try {
            Writer.writeComparison("------------------------------------------------------<<<<< >>>>>>---------------------------------------------------------\n");
            Runner runnerBfs = new Runner("BFS");
            runnerBfs.runBFS();

            Runner runnerDfs = new Runner("DFS");
            runnerDfs.runDFS();

            Writer.writeComparison("COMPARING For num miss initial = " + Initializer.numberOfMissionariesInitial 
                    + " , num can initial = " + Initializer.numberOfCannibalsInitial 
                    + " , max ppl in boat = " + Initializer.maxNumberOfPeopleInBoat
                    + " , max limit of expanded nodes = " + Initializer.maxNumberOfExpandedNodes
                    + " and max time limit(in seconds) = " + (Initializer.maxTimeInMs/1000));
            Writer.writeComparison("For BFS: Number of nodes expanded was " + runnerBfs.numNodesExpanded + " , and number of "
                    + "nodes explored was " + runnerBfs.numNodesExplored);
            Writer.writeComparison("For DFS: Number of nodes expanded was " + runnerDfs.numNodesExpanded + " , and number of "
                    + "nodes explored was " + runnerDfs.numNodesExplored);

            Writer.writeComparison("For BFS,  Time elapsed in milliseconds was :  " + runnerBfs.timeElapsed);
            Writer.writeComparison("For DFS,  Time elapsed in milliseconds was :  " + runnerDfs.timeElapsed);
            Writer.writeComparison("==========================================<<<<<<   >>>>>>============================================\n");
            Writer.writeComparison("\n\n");
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    private static void test() {
        Node node = Initializer.getInitialNode();
        System.out.println(node);
        Helper helper = new Helper();

        List<Node> list = helper.getChildren(node);
        Initializer.printList(list);

        System.out.println("-------------------------------");
        list = helper.getChildren(list.get(2));
        Initializer.printList(list);
    }

}
