package com.shopx;

/**
 * [OOP CONCEPT: POLYMORPHISM VIA INTERFACE / STRATEGY PATTERN]
 * Mirrors C++ function pointer strategy (double (*disc)(double)).
 * In Java, functional interfaces represent first-class polymorphic algorithms.
 */
@FunctionalInterface
public interface DiscountStrategy {
    double applyDiscount(double grossTotal);

    // Standard strategies
    DiscountStrategy NO_DISCOUNT = gross -> gross;
    DiscountStrategy FESTIVE_DISCOUNT = gross -> gross * 0.90; // 10% festive discount
}
