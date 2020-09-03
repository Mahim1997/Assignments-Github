/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package problem_2;

/**
 *
 * @author esfs
 */
public class Customer {

    protected String customerName;
    protected String coffeeType;

    public Customer(String customerName) {
        this.customerName = customerName;
    }

    public Customer(String customerName, String coffeeType) {
        this.customerName = customerName;
        this.coffeeType = coffeeType;
    }

    public String getCustomerName() {
        return customerName;
    }

    public void setCustomerName(String customerName) {
        this.customerName = customerName;
    }

    public String getCoffeeType() {
        return coffeeType;
    }

    public void setCoffeeType(String coffeeType) {
        this.coffeeType = coffeeType;
    }

}
