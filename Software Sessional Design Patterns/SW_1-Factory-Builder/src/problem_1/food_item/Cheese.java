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
public class Cheese extends FoodItem {

    private static int numberOfCheeseBars = 0;

    public Cheese() {
        this.foodItemType = UtilItems.CHEESE;
        this.foodItemName = "REGULAR CHEESE";
        this.costPerUnit = FoodItem.calculateCost(UtilItems.CHEESE);

//        Cheese.numberOfCheeseBars++;
    }

    public Cheese(String cheeseName) {
        this.foodItemType = UtilItems.CHEESE;
        this.foodItemName = cheeseName;
        this.costPerUnit = FoodItem.calculateCost(UtilItems.CHEESE);
    }

    @Override
    public void printFoodItem() {
        System.out.println("CHEESE < " + this.foodItemName + " >, Cost = " + this.costPerUnit + " Tk.");
    }

    @Override
    public void incrementQuantity() {
        Cheese.numberOfCheeseBars ++ ;
    }

    @Override
    public void decrementQuantity() {
        Cheese.numberOfCheeseBars -- ;
    }

 

    @Override
    public int getQuantity() {
        return Cheese.numberOfCheeseBars;
    }

    public String toString(){
        return "CHEESE < " + this.foodItemName + " > , Cost = " + this.costPerUnit + " Tk.\n";
    }
    
}
