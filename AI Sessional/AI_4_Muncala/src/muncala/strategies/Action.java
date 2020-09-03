/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package muncala.strategies;

import java.util.Objects;
import muncala.GameState;

/**
 *
 * @author esfs
 */
public class Action {

    GameState newState;
    int idxMoveFromParent;

    public Action(GameState newState, int idxMoveFromParent) {
        this.newState = newState;
        this.idxMoveFromParent = idxMoveFromParent;
    }

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
        final Action other = (Action) obj;
        if (this.idxMoveFromParent != other.idxMoveFromParent) {
            return false;
        }
        if (!Objects.equals(this.newState, other.newState)) {
            return false;
        }
        return true;
    }

}
