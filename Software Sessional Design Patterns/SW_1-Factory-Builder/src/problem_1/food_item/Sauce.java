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
public class Sauce extends FoodItem {

    private static int numberofSauce = 0;

    public Sauce() {
        this.foodItemType = UtilItems.SAUCE;
        this.foodItemName = "REGULAR SAUCE";
        this.costPerUnit = FoodItem.calculateCost(UtilItems.SAUCE);
//        Sauce.numberofSauce++;
    }

    public Sauce(String sauceName) {
        this.foodItemType = UtilItems.SAUCE;
        this.foodItemName = sauceName;
        this.costPerUnit = FoodItem.calculateCost(UtilItems.SAUCE);
//        Sauce.numberofSauce++;
    }

    @Override
    public void printFoodItem() {
        System.out.println("SAUCE < " + this.foodItemName + " >, Cost = " + this.costPerUnit + " Tk.");
    }

    @Override
    public void incrementQuantity() {
        Sauce.numberofSauce++;
    }

    @Override
    public void decrementQuantity() {
        Sauce.numberofSauce--;
    }

    @Override
    public int getQuantity() {
        return Sauce.numberofSauce;
    }

    public String toString() {
        return "SAUCE < " + this.foodItemName + " >, Cost = " + this.costPerUnit + " Tk.\n";
    }
}
