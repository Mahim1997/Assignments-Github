package muncala.additional;

import java.util.LinkedList;
import java.util.List;
import java.util.ListIterator;
import muncala.GameState;

public class Node {

    GameState state;
    List<Node> children;
    String name;

    double eval;
    double alpha;
    double beta;
    int depth;
    boolean isMaxNode;
    Node parent;

    //Default Constructor
    public Node() {
        this.alpha = Double.NEGATIVE_INFINITY;
        this.beta = Double.POSITIVE_INFINITY;
        this.children = new LinkedList<>();
    }
    //Constructor for alpha beta method

    public Node(String name, GameState game, double eval, double alpha, double beta, int depth, boolean max, Node parent) {
        this.name = name;
        this.state = new GameState(game);
        this.eval = eval;
        this.alpha = alpha;
        this.beta = beta;
        this.depth = depth;
        this.isMaxNode = max;
        this.parent = parent;
        this.children = new LinkedList<Node>();
    }

    //Copy Constructor
    public Node(Node x) {
        name = x.name;
        state = new GameState(x.state);
        eval = x.eval;
        alpha = x.alpha;
        beta = x.beta;
        depth = x.depth;
        isMaxNode = x.isMaxNode;
        parent = x.parent;
        children = new LinkedList<Node>();
        ListIterator<Node> listIterator = x.children.listIterator();
        while (listIterator.hasNext()) {
            children.add(listIterator.next());
        }
    }
}
