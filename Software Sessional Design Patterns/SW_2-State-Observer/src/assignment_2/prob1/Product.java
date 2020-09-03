package assignment_2.prob1;

public class Product  {

    protected String foodType;
    protected String name;
    protected int cost;
    protected int numberOfItems;

    public Product(String name,int c ,int n) {
        setFoodItem(name, c,n);
    }

     
    public void setFoodItem(String name, int c,int n) {
        this.foodType = "DRINKS";
        this.name = name;
        this.cost = c;
        this.numberOfItems = n;
    }

    public int getNumberOfItems() {
        return numberOfItems;
    }

    public void setNumberOfItems(int numberOfItems) {
        this.numberOfItems = numberOfItems;
    }

    @Override
    public String toString() {
        return "Drinks{" + "foodType=" + foodType + ", name=" + name + ", cost=" + cost + " noOfItems = " + numberOfItems + '}';
    }

  
    public void printProduct() {
        System.out.println(this.toString());
    }

    public String getFoodType() {
        return foodType;
    }

    public void setFoodType(String foodType) {
        this.foodType = foodType;
    }

    public String getName() {
        return name;
    }

    public void setName(String name) {
        this.name = name;
    }

    public int getCost() {
        return cost;
    }

    public void setCost(int cost) {
        this.cost = cost;
    }

    public void decrementNumber() {
        this.numberOfItems -- ;
    }
}
