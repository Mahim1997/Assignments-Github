/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package problem_1.builder;

import problem_1.food_item.*;
import problem_1.people.UtilItems;
import problem_1.product.Burger;

/**
 *
 * @author esfs
 */
public class ChickenBurger_Builder implements Builder {

    private Burger chickenBurger;

    public ChickenBurger_Builder() {
        chickenBurger = new Burger(UtilItems.CHICKEN_BURGER);
    }

    @Override
    public void addPatty(String pattyType) {
        chickenBurger.add(new Patty(pattyType));
    }

    @Override
    public void addSauce(String sauceType) {
        chickenBurger.add(new Sauce(sauceType));
    }

    @Override
    public void addCheese(String cheeseType) {
        chickenBurger.add(new Cheese(cheeseType));
    }

    @Override
    public Burger getFinalBurger() {
        return chickenBurger;
    }

    @Override
    public void makeFinalPayment() {
        chickenBurger.calculateFinalCost();
    }

}
