package problem1;

import java.util.*;

public class Helper {

    public Helper() {

    }

    public List<Node> getChildren(Node givenNode) {
        List<Node> list = new ArrayList<>();

        int maxlimit = Initializer.maxNumberOfPeopleInBoat;

        int numCan, numMiss, numCanOpp, numMissOpp;
        String nowAt, nextAt, move;
        if (givenNode.boatLocation.equalsIgnoreCase("L")) {
            numCan = givenNode.numCanLeft;
            numMiss = givenNode.numMisLeft;
            numCanOpp = givenNode.numCanRight;
            numMissOpp = givenNode.numMisRight;
            nowAt = givenNode.boatLocation;
            nextAt = Initializer.getNextLocation(nowAt);

        } else {
            numCanOpp = givenNode.numCanLeft;
            numMissOpp = givenNode.numMisLeft;
            numCan = givenNode.numCanRight;
            numMiss = givenNode.numMisRight;
            nowAt = givenNode.boatLocation;
            nextAt = Initializer.getNextLocation(nowAt);
        }
        //Move only cannibals ....
        for (int i = 1; i <= maxlimit; i++) {
            if (numCan >= i) {
                int newNumCan = numCan;
                int newNumCanOpp = numCanOpp;
                newNumCan -= i;
                newNumCanOpp += i;
                move = "Move " + i + " cannibals from " + Initializer.getLocationName(nowAt) + " to " + Initializer.getLocationName(nextAt);
                Node node = formNode(nowAt, newNumCan, numMiss, numMissOpp, newNumCanOpp, givenNode, move);
                list.add(node);
            }
        }

        //Move only missionaries
        for (int i = 1; i <= maxlimit; i++) {
            if (numMiss >= i) {
                int newNumMiss = numMiss;
                int newNumMissOpp = numMissOpp;
                newNumMiss -= i;
                newNumMissOpp += i;
                move = "Move " + i + " missionaries from " + Initializer.getLocationName(nowAt) + " to " + Initializer.getLocationName(nextAt);
                Node node = formNode(nowAt, numCan, newNumMiss, newNumMissOpp, numCanOpp, givenNode, move);
                list.add(node);
            }
        }

        //Move both ...
        for (int total = 1; total <= maxlimit; total++) {
            for (int cannibalIter = 1; cannibalIter <= total; cannibalIter++) {
                int missionaryIter = total - cannibalIter;
                if ((missionaryIter >= 1) && (numCan >= cannibalIter) && (numMiss >= missionaryIter)) {
                    //OTHERWISE just cannibalIter
                    int newNumMiss = numMiss;
                    int newNumMissOpp = numMissOpp;
                    int newNumCan = numCan;
                    int newNumCanOpp = numCanOpp;
                    newNumCan -= cannibalIter;
                    newNumCanOpp += cannibalIter;
                    newNumMiss -= missionaryIter;
                    newNumMissOpp += missionaryIter;
//  public Node(int numCanLeft, int numMisLeft, String boatLocation, int numMisRight, int numCanRight) 
                    move = "Move " + cannibalIter + " cannibals and " + missionaryIter + " missionaries from " + Initializer.getLocationName(nowAt) + " to " + Initializer.getLocationName(nextAt);
                    Node node = formNode(nowAt, newNumCan, newNumMiss, newNumMissOpp, newNumCanOpp, givenNode, move);
                    list.add(node);
                }
            }
        }

        return list;
    }

    public Node formNode(String nowAt, int numCan, int numMiss, int numMissOpp, int numCanOpp, Node givenNode, String move) {
        if (nowAt.equalsIgnoreCase("L")) {  //children's boat should be at "R"
            Node node = new Node(numCan, numMiss, "R", numMissOpp, numCanOpp); //public Node(int numCanLeft, int numMisLeft, String boatLocation, int numMisRight, int numCanRight)
            node.parentNode = givenNode;
            node.moveFromParent = move;

            return node;

        } else {        //children's boat should be at "L"
            Node node = new Node(numCanOpp, numMissOpp, "L", numMiss, numCan);
            node.parentNode = givenNode;
            node.moveFromParent = move;

            return node;

        }

    }

