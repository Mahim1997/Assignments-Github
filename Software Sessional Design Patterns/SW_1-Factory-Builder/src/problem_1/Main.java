/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package problem_1;

import java.io.*;
import java.util.*;
import problem_1.people.*;
import problem_1.product.Burger;

/**
 *
 * @author esfs
 */
public class Main {

    /**
     * @param args the command line arguments
     */
    static Scanner sc = new Scanner(System.in);

    public static void main(String[] args) {
        // TODO code application logic here
        Waiter waiter = new Waiter();
        runner(waiter);
    }

    private static void makeNewClient(String clientName, Order order, Waiter waiter) {
//        System.out.println("INSIDE makeNewClient==>> Client Name: " + client);
//        System.out.println("PRINTING ORDER ===>> ");
//        order.printOrder();
//        makeNewClient(client, order.burgerType, order.pattyList, order.cheeseList, order.sauceList, waiter);
        Client client = new Client(clientName);
        client.setOrder(order);

        client.order(waiter);

        System.out.println("-------<><><><><><><><><><><><>-----------");
        Burger burger = waiter.construct(waiter.getBuilder(order.getBurgerType()));
        client.printClient();
        burger.printBurger();
        System.out.println("-------<><><><><><><><><><><><>-----------");
    }

    private static void runner(Waiter waiter) {
        System.out.println("Enter Client Name: ");
        String clientName = sc.nextLine();
        String burgerType = "";
        Order order = null;
        while (true) {
            System.out.println("O/o- Order\tE/e- Exit");
            String s = sc.nextLine();
            if (s.equalsIgnoreCase("O")) {
                order = getListOfOrders();
            } else if (s.equalsIgnoreCase("E")) {
                break;
            } else {
                continue;
            }
        }
        makeNewClient(clientName, order, waiter);
    }

    private static Order getListOfOrders() {
        System.out.println("Enter Burger Type: C/c for Chicken\t B/b for Beef");
        String burgerType = sc.nextLine();
        if (burgerType.contains("B")) {
            burgerType = UtilItems.BEEF_BURGER;
        } else if (burgerType.contains("C")) {
            burgerType = UtilItems.CHICKEN_BURGER;
        }

        List<String> pattyList = getPattyList(burgerType);
        List<String> cheeseList = getCheeseList();
        List<String> sauceList = getSauceList();
        Order order = new Order(burgerType, pattyList, cheeseList, sauceList);
        return order;
    }

    private static List<String> getPattyList(String burgerType) {
        List<String> list = new ArrayList<>();
        System.out.println("PATTY MENU: <E/e for exit>");
        System.out.println("1- " + burgerType + "\t2- " + burgerType + " with Cheese");
        System.out.println("3- " + burgerType + " Deep Fried" + "\t4- " + burgerType + " With Bacon\n");
        while (true) {
            System.out.println("===-->>>ORDER PATTY... ");
            String choice = sc.nextLine();
            if (choice.equalsIgnoreCase("E")) {
                break;
            }
            list.add(choice);

        }
        System.out.println("+++>>> PATTIES ORDERED... ");
        UtilItems.printList(list);
        return list;
    }

    private static List<String> getCheeseList() {
        List<String> list = new ArrayList<>();
        System.out.println("Cheese MENU: <E/e for exit>");
        while (true) {
            System.out.println("1- Normal Cheese\t2- Cheddar Cheese\n3- Crushed Cheese\t4- Molten Cheese");
            String choice = sc.nextLine();

            if (choice.equalsIgnoreCase("E")) {
                break;
            }
            list.add(choice);
        }
        return list;
    }

    private static List<String> getSauceList() {
        List<String> list = new ArrayList<>();
        System.out.println("Sauce MENU: <E/e for exit>");
        while (true) {
            System.out.println("1- Normal Ketchup" + "\t2- Hot Sauce");
            System.out.println("3- White Sauce\t4- Garlic Sauce");
            String choice = sc.nextLine();

            if (choice.equalsIgnoreCase("E")) {
                break;
            }
            list.add(choice);
        }
        return list;
    }

}
/*


Mahim
o
Chicken
Chicken 
Chicken with Cheese
e
Cheddar Cheese
Cheddar Cheese 
Molten Cheese 
e
White Sauce
Hot Sauce 
e
e

*/