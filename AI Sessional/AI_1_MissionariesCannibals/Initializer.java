package problem1;

import java.util.List;
/*
The missionaries and cannibals problem, which is a famous problem in AI,  is usually stated as follows. 
Three missionaries and three cannibals are on one side of a river, along with a boat that can hold one or two people. 
Find a way to get everyone to the other side without ever leaving a group of missionaries in one place outnumbered by the cannibals in that place. 

In this assignment, your task is to do the problem formulation so it can be solved by searching, 
and do the computer implementation in order to experimentally compare the performance of the BFS and the DFS search strategy. 
For performance comparison, you may use time, number of nodes explored, number of nodes expanded, effective branching factor etc.

In addition, your computer implementation need to be able to deal with a scaled-up version of this problem (for example, a problem with 
five missionaries and five cannibals). The implementation may have m number of missionaries, c number of cannibals, 
k number of maximum allowable passengers in the boat. There should a search cut-off limit 
(for example, termination after 30 seconds, or after 1,000,000 nodes have been expanded) 
which you should be able to vary.
*/
public class Initializer {
//7, 7, 4
//These are just like that
    public static int numberOfCannibalsInitial = 3; 
    public static int numberOfMissionariesInitial = 3;
    public static int maxNumberOfPeopleInBoat = 2;

    private static final String initalBoatLocation = "L";
    public static int maxNumberOfExpandedNodes = 100000;
    public static String bfsFileName = "BFS Output.txt";
    public static String dfsFileName = "DFS Output.txt";
    public static String compareFile = "Comparisons For BFS and DFS.txt";
    public static String inputFile = "Input.txt";

    
    public static long maxTimeInMs = 100000; //10s
    static String outputFile = "Output Moves.txt";
    
    public static Node getInitialNode() {
        Node node = new Node(Initializer.numberOfCannibalsInitial, Initializer.numberOfMissionariesInitial, initalBoatLocation, 0, 0);
        return node;
    }

    public static Node getFinalNode() {
        Node node = new Node(0, 0, initalBoatLocation, Initializer.numberOfMissionariesInitial, Initializer.numberOfCannibalsInitial);
        node.reverseLocation();
        return node;
    }

    public static String getLocationName(String s) {
        if (s.contains("l") || s.contains("L")) {
            return "Left ";
        } else {
            return "Right";
        }
    }

    public static String getNextLocation(String s) {
        if (s.equalsIgnoreCase("L")) {
            return "R";
        } else {
            return "L";
        }
    }

    public static void printList(List<Node> list) {
        System.out.println("");
        list.forEach((n) -> {
            System.out.println(n.toString());
        });
        System.out.println("");
    }

    public static String getStringForNode(String c, int num) {
        String s = "";
        for (int i = 0; i < num; i++) {
            s += c;
        }
        return s;
    }

}
 