    public int printFinalPath(Node finalNode, String which, List<Node> visitedList, long startTime, long endTime) {
        if (which.toUpperCase().contains("BFS".toUpperCase())) {
            List<Node> finalPath = new ArrayList<>();
            Stack<Node> stack = new Stack<>();
            Node node = finalNode;
            while (true) {
                if (node == null) {
                    break;
                }
                stack.push(node);
                node = node.parentNode;

            }
            while (stack.isEmpty() == false) {
                Node n = stack.pop();
                finalPath.add(n);
            }

            int x = printVisitedList(visitedList, which);

            Writer.writeOutput("\n\n=====================PRINTING Final Solution(BFS) ====================\n\n");
            String str = "Parameters are, Num Cannibals: " + Initializer.numberOfCannibalsInitial
                        + " , Num Missionaries: " + Initializer.numberOfMissionariesInitial
                        + ", Max num people in a boat: " + Initializer.maxNumberOfPeopleInBoat
                        + ", Max num of expanded nodes(limit): " + Initializer.maxNumberOfExpandedNodes
                        + ", Max Time in sec : " + (Initializer.maxTimeInMs/1000);
            Writer.writeOutput(str);
            Writer.writeOutput("\nNumber of moves required(BFS) = " + (finalPath.size() - 1) + "\n");
            Node nowNode, nextNode;
            Writer.writeOutput(finalPath.get(0).print());
//            for (int i = 0; i < (finalPath.size() - 1); i++) {
            for (int i = 0; i < (finalPath.size() - 1); i++) {
                //nowNode = finalPath.get(i);
                nextNode = finalPath.get(i + 1);
//                nextNode = finalPath.get(i + 1);
                String s = new String();
                s = "\n\t\t\t\t\t\t\t\t" + nextNode.moveFromParent + "\n\n";//+ nextNode.print();
                if (nextNode.boatLocation.equalsIgnoreCase("L")) {
                    s += nextNode.print();
                } else {
                    s += ("\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t" + nextNode.print());
                }
//                s = (i+1) + ". " +  nowNode.print() + " -->>  " + nextNode.moveFromParent + "  --->>> " + nextNode.print();

                Writer.writeOutput(s);
            }

            Writer.writeOutput("\n\n=====================DONE PRINTING Final Solution(BFS)====================\n\n");

            return x;
        } else {
            List<Node> finalPath = new ArrayList<>();
            Stack<Node> stack = new Stack<>();
            Node node = finalNode;
            while (true) {
                if (node == null) {
                    break;
                }
                stack.push(node);
                node = node.parentNode;

            }
            while (stack.isEmpty() == false) {
                Node n = stack.pop();
                finalPath.add(n);
            }

            int x = printVisitedList(visitedList, which);

            Writer.writeOutput("\n\n=====================PRINTING Final Solution(DFS)====================\n\n");
            String str = "Parameters are, Num Cannibals: " + Initializer.numberOfCannibalsInitial
                        + " , Num Missionaries: " + Initializer.numberOfMissionariesInitial
                        + ", Max num people in a boat: " + Initializer.maxNumberOfPeopleInBoat
                        + ", Max num of expanded nodes(limit): " + Initializer.maxNumberOfExpandedNodes
                        + ", Max Time in sec : " + (Initializer.maxTimeInMs/1000);
            Writer.writeOutput(str);
            Writer.writeOutput("\nNumber of moves required(DFS) = " + (finalPath.size() - 1) + "\n");
            Node nowNode, nextNode;
            Writer.writeOutput(finalPath.get(0).print());
            for (int i = 0; i < (finalPath.size() - 1); i++) {
//                nowNode = finalPath.get(i);
                nextNode = finalPath.get(i + 1);
                String s = new String();
                s = "\n\t\t\t\t\t\t\t\t" + nextNode.moveFromParent + "\n\n";//+ nextNode.print();
                if (nextNode.boatLocation.equalsIgnoreCase("L")) {
                    s += nextNode.print();
                } else {
                    s += ("\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t" + nextNode.print());
                }
//                s = (i+1) + ". " + nowNode.print() + " -->>  " + nextNode.moveFromParent + "  --->>> " + nextNode.print();
                Writer.writeOutput(s);
            }
            Writer.writeOutput("\n\n=====================DONE PRINTING Final Solution(DFS)====================\n\n");

            return x;
        }

    }

