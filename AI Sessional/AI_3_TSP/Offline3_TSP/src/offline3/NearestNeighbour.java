package offline3;

import java.util.Arrays;
import java.util.LinkedList;
import java.util.List;

public class NearestNeighbour implements IConstructor {

    Point[] points;
    int numberOfPoints;
    List<Point> tour;
    int indexGiven;

    public NearestNeighbour(List<Point> list) {
        indexGiven = IConstructor.MAX_LIMIT;
        this.formConstructor(list);
    }

    @Override
    public void formConstructor(List<Point> list) {
        this.points = new Point[list.size()];
        for (int i = 0; i < list.size(); i++) {
            points[i] = list.get(i);
            points[i].setVisited(false);
        }
        this.numberOfPoints = points.length;
        this.tour = new LinkedList<>();
    }

    @Override
    public List<Point> getTour() {
        return this.tour;
    }

    @Override
    public Point getClosestPoint_Unvisited(Point point) {
//        System.out.println("Finding point for " + point);
        double minCost = IConstructor.MAX_LIMIT;
        Point closest = null, iteratorPoint;
        for (int i = 0; i < points.length; i++) {
            iteratorPoint = points[i];
            if (iteratorPoint.isVisited() == true) {
                continue;
            }
            if (point.equals(iteratorPoint) == true) {
                continue;
            }
            double dis = point.distance(iteratorPoint);
            if (dis < minCost) {
                minCost = dis;
                closest = iteratorPoint;
            }
        }
//        System.out.println("Returning for point = " + point + " , closest = " + closest);
        return closest;
    }

    @Override
    public void clearTour() {
        this.tour.clear();
        for (Point point : this.points) {
            point.setVisited(false);
        }
    }
    
    @Override
    public void constructTour() {
        /*
            1. Start with any node in the beginning
            2. Find unvisited node closest to the last node added to the tour. Add this node to the tour.
            3. Repeat Step 2 until all nodes are contained in the tour. Then join the first and last nodes.
         */
        int startIdx = ((int) (Math.random() * this.numberOfPoints));
        
        
        if(indexGiven < points.length  && Main.isTESTING){
            startIdx = indexGiven;
        }
//        startIdx = 3;
        this.clearTour();

        Point startingPoint = points[startIdx];
        startingPoint.setVisited(true);
        tour.add(startingPoint);
    
        
        Point closest = startingPoint;
        while (tour.size() < points.length) {
            closest = getClosestPoint_Unvisited(closest);
            closest.setVisited(true);
            tour.add(closest);
        }
        tour.add(startingPoint);
    }

    @Override
    public void setIndex(int k) {
         indexGiven = k;
    }



}
