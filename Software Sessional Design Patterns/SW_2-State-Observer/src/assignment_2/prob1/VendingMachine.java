package assignment_2.prob1;

import java.util.List;

public class VendingMachine {   //This is Context

    private State state;
    private Product currentProduct;
    private int machineMoney;
    private int currentChange;

    public int getCurrentChange() {
        return currentChange;
    }

    public void setCurrentChange(int currentChange) {
        this.currentChange = currentChange;
    }
    private List<Product> products;

    public VendingMachine(List<Product> list) {
        state = new WaitingState(this);   //default state is null here.
        products = list;
        currentChange = 0;
    }

    public void request(String p, int amount) {
        this.state.handle(p, amount);
    }

    ///-----------GETTER AND SETTERS
    public void setState(State s) {
        this.state = s;
        //System.out.println("======>>>> FOR DEBUGG... Inside setState(state) ... this.state = " + this.state.toString());
    }

    public State getState() {
        return this.state;
    }

    public Product getCurrentProduct() {
        return currentProduct;
    }

    public void setCurrentProduct(Product currentProduct) {
        this.currentProduct = currentProduct;
    }

    public List<Product> getProducts() {
        return products;
    }

    public void setProducts(List<Product> products) {
        this.products = products;
    }

    public void printListOfItems() {
         
        int x = 0;
        for (int i = 0; i < products.size(); i++) {
            x += products.get(i).getNumberOfItems();
        }
        System.out.println("Total Number of items = " + x);
        products.forEach((p) -> {
            System.out.println(p);
        });

    }

    public int getMachineMoney() {
        return machineMoney;
    }

    public void setMachineMoney(int machineMoney) {
        this.machineMoney = machineMoney;
    }

    public Product getProduct(String name) {
        for (int i = 0; i < products.size(); i++) {
            if (products.get(i).getName().equalsIgnoreCase(name) == true) {
                return products.get(i);
            }
        }
        return null;
    }

    public void decrementItem(String name) {
        Product prod = getProduct(name);
        prod.decrementNumber();
        int cost = prod.getCost();
        this.currentProduct = prod;
        this.machineMoney += cost;
    }

    public void printMachine() {
        System.out.println("-----------------------------PRINTING MACHINE BEGIN------------------------------------");
        System.out.println("Machine Money = " + this.machineMoney);
        System.out.println("Current Change = " + this.currentChange);
        System.out.println("Current State = " + this.state.toString());
        printListOfItems();

        System.out.println("-----------------------------PRINTING MACHINE DONE------------------------------------");
    }
}
