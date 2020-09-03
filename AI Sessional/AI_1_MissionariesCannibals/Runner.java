package problem1;

import java.util.*;

public class Runner {

    Helper helper;
    Stack<Node> stack;
    List<Node> closedList;
    public int numNodesExpanded = 0;
    public int numNodesExplored = 0;
    public List<Node> finalPath;
    public long timeElapsed;
    Queue<Node> queue;

    public Runner(String s) {
        this.helper = new Helper();
        this.closedList = new ArrayList<>();
        this.finalPath = new ArrayList<>();
        if (s.toUpperCase().contains("BFS".toUpperCase())) {
            this.queue = new LinkedList<>();
            this.stack = null;
        } else if (s.toUpperCase().contains("DFS".toUpperCase())) {
            this.stack = new Stack<>();
            this.queue = null;
        } else {
            this.stack = null;
            this.queue = null;
            this.helper = null;
            this.closedList = null;
            this.finalPath = null;
        }
    }

    public void runBFS() {
        Node startNode = Initializer.getInitialNode();

        queue.add(startNode);
        long startTime = System.currentTimeMillis();
        while (queue.isEmpty() == false) {  //While queue is not empty  ..... 

            long endTime = System.currentTimeMillis();
            this.timeElapsed = (endTime - startTime);

            this.numNodesExplored = this.closedList.size() + this.queue.size();    //Number of expanded nodes = number of visited nodes + number of nodes yet to be visited.

            if (this.timeElapsed >= Initializer.maxTimeInMs) {
                Writer.writeComparison("\n<Time Limit exceeded for BFS>"
                        + " NO solution exists for BFS .. Initial number of cannibals = " + Initializer.numberOfCannibalsInitial
                        + " , missionaries = " + Initializer.numberOfMissionariesInitial
                        + " , max number of possible nodes expanded = " + Initializer.maxNumberOfExpandedNodes
                        + ", and max time limit in seconds = " + (Initializer.maxTimeInMs / 1000));
                this.numNodesExpanded = helper.printVisitedList(closedList, "BFS");
                return;
            }

            if (this.numNodesExpanded >= Initializer.maxNumberOfExpandedNodes) {
                Writer.writeComparison("\n<Limit exceeded for BFS, max num of expanded nodes>"
                        + " NO solution exists for BFS .. Initial number of cannibals = " + Initializer.numberOfCannibalsInitial
                        + " , missionaries = " + Initializer.numberOfMissionariesInitial
                        + " , max number of possible nodes expanded = " + Initializer.maxNumberOfExpandedNodes
                        + ", and max time limit in seconds = " + (Initializer.maxTimeInMs / 1000));

                this.numNodesExpanded = helper.printVisitedList(closedList, "BFS");
                return;
            }

            Node node = queue.remove();

//            this.numNodesExplored++;
            if (this.closedList.contains(node) == true) {
                continue;
            } else {
                this.closedList.add(node);
                this.numNodesExpanded = this.closedList.size();
                this.numNodesExplored = this.closedList.size() + this.queue.size();
            }

            List<Node> childrenList = this.helper.getChildren(node);

            for (Node n : childrenList) {
                //Check the child whether it is accepting state, if so then terminate ...
                if (n.isFinalNode()) {
                    this.numNodesExplored = this.closedList.size() + this.queue.size();    //If any other child node was added to queue in between
                    this.numNodesExpanded = this.helper.printFinalPath(n, "BFS", this.closedList, startTime, endTime);

                    return;
                }
                //Check if the child is valid node or not, if valid then insert it into queue
                if (n.isValidNode() == true) {
                    if(queue.contains(n)==false){
                        queue.add(n);
                    }
                }

            }

        }

        Writer.writeComparison("\n<Queue becomes empty> No solution exists for BFS .. Initial number of cannibals = " + Initializer.numberOfCannibalsInitial
                + " , missionaries = " + Initializer.numberOfMissionariesInitial
                + " , max number of possible nodes expanded = " + Initializer.maxNumberOfExpandedNodes
                + ", and max time limit in sec = " + (Initializer.maxTimeInMs / 1000));
        helper.printVisitedList(closedList, "BFS");
    }

    public void runDFS() {
        Node startNode = Initializer.getInitialNode();

        stack.push(startNode);
        long startTime = System.currentTimeMillis();
        while (stack.isEmpty() == false) {  //While queue is not empty  ..... 

            long endTime = System.currentTimeMillis();
            this.timeElapsed = (endTime - startTime);
            this.numNodesExplored = this.closedList.size() + this.stack.size();
            if (this.timeElapsed >= Initializer.maxTimeInMs) {
                Writer.writeComparison("\n<Limit exceeded for DFS, max num of expanded nodes>"
                        + " No solution exists for DFS .. Initial number of cannibals = " + Initializer.numberOfCannibalsInitial
                        + " , missionaries = " + Initializer.numberOfMissionariesInitial
                        + " , and max number of possible nodes expanded = " + Initializer.maxNumberOfExpandedNodes);

                this.numNodesExpanded = helper.printVisitedList(closedList, "DFS");

                return;
            }

            if (this.numNodesExpanded >= Initializer.maxNumberOfExpandedNodes) {
                Writer.writeComparison("\n<Limit exceeded for DFS, max num of expanded nodes>"
                        + " No solution exists for DFS .. Initial number of cannibals = " + Initializer.numberOfCannibalsInitial
                        + " , missionaries = " + Initializer.numberOfMissionariesInitial
                        + " , max number of possible nodes expanded = " + Initializer.maxNumberOfExpandedNodes
                        + " , and max time limit(in sec) = " + (Initializer.maxTimeInMs / 1000));

                this.numNodesExpanded = helper.printVisitedList(closedList, "DFS");

                return;
            }

            Node node = stack.pop();

            if (closedList.contains(node) == true) {
                continue;
            } else {
                closedList.add(node);
                this.numNodesExpanded = closedList.size();
                this.numNodesExplored = this.closedList.size() + this.stack.size();
            }

            List<Node> childrenList = helper.getChildren(node);

            for (Node n : childrenList) {
                if (n.isFinalNode()) {
                    this.numNodesExplored = this.closedList.size() + this.stack.size();    //If any other child was added to stack in between
                    this.numNodesExpanded = helper.printFinalPath(n, "DFS", this.closedList, startTime, endTime);

                    return;
                }
                if (n.isValidNode()) {
                    //this.numNodesExplored++;  
                    if(stack.contains(n) == false){
                        stack.push(n);
                    }
                    
                }

            }

        }

        Writer.writeComparison("\n<Stack becomes empty> No solution exists for DFS .. Initial number of cannibals = " + Initializer.numberOfCannibalsInitial
                + " , missionaries = " + Initializer.numberOfMissionariesInitial
                + " , max number of possible nodes expanded = " + Initializer.maxNumberOfExpandedNodes
                + " , and max time limit (in sec) = " + (Initializer.maxTimeInMs / 1000));
        helper.printVisitedList(closedList, "DFS");
    }

}
