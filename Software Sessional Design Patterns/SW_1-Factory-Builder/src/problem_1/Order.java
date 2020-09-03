/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package problem_1;

import java.util.ArrayList;
import java.util.List;
import problem_1.people.UtilItems;

/**
 *
 * @author esfs
 */
public class Order {

    protected String burgerType;
    protected List<String> pattyList;
    protected List<String> cheeseList;
    protected List<String> sauceList;

    public Order() {
        this.cheeseList = new ArrayList<>();
        this.pattyList = new ArrayList<>();
        this.sauceList = new ArrayList<>();
    }

    public String getBurgerType() {
        return burgerType;
    }

    public void setBurgerType(String burgerType) {
        this.burgerType = burgerType;
    }

    public List<String> getPattyList() {
        return pattyList;
    }

    public void setPattyList(List<String> pattyList) {
        this.pattyList = pattyList;
    }

    public List<String> getCheeseList() {
        return cheeseList;
    }

    public void setCheeseList(List<String> cheeseList) {
        this.cheeseList = cheeseList;
    }

    public List<String> getSauceList() {
        return sauceList;
    }

    public void setSauceList(List<String> sauceList) {
        this.sauceList = sauceList;
    }

    public Order(String burgerType, List<String> pattyList, List<String> cheeseList, List<String> sauceList) {
        this.burgerType = burgerType;
        this.pattyList = pattyList;
        this.cheeseList = cheeseList;
        this.sauceList = sauceList;
    }

    public void printOrder() {
        System.out.println("<PRINTING ORDER> ===> ");
        System.out.println("Burger Type: " + burgerType);
        System.out.print("Patty: ");
        UtilItems.printList(pattyList);
        System.out.print("Cheese: ");
        UtilItems.printList(cheeseList);
        System.out.print("Sauce: ");
        UtilItems.printList(sauceList);
    }

}
