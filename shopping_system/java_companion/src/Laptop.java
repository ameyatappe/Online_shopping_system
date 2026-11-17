package com.shopx;

/**
 * [OOP CONCEPT: MULTILEVEL INHERITANCE & METHOD OVERRIDING]
 * Laptop extends Electronics, which extends Product.
 * Overrides getFinalPrice() to include hardware insurance on top of GST.
 */
public class Laptop extends Electronics {
    private final double insuranceFee;

    public Laptop(int id, String name, double basePrice, int stock, int warrantyYears, double insuranceFee) {
        super(id, name, basePrice, stock, warrantyYears);
        this.insuranceFee = insuranceFee;
    }

    public Laptop(int id, String name, double basePrice, int stock) {
        this(id, name, basePrice, stock, 2, basePrice * 0.02); // 2-year warranty, 2% insurance
    }

    @Override
    public double getFinalPrice() {
        // Base Electronics price with GST + accidental insurance
        return super.getFinalPrice() + insuranceFee;
    }

    @Override
    public String getCategory() {
        return "Laptop";
    }

    @Override
    public String toString() {
        return super.toString() + " [Included: Insurance ₹" + String.format("%.2f", insuranceFee) + "]";
    }
}
