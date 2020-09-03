/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package problem_2.coffees;

/**
 *
 * @author esfs
 */
public class CoffeeB implements Coffee {

    private String coffeeType;
    ///boolean to represent whether it contains those things... 
    private String milk;
    private String caffeine;
    private String sugar;

    private String cost;

    public CoffeeB() {
        coffeeType = "Coffee B";
        milk = "YES";
        caffeine = "YES";
        sugar = "YES";
        cost = "175 TK";
    }

    @Override
    public void printCoffee() {
        System.out.println("-------------******-------------");
        System.out.println("<Printing Coffee>.... \n");

        System.out.println("Coffee Type: " + coffeeType);

        System.out.println("Doest it contain Milk? : " + milk);
        System.out.println("Does it contain Sugar?: " + sugar);
        System.out.println("Does it contain Caffeine?: " + caffeine);
        System.out.println("Cost: " + cost);
        System.out.println("-------------******-------------");
    }
}
