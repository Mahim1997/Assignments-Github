package offline3;

import java.util.ArrayList;
import java.util.Collections;
import java.util.List;

public class Combination {

    public List<Point> combination(int i, int j, int k, List<Point> givenTour, int choice) {
        List<Point> tour = new ArrayList<>(givenTour);
        switch (choice) {
            case 1:
                return combination1(i, j, k, tour);
            case 2:
                return combination2(i, j, k, tour);
            case 3:
                return combination3(i, j, k, tour);
            case 4:
                return combination4(i, j, k, tour);
            case 5:
                return combination5(i, j, k, tour);
            case 6:
                return combination6(i, j, k, tour);
            case 7:
                return combination7(i, j, k, tour);
            default:
                break;
        }

        return null;
    }

    private List<Point> reverse_tour(List<Point> tour, int i, int j) {        // [startLimit, endLimit)
        List<Point> subList = tour.subList(i, j);
        Collections.reverse(subList);
        for (int iter = i; iter < j; iter++) {
            tour.set(iter, subList.get(iter - i));
        }

        return tour;
    }

    private List<Point> combination1(int i, int j, int k, List<Point> tour) {
        List<Point> latestTourFormed = new ArrayList<>();

        //Add elements upto i
        for (int iter = 0; iter <= i; iter++) {
            latestTourFormed.add(tour.get(iter));
        }

        //Reversed adding between j and i [SWAP]
        for (int iter = j + 1; iter <= k; iter++) {
            latestTourFormed.add(tour.get(iter));
        }
        for (int iter = i + 1; iter <= j; iter++) {
            latestTourFormed.add(tour.get(iter));
        }
        //Reversed adding done

        //Final adding elements
        for (int iter = k + 1; iter < tour.size(); iter++) {
            latestTourFormed.add(tour.get(iter));
        }

        return latestTourFormed;
    }

    private List<Point> combination2(int i, int j, int k, List<Point> tour) {
        List<Point> latestTourFormed = new ArrayList<>();

        //Reverse the tour from (i to j)
        tour = reverse_tour(tour, i + 1, j + 1);

        //Add elements upto i
        for (int iter = 0; iter <= i; iter++) {
            latestTourFormed.add(tour.get(iter));
        }

        //Normal adding between i and j
        for (int iter = i + 1; iter <= j; iter++) {
            latestTourFormed.add(tour.get(iter));
        }
        for (int iter = j + 1; iter <= k; iter++) {
            latestTourFormed.add(tour.get(iter));
        }
        //Normal adding done

        //Final adding elements
        for (int iter = k + 1; iter < tour.size(); iter++) {
            latestTourFormed.add(tour.get(iter));
        }

        return latestTourFormed;
    }

    private List<Point> combination3(int i, int j, int k, List<Point> tour) {
        List<Point> latestTourFormed = new ArrayList<>();

        //Reverse the tour from (i to j)
        tour = reverse_tour(tour, i + 1, j + 1);

        //Add elements upto i
        for (int iter = 0; iter <= i; iter++) {
            latestTourFormed.add(tour.get(iter));
        }

        //Reverse adding between i and j
        for (int iter = j + 1; iter <= k; iter++) {
            latestTourFormed.add(tour.get(iter));
        }
        for (int iter = i + 1; iter <= j; iter++) {
            latestTourFormed.add(tour.get(iter));
        }
        //Reverse adding done

        //Final adding elements
        for (int iter = k + 1; iter < tour.size(); iter++) {
            latestTourFormed.add(tour.get(iter));
        }

        return latestTourFormed;
    }

