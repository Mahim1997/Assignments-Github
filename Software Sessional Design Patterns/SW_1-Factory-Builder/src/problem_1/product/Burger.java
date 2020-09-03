/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package problem_1.product;

import java.util.ArrayList;
import java.util.List;
import problem_1.food_item.FoodItem;

/**
 *
 * @author esfs
 */
public class Burger {

    private String typeOfBurger;
    private List<FoodItem> listOfItems;
    private int finalPayment;

    public Burger(String typeOfBurger) {
        this.listOfItems = new ArrayList<>();
        this.typeOfBurger = typeOfBurger;
        this.finalPayment = 0;
    }

    public void printBurger() {
        System.out.println("---------------------------******-------------------------");
        System.out.println("<PRINTING BURGER..>\n");
        System.out.println("Type Of Burger: " + typeOfBurger);
        if (listOfItems.size() == 0) {
            System.out.println("BURGER Still Incomplete.");
            System.out.println("---------------------------******-------------------------");
            return;
        }
        for (FoodItem s : listOfItems) {
            System.out.println(s.toString());
        }
        calculateFinalCost();
        System.out.println("Total Cost = " + this.finalPayment + " Tk.");
        System.out.println("---------------------------******-------------------------");
    }

    public void calculateFinalCost() {
        int totalCost = 0;
        FoodItem food = null;
        for (int i = 0; i < listOfItems.size(); i++) {
            food = listOfItems.get(i);
            totalCost += food.getCostPerUnit();
        }
        this.finalPayment = totalCost;
    }

    public String getTypeOfBurger() {
        return typeOfBurger;
    }

    public void add(FoodItem whatToAdd) {
        listOfItems.add(whatToAdd);
    }

    public void incrementCostBy(int incCost) {
        this.finalPayment += incCost;
    }
}
