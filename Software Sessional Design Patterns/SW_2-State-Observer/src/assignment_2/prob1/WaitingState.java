package assignment_2.prob1;

public class WaitingState implements State {

    private final String description;
    private VendingMachine context;

    public WaitingState(VendingMachine context) {
        description = "This State waits for currency..";
        this.context = context;
    }

    @Override
    public String toString() {
        return "WaitingState{" + "description=" + description + '}';
    }

    @Override
    public void doAction(VendingMachine context) {
        context.setState(this);
    }

    @Override
    public State getNextState(String name, int amount) {
        State nextState = null;
        nextState = new NoProduct_InventoryDep_State(context);
        for (int i = 0; i < context.getProducts().size(); i++) {
            Product prod = context.getProducts().get(i);
            //System.out.println("INSIDE getNextStae ... name = " + name+ ", printing product = " + prod.toString());
            if ((prod.getName().equalsIgnoreCase(name) == true) && (prod.getNumberOfItems() >= 1)) {

                //Does exist here.
                int cost = prod.getCost();
                if (amount == cost) {
                    //EKDOM MILSE GIVE PRODUCT WITH NO CHANGE..
                    nextState = new ProductAndNoChange_State(context);
                } else if (amount > cost) {
                    //GIVE CHANGE 
                    int change = (amount - cost);
                    if (this.context.getMachineMoney() >= change) {
                        nextState = new ProductAndChange_State(context);
                    }else{
                        nextState = new NoProduct_InsufMachineMoney(context);
                    }
                } else {
                    //NOT ENOUGH MONEY GIVEN.
                    nextState = new NoProduct_InsuffCurrency_State(context);
                }
            }
        }

        return nextState;
    }

    @Override
    public void handle(String p, int amount) {
        //NOTHING TO DO IN WAITING STATE ...
        System.out.println("----------====>>>>INSIDE WaitingState.handle() .. Just Waiting for next input.. p = " + p);
    }
}
