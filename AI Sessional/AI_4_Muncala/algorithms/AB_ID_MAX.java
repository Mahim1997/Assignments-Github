package muncala.algorithms;

import java.util.ArrayList;
import java.util.List;
import muncala.GameState;
import muncala.interfaces.Util;
import static muncala.interfaces.Util.*;

public class AB_ID_MAX {

    private ListBuffer listBuffer;
    private MiniMax_PlayerMax maxPlayer;
    private GameState state;

    public void setState(GameState s) {
        listBuffer.buffer = new ArrayList<>();
        this.state = s;
    }

    public AB_ID_MAX(GameState s, MiniMax_PlayerMax m) {
        this.maxPlayer = m;
        this.listBuffer = new ListBuffer();
        this.state = s;
    }

    public int runIterativeDeepening() {
        Thread[] threads_arr = new Thread[MAX_DEPTH];
        MyTask[] myTasks = new MyTask[MAX_DEPTH];

        for (int i = 1; i < myTasks.length; i++) {
            int depth = i;
            myTasks[i] = new MyTask(("Task_" + i), maxPlayer, state, listBuffer, depth);
        }

        for (int i = 0; i < threads_arr.length; i++) {
            threads_arr[i] = new Thread(myTasks[i]);
        }
        for (int i = 0; i < threads_arr.length; i++) {
            threads_arr[i].start();
        }

        try {
            Thread.sleep(Util.TIMEOUT_MILLI_SECONDS);
//            Thread.sleep(700);
        } catch (InterruptedException ex) {
            //After waking up just stop the task later ....
//            System.out.println("=_-_=> Main interrupted excpeiton !! ");
        }
//        System.out.println("==>>>MAIN THREAD IS AWAKENED !!!");

        for (int i = 1; i < threads_arr.length; i++) {  //thread[0] aagei morey
//            myTasks[i].stopThread();
            threads_arr[i].stop();
        }

//        System.out.println("-->>>ALL OTHER THREADS ARE STOPPED !! ??? ");
        List<Buffer> list = this.listBuffer.getBufferList();
//        System.out.println("-->>Printing buffer ... size = " + list.size());
        for (Buffer b : list) {
//            System.out.println(b.toString());
        }
        Buffer maxBuff = null, currentBuff;
        for (int i = 0; i < list.size(); i++) {
            if (i == 0) {
                maxBuff = list.get(i);
            } else {
                currentBuff = list.get(i);
                if (maxBuff.depth < currentBuff.depth) {
                    maxBuff = currentBuff;
                }
            }
        }
//        System.out.println("->maxBuff.depth = " + maxBuff.depth + " , bestIdx = " + maxBuff.bestIdx);
        return maxBuff.bestIdx;

//        return -1;
    }

}

class MyTask implements Runnable {

    public volatile boolean keepRunning = true;
    MiniMax_PlayerMax maxPlayer;
    GameState state;
    final ListBuffer listBuffer;
    int depthToSearch;
    String name;

    public MyTask(String n, MiniMax_PlayerMax maxPlayer, GameState state, ListBuffer listBuffer, int d) {
        this.maxPlayer = maxPlayer;
        this.state = state;
        this.listBuffer = listBuffer;
        this.depthToSearch = d;
        this.name = n;
    }

    public void stopThread() {
//        System.out.println("->=>->=> Calling stopThread in name = " + name + " , depth = " + depthToSearch);
        this.keepRunning = false;
    }

    @Override
    public void run() {
//        while (keepRunning == true) {

//        while (!Thread.interrupted()) {
//            System.out.println("For name = " + name + " , depth = " + depthToSearch + " , keepRunning = " + keepRunning);
//            System.out.println("=..=..==> Inside run() of name = " + name + " , depth = " + depthToSearch + " , keppRunning = " + keepRunning);
        int bestMove = this.maxPlayer.getBestMove(state, depthToSearch);
//        System.out.println("->Best move got for depth = " + depthToSearch + " is idx = " + bestMove);
        listBuffer.writeToBuffer(depthToSearch, bestMove);
//        }
//        System.out.println("-->>Exiting run() method for depth = " + depthToSearch);
    }

}
