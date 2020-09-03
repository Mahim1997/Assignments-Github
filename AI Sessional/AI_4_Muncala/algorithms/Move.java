package muncala.algorithms;

import java.util.Objects;
import muncala.GameState;

public class Move {
    GameState newState;
    int idxMoveFromParent;

    public GameState getNewState() {
        return newState;
    }

    public void setNewState(GameState newState) {
        this.newState = newState;
    }

    public int getIdxMoveFromParent() {
        return idxMoveFromParent;
    }

    public void setIdxMoveFromParent(int idxMoveFromParent) {
        this.idxMoveFromParent = idxMoveFromParent;
    }

    public Move(GameState newState, int idxMoveFromParent) {
        this.newState = newState;
        this.idxMoveFromParent = idxMoveFromParent;
    }

    @Override
    public int hashCode() {
        int hash = 7;
        hash = 71 * hash + Objects.hashCode(this.newState);
        hash = 71 * hash + this.idxMoveFromParent;
        return hash;
    }

    @Override
    public boolean equals(Object obj) {
        if (this == obj) {
            return true;
        }
        if (obj == null) {
            return false;
        }
        if (getClass() != obj.getClass()) {
            return false;
        }
        final Move other = (Move) obj;
        if (this.idxMoveFromParent != other.idxMoveFromParent) {
            return false;
        }
        if (!Objects.equals(this.newState, other.newState)) {
            return false;
        }
        return true;
    }
    
}
