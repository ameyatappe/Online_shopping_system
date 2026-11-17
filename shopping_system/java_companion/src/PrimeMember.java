package com.shopx;

/**
 * [OOP CONCEPT: INHERITANCE, INTERFACE IMPLEMENTATION & RUNTIME POLYMORPHISM]
 * PrimeMember demonstrates:
 * - Single Inheritance: extends Customer to reuse customer identity, address, and history.
 * - Multiple Interface Inheritance: implements LoyaltyRewardable (Java's clean alternative
 *   to C++ multiple class inheritance).
 * - Method Overriding (Runtime Polymorphism): overrides calculateTierDiscount(),
 *   calculateDeliveryFee(), getCustomerType(), and placeOrder().
 * - Encapsulation: loyalty points are encapsulated and can only grow via earned shopping orders.
 */
public class PrimeMember extends Customer implements LoyaltyRewardable {
    private int loyaltyPoints;
    private final double exclusiveDiscountRate;

    public PrimeMember(String name, String city, int pinCode) {
        super(name, city, pinCode);
        this.loyaltyPoints = 0;
        this.exclusiveDiscountRate = 0.05; // 5% Prime Member exclusive discount
    }

    public PrimeMember(String name) {
        this(name, "Pune", 411001);
    }

    @Override
    public String getCustomerType() {
        return "Prime Member";
    }

    @Override
    public boolean isPrime() {
        return true;
    }

    @Override
    public double getExclusiveDiscountRate() {
        return exclusiveDiscountRate;
    }

    @Override
    public int getLoyaltyPoints() {
        return loyaltyPoints;
    }

    @Override
    public void addLoyaltyPoints(int points) {
        if (points > 0) {
            this.loyaltyPoints += points;
        }
    }

    @Override
    public String getRewardsSummary() {
        return "⭐ Prime Rewards Balance: " + loyaltyPoints + " loyalty points";
    }

    /**
     * [OOP: METHOD OVERRIDING]
     * Overrides Customer.calculateTierDiscount: Prime Members receive 5% off every order.
     */
    @Override
    public double calculateTierDiscount(double subtotal) {
        return subtotal * exclusiveDiscountRate;
    }

    /**
     * [OOP: METHOD OVERRIDING]
     * Overrides Customer.calculateDeliveryFee: Prime Members enjoy FREE Priority Delivery on all orders.
     */
    @Override
    public double calculateDeliveryFee(double subtotal) {
        return 0.0;
    }

    /**
     * [OOP: RUNTIME POLYMORPHISM & METHOD OVERRIDING]
     * Extends base checkout behavior: after placing order via base logic,
     * calculates loyalty points (5 points per ₹100 spent) and credits points.
     */
    @Override
    public double placeOrder(Cart cart, DiscountStrategy discountStrategy) {
        double grossSubtotal = cart.calculateSubtotal();
        double paidAmount = super.placeOrder(cart, discountStrategy);

        // Award 5 loyalty points per ₹100 of gross order value
        int pointsEarned = (int) (grossSubtotal / 100.0) * 5;
        addLoyaltyPoints(pointsEarned);

        System.out.printf("    ⭐ [Prime Perks Applied] Free Priority Express Delivery + 5%% Member Discount!\n");
        System.out.printf("    ⭐ [Loyalty Points] +%d pts earned on this order! Total Points: %d\n",
                pointsEarned, loyaltyPoints);

        return paidAmount;
    }

    @Override
    public void displayProfile() {
        super.displayProfile();
        System.out.printf("    ⭐ Status: ACTIVE PRIME SUBSCRIBER | %s\n", getRewardsSummary());
    }
}
