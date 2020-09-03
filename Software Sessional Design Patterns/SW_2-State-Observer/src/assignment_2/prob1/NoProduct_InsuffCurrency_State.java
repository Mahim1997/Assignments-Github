package assignment_2.prob1;

public class NoProduct_InsuffCurrency_State implements State {

    private final String description;
    private VendingMachine context;

    @Override
    public void doAction(VendingMachine context) {
        context.setState(this);
    }

    public NoProduct_InsuffCurrency_State(VendingMachine context) {
        description = "This State gives No Product due to Insufficient Currency.";
        this.context = context;
    }

    public String getDescription() {
        return description;
    }

    @Override
    public String toString() {
        return "NoProduct_InsuffCurrency_State{" + "description=" + description + '}';
    }

    @Override
    public State getNextState(String name, int amount) {
        return new WaitingState(context);
    }

    @Override
    public void handle(String p, int amount) {
        Product prod = context.getProduct(p);
        String s = "\n--------------------*****-------------------\n";
        s += "NOT RETURNING THE PRODUCT ";
        s += p;
        s += " Since Insufficient Currency ";
        s += " Products's price is ";
        s += prod.getCost();
        s += " and amount given is ";
        s += amount ;
        s += "\n--------------------*****-------------------";
        System.out.println(s);
    }
}