    public int printVisitedList(List<Node> visitedList, String which) {
        /*
        if (which.toUpperCase().contains("BFS".toUpperCase())) {
            Writer.writeComparison("\n============ Print final visited list for BFS ==================\n");
        } else {
            Writer.writeComparison("\n============ Print final visited list for DFS ==================\n");
        }
        for (int iter = 0; iter < visitedList.size(); iter++) {
            Node n = visitedList.get(iter);
            if (which.toUpperCase().contains("BFS".toUpperCase())) {
                Writer.writeComparison((iter + 1) + ". " + n.print());
            } else {
                Writer.writeComparison((iter + 1) + ". " + n.print());
            }

        }
        if (which.toUpperCase().contains("BFS".toUpperCase())) {
            Writer.writeComparison("\n============ DONE Print final visited list for BFS ==================\n");
        } else {
            Writer.writeComparison("\n============ DONE Print final visited list for DFS ==================\n");
        }
         */
        return visitedList.size();
    }

}
/*
    public int printVisitedList(List<Node> visitedList, String which) {

        if (which.toUpperCase().contains("BFS".toUpperCase())) {
            Writer.writeToBFS("\n============ Print final visited list ==================\n");
        } else {
            Writer.writeToDFS("\n============ Print final visited list ==================\n");
        }
        for (int iter = 0; iter < visitedList.size(); iter++) {
            Node n = visitedList.get(iter);
            if (which.toUpperCase().contains("BFS".toUpperCase())) {
                Writer.writeToBFS((iter + 1) + ". " + n.print());
            } else {
                Writer.writeToDFS((iter + 1) + ". " + n.print());
            }

        }
        return visitedList.size();
    }

 */

 /*
    public int printFinalPath(Node finalNode, String which, List<Node> visitedList) {
        if (which.toUpperCase().contains("BFS".toUpperCase())) {
            List<Node> finalPath = new ArrayList<>();
            Stack<Node> stack = new Stack<>();
            Node node = finalNode;
            while (true) {
                if (node == null) {
                    break;
                }
                stack.push(node);
                node = node.parentNode;

            }
            while (stack.isEmpty() == false) {
                Node n = stack.pop();
                finalPath.add(n);
            }

            int x = printVisitedList(visitedList, which);

            Writer.writeToBFS("\n\n=====================PRINTING Final Solution(BFS)====================\n\n");
            Node nowNode, nextNode;
            for (int i = 0; i < (finalPath.size() - 1); i++) {
                nowNode = finalPath.get(i);
                nextNode = finalPath.get(i + 1);
                String s;

                s = nowNode.print() + " -->>  " + nextNode.moveFromParent + "  --->>> " + nextNode.print();
                Writer.writeToBFS(s);
            }
            Writer.writeToBFS("\n\n=====================DONE PRINTING Final Solution(BFS)====================\n\n");

            return x;
        } else {
            List<Node> finalPath = new ArrayList<>();
            Stack<Node> stack = new Stack<>();
            Node node = finalNode;
            while (true) {
                if (node == null) {
                    break;
                }
                stack.push(node);
                node = node.parentNode;

            }
            while (stack.isEmpty() == false) {
                Node n = stack.pop();
                finalPath.add(n);
            }

            int x = printVisitedList(visitedList, which);

            Writer.writeToDFS("\n\n=====================PRINTING Final Solution(DFS)====================\n\n");
            Node nowNode, nextNode;
            for (int i = 0; i < (finalPath.size() - 1); i++) {
                nowNode = finalPath.get(i);
                nextNode = finalPath.get(i + 1);
                String s;

                s = nowNode.print() + " -->>  " + nextNode.moveFromParent + "  --->>> " + nextNode.print();
                Writer.writeToDFS(s);
            }
            Writer.writeToDFS("\n\n=====================DONE PRINTING Final Solution(DFS)====================\n\n");

            return x;
        }

    }
 */

 /*
   public Helper(String whichMethod) {
        if(whichMethod.toUpperCase().equals("BFS".toUpperCase())){
            fileName = Initializer.bfsFileName;
        }else{
            fileName = Initializer.dfsFileName;
        }
    }
 */
