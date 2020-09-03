package a_star_search;

import java.util.*;

class Processor {

    public Set<Node> closeList; // Expanded/Visited List ... Whichever POPPED i.e. which was already expanded

    PriorityQueue<Node> priorityQueue;  //This is the open list

    public Node initialNode;
    public Node goalNode;

    public Processor(Node initialNode, Node goalNode) {
        this.closeList = new HashSet<>();

        this.initialNode = initialNode;
        this.goalNode = goalNode;

        this.priorityQueue = new PriorityQueue<>((Node o1, Node o2) -> {
            if (o1.f_value > o2.f_value) {
                return 1;
            } else if (o1.f_value < o2.f_value) {
                return -1;
            } else {
                return 0;
            }
        });
    }

    public void process() {
        this.initialNode.g_value = 0;
        this.priorityQueue.add(this.initialNode);
        this.initialNode.setF_value(Initializer.f_function(this.initialNode, this.goalNode)); //f(n) = g(n) + h(n)

        int iter = 0;

        Node current = null;

        System.out.println("Processing ..... ");

        while (priorityQueue.isEmpty() == false) {    //Priority Queue is the open set
            current = priorityQueue.remove(); //Node having the lowest 'f' value...
            
            if (current.equals(this.goalNode)) {  //CHECKING IF GOAL NODE OR NOT !
                System.out.println("--->>>GOAL NODE FOUND !!!");
                constructPath(current);
                return;
            }
            for (Node neighbour : current.getNeighbours()) {
                if ((this.closeList.contains(neighbour) == true) || (neighbour == null)) {
                    continue;
                }
                int tentative_g_score = current.getG_value() + 1;   //DISTANCE between nodes ++1
                neighbour.parentNode = current;
                neighbour.g_value = tentative_g_score;
                neighbour.f_value = neighbour.g_value + neighbour.get_h();
                this.priorityQueue.add(neighbour);
            }
            this.closeList.add(current);
        }

    }

    private void constructPath(Node current) {
        Stack<Node> stack = new Stack<>();
        Node node = current;
        while (true) {
            if (node.equals(this.initialNode)) {
                stack.add(node);
                break;
            }
            stack.add(node);
            node = node.parentNode;
        }
        //Pop Priority queue into a SET then calculate that set's size to get correct number of EXPLORED nodes #
        Set<Node> set = new HashSet<>();
        while (this.priorityQueue.isEmpty() == false) {
            Node head = this.priorityQueue.remove();
            set.add(head);
        }

        String str = "Number of expanded nodes = " + this.closeList.size() + " , number of explored nodes = "
                + (set.size() + this.closeList.size()); //INSTEAD OF this.priorityQueue.size() , use set.size()
        Writer.writeOutput(str); //priority queue teh any time thaka manei explored ... 
        //any time chilo -> closedList , akhon ase -> priority queue
        String str2 = "Number of expanded nodes = " + this.closeList.size() + "\nNumber of explored nodes = "
                + (set.size() + this.closeList.size());
        String strAnalysis = "\n" + str2 + "\nNumber of moves needed : " + (stack.size() - 1) + "\n";
        Writer.writeAnalysis(strAnalysis);
        System.out.println("\n<======== Using " + Initializer.getCurrentMethod() + " ========>" + strAnalysis);
        Writer.writeOutput("Number of steps = " + (stack.size() - 1) + ", Printing steps to reach goal node ....");
        while (stack.isEmpty() == false) {
            node = stack.pop();
            Writer.writeOutput(node.toString());
            Writer.writeOutput("\n");
        }

    }

    public String coord(int row, int col) {
        return ("(" + row + ", " + col + ")");
    }

    public static void printList(List<Node> list) {
        list.forEach(System.out::println);
    }

    public static void print2DArray(int[][] arr) {
        for (int[] arr1 : arr) {
            for (int i = 0; i < arr1.length; i++) {
                System.out.print(arr1[i] + ", ");
            }
            System.out.println("");
        }
    }

    public void printArray(int[] arr) {
        for (int x : arr) {
            System.out.print(x + ", ");
        }
        System.out.println("");
    }

    public void process_with_HashMap() {
        HashMap<Node, Integer> map = new HashMap<>();   //Has the node, and 'g' for each node
        this.initialNode.setG_value(0);
        this.priorityQueue.add(this.initialNode);

        map.put(this.initialNode, this.initialNode.getG_value());

        this.initialNode.setF_value(Initializer.f_function(this.initialNode, this.goalNode)); //f(n) = g(n) + h(n)

        int iter = 0;

        Node current = null;

        System.out.println("Processing ..... ");

        while (priorityQueue.isEmpty() == false) {    //Priority Queue is the open set
            //Remove current from openSet
            current = priorityQueue.remove(); //Node having the lowest 'f' value...

            //Add current to closed list
            this.closeList.add(current);

            if (current.equals(this.goalNode)) {  //CHECKING IF GOAL NODE OR NOT !
                System.out.println("--->>>GOAL NODE FOUND !!!");
                constructPath(current);
                return;
            }

            for (Node neighbour : current.getNeighbours()) {

                if ((this.closeList.contains(neighbour) == true) || (neighbour == null)) {
                    //Already present in closedList .. so continue;
                    continue;
                }

                int tentative_g_score = current.getG_value() + 1;   //DISTANCE between nodes ++1

                //EVEN IF PRIORITY QUEUE TEH AASE, KONO PROBLEM NAI, ektar beshi ashle no problem karon uporer ta i.e. lowest cost er tai popped hobe
                //If neighbour has a 'g' less than/equal to current.g + 1 , then CHANGE neighbour node's 'g' and add it to priorityQueue
                neighbour.parentNode = current;
                if ((map.containsKey(neighbour) == true) && (this.priorityQueue.contains(neighbour) == true)) {

                    //Already neighbour was present ..... 
                    int g_of_neighbour = map.get(neighbour);
                    if (tentative_g_score < g_of_neighbour) {
                        //ONLY NOW WE NEED TO UPDATE 'G' OF THE neighbour
                        neighbour.g_value = tentative_g_score;
                        neighbour.f_value = neighbour.g_value + neighbour.get_h();
                        map.replace(neighbour, neighbour.g_value);  //UPDATE KEY IN HASHMAP
                        this.priorityQueue.remove(neighbour);
                        this.priorityQueue.add(neighbour);
                    }

                } else {

                    neighbour.g_value = tentative_g_score;
                    neighbour.f_value = neighbour.g_value + neighbour.get_h();

                    this.priorityQueue.add(neighbour);
                    map.put(neighbour, neighbour.getG_value());
                }
            }

        }

    }

}
