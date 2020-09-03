package offline3;

import java.util.List;


public interface IConstructor {
    public static final int MAX_LIMIT = 10000;
    public static final String nearestNeighbor = "NearestNeighbour";
    public static final String nearestInsertion = "NearestInsertion";
    public static final String cheapestInsertion = "CheapestInsertion";
    
    public void formConstructor(List<Point> list);
    public void constructTour();
    public List<Point> getTour();
    public void clearTour();
    public void setIndex(int k);
    Point getClosestPoint_Unvisited(Point p);
}
