/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package problem_1.people;

import java.util.ArrayList;
import java.util.List;
import problem_1.builder.BeefBurger_Builder;
import problem_1.product.Burger;
import problem_1.builder.Builder;
import problem_1.builder.ChickenBurger_Builder;

/**
 *
 * @author esfs
 */
public class Waiter {

    Builder builder;

    private List<String> cheeseList;
    private List<String> pattyList;
    private List<String> sauceList;

    public void getOrder(String cheese, String patty, String sauce) {
        this.cheeseList = new ArrayList<>();
        this.pattyList = new ArrayList<>();
        this.sauceList = new ArrayList<>();
        //type is defined in Builder reference.
    }

    public Burger construct(Builder bld) {
        builder = bld;
        
        for(int i=0; i<pattyList.size(); i++){
            builder.addPatty(pattyList.get(i));
        }
        for(int i=0; i<cheeseList.size(); i++){
            builder.addCheese(cheeseList.get(i));
        }
        for(int i=0; i<sauceList.size(); i++){
            builder.addSauce(sauceList.get(i));
        }        
 
        builder.makeFinalPayment();
        return builder.getFinalBurger();
    }

    /*    void getOrder(String burgerType, String cheese, String patty, String sauce) {

        this.cheeseType = cheese;
        this.pattyType = patty;
        this.sauceType = sauce;
    }
     */
    public Builder getBuilder(String burgerType) {
        if (burgerType.equalsIgnoreCase(UtilItems.BEEF_BURGER)) {
            return new BeefBurger_Builder();
        } else if (burgerType.equalsIgnoreCase(UtilItems.CHICKEN_BURGER)) {
            return new ChickenBurger_Builder();
        }
        return null;
    }

    void getOrder(String burgerType, List<String> cheeseList, List<String> pattyList, List<String> sauceList) {
        this.cheeseList = cheeseList;
        this.pattyList = pattyList;
        this.sauceList = sauceList;
    }
}
