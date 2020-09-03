package assignment_2.prob2;

import java.util.ArrayList;
import java.util.List;

public class StudentObserver extends Observer implements Student {

    private String studentName;
    private String studentRoll;
    private List<String> messages;
    private String studentEmail;

    public StudentObserver(Subject subject) {
        this.subject = subject;
        this.messages = new ArrayList<String>();    //All messages
    }

    @Override
    public void subscribe() {
        this.subject.addObserver(this);
    }

    @Override
    public void unsubscribe() {
        this.subject.removeObserver(this);
    }

    @Override
    public void update() {
        System.out.print("Inside " + this.toString() + " \nUpdated message is : ");
        System.out.println(this.subject.getCurrentMessage());
        this.messages.add(this.subject.getCurrentMessage());    //To Store in list of messages ... 
    }

 

    public void printAllMessages() {
        System.out.println("-------------------------***<<Inside printAllMessages>****------------------------");
        System.out.println("Inside .. " + this.toString());
        for (String s : messages) {
            System.out.println(s);
        }
        System.out.println("-------------------------***<<DONE printAllMessages>****------------------------");
    }

    public void printCurrentMessage() {
        if (messages.size() >= 1) {
            System.out.println("Inside .. " + this.toString());
            System.out.println("Current msg : " + messages.get(messages.size() - 1));
        }
    }

    //-----------------------------------------------------FOR STUDENT INTERFACE.. ---------------------------------------------------------
    public String getStudentName() {
        return studentName;
    }

    public void setStudentName(String studentName) {
        this.studentName = studentName;
    }

    public String getStudentRoll() {
        return studentRoll;
    }

    public void setStudentRoll(String studentRoll) {
        this.studentRoll = studentRoll;
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

    public List<String> getMessages() {
        return messages;
    }

    public void setMessages(List<String> messages) {
        this.messages = messages;
    }

    public String getStudentEmail() {
        return studentEmail;
    }

    public void setStudentEmail(String studentEmail) {
        this.studentEmail = studentEmail;
    }

    @Override
    public String toString() {
        return "StudentObserver{" + "studentName=" + studentName + ", studentRoll=" + studentRoll + ", studentEmail=" + studentEmail + '}';
    }
}
