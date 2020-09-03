/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package problem_1.people;

import java.util.List;

/**
 *
 * @author esfs
 */
public interface UtilItems {

    public static final int PATTY_IDX = 0;
    public static final int SAUCE_IDX = 1;
    public static final int CHEESE_IDX = 2;

    public static final int PATTY_COST = 100;
    public static final int CHEESE_COST = 50;
    public static final int SAUCE_COST = 20;

    public static final String PATTY = "PATTY";
    public static final String CHEESE = "CHEESE";
    public static final String SAUCE = "SAUCE";
    
    public static final String BEEF_BURGER = "BEEF_BURGER";
    public static final String CHICKEN_BURGER = "CHICKEN_BURGER";

    public static void printList(List<String> list){
        list.forEach((s) -> {
            System.out.print(s + " , ");
        });
        System.out.println("");
    }
    
}
