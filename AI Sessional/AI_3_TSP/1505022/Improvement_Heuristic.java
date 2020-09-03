 
package offline3;
 
import java.util.List;

public abstract class Improvement_Heuristic {
    public static final String twoOPT = "TWO_OPT";
    public static final String threeOPT = "THREE_OPT";
    
    
    List<Point> tour;
    double costOfTour;
    
    
    public void setInitialTour(List<Point> initialTour){
        this.tour = initialTour;
        setInitialCost();
    }
    public void setInitialCost(){
        this.costOfTour = Loader.getCost(tour);
    }
    public List<Point> getTour(){
        return this.tour;
    }
    abstract void improveTour();
}







/*
package offline3;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.Collections;
import java.util.List;

public class Two_Opt extends Improvement_Heuristic {

    public double nowTourCost;
    private List<Point> existingRoute;
    private List<Point> newRoute;

    public Two_Opt(List<Point> tour) {
        this.tour = tour;
        this.tour.remove(this.tour.size() - 1); //Remove the repeating element
        this.existingRoute = this.tour;
        this.costOfTour = Loader.getCostWithoutRepeat(tour);
        nowTourCost = IConstructor.MAX_LIMIT;

        newRoute = null;
    }


    
//         2optSwap(route, i, k) {
//       1. take route[0] to route[i-1] and add them in order to new_route
//       2. take route[i] to route[k] and add them in reverse order to new_route
//       3. take route[k+1] to end and add them in order to new_route
//       return new_route;
//   }
    
    List<Point> _2OptSwap(List<Point> route, int i, int k) {
        List<Point> list = new ArrayList<>();
        for (int iter = 0; iter < i; iter++) {
            //0 to i-1
            list.add(route.get(iter));
        }
        for (int iter = k; iter >= i; iter--) {
            list.add(route.get(iter));
        }
        for (int iter = k + 1; iter < route.size(); iter++) {
            list.add(route.get(iter));
        }

        return list;
    }

//    repeat until no improvement is made {
//       start_again:
//       best_distance = calculateTotalDistance(existing_route)
//       for (i = 1; i < number of nodes eligible to be swapped - 1; i++) {
//           for (k = i + 1; k < number of nodes eligible to be swapped; k++) {
//               new_route = 2optSwap(existing_route, i, k)
//               new_distance = calculateTotalDistance(new_route)
//               if (new_distance < best_distance) {
//                   existing_route = new_route
//                   goto start_again
//               }
//           }
//       }
//   }
    @Override
    public void improveTour() {
        if (Main.isTESTING) {
            improveTour2();
            return;
        }
        while (true) {
            double Curr = Loader.getCostWithoutRepeat(tour);
            boolean Changed = false;

            for (int i = 0; i < tour.size(); i++) {
                for (int j = i + 2; j < tour.size(); j++) {
                    tour = reverse(tour, i + 1, j); //Actually reverses from i+1 to j. [j+1 is the barrier]
                    double NewCost = Loader.getCostWithoutRepeat(tour);
                    if (NewCost < Curr) {
                        Changed = true;
                        break;
                    }

                    tour = reverse(tour, i + 1, j);
                }
                if (Changed) {
                    break;
                }
            }

            if (!Changed) {
//                newRoute = tour;
                break;
            }
        }
    }

    static void printArray(Point[] arr) {
        for (Point p : arr) {
            System.out.println(p);
        }
        System.out.println("");
    }

    static void printTourOnlyIndex(List<Point> list) {
        list.forEach((p) -> {
            System.out.print(p.getIndex() + " -> ");
        });
        System.out.println("");
    }

    List<Point> reverse(List<Point> list, int i, int j) {

        Point[] initialTour = new Point[list.size()];
        initialTour = list.toArray(initialTour);

        Point[] newerTour = new Point[list.size()];
        for (int c = 0; c <= i - 1; c++) {
            newerTour[c] = initialTour[c];
        }
        int change = 0;
        for (int d = i; d <= j; d++) {
            newerTour[d] = initialTour[d - change];
            change++;
        }
        for (int e = j + 1; e < list.size(); e++) {
            newerTour[e] = initialTour[e];
        }

        System.out.println("Using idx = " + initialTour[0].getIndex() + " , Inside reverse().... printing initial Tour array i = " + i + " , j = " + j);
        printArray(initialTour);

        System.out.println("After reverse().... printing Reversed Tour array");
        printArray(newerTour);

        return Arrays.asList(newerTour);
    }

    @Override
    public List<Point> getTour() {
//        this.newRoute.add(this.newRoute.get(0));
//        return this.newRoute;
//TEST
        System.out.println("Inside getTour() newRoute cost = " + Loader.getCostWithoutRepeat(newRoute) + " , ex route c = " + Loader.getCostWithoutRepeat(existingRoute));
        if (Loader.getCostWithoutRepeat(newRoute) > Loader.getCostWithoutRepeat(existingRoute)) {
            newRoute = existingRoute;
        }
//        this.newRoute = this.existingRoute;
//        System.out.println("Inside getTour() .. size of newRoute = " + newRoute.size());
        this.newRoute.add(this.newRoute.get(0));
        return this.newRoute;
    }

    void improveTour3() {
        double new_distance, best_distance;
        boolean start_again = false;
        //last city (cycle) was already removed in the constructor.
        this.existingRoute = this.tour;
        System.out.print("INITALLY PRINTING ORIGINAL TOUR : ");
        printTourOnlyIndex(existingRoute);
        while (true) {
            best_distance = Loader.getCostWithoutRepeat(existingRoute);
            start_again = false;

            for (int i = 0; i < existingRoute.size() - 1; i++) {
                for (int j = i + 1; j < existingRoute.size(); j++) {

                    newRoute = _2OptSwap(existingRoute, i + 1, j);
                    new_distance = Loader.getCostWithoutRepeat(newRoute);

                    String s = ("i = " + i + " ,  j = " + j);
                    s += ("\nEx  ");
                    s += ("(" + String.valueOf(Loader.getCostWithoutRepeat(existingRoute)) + ") :\n");
                    s += printTourOnlyIdx(existingRoute);
                    s += ("\nNew ");
                    s += ("(" + String.valueOf(Loader.getCostWithoutRepeat(newRoute)) + ") :\n");
                    s += printTourOnlyIdx(newRoute);
                    s += ("\n");
                    System.out.println(s);

                    if (new_distance < best_distance) {
                        start_again = true;
                        existingRoute = newRoute;
                        break;
                    }
//                    existingRoute = _2OptSwap(existingRoute, i + 1, j);
                }
                if (start_again == true) {
                    break;
                }
            }

            if (start_again == false) {
//                newRoute = existingRoute;
                break;
            }
        }

    }

    void improveTour2() {
        boolean flag = false;
        if (flag) {
            improveTour3();
            return;
        }
        double new_distance, best_distance;
        boolean start_again = false;
        //last city (cycle) was already removed in the constructor.
        this.existingRoute = this.tour;
        System.out.print("INITALLY PRINTING ORIGINAL TOUR : ");
        printTourOnlyIndex(existingRoute);
        while (true) {
            best_distance = Loader.getCostWithoutRepeat(existingRoute);
            start_again = false;

            for (int i = 0; i < existingRoute.size(); i++) {
//                for (int j = i + 1; j < existingRoute.size() ; j++) {
                for (int j = i + 1; j < existingRoute.size(); j++) {
                    String s = ("i = " + i + " ,  j = " + j);
                    s += ("\nEx : ");
                    s += printTourOnlyIdx(existingRoute);

//                    newRoute = _2OptSwap(existingRoute, i, j);     //Should start from i+1
                    newRoute = _2OptSwap(existingRoute, i + 1, j);     //Should start from i+1

                    s += ("\nNew : ");
                    s += printTourOnlyIdx(newRoute);
                    s += ("\n");
//                    System.out.println(s);

                    new_distance = Loader.getCostWithoutRepeat(newRoute);

                    if (new_distance < best_distance) {
                        start_again = true;
                        existingRoute = newRoute;
                        break;
                    }
//                    existingRoute = _2OptSwap(existingRoute, i + 1, j);
                }
                if (start_again == true) {
                    break;
                }
            }

            if (start_again == false) {
//                newRoute = existingRoute;
                break;
            }
        }
    }

    String printTourOnlyIdx(List<Point> tour) {
        String s = "";
        for (int i = 0; i < tour.size(); i++) {
            Point p = tour.get(i);
            s += String.valueOf(p.getIndex());
            if (i != tour.size() - 1) {
                s += (" -> ");
            }
        }
        return s;
    }
}


//                while (true) {
//            best_distance = Loader.getCostWithoutRepeat(existingRoute);
//            start_again = false;
//            
//            for (int i = 0; i < existingRoute.size() - 1; i++) {
//                for (int j = i + 1; j < existingRoute.size(); j++) {
//                    
//                    newRoute = _2OptSwap(existingRoute, i + 1, j);
//                    new_distance = Loader.getCostWithoutRepeat(newRoute);
//                    
//                    if (new_distance < best_distance) {
//                        start_again = true;
//                        existingRoute = newRoute;
//                        break;
//                    }
////                    existingRoute = _2OptSwap(existingRoute, i + 1, j);
//                }
//                if (start_again == true) {
//                    break;
//                }
//            }
//
//            if (start_again == false) {
////                newRoute = existingRoute;
//                break;
//            }
//        }
 

*/