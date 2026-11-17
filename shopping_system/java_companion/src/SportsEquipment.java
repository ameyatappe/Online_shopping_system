package com.shopx;

/**
 * [OOP CONCEPT: HIERARCHICAL INHERITANCE]
 * SportsEquipment inherits from Product and encapsulates sports-specific attributes (material).
 * Implements getFinalPrice() with 12% GST.
 */
public class SportsEquipment extends Product {
    private final String material;

    public SportsEquipment(int id, String name, double basePrice, int stock, String material) {
        super(id, name, basePrice, stock);
        this.material = material != null ? material : "Standard";
    }

    public String getMaterial() {
        return material;
    }

    @Override
    public double getFinalPrice() {
        // 12% GST on Sports equipment
        return basePrice * 1.12;
    }

    @Override
    public String getCategory() {
        return "Sports";
    }

    @Override
    public String toString() {
        return super.toString() + " [Material: " + material + "]";
    }
}
