package assignment_2.prob2;

import java.util.List;

public abstract class Observer implements Student{
    
    public Subject subject;
    public abstract void update(); //to be overwritten ... 
    public abstract void subscribe();
    public abstract void unsubscribe();
    public abstract void printAllMessages();
    public abstract void printCurrentMessage();
}
