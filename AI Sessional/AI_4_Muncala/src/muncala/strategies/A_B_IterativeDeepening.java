package muncala.strategies;

import java.util.List;
import muncala.GameState;

import static muncala.interfaces.Util.MAX_DEPTH;
import static muncala.interfaces.Util.TIMEOUT_MILLI_SECONDS;

public class A_B_IterativeDeepening extends Strategies {

    Strategies alphaBeta;
    int maxDepth;
    MyBufferList listBuffer;
    Pair bestChoice;
    public A_B_IterativeDeepening(String playerType) {
        this.alphaBeta = new AlphaBeta(playerType);
//        this.alphaBeta = new MiniMax(playerType);
        super.setPlayerType(playerType);
        super.setStrategyName(Strategies.ALPHA_BETA_ITERATIVE_DEEPENING);
        this.maxDepth = MAX_DEPTH;
        this.listBuffer = new MyBufferList();
    }

    @Override
    public int getBestMove(GameState state, int depth) {
        this.listBuffer.clearBuffer();
        return getBestMoveFromIterativeDeepening();
    }

    public int getBestMoveFromIterativeDeepening() {
        Thread[] threads_arr = new Thread[MAX_DEPTH];
        Task_Runner_Depth[] myTasks = new Task_Runner_Depth[MAX_DEPTH];

        for (int i = 1; i < myTasks.length; i++) {
            int depth = i;
            myTasks[i] = new Task_Runner_Depth(("Task_" + i), alphaBeta, super.currentGameState, listBuffer, depth);
        }

        for (int i = 0; i < threads_arr.length; i++) {
            threads_arr[i] = new Thread(myTasks[i]);
        }
        for (int i = 0; i < threads_arr.length; i++) {
            threads_arr[i].start();
        }

        try {
            Thread.sleep(TIMEOUT_MILLI_SECONDS);
        } catch (InterruptedException ex) {
        }

        for (int i = 1; i < threads_arr.length; i++) {  //thread[0] aagei morey
            threads_arr[i].stop();  //volatile boolean doesnot work since THREADS go to recursion
        }

//        System.out.println("-->>>ALL OTHER THREADS ARE STOPPED !! ??? ");
        List<Pair> list = this.listBuffer.getBufferList();
//        System.out.println("-->>Printing buffer ... size = " + list.size());
        for (Pair b : list) {
//            System.out.println(b.toString());
        }
        Pair maxPair = null, currentPair;
        for (int i = 0; i < list.size(); i++) {
            if (i == 0) {
                maxPair = list.get(i);
            } else {
                currentPair = list.get(i);
                if (maxPair.depth < currentPair.depth) {
                    maxPair = currentPair;
                }
            }
        }
        this.bestChoice = maxPair;
        return maxPair.bestIdx;
    }

    @Override
    protected int max(GameState state, int alpha, int beta, int depth) {
        return alphaBeta.max(state, alpha, beta, depth);
    }

    @Override
    protected int min(GameState state, int alpha, int beta, int depth) {
        return alphaBeta.min(state, alpha, beta, depth);
    }

    @Override
    protected int max(GameState state, int depth) {
        return alphaBeta.max(state, depth);
    }

    @Override
    protected int min(GameState state, int depth) {
        return alphaBeta.min(state, depth);
    }

}
