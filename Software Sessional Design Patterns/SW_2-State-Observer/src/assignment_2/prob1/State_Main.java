package assignment_2.prob1;

import java.util.ArrayList;
import java.util.List;
import java.util.Scanner;

public class State_Main {

    static Scanner sc = new Scanner(System.in);

    public static void main(String[] args) {

        run();

    }

    private static void run() {
        VendingMachine machine = new VendingMachine(getInitialListOfProducts());
        machine.setMachineMoney(300);
        machine.printListOfItems();
        WaitingState waitingState = new WaitingState(machine);
  
        /*   ProductAndNoChange_State pState1 = new ProductAndNoChange_State(machine);
        ProductAndChange_State pState2 = new ProductAndChange_State(machine);
        NoProduct_InventoryDep_State noProdInventoryState = new NoProduct_InventoryDep_State(machine);
        NoProduct_InsuffCurrency_State noProdInsufficientState = new NoProduct_InsuffCurrency_State(machine);
    */
        State state2 = null;    //TO EVALUATE AT RUN TIME ... by the waitingState .

        while (true) {
            System.out.println("\nSTART!!!");
            waitingState.doAction(machine);
            System.out.println("\n\n");
            //showMenu(machine.getProducts());
            String nameOfOrderItem = getProductFromMachine();
            int amountGiven = getAmountFromMachine();
            System.out.println("Inside Main: NameTyped = " + nameOfOrderItem + " , Amount = " + amountGiven);
            System.out.println("");
            
            
            state2 = waitingState.getNextState(nameOfOrderItem, amountGiven);
            state2.doAction(machine);
            machine.request(nameOfOrderItem, amountGiven);
            System.out.println("+++++--------------->>>> AFTER MACHINE.REQUEST ... printing machine ... ");
            machine.printMachine();
            
            System.out.println(""); System.out.println(""); System.out.println("");
        }
    }

    private static List<Product> getInitialListOfProducts() {
        List<Product> list = new ArrayList<>();
        list.add(new Product("Dew", 40 , 7)); //7 Mountain Dews.
        list.add(new Product("Coke", 20 ,4));
        list.add(new Product("Sprite", 30 ,5));
        list.add(new Product("Fanta", 60 ,6));
        list.add(new Product("Mojo", 15 ,10));
        list.add(new Product("RedBull", 120 ,5));

        return list;
    }

    private static String getProductFromMachine() {
        System.out.println("Please Enter Name of Product : ");
        String name = sc.next();
        return name;
    }

    private static int getAmountFromMachine() {
        System.out.println("Please Enter Amount : ");
        int amount = sc.nextInt();
        return amount;
    }

    private static void showMenu(List<Product> products) {
        System.out.println("=============<<< PRINTING MENU >>>===============");

        products.forEach((p) -> {
            System.out.println("Food Type: " + p.getFoodType() + " , Name : "
                    + p.getName() + " , Cost : " + p.getCost() + " NumberOfItems = " + p.getNumberOfItems());
        });

        System.out.println("=============<<< DONE PRINTING MENU >>>===============");
    }

}
/*
    Dew 20
    Dew 25
    Dew 30
    Coke 400
    Coke 20
    Coke 40
    ALALALLALA 20
*/