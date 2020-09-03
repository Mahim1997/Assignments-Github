package offline3;

import java.util.List;


public class Helper {

   
    public static void printList(List<Point> list){
        list.forEach((p) -> {
            System.out.print(p.toString() + " ");
        });
        System.out.println();
    }
    public static void printArray(Point[]arr){
        for(Point p: arr){
            System.out.print(p.toString() + " ");
        }
        System.out.println();
    }
}
