package muncala.strategies;

import muncala.GameState;
 
public class Task_Runner_Depth implements Runnable{

    public volatile boolean keepRunning = true;
    Strategies whichPlayer;
    GameState state;

    MyBufferList listBuffer;
    
    int depthToSearch;
    String name;

    public Task_Runner_Depth(String n, Strategies whichPlayer, GameState state, MyBufferList bl, int d) {
        this.whichPlayer = whichPlayer;
        this.state = state;
        this.depthToSearch = d;
        this.name = n;
        this.listBuffer = bl;
    }

    @Override
    public void run() {
        int bestMove = this.whichPlayer.getBestMove(state, depthToSearch);
        listBuffer.writeToBuffer(depthToSearch, bestMove);
    }

}
