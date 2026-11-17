package com.shopx;

/**
 * [OOP CONCEPT: HIERARCHICAL INHERITANCE]
 * Grocery inherits from Product.
 * Implements getFinalPrice() with 5% GST for packaged food staples.
 */
public class Grocery extends Product {
    public Grocery(int id, String name, double basePrice, int stock) {
        super(id, name, basePrice, stock);
    }

    @Override
    public double getFinalPrice() {
        // 5% GST on packaged groceries
        return basePrice * 1.05;
    }

    @Override
    public String getCategory() {
        return "Grocery";
    }
}
