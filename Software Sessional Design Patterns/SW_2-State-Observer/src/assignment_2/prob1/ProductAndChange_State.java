package assignment_2.prob1;

public class ProductAndChange_State implements State {

    private final String description;
    private VendingMachine context;
    private int change;

    @Override
    public void doAction(VendingMachine context) {
        context.setState(this);
    }

    public ProductAndChange_State(VendingMachine context) {
        description = "This State gives Product with Change..";
        this.context = context;
    }

    @Override
    public String toString() {
        return "ProductAndChange_State{" + "description=" + description + '}';
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
        Product product = context.getProduct(p);
        int change = amount - product.getCost();
        String s = "\n--------------------*****-------------------\n";
        s += "Returning the Product ";
        s += p;
        s += " With Change of ";
        s += change ;
        s += "\n--------------------*****-------------------";
        System.out.println(s);
        context.decrementItem(p);
        context.setCurrentChange(change);
        context.setMachineMoney(context.getMachineMoney() - change);
    }

}
