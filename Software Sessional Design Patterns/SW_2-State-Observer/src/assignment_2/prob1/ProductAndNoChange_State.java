package assignment_2.prob1;

public class ProductAndNoChange_State implements State {

    private final String description;
    private VendingMachine context; ///NEED THIS TO ACCESS THE LIST OF FOODITEMS IN VENDING MACHINE

    @Override
    public void doAction(VendingMachine context) {
        context.setState(this);
    }

    public ProductAndNoChange_State(VendingMachine context) {
        description = "This State gives Product With No Change.";
        this.context = context;
    }

    public String getDescription() {
        return description;
    }

    @Override
    public String toString() {
        return "ProductAndNoChange_State{" + "description=" + description + '}';
    }

 

    @Override
    public State getNextState(String name, int amount) {
        return new WaitingState(context);
    }

    @Override
    public void handle(String p, int amount) {
        //System.out.println("----------====>>>>INSIDE ProductAndNoChange.handle() .. Just Waiting for next input.. p = " + p);
        String s = "\n--------------------*****-------------------\n";
        s += "Returning the Product ";
        s += p;
        s += " With NO CHANGE";
        s += "\n--------------------*****-------------------";
        System.out.println(s);
        context.decrementItem(p);
    }

}
