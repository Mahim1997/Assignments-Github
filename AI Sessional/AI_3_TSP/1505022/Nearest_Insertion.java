package offline3;

import java.util.Arrays;
import java.util.Collections;
import java.util.LinkedList;
import java.util.List;

public class Nearest_Insertion implements IConstructor {

    Point[] points;
    int numberOfPoints;
    List<Point> tour;

    public int indexGiven;

    public Nearest_Insertion(List<Point> list) {
        formConstructor(list);
    }

    @Override
    public List<Point> getTour() {
        return this.tour;
    }

    @Override
    public Point getClosestPoint_Unvisited(Point point) {
//        System.out.println("Finding point for " + point);
        double minCost = IConstructor.MAX_LIMIT;
        Point closest = null, p;
        for (int i = 0; i < points.length; i++) {
            p = points[i];
            if (p.isVisited() == true) {
                continue;
            }
            if (point == p) {
                continue;
            }
            double dis = point.distance(p);
            if (dis < minCost) {
                minCost = dis;
                closest = p;
            }
        }
//        System.out.println("Returning for point = " + point + " , closest = " + closest);
        return closest;
    }

    @Override
    public void formConstructor(List<Point> list) {
        this.points = new Point[list.size()];
        for (int i = 0; i < list.size(); i++) {
            points[i] = list.get(i);
        }
        this.numberOfPoints = points.length;
        this.tour = new LinkedList<>();

    }

    @Override
    public void clearTour() {
        this.tour.clear();
        for (Point p : this.points) {
            p.setVisited(false);
        }
    }

    void addToTour(Point point) {
        point.setVisited(true);
        this.tour.add(point);
    }

    @Override
    public void constructTour() {
        /*
            Step 1. Start with a sub-graph consisting of node i only.
            Step 2. Find node r such that Cir is minimal and form sub-tour i-r-i.
            Step 3. (Selection step) Given a sub-tour, find node r not in the sub-tour closest to any node j in the sub-tour; i.e. with minimal Crj
            Step 4. (Insertion step) Find the arc (i, j) in the sub-tour which minimizes Cir + Crj - Cij . Insert r between i and j.
            Step 5. If all the nodes are added to the tour, stop. Else go to step 3
         */
        //Clear tour
        clearTour();

        //starting node 'i' is formed randomly
        int startIdx = ((int) (Math.random() * this.numberOfPoints));

        //test using index ...
        if ((indexGiven < this.points.length) && Main.isTESTING) {
            startIdx = indexGiven;
        }
        //test done ....

        Point starting_i_Point = this.points[startIdx];
        addToTour(starting_i_Point);

        //Starting 'r' node is formed
        Point starting_r_Point = getClosestPoint_Unvisited(starting_i_Point);
        addToTour(starting_r_Point);

        while (tour.size() < numberOfPoints) {
            int tourSizeNow = tour.size();
            Point r_node = get_R_Node_ClosestToAnyNodeInSubTour();
            r_node.setVisited(true);

            Point j_node = get_J_Node_Arc(r_node);

            addToTour(r_node);

            for (int i = tourSizeNow - 1; i >= 0; i--) {    //Iterate until before the latest point 'the r_point'
//                if (tour.get(i).getIndex() == j_node.getIndex()) {
//                    break;
//                }
                if (tour.get(i).equals(j_node)) {
                    break;
                }
                Collections.swap(tour, i, i + 1);   //To push r_Point in between i_Point and j_Point , we need to shift all others...
            }
        }
        //Add starting point to get whole cycle(tour)
        addToTour(tour.get(0));
    }

    private Point get_R_Node_ClosestToAnyNodeInSubTour() {
        double minCost = IConstructor.MAX_LIMIT;
        Point closest = null;
        for (int i = 0; i < this.points.length; i++) {
            Point iteratorPoint = this.points[i];
            if (iteratorPoint.isVisited() == false) {   //NEED TO ONLY SEARCH THE SELECTED POINTS IN TOUR...
                continue;
            }
            Point nearest = getClosestPoint_Unvisited(iteratorPoint);
            double dis = Point.getEucladianDistance(iteratorPoint, nearest);
            if (dis < minCost) {
                minCost = dis;
                closest = nearest;
            }
        }
        return closest;
    }

    private Point get_J_Node_Arc(Point r_node) {
        Point closest = null;
        double minCost = IConstructor.MAX_LIMIT;
        for (int i = 0; i < tour.size(); i++) {
            Point j_Point = tour.get(i);
            Point i_Point = tour.get((i + 1) % (tour.size()));

            double dis = Point.getEucladianDistance(j_Point, r_node)
                    + Point.getEucladianDistance(i_Point, r_node)
                    - Point.getEucladianDistance(j_Point, i_Point);

            if (dis < minCost) {
                minCost = dis;
                closest = j_Point;
            }
        }
        return closest;
    }

    @Override
    public void setIndex(int k) {
        this.indexGiven = k;
    }

}
