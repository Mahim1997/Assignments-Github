package problem1;

import java.util.Objects;

public class Node {

    public int numCanLeft;
    public int numMisLeft;
    public String boatLocation;
    public int numMisRight;
    public int numCanRight;

    public String moveFromParent;
    public Node parentNode;

    public Node(int numCanLeft, int numMisLeft, String boatLocation, int numMisRight, int numCanRight) {
        this.numCanLeft = numCanLeft;
        this.numMisLeft = numMisLeft;
        this.boatLocation = boatLocation;
        this.numMisRight = numMisRight;
        this.numCanRight = numCanRight;
        this.parentNode = null;
    }

    public Node(Node n) {
        this.numCanLeft = n.numCanLeft;
        this.numMisLeft = n.numMisLeft;
        this.boatLocation = n.boatLocation;
        this.numMisRight = n.numMisRight;
        this.numCanRight = n.numCanRight;
        this.parentNode = n.parentNode;
    }

    public Node() {
        this.parentNode = null;
    }

    @Override
    public String toString() {
//        return "Node{" + '}';
        String s = "";
//        s = "< "  +  this.numCanLeft + ", " + this.numMisLeft + ", " + this.boatLocation + ", " + this.numMisRight + ", " + this.numCanRight + " >";
        s = "<" + Initializer.getStringForNode("C", numCanLeft) + " "
                + Initializer.getStringForNode("M", numMisLeft) + "  "
                + this.boatLocation + "  "
                + Initializer.getStringForNode("M", numMisRight) + " "
                + Initializer.getStringForNode("C", numCanRight) + " "
                + ">";
        if (this.moveFromParent != null) {
            s += " " + this.moveFromParent;
        }
        return s;
    }

    public boolean isFinalNode() {
        Node n = Initializer.getFinalNode();
        return this.equals(n);
    }

    public boolean isValidNode() {
        if ((numCanRight > numMisRight) && (numMisRight > 0)) {
            return false;
        }
        if ((numCanLeft > numMisLeft) && (numMisLeft > 0)) {
            return false;
        }

        return true;
    }

    public void reverseLocation() {
        this.boatLocation = Initializer.getNextLocation(this.boatLocation);
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
        final Node other = (Node) obj;
        if (this.numCanLeft != other.numCanLeft) {
            return false;
        }
        if (this.numMisLeft != other.numMisLeft) {
            return false;
        }
        if (this.numMisRight != other.numMisRight) {
            return false;
        }
        if (this.numCanRight != other.numCanRight) {
            return false;
        }
        return Objects.equals(this.boatLocation, other.boatLocation);
    }

    @Override
    public int hashCode() {
        int hash = 7;
        hash = 59 * hash + this.numCanLeft;
        hash = 59 * hash + this.numMisLeft;
        hash = 59 * hash + Objects.hashCode(this.boatLocation);
        hash = 59 * hash + this.numMisRight;
        hash = 59 * hash + this.numCanRight;
        return hash;
    }

    String print(){
        return "<"
                + "C = " + this.numCanLeft 
                + ", M = " + this.numMisLeft
                + ", " + Initializer.getLocationName(this.boatLocation)
                + ", M = " + this.numMisRight
                + ", C = " + this.numCanRight
                + ">";
     
    }

}
/*
    String print() {
        return "<" + Initializer.getStringForNode("C", numCanLeft) + " "
                + Initializer.getStringForNode("M", numMisLeft) + "  "
                + this.boatLocation + "  "
                + Initializer.getStringForNode("M", numMisRight) + " "
                + Initializer.getStringForNode("C", numCanRight) + " "
                + ">";
    }
*/
/*
    public Node(String s) {
        if (s.toUpperCase().contains(("start").toUpperCase())) {
            this.numCanLeft = Initializer.numberOfCannibalsInitial;
            this.numMisLeft = Initializer.numberOfMissionariesInitial;
            this.boatLocation = "L";        //Left initial always...
            this.numMisRight = 0;
            this.numCanRight = 0;
            this.parentNode = null;
        }
    }

*/
