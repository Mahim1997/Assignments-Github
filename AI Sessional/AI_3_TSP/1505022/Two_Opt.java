package offline3;

import java.util.ArrayList;
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


    /*
         2optSwap(route, i, k) {
       1. take route[0] to route[i-1] and add them in order to new_route
       2. take route[i] to route[k] and add them in reverse order to new_route
       3. take route[k+1] to end and add them in order to new_route
       return new_route;
   }
     */
    
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

    /*repeat until no improvement is made {
       start_again:
       best_distance = calculateTotalDistance(existing_route)
       for (i = 0; i < number of nodes eligible to be swapped - 1; i++) {
           for (k = i + 1; k < number of nodes eligible to be swapped; k++) {
               new_route = 2optSwap(existing_route, i + 1, k)
               new_distance = calculateTotalDistance(new_route)
               if (new_distance < best_distance) {
                   existing_route = new_route
                   goto start_again
               }
           }
       }
   }*/
    @Override
    void improveTour() {
        double new_distance, best_distance;
        boolean should_runAgain = false;
        //last city (cycle) was already removed in the constructor.
        this.existingRoute = this.tour;
        
        while (true) {
            best_distance = Loader.getCostWithoutRepeat(existingRoute);
            should_runAgain = false;
            
            for (int i = 0; i < existingRoute.size() - 1; i++) {
                for (int k = i + 1; k < existingRoute.size(); k++) {
                    
                    newRoute = _2OptSwap(existingRoute, i + 1, k);
                    new_distance = Loader.getCostWithoutRepeat(newRoute);
                    
                    if (new_distance < best_distance) {
                        should_runAgain = true;
                        existingRoute = newRoute;
                        break;
                    }
//                    existingRoute = _2OptSwap(existingRoute, i + 1, j);
                }
                if (should_runAgain == true) {
                    break;
                }
            }

            if (should_runAgain == false) {
//                newRoute = existingRoute;
                break;
            }
        }

    }


    @Override
        public List<Point> getTour() {
        this.newRoute.add(this.newRoute.get(0));
        return this.newRoute;
    }
}

/*
                while (true) {
            best_distance = Loader.getCostWithoutRepeat(existingRoute);
            start_again = false;
            
            for (int i = 0; i < existingRoute.size() - 1; i++) {
                for (int j = i + 1; j < existingRoute.size(); j++) {
                    
                    newRoute = _2OptSwap(existingRoute, i + 1, j);
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
 */
