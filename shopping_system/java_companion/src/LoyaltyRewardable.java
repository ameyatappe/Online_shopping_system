package com.shopx;

/**
 * [OOP CONCEPT: INTERFACE ABSTRACTION]
 * In Java, multiple class inheritance is not permitted, preventing the classic
 * diamond problem. Instead, Java utilizes interfaces for multiple type contracts.
 * 
 * LoyaltyRewardable defines the contract for members eligible for loyalty rewards,
 * points accumulation, and member discount rates.
 */
public interface LoyaltyRewardable {
    /**
     * Gets current loyalty points balance.
     */
    int getLoyaltyPoints();

    /**
     * Adds loyalty points earned from an order.
     * @param points points to add
     */
    void addLoyaltyPoints(int points);

    /**
     * Exclusive membership discount rate (e.g., 0.05 for 5% off).
     */
    double getExclusiveDiscountRate();

    /**
     * Formatted rewards summary.
     */
    String getRewardsSummary();
}
