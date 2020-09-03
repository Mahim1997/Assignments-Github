/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package problem_1.food_item;

import problem_1.people.*;

/**
 *
 * @author esfs
 */
public abstract class FoodItem {

    protected String foodItemName;
//    protected int quantity; //eg. double patty etc.
    protected int costPerUnit;
    protected String foodItemType;

    protected static int calculateCost(String foodItem) {
        if(foodItem.toUpperCase().contains("PATTY"))
            return UtilItems.PATTY_COST;
        else if(foodItem.toUpperCase().contains("CHEESE")){
            return UtilItems.CHEESE_COST;
        }
        else if(foodItem.toUpperCase().contains("SAUCE")) {
            return UtilItems.SAUCE_COST;
        }
        else 
            return 100;
    }

    public String getFoodItemName() {
        return foodItemName;
    }

    public void setFoodItemName(String foodItemName) {
        this.foodItemName = foodItemName;
    }

 

    
    public abstract void printFoodItem();
    public int getCostPerUnit(){
        return this.costPerUnit;
    }
    public abstract void incrementQuantity();
    public abstract void decrementQuantity();
    public abstract int getQuantity();
 
}
