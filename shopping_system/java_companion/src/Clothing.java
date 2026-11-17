package com.shopx;

/**
 * [OOP CONCEPT: HIERARCHICAL INHERITANCE]
 * Clothing inherits from Product and encapsulates apparel-specific attributes (size).
 * Implements getFinalPrice() with 5% GST.
 */
public class Clothing extends Product {
    private final String size;

    public Clothing(int id, String name, double basePrice, int stock, String size) {
        super(id, name, basePrice, stock);
        this.size = size != null ? size : "M";
    }

    public String getSize() {
        return size;
    }

    @Override
    public double getFinalPrice() {
        // 5% GST on Clothing
        return basePrice * 1.05;
    }

    @Override
    public String getCategory() {
        return "Clothing";
    }

    @Override
    public String toString() {
        return super.toString() + " [Size: " + size + "]";
    }
}
