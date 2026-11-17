package com.shopx;

import java.util.ArrayList;
import java.util.List;

/**
 * ==============================================================================
 * SHOPX - OBJECT ORIENTED PROGRAMMING (OOP) DEMONSTRATION IN JAVA
 * ==============================================================================
 * This companion module demonstrates the four foundational OOP pillars:
 * 
 * 1. ENCAPSULATION:
 *    - Private attributes with controlled access via getters, setters, and business methods.
 *    - Defensive validation preventing invalid state (negative prices, insufficient stock).
 *    - Encapsulated order histories and loyalty point balances.
 * 
 * 2. INHERITANCE:
 *    - Hierarchical Inheritance: Product -> Electronics, Clothing, Grocery, SportsEquipment.
 *    - Multilevel Inheritance: Product -> Electronics -> Laptop.
 *    - Single Inheritance + Interface: Customer -> PrimeMember implements LoyaltyRewardable.
 * 
 * 3. POLYMORPHISM:
 *    - Dynamic Method Binding (Runtime Polymorphism):
 *      Base reference Customer invokes overridden methods in PrimeMember at runtime.
 *      Base reference Product invokes overridden getFinalPrice() at runtime.
 *    - Compile-Time Polymorphism (Method Overloading):
 *      Cart.addItem(Product) and Cart.addItem(Product, int).
 *    - Functional Polymorphism:
 *      DiscountStrategy (Strategy Pattern) passed as first-class lambda expressions.
 * 
 * 4. ABSTRACTION:
 *    - Abstract class Product defines essential contracts while hiding tax implementation.
 *    - Interface LoyaltyRewardable specifies reward behavior without dictating class hierarchy.
 * ==============================================================================
 */
public class ShopXDemo {

    public static String repeat(char c, int count) {
        StringBuilder sb = new StringBuilder(count);
        for (int i = 0; i < count; i++) sb.append(c);
        return sb.toString();
    }

    public static void printHeader(String title) {
        System.out.println("\n" + repeat('=', 75));
        System.out.println("  " + title);
        System.out.println(repeat('=', 75));
    }

    public static void printSub(String title) {
        System.out.println("\n--- " + title + " ---");
    }

