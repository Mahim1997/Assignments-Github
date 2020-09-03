package muncala.algorithms;

import java.util.ArrayList;
import java.util.List;

public class Buffer {
    int depth;
    int bestIdx;

    public Buffer(int depth, int bestIdx) {
        this.depth = depth;
        this.bestIdx = bestIdx;
    }

    @Override
    public int hashCode() {
        int hash = 7;
        hash = 73 * hash + this.depth;
        hash = 73 * hash + this.bestIdx;
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
        final Buffer other = (Buffer) obj;
        if (this.depth != other.depth) {
            return false;
        }
        if (this.bestIdx != other.bestIdx) {
            return false;
        }
        return true;
    }

    @Override
    public String toString() {
        return "Buffer{" + "depth=" + depth + ", bestIdx=" + bestIdx + '}';
    }
    
}
class ListBuffer {

    public List<Buffer> buffer = new ArrayList<>();

    public List<Buffer> getBufferList() {
        return this.buffer;
    }

    public synchronized void writeToBuffer(int depth, int bestIdx) {
        if (!buffer.contains(new Buffer(depth, bestIdx))) {
            buffer.add(new Buffer(depth, bestIdx));
        }

    }
}