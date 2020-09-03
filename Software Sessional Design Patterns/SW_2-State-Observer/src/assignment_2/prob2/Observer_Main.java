package assignment_2.prob2;

import java.util.Scanner;

public class Observer_Main {

    static Scanner sc = new Scanner(System.in);

    public static void main(String[] args) {
        runTimeTest();
        //compiledTesting();

        //run1();
    }

    private static void runTimeTest() {
        //Run Time Not Possible Without GUI ?? 
        Subject subject = new Subject();
        subject.setStudentName("CR");
        subject.setStudentRoll("007");

        Observer o = new StudentObserver(subject);

        while (true) {
            String line = sc.nextLine();
            if(line.equals("EXIT")){
                break;
            }
            else if (line.contains("msg:") == true) {
                subject.setCurrentMessage(line);
            } else if (line.toLowerCase().contains("new user:")) {
                String []arr = setStudent(line);
                if(arr.length > 1){
                    o = new StudentObserver(subject);
                    String name = arr[0].trim();
                    String roll = arr[1].trim();
                    String mail = arr[2].trim();
                    o.setStudent(name, roll, mail);
                    o.subscribe();
                }
            } else if(line.toLowerCase().contains("delete user:")){
                String []arr = line.split(":");
                if(arr.length > 1){
                    String name = arr[1].trim();
                    o = subject.getObserver(name);
                    //System.out.println("PRINITNG o = " + o.toString());
                    if(o != null)
                        o.unsubscribe();
                    else
                        System.out.println("Observer with name " + name + " does not exist.");
                }
            }
        }
    }

    private static String[] setStudent(String line) {
        String[] arrInitial = line.split(":");
        if(arrInitial.length < 2)
            return null ;
        String[] arrFinal = arrInitial[1].split(",");
  
  
        
        return arrFinal;
    }

    private static void compiledTesting() {
        Subject subject = new Subject();
        subject.setStudentName("CR");
        subject.setStudentRoll("007");

        Observer o1 = new StudentObserver(subject);

        Observer o2 = new StudentObserver(subject);

        Observer o3 = new StudentObserver(subject);

        Observer o4 = new StudentObserver(subject);

        Observer o5 = new StudentObserver(subject);

        Observer o6 = new StudentObserver(subject);

        Observer o7 = new StudentObserver(subject);

        o1.setStudent("ObserverName1", "Roll_1", "email_1@gmail.com");
        o2.setStudent("ObserverName2", "Roll_2", "email_2@gmail.com");
        o3.setStudent("ObserverName3", "Roll_3", "email_3@gmail.com");
        o4.setStudent("ObserverName4", "Roll_4", "email_4@gmail.com");
        o5.setStudent("ObserverName5", "Roll_5", "email_5@gmail.com");
        o6.setStudent("ObserverName6", "Roll_6", "email_6@gmail.com");
        o7.setStudent("ObserverName7", "Roll_7", "email_7@gmail.com");

        o1.subscribe();
        o2.subscribe();
        o3.subscribe();
        o4.subscribe();
        o5.subscribe();
        o6.subscribe();
        o7.subscribe();

        subject.setCurrentMessage("This is message test number 1");

        o4.unsubscribe();

        subject.setCurrentMessage("This is message test number 2");

        o5.unsubscribe();
        o6.unsubscribe();

        subject.setCurrentMessage("This is message test number 3");

        o4.subscribe();

        subject.setCurrentMessage("This is message test number 4");

        o1.printAllMessages();
        o1.printCurrentMessage();
    }

    private static void run1() {
//        for(int i=1; i<=7; i++){
//            System.out.println("o" + (i) + ".subscribe();");
//        }
        for (int i = 1; i <= 7; i++) {
            System.out.println("o" + i + ".setStudent(\"ObserverName" + i + "\", \"Roll_" + i + "\", \"email_" + i + "@gmail.com\");");
        }
    }

}
/*
new user: Mahim, 22, vipergone@gmail.com
new user: Ronaldo, 33, siiiiiiiiu@ronaldo.CR7
new user: Harry Kane, 31, hKane@gmail.com
msg: Portugal gonna win the World Cup 
delete user: Ronaldo 
delete user: RON
*/