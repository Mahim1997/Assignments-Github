/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package problem_2;

import java.util.Scanner;
import problem_2.coffees.Coffee;

/**
 *
 * @author esfs
 */
public class Main {

    static Scanner sc = new Scanner(System.in);

    public static void main(String[] args) {
        runner();

    }

    private static void runner() {
        System.out.println("Enter Your Name: ");
        String name = sc.nextLine();
        System.out.println("A/a- CoffeeA\tB/b- CoffeeB\tC/c- CoffeeC");
        String coffeeType = sc.nextLine();
        if (coffeeType.contains("A")) {
            coffeeType = "CoffeeA";
        } else if (coffeeType.contains("B")) {
            coffeeType = "CoffeeB";
        } else {
            coffeeType = "CoffeeC";
        }
        Customer customer = new Customer(name, coffeeType);
        CoffeeFactory factory = CoffeeFactory.getCoffeeFactory();
        Coffee coffee = factory.getCoffee(coffeeType);
        coffee.printCoffee();
        CoffeeFactory fac2 = CoffeeFactory.getCoffeeFactory();
        System.out.println("CHECKING WHTEHER BOTH COFFEE FACTORIES ARE EQUAL .. " + (fac2 == factory));
    }
}