    private List<Point> combination4(int i, int j, int k, List<Point> tour) {
        List<Point> latestTourFormed = new ArrayList<>();

        //Reverse tour from (j, k);
        tour = reverse_tour(tour, j + 1, k + 1);
        for (int iter = 0; iter <= i; iter++) {
            latestTourFormed.add(tour.get(iter));
        }
        //Normal adding
        for (int iter = i + 1; iter <= j; iter++) {
            latestTourFormed.add(tour.get(iter));
        }
        for (int iter = j + 1; iter <= k; iter++) {
            latestTourFormed.add(tour.get(iter));
        }
        for (int iter = k + 1; iter < tour.size(); iter++) {
            latestTourFormed.add(tour.get(iter));
        }

        return latestTourFormed;
    }

    private List<Point> combination5(int i, int j, int k, List<Point> tour) {
        List<Point> latestTourFormed = new ArrayList<>();

        //Reverse tour from (j, k);
        tour = reverse_tour(tour, j + 1, k + 1);
        for (int iter = 0; iter <= i; iter++) {
            latestTourFormed.add(tour.get(iter));
        }
        //Reverse adding
        for (int iter = j + 1; iter <= k; iter++) {
            latestTourFormed.add(tour.get(iter));
        }
        for (int iter = i + 1; iter <= j; iter++) {
            latestTourFormed.add(tour.get(iter));
        }

        for (int iter = k + 1; iter < tour.size(); iter++) {
            latestTourFormed.add(tour.get(iter));
        }

        return latestTourFormed;
    }

    private List<Point> combination6(int i, int j, int k, List<Point> tour) {
        List<Point> latestTourFormed = new ArrayList<>();

        //Reverse the tour from (i to j)
        tour = reverse_tour(tour, i + 1, j + 1);

        //Reverse also from (j, k)
        tour = reverse_tour(tour, j + 1, k + 1);

        //Add elements upto i
        for (int iter = 0; iter <= i; iter++) {
            latestTourFormed.add(tour.get(iter));
        }

        //Normal adding between i and j
        for (int iter = i + 1; iter <= j; iter++) {
            latestTourFormed.add(tour.get(iter));
        }
        for (int iter = j + 1; iter <= k; iter++) {
            latestTourFormed.add(tour.get(iter));
        }
        //Normal adding done

        //Final adding elements
        for (int iter = k + 1; iter < tour.size(); iter++) {
            latestTourFormed.add(tour.get(iter));
        }

        return latestTourFormed;
    }

    private List<Point> combination7(int i, int j, int k, List<Point> tour) {
        List<Point> latestTourFormed = new ArrayList<>();

        //Reverse the tour from (i to j)
        tour = reverse_tour(tour, i + 1, j + 1);

        //Reverse also from (j, k)
        tour = reverse_tour(tour, j + 1, k + 1);

        //Add elements upto i
        for (int iter = 0; iter <= i; iter++) {
            latestTourFormed.add(tour.get(iter));
        }

        //Reverse adding between i and j
        for (int iter = j + 1; iter <= k; iter++) {
            latestTourFormed.add(tour.get(iter));
        }
        for (int iter = i + 1; iter <= j; iter++) {
            latestTourFormed.add(tour.get(iter));
        }
        //Reverse adding done

        //Final adding elements
        for (int iter = k + 1; iter < tour.size(); iter++) {
            latestTourFormed.add(tour.get(iter));
        }

        return latestTourFormed;
    }

    List<Point> getTheBestCombination(int i, int j, int k, List<Point> tour) {
        List<Point> bestList = null;
        ArrayList<List<Point>> listOFLists = new ArrayList<>();
        
        for(int combination=1; combination<=7; combination++){
            listOFLists.add(this.combination(i, j, k, tour, combination));
        }
        
        double best = IConstructor.MAX_LIMIT;
        for(List<Point> iter: listOFLists){
            double cost = Loader.getCostWithoutRepeat(iter);
            if(cost < best){
                best = cost;
                bestList = iter;
            }
        }
        
        return bestList;
    }
}
/*
1. reverse ( i+ 1, j + 1)

2. reverse ( j + 1, k + 1)

3. add in reversed order from j+1 to k first then from  i+1 to j last

out of three choices, should AT LEAST pick one choice

*/