package assignment_2.prob1;

public class NoProduct_InsufMachineMoney implements State {

    private final String description;
    private VendingMachine context;

    public NoProduct_InsufMachineMoney(VendingMachine context) {
        this.description = "This State gives NO PRODUCT Due to Machine not having sufficient change.";
        this.context = context;
    }

    @Override
    public void doAction(VendingMachine context) {
        context.setState(this);
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
        s += " Since Machine Does Not Have Sufficient Change.";
        s += "\n--------------------*****-------------------";
        System.out.println(s);
    }

    @Override
    public String toString() {
        return "NoProduct_InsufMachineMoney{" + "description=" + description + '}';
    }

}
