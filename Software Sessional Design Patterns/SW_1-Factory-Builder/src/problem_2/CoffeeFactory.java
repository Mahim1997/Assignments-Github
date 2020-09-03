/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package problem_2;

import problem_2.coffees.*;

/**
 *
 * @author esfs
 */
public class CoffeeFactory {

    private static CoffeeFactory instance = null;

    private CoffeeFactory() {
        //Constructor
    }

    public static CoffeeFactory getCoffeeFactory() {

        if (instance == null) {
            instance = new CoffeeFactory();
        }
        return instance;
    }

    public Coffee getCoffee(String coffeeType) {
        if (coffeeType.contains("A")) {
            return new CoffeeA();
        } else if (coffeeType.contains("B")) {
            return new CoffeeB();
        } else if (coffeeType.contains("C")) {
            return new CoffeeC();
        }
        return null;
    }
}
