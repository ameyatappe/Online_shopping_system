package com.shopx;

/**
 * [OOP CONCEPT: HIERARCHICAL INHERITANCE & POLYMORPHISM]
 * Electronics inherits from Product and adds warranty details.
 * Implements getFinalPrice() by applying standard 18% GST.
 */
public class Electronics extends Product {
    private final int warrantyYears;

    public Electronics(int id, String name, double basePrice, int stock, int warrantyYears) {
        super(id, name, basePrice, stock);
        if (warrantyYears < 0) {
            throw new IllegalArgumentException("Warranty cannot be negative.");
        }
        this.warrantyYears = warrantyYears;
    }

    public int getWarrantyYears() {
        return warrantyYears;
    }

    @Override
    public double getFinalPrice() {
        // 18% GST on Electronics
        return basePrice * 1.18;
    }

    @Override
    public String getCategory() {
        return "Electronics";
    }

    @Override
    public String toString() {
        return super.toString() + " (" + warrantyYears + " yr warranty)";
    }
}
