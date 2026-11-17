# ShopX — Java OOP Companion Module

This module is an academic companion demonstrating the four foundational Object-Oriented Programming (OOP) concepts in **Java**, designed to complement the primary **C++** backend of ShopX.

---

## 🏛️ OOP Concepts Demonstrated

### 1. Encapsulation
* **Information Hiding**: Attributes (`id`, `name`, `basePrice`, `stock`, `customerId`, `orderHistory`, `loyaltyPoints`) are marked `private` or `protected`.
* **State Validation**: Mutators and constructors enforce domain rules (e.g., negative prices, negative stock, and empty product/customer names throw `IllegalArgumentException`).
* **Protected Operations**: Stock cannot be reduced arbitrarily; `reduceStock(int quantity)` guards against over-allocation (`IllegalStateException`).
* **Defensive Copying**: `Cart.getItems()` and `Customer.getOrderHistory()` return unmodifiable lists (`Collections.unmodifiableList`), preventing external code from corrupting internal collections.

### 2. Inheritance
* **Hierarchical Inheritance**:
  * `Product` (Abstract Base Class)
    * ├── `Electronics`
    * ├── `Clothing`
    * ├── `Grocery`
    * └── `SportsEquipment`
* **Multilevel Inheritance**:
  * `Product` → `Electronics` → `Laptop`
* **Single Inheritance with Interface Implementation**:
  * `Customer` → `PrimeMember implements LoyaltyRewardable`
  * *Academic Note on C++ vs. Java*: C++ demonstrates Multiple Class Inheritance (`class PrimeMember : public Customer, public Loyalty`), whereas Java models this via Interface Realization (`PrimeMember extends Customer implements LoyaltyRewardable`), avoiding the diamond problem.

### 3. Polymorphism
* **Dynamic Method Binding (Runtime Polymorphism)**:
  * When iterating over a polymorphic collection `Customer[] shoppers = { regular, prime }`, calls to `shopper.getCustomerType()`, `shopper.calculateTierDiscount()`, and `shopper.placeOrder()` are dynamically dispatched to the concrete instance.
  * When iterating over `Product` items in a `Cart`, `item.getProduct().getFinalPrice()` dynamically invokes category-specific tax and duty formulas.
* **Compile-Time Polymorphism (Method Overloading)**:
  * `Cart.addItem(Product product)`
  * `Cart.addItem(Product product, int quantity)`
* **Functional Polymorphism (Strategy Pattern)**:
  * `DiscountStrategy` is a `@FunctionalInterface` allowing festive or promotional discounts to be passed as lambdas (`gross -> gross * 0.90`).

### 4. Abstraction
* **Abstract Base Class**: `Product` declares pure abstract contracts:
  * `public abstract double getFinalPrice();`
  * `public abstract String getCategory();`
  * Consumers interact with the general concept of a `Product` without needing to know specific GST, warranty, or insurance rules.
* **Interface Abstraction**: `LoyaltyRewardable` defines the capabilities of reward programs without coupling to any specific customer class implementation.

---

## 🚀 How to Compile and Run

### Option 1: Automatic Runner (Recommended)
Simply double-click or run from terminal:
```cmd
cd shopping_system\java_companion
compile_and_run.bat
```
*(This script automatically checks for `javac` and falls back to the bundled portable `ecj.jar` compiler if `javac` is not in your system PATH.)*

### Option 2: Using standard `javac` & `java`
```cmd
cd shopping_system\java_companion
mkdir bin
javac -d bin src\*.java
java -cp bin com.shopx.ShopXDemo
```

### Option 3: Using bundled `ecj.jar`
```cmd
cd shopping_system\java_companion
mkdir bin
java -jar ecj.jar -8 -d bin src\*.java
java -cp bin com.shopx.ShopXDemo
```
