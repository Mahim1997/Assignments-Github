/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package problem_1.food_item;

import problem_1.people.UtilItems;

/**
 *
 * @author esfs
 */
public class Patty extends FoodItem {

    private static int numberOfPatties = 0;

    public Patty() {
        this.foodItemType = UtilItems.PATTY;
        this.foodItemName = "REGULAR PATTY";
        this.costPerUnit = FoodItem.calculateCost(UtilItems.PATTY);

//        Patty.numberOfPatties ++;
    }

    public Patty(String pattyName) {
        this.foodItemType = UtilItems.PATTY;
        this.foodItemName = pattyName;
        this.costPerUnit = FoodItem.calculateCost(UtilItems.PATTY);

    }

    @Override
    public void printFoodItem() {
        System.out.println("PATTY < " + this.foodItemName + " > , Cost = " + this.costPerUnit + " Tk.");

    }

    @Override
    public void incrementQuantity() {
        Patty.numberOfPatties++;
    }

    @Override
    public void decrementQuantity() {
        Patty.numberOfPatties--;
    }

    @Override
    public int getQuantity() {
        return Patty.numberOfPatties;
    }

    public String toString() {
        return "PATTY < " + this.foodItemName + " > , Cost = " + this.costPerUnit + " Tk.\n";
    }
}
