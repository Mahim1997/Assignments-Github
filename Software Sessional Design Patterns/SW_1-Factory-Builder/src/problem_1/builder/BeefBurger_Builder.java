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
public class BeefBurger_Builder implements Builder {

    private Burger beefBurger;

    public BeefBurger_Builder() {
        beefBurger = new Burger(UtilItems.BEEF_BURGER);
    }

    @Override
    public Burger getFinalBurger() {
        return beefBurger;
    }

    @Override
    public void addPatty(String pattyType) {
        beefBurger.add(new Patty(pattyType));
    }

    @Override
    public void addSauce(String sauceType) {
        beefBurger.add(new Sauce(sauceType));
    }

    @Override
    public void addCheese(String cheeseType) {
        beefBurger.add(new Cheese(cheeseType));
    }

    @Override
    public void makeFinalPayment() {
        beefBurger.calculateFinalCost();
    }

}
