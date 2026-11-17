package com.shopx;

/**
 * [OOP CONCEPT: ABSTRACTION & ENCAPSULATION]
 * Product is an abstract base class defining common attributes and contracts
 * for all sellable items in the ShopX ecosystem.
 * 
 * - Abstraction: Exposes contract getFinalPrice() and getCategory() while hiding
 *   category-specific tax and surcharge calculations.
 * - Encapsulation: Fields are private/protected; access is controlled via
 *   validated getters and setters.
 */
public abstract class Product {
    private final int id;
    private String name;
    protected double basePrice;
    protected int stock;

    public Product(int id, String name, double basePrice, int stock) {
        if (id <= 0) {
            throw new IllegalArgumentException("Product ID must be positive.");
        }
        if (name == null || name.trim().isEmpty()) {
            throw new IllegalArgumentException("Product name cannot be empty.");
        }
        if (basePrice < 0) {
            throw new IllegalArgumentException("Price cannot be negative: ₹" + basePrice);
        }
        if (stock < 0) {
            throw new IllegalArgumentException("Stock cannot be negative: " + stock);
        }
        this.id = id;
        this.name = name;
        this.basePrice = basePrice;
        this.stock = stock;
    }

    // Getters and Setters (Encapsulation)
    public int getId() {
        return id;
    }

    public String getName() {
        return name;
    }

    public void setName(String name) {
        if (name == null || name.trim().isEmpty()) {
            throw new IllegalArgumentException("Product name cannot be empty.");
        }
        this.name = name;
    }

    public double getBasePrice() {
        return basePrice;
    }

    public void setBasePrice(double basePrice) {
        if (basePrice < 0) {
            throw new IllegalArgumentException("Price cannot be negative.");
        }
        this.basePrice = basePrice;
    }

    public int getStock() {
        return stock;
    }

    public void reduceStock(int quantity) {
        if (quantity <= 0) {
            throw new IllegalArgumentException("Quantity must be greater than zero.");
        }
        if (quantity > stock) {
            throw new IllegalStateException("Insufficient stock for " + name + " (Requested: " + quantity + ", Available: " + stock + ")");
        }
        this.stock -= quantity;
    }

    /**
     * [OOP CONCEPT: PURE ABSTRACTION]
     * Abstract method that every concrete product category must implement.
     * Calculates tax, warranty, or duties dynamically.
     */
    public abstract double getFinalPrice();

    /**
     * Returns the category name.
     */
    public abstract String getCategory();

    @Override
    public String toString() {
        return String.format("[ID #%d] %-20s | Cat: %-12s | Base: ₹%.2f | Final (incl. tax): ₹%.2f | Stock: %d",
                id, name, getCategory(), basePrice, getFinalPrice(), stock);
    }
}
