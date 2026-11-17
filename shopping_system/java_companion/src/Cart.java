package com.shopx;

import java.util.ArrayList;
import java.util.Collections;
import java.util.List;

/**
 * [OOP CONCEPT: ENCAPSULATION & METHOD OVERLOADING]
 * Cart encapsulates cart line items and protects internal state.
 * Demonstrates:
 * - Encapsulation: items list is private and guarded against arbitrary external modification.
 * - Method Overloading (Compile-Time Polymorphism): addItem(Product), addItem(Product, int).
 * - Aggregation / Polymorphic invocation: calls item.product.getFinalPrice() via runtime polymorphism.
 */
public class Cart {
    public static class CartItem {
        private final Product product;
        private int quantity;

        public CartItem(Product product, int quantity) {
            this.product = product;
            this.quantity = quantity;
        }

        public Product getProduct() { return product; }
        public int getQuantity() { return quantity; }
        public void addQuantity(int q) { this.quantity += q; }
        public double getLineTotal() { return product.getFinalPrice() * quantity; }
    }

    private final List<CartItem> items = new ArrayList<>();

    // Overloaded Method 1: Add single unit (Compile-time polymorphism)
    public void addItem(Product product) {
        addItem(product, 1);
    }

    // Overloaded Method 2: Add specific quantity
    public void addItem(Product product, int quantity) {
        if (product == null) {
            throw new IllegalArgumentException("Product cannot be null.");
        }
        if (quantity <= 0) {
            throw new IllegalArgumentException("Quantity must be greater than zero.");
        }
        if (quantity > product.getStock()) {
            throw new IllegalStateException("Cannot add " + quantity + " units of " + product.getName() + " (Only " + product.getStock() + " in stock).");
        }

        for (CartItem item : items) {
            if (item.getProduct().getId() == product.getId()) {
                if (item.getQuantity() + quantity > product.getStock()) {
                    throw new IllegalStateException("Total in cart exceeds available stock.");
                }
                item.addQuantity(quantity);
                return;
            }
        }
        items.add(new CartItem(product, quantity));
    }

    public List<CartItem> getItems() {
        return Collections.unmodifiableList(items);
    }

    public boolean isEmpty() {
        return items.isEmpty();
    }

    public int getItemCount() {
        int count = 0;
        for (CartItem item : items) count += item.getQuantity();
        return count;
    }

    /**
     * Polymorphically calculates total cart value using getFinalPrice().
     */
    public double calculateSubtotal() {
        double subtotal = 0.0;
        for (CartItem item : items) {
            subtotal += item.getLineTotal();
        }
        return subtotal;
    }

    public void commitOrder() {
        for (CartItem item : items) {
            item.getProduct().reduceStock(item.getQuantity());
        }
        items.clear();
    }

    public void clear() {
        items.clear();
    }

    public void printCart() {
        if (isEmpty()) {
            System.out.println("  (Your cart is currently empty)");
            return;
        }
        System.out.println("  ---------------- Cart Items ----------------");
        for (CartItem item : items) {
            System.out.printf("  * %-20s x %-2d = ₹%.2f (Unit: ₹%.2f)\n",
                    item.getProduct().getName(),
                    item.getQuantity(),
                    item.getLineTotal(),
                    item.getProduct().getFinalPrice());
        }
        System.out.printf("  Subtotal (all items incl. tax): ₹%.2f\n", calculateSubtotal());
        System.out.println("  --------------------------------------------");
    }
}
