package offline3;

class Point {

    private double x;
    private double y;
    private boolean visited;

    public boolean isVisited() {
        return visited;
    }

    public void setVisited(boolean visited) {
        this.visited = visited;
    }

    public Point(double x, double y, int index) {
        this.x = x;
        this.y = y;
        this.index = index;
        this.visited = false;
    }

    public int getIndex() {
        return index;
    }

    public void setIndex(int index) {
        this.index = index;
    }

    public Point(double x, double y) {
        this.x = x;
        this.y = y;
        this.visited = false;
        this.index = -1;
    }

    public Point() {
    }

    void setPoint(double x, double y) {
        this.x = x;
        this.y = y;
    }

    public String getPoint() {
        return "( " + x + ", " + y + " )";
    }

    @Override
    public int hashCode() {
        int hash = 5;
        hash = 17 * hash + (int) (Double.doubleToLongBits(this.x) ^ (Double.doubleToLongBits(this.x) >>> 32));
        hash = 17 * hash + (int) (Double.doubleToLongBits(this.y) ^ (Double.doubleToLongBits(this.y) >>> 32));
        hash = 17 * hash + this.index;
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
        final Point other = (Point) obj;
        if (Double.doubleToLongBits(this.x) != Double.doubleToLongBits(other.x)) {
            return false;
        }
        if (Double.doubleToLongBits(this.y) != Double.doubleToLongBits(other.y)) {
            return false;
        }
        if (this.index != other.index) {
            return false;
        }
        return true;
    }

    public double distance(Point p) {
        return Math.sqrt(((this.x - p.x) * (this.x - p.x)) + ((this.y - p.y) * (this.y - p.y)));
    }

    public static double getEucladianDistance(Point p1, Point p) {
        return Math.sqrt(((p1.x - p.x) * (p1.x - p.x)) + ((p1.y - p.y) * (p1.y - p.y)));
    }
    private int index;

    public void setPoint(Point p) {
        setPoint(p.x, p.y);
    }

    public double getX() {
        return x;
    }

    public void setX(double x) {
        this.x = x;
    }

    public double getY() {
        return y;
    }

    public void setY(double y) {
        this.y = y;
    }

    @Override
    public String toString() {
//        return "Point{" + "x=" + x + ", y=" + y + '}';
        return "( " + x + ", " + y + " ) and index = " + index + " isVisited = " + visited;
    }

    void setPoint(double x, double y, int idx) {
        this.x = x;
        this.y = y;
        this.index = idx;
    }

}
