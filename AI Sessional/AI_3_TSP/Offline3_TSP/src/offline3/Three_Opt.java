package offline3;

import java.util.ArrayList;
import java.util.Collections;
import java.util.List;

public class Three_Opt extends Improvement_Heuristic {

    Combination combo;

    public Three_Opt(List<Point> tour) {
        this.tour = tour;
        this.costOfTour = Loader.getCost(tour);
        this.tour.remove(this.tour.size() - 1); //Remove last one
        this.combo = new Combination();
    }

    void printList(List<Point> list) {
        for (int i = 0; i < list.size(); i++) {
            Point p = list.get(i);
            System.out.print(p.getIndex());
            if (i != list.size() - 1) {
                System.out.print(" , ");
            }
        }
        System.out.println("");
    }

    void _3opt() {
        List<Point> newTour;
        double best_cost, new_cost;
        boolean should_runAgain;

        while (true) {
            best_cost = Loader.getCostWithoutRepeat(tour);
            should_runAgain = false;

            for (int i = 0; i < tour.size(); i++) {
                for (int j = i + 2; j < tour.size(); j++) {
                    for (int k = j + 2; k < tour.size(); k++) {
//                        for (int combination = 1; combination <= 7; combination++) {
//                            newTour = combo.combination(i, j, k, tour, combination); //directly reference na diye copy dewa better because reverse er kahini
                        newTour = combo.getTheBestCombination(i, j, k, tour);
                        new_cost = Loader.getCostWithoutRepeat(newTour);
                        if (new_cost < best_cost) {
                            should_runAgain = true;
                            tour = newTour;
                            break;
                        }
//                        }
                        if (should_runAgain == true) {
                            break;
                        }
                    }
                    if (should_runAgain == true) {
                        break;
                    }
                }
            }
            if (should_runAgain == false) {
                break;
            }
        }

    }

    @Override
    void improveTour() {
        _3opt();
    }

    @Override
    public List<Point> getTour() {
        this.tour.add(this.tour.get(0));
        return this.tour;
    }

}
