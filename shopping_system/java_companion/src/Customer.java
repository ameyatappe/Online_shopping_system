package com.shopx;

import java.util.ArrayList;
import java.util.Collections;
import java.util.List;

/**
 * [OOP CONCEPT: ENCAPSULATION & BASE CLASS FOR INHERITANCE]
 * Customer encapsulates user identity, delivery address, and order history.
 * Acts as the base class for specialized customer tiers (e.g. PrimeMember).
 */
public class Customer {
    private static int idCounter = 1001;

    private final int customerId;
    private final String name;
    private String city;
    private int pinCode;
    protected final List<Double> orderHistory = new ArrayList<>();

    public Customer(String name, String city, int pinCode) {
        if (name == null || name.trim().isEmpty()) {
            throw new IllegalArgumentException("Customer name cannot be empty.");
        }
        this.customerId = idCounter++;
        this.name = name;
        this.city = city != null ? city : "Pune";
        this.pinCode = pinCode > 0 ? pinCode : 411001;
    }

    public Customer(String name) {
        this(name, "Pune", 411001);
    }

    public int getCustomerId() {
        return customerId;
    }

    public String getName() {
        return name;
    }

    public String getCity() {
        return city;
    }

    public void setCity(String city) {
        this.city = city;
    }

    public int getPinCode() {
        return pinCode;
    }

    public void setPinCode(int pinCode) {
        this.pinCode = pinCode;
    }

    public List<Double> getOrderHistory() {
        return Collections.unmodifiableList(orderHistory);
    }

    /**
     * [OOP CONCEPT: RUNTIME POLYMORPHISM - VIRTUAL METHOD EQUIVALENT]
     * In Java, all non-static, non-final methods are virtual by default.
     * Subclasses can override getCustomerType() to describe their tier.
     */
    public String getCustomerType() {
        return "Regular Customer";
    }

    public boolean isPrime() {
        return false;
    }

    /**
     * Calculates customer-tier exclusive discount.
     * Regular customers receive 0% tier discount.
     */
    public double calculateTierDiscount(double subtotal) {
        return 0.0;
    }

    /**
     * Standard delivery is ₹50 for orders below ₹1,000; free above ₹1,000.
     */
    public double calculateDeliveryFee(double subtotal) {
        return subtotal >= 1000.0 ? 0.0 : 50.0;
    }

    /**
     * [OOP CONCEPT: DYNAMIC METHOD BINDING]
     * Executes order checkout polymorphically based on customer type.
     */
    public double placeOrder(Cart cart, DiscountStrategy discountStrategy) {
        if (cart.isEmpty()) {
            throw new IllegalStateException("Cannot place order: Cart is empty.");
        }

        double grossSubtotal = cart.calculateSubtotal();
        double festiveAdjusted = discountStrategy.applyDiscount(grossSubtotal);
        double tierDiscount = calculateTierDiscount(festiveAdjusted);
        double deliveryFee = calculateDeliveryFee(grossSubtotal);
        double finalPayable = festiveAdjusted - tierDiscount + deliveryFee;

        // Commit stock reduction and clear cart
        cart.commitOrder();
        orderHistory.add(finalPayable);

        System.out.printf("  [Order Success] %s (#%d) placed an order.\n", name, customerId);
        System.out.printf("    Gross Subtotal:     ₹%.2f\n", grossSubtotal);
        if (festiveAdjusted < grossSubtotal) {
            System.out.printf("    Festive Discount:  -₹%.2f\n", (grossSubtotal - festiveAdjusted));
        }
        if (tierDiscount > 0) {
            System.out.printf("    Tier Discount:     -₹%.2f\n", tierDiscount);
        }
        System.out.printf("    Delivery Fee:      +₹%.2f %s\n", deliveryFee, (deliveryFee == 0 ? "(Free Delivery)" : ""));
        System.out.printf("    Total Amount Paid:  ₹%.2f\n", finalPayable);

        return finalPayable;
    }

    public void displayProfile() {
        System.out.printf("  Customer #%d: %s | Location: %s (%d) | Tier: %s | Total Orders: %d\n",
                customerId, name, city, pinCode, getCustomerType(), orderHistory.size());
    }
}
