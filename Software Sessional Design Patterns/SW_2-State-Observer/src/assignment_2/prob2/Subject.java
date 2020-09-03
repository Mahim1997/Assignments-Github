package assignment_2.prob2;

import java.util.ArrayList;
import java.util.List;

public class Subject implements Student {

    private final List<Observer> observers;
    //private int state;
    private String currentMessage;

    private String studentName;
    private String studentRoll;
    private String studentEmail;

    public Subject() {
        this.observers = new ArrayList<>();
    }

    public void addObserver(Observer o) {
        boolean flag = this.observers.add(o);
        if (flag) {
            System.out.println("--->>> Inside Subject: The Observer " + o + " is added.");
        }
    }

    public void removeObserver(Observer o) {
        boolean flag = this.observers.remove(o);
        if (flag) {
            System.out.println("==>>> INSIDE Subject: The Observer " + o + " is removed.");
        }
    }

    public void notifyAllObservers() {
        for (Observer o : observers) {
            o.update();
        }
    }

    public String getCurrentMessage() {
        return this.currentMessage;
    }

    public void setCurrentMessage(String msg) {
        this.currentMessage = msg;
        notifyAllObservers();
    }

    @Override
    public String toString() {
        return "Subject{" + "studentName=" + studentName + ", studentRoll=" + studentRoll + ", studentEmail=" + studentEmail + '}';
    }

    //-----------------------------------------------------FOR STUDENT INTERFACE.. ---------------------------------------------------------
    @Override
    public void setStudentName(String name) {
        this.studentName = name;
    }

    @Override
    public void setStudentRoll(String roll) {
        this.studentRoll = roll;
    }

    @Override
    public String getStudentName() {
        return this.studentName;
    }

    public String getStudentEmail() {
        return studentEmail;
    }

    public void setStudentEmail(String studentEmail) {
        this.studentEmail = studentEmail;
    }

    @Override
    public String getStudentRoll() {
        return this.studentRoll;
    }

    @Override
    public void printStudent() {
        System.out.println(this.toString());
    }

    @Override
    public void setStudent(String name, String roll) {
        this.studentName = name;
        this.studentRoll = roll;
    }

    @Override
    public void setStudent(String name, String roll, String email) {
        this.studentName = name;
        this.studentRoll = roll;
        this.studentEmail = email;
    }
    public Observer getObserver(String name){
        for(Observer o: this.observers){
            if(o.getStudentName().equalsIgnoreCase(name))
                return o ;
        }
        return null ;
    }
}
