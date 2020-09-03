/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package problem_1.builder;

import problem_1.food_item.FoodItem;
import problem_1.product.Burger;

/**
 *
 * @author esfs
 */
public interface Builder {

    public void addPatty(String pattyType);

    public void addSauce(String sauceType);

    public void addCheese(String cheeseType);

    public void makeFinalPayment();
    
    public Burger getFinalBurger();
}