    public static void main(String[] args) {
        printHeader("ShopX — Online Shopping System (Java OOP Companion Demonstration)");

        // --------------------------------------------------------------------------
        // PILLAR 1: ABSTRACTION & INHERITANCE IN PRODUCT HIERARCHY
        // --------------------------------------------------------------------------
        printSub("1. ABSTRACTION & INHERITANCE — Product Catalog");
        System.out.println("Abstract Product base class extended by specialized category classes.");
        System.out.println("Each subclass computes final price with specific tax/insurance rules:\n");

        List<Product> catalog = new ArrayList<>();
        catalog.add(new Laptop(101, "Dell XPS Laptop", 55000.00, 5));
        catalog.add(new Electronics(102, "Smartphone 5G", 20000.00, 10, 1));
        catalog.add(new Electronics(103, "Wireless Headphones", 2500.00, 25, 1));
        catalog.add(new Electronics(104, "Fast-Charge Powerbank", 1499.00, 20, 1));
        catalog.add(new Clothing(201, "Denim Jacket", 2200.00, 15, "L"));
        catalog.add(new Clothing(202, "Cotton T-Shirt", 599.00, 40, "M"));
        catalog.add(new Grocery(301, "Basmati Rice 5kg", 450.00, 60));
        catalog.add(new Grocery(302, "Extra Virgin Olive Oil 1L", 700.00, 30));
        catalog.add(new SportsEquipment(401, "English Willow Cricket Bat", 2499.00, 12, "English Willow Grade 1"));

        for (Product p : catalog) {
            System.out.println("  " + p);
        }

        // --------------------------------------------------------------------------
        // PILLAR 2: ENCAPSULATION & DATA INTEGRITY
        // --------------------------------------------------------------------------
        printSub("2. ENCAPSULATION — Validation & State Protection");
        System.out.println("Verifying that encapsulated setters and stock methods reject invalid operations:");

        Product testProduct = catalog.get(5); // Cotton T-Shirt
        try {
            System.out.println("  Attempting to set negative price (-₹150)...");
            testProduct.setBasePrice(-150);
        } catch (IllegalArgumentException e) {
            System.out.println("  [ENCAPSULATION SHIELD ACTIVE] Blocked: " + e.getMessage());
        }

        try {
            System.out.println("  Attempting to reduce stock beyond availability (requesting 999 units)...");
            testProduct.reduceStock(999);
        } catch (IllegalStateException e) {
            System.out.println("  [ENCAPSULATION SHIELD ACTIVE] Blocked: " + e.getMessage());
        }

        // --------------------------------------------------------------------------
        // PILLAR 3: POLYMORPHISM — METHOD OVERLOADING (COMPILE-TIME)
        // --------------------------------------------------------------------------
        printSub("3. COMPILE-TIME POLYMORPHISM — Method Overloading in Cart");
        System.out.println("Demonstrating Cart.addItem() overloaded signatures:");

        Cart demoCart = new Cart();
        demoCart.addItem(catalog.get(3));       // addItem(Product) -> 1 unit of Powerbank
        demoCart.addItem(catalog.get(7), 2);    // addItem(Product, int) -> 2 units of Olive Oil
        System.out.println("  Overloaded addItem calls resolved successfully. Cart contents:");
        demoCart.printCart();

        // --------------------------------------------------------------------------
        // PILLAR 4: RUNTIME POLYMORPHISM & PRIME MEMBERSHIP EXPERIENCE
        // --------------------------------------------------------------------------
        printSub("4. RUNTIME POLYMORPHISM & FUNCTIONAL PRIME MEMBERSHIP");
        System.out.println("Creating two shoppers: a Regular Customer and a Prime Member.");
        System.out.println("Both are held in a polymorphic Customer[] array.");
        System.out.println("The JVM dynamically resolves the correct method at runtime:\n");

        Customer regularCustomer = new Customer("Rahul Sharma", "Mumbai", 400001);
        PrimeMember primeMember = new PrimeMember("Priya Patel", "Pune", 411038);

        Customer[] shoppers = { regularCustomer, primeMember };

        for (Customer shopper : shoppers) {
            System.out.println("\n------------------------------------------------------------");
            System.out.println("  CURRENT SHOPPER: " + shopper.getName() + " (" + shopper.getCustomerType() + ")");
            System.out.println("------------------------------------------------------------");
            shopper.displayProfile();

            // Create cart for this shopper with identical items:
            // 1 Cotton T-Shirt + 1 Basmati Rice 5kg
            Cart shopperCart = new Cart();
            shopperCart.addItem(catalog.get(5), 1); // Cotton T-Shirt
            shopperCart.addItem(catalog.get(6), 1); // Basmati Rice 5kg

            System.out.println("\n  Cart to purchase:");
            shopperCart.printCart();

            System.out.println("\n  Executing checkout with Festive Discount (10% off)...");
            // Dynamic Binding: placeOrder() executes Customer vs PrimeMember logic
            shopper.placeOrder(shopperCart, DiscountStrategy.FESTIVE_DISCOUNT);

            System.out.println("\n  Updated Profile after checkout:");
            shopper.displayProfile();
        }

        // --------------------------------------------------------------------------
        // SECOND ORDER: DEMONSTRATING REPEAT POINTS ACCUMULATION IN PRIME
        // --------------------------------------------------------------------------
        printSub("5. PRIME MEMBER POINTS PERSISTENCE ACROSS SESSIONS");
        System.out.println("Prime Member Priya places a second larger order (Laptop):");

        Cart secondCart = new Cart();
        secondCart.addItem(catalog.get(0), 1); // Dell Laptop
        primeMember.placeOrder(secondCart, DiscountStrategy.NO_DISCOUNT);

        System.out.println("\nFinal Prime Member Summary:");
        primeMember.displayProfile();

        printHeader("DEMONSTRATION COMPLETED SUCCESSFULLY (All 4 OOP Concepts Verified)");
    }
}
