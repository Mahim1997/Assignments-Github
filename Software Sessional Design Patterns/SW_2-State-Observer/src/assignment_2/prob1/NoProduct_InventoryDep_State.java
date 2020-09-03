package assignment_2.prob1;

public class NoProduct_InventoryDep_State implements State {

    private final String description;
    private VendingMachine context;

    @Override
    public void doAction(VendingMachine context) {
        context.setState(this);
    }

    public NoProduct_InventoryDep_State(VendingMachine context) {
        description = "This State gives No Product due to Inventory Depletion..";
        this.context = context;
    }

    @Override
    public String toString() {
        return "NoProduct_InventoryDep_State{" + "description=" + description + '}';
    }

    public String getDescription() {
        return description;
    }

    @Override
    public State getNextState(String name, int amount) {
        return new WaitingState(context);
    }

    @Override
    public void handle(String p, int amount) {
   
        String s = "\n--------------------*****-------------------\n";
        s += "NOT RETURNING THE PRODUCT ";
        s += p;
        s += " Since This Does Not Exist";
        s += "\n--------------------*****-------------------";
        System.out.println(s);
    }
}
