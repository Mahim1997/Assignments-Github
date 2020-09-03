/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package problem_1.people;

import java.util.ArrayList;
import java.util.List;
import problem_1.Order;
import problem_1.people.Waiter;

/**
 *
 * @author esfs
 */
public class Client {

    private String burgerType;

    private Order order;

    private String clientName;
    private static int transactionID = 0;
    //and other client Informations

    public static int getTransactionID() {
        return transactionID;
    }

    public Client(String clientName) {
        this.clientName = clientName;
        Client.transactionID++;
        order = new Order();
    }

    public String getBurgerType() {
        return burgerType;
    }

    public void setBurgerType(String burgerType) {
        this.burgerType = burgerType;
    }

    public String getClientName() {
        return clientName;
    }

    public void setClientName(String clientName) {
        this.clientName = clientName;
    }

    /*public void setOrder(List<String> pattyList, List<String> cheeseList, List<String> sauceList) {
        this.pattyList = pattyList;
        this.cheeseList = cheeseList;
        this.sauceList = sauceList;
    }*/
    
    public void setOrder(Order order){
        this.order = order;
    }

/*    public void printClient() {
        System.out.println("Client Name: " + clientName);
        System.out.println("<PRINTING ORDER> Transaction ID : #" + transactionID);
        System.out.println("Burger Type: " + getBurgerType());
        System.out.print("PATTIES => ");
        UtilItems.printList(pattyList);
        System.out.print("CHEESE => ");
        UtilItems.printList(cheeseList);
        System.out.print("SAUCE => ");
        UtilItems.printList(sauceList);
    }
*/
    public void printClient(){
        System.out.println("Client Name: " + clientName);
        order.printOrder();
    }
    
    public void order(Waiter waiter) {
//        waiter.getOrder(burgerType, cheeseList, pattyList, sauceList);
        waiter.getOrder(burgerType, order.getPattyList(), order.getCheeseList(), order.getSauceList());
    }

    public Order getOrder() {
        return order;
    }

 
    /*public void setOrder(String burgerType, String cheese, String patty, String sauce) {
        this.burgerType = burgerType;
        this.cheeseType = cheese;
        this.pattyType = patty;
        this.sauceType = sauce;
        //type is defined in Builder reference.
    }
     */
}
