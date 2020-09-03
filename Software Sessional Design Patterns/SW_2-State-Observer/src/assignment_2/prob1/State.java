package assignment_2.prob1;

public interface State {

    public void doAction(VendingMachine context);

    public String toString();
 
    
    public State getNextState(String name, int amount);
    
    public void handle(String p, int amount);
}
