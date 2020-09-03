package offline3;

import java.util.Collections;
import java.util.LinkedList;
import java.util.List;

public class Cheapest_Insertion implements IConstructor {

    Point[] points;
    int numberOfPoints;
    List<Point> tour;
    int indexGiven;

    public Cheapest_Insertion(List<Point> list) {
        formConstructor(list);
        indexGiven = 0;
    }

    @Override
    public void formConstructor(List<Point> list) {
        this.points = new Point[list.size()];
        for (int i = 0; i < list.size(); i++) {
            points[i] = list.get(i);
        }
        this.numberOfPoints = points.length;
        this.tour = new LinkedList<>();
//        this.visited = new boolean[points.length];
//        Arrays.fill(visited, false);
    }

    void addToTour(Point point) {
//        this.visited[point.getIndex()] = true;
        point.setVisited(true);
        this.tour.add(point);
//        System.out.println("Added " + point);
    }

    @Override
    public void constructTour() {
        /*
            Step 1. Start with a sub-graph consisting of node i only.
            Step 2. Find node r such that Cir is minimal and form sub-tour i-r-i.
            Step 3. Find (i, j) in sub-tour and node r, not in the sub-tour, such that Cir + Crj - Cij is minimal. Insert r between i and j.
         */
        //Clear tour
        clearTour();

        //starting node 'i' is formed randomly
        int startIdx = ((int) (Math.random() * this.numberOfPoints));

        //test using index ...
//        startIdx = indexGiven;
        //test done ...

        Point starting_i_Point = this.points[startIdx];
        addToTour(starting_i_Point);
        //Starting 'r' node is formed
        Point starting_r_Point = getClosestPoint_Unvisited(starting_i_Point);
        addToTour(starting_r_Point);

        while (tour.size() < numberOfPoints) {

            double minCost = IConstructor.MAX_LIMIT;
            int size = tour.size();
            Point final_i_Point = null, final_r_Point = null;

            for (int iter = 0; iter < tour.size(); iter++) {

                Point i_Point = tour.get(iter);
                Point j_Point = tour.get((iter + 1) % tour.size());

                for (int iter2 = 0; iter2 < numberOfPoints; iter2++) {
                    Point r_Point = points[iter2];
                    if (r_Point.isVisited() == true) {
//                        System.out.println("Node " + nodePoint + " is visited so continue; ");
                        continue;
                    }
                    double dis = Point.getEucladianDistance(i_Point, r_Point)
                            + Point.getEucladianDistance(j_Point, r_Point)
                            - Point.getEucladianDistance(i_Point, j_Point);

                    if (dis < minCost) {
                        minCost = dis;
                        final_i_Point = i_Point;
                        final_r_Point = r_Point;
                    }
                }

            }
//            System.out.println("Adding " + z + " to tour inside while loop ");

            addToTour(final_r_Point);

            for (int iter = size - 1; iter >= 0; iter--) {
                if (tour.get(iter).equals(final_i_Point)) {
                    break;
                }
                Collections.swap(tour, iter, iter + 1); //To push r_Point in between i_Point and j_Point , we need to shift all others...
            }
        }

        //add starting point again to complete cycle
        addToTour(tour.get(0));

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
            if (point.equals(p)) {
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
    public List<Point> getTour() {
        return this.tour;
    }

    @Override
    public void clearTour() {
        for (Point p : points) {
            p.setVisited(false);
        }
        this.tour.clear();
    }

    @Override
    public void setIndex(int k) {
        this.indexGiven = k;
    }

}
