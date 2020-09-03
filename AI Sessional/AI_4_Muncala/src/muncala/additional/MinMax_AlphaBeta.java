package muncala.additional;

import java.util.ListIterator;
import muncala.GameState;

public class MinMax_AlphaBeta {

    int depth;
/*
    public MinMax_AlphaBeta(int depth) {
        this.depth = depth;
    }

    public void alphabeta(GameState g) {
        try {
//            FileOutputStream fileOutputStreamTraverseLog = new FileOutputStream("traverse_log.txt");
//            OutputStreamWriter outputStreamWriterTraverseLog = new OutputStreamWriter(fileOutputStreamTraverseLog);
//            BufferedWriter bufferedWriterTraverseLog = new BufferedWriter(outputStreamWriterTraverseLog);

//            bufferedWriterTraverseLog.write("Node,Depth,Value,Alpha,Beta");
//            bufferedWriterTraverseLog.newLine();
            Node root = new Node("root", g, Double.NEGATIVE_INFINITY, Double.NEGATIVE_INFINITY, Double.POSITIVE_INFINITY, 0, true, null);

            getAllMoves(root, bufferedWriterTraverseLog);
            this.nextState(root);

//            bufferedWriterTraverseLog.close();
//            outputStreamWriterTraverseLog.close();
//            fileOutputStreamTraverseLog.close();
        } catch (Exception ex) {
            System.out.println("Exception in AlphaBeta : " + ex.getMessage());
        }
    }

    public void nextState(Node root)  {
        ListIterator<Node> listIterator = root.children.listIterator();
        while (listIterator.hasNext()) {
            Node temp = listIterator.next();
            if (temp.eval == root.eval) {
                GameState move = null;
                if (temp.state.getGetAnotherTurn()) {
                    Node x = getNextState(temp);
                    move = new Game(x.game);
                } else {
                    move = new Game(temp.game);
                }
                writeNextState(move);
                break;
            }
        }
    }
    public Node getNextState(Node n){
        Node nextMove=null;
        ListIterator<Node> listIterator=n.children.listIterator();
        while(listIterator.hasNext()){
            Node temp=listIterator.next();
            if(temp.eval==n.eval){
                if(temp.state.getGetAnotherTurn()){
                    nextMove=getNextState(temp);
                }
                else{
                    nextMove=new Node(temp);
                    break;
                }
            }
        }
        return nextMove;
    }
*/
}
