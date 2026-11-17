# ShopX — Online Shopping System (C++ & Java OOP)

A full-stack object-oriented shopping application with **a robust C++ OOP backend, premium web interface, terminal console app, and an academic Java OOP companion module**.

| Component | What it is | How to run |
|---|---|---|
| **Web UI** | C++ server (`server.cpp`) + interactive dashboard (`web/index.html`) | Run `build.bat` then `.\shop_server.exe` → open `http://localhost:8080` |
| **Console** | Interactive menu-driven C++ terminal application | Run `build.bat` then `.\shop_console.exe` |
| **Java Module** | Standalone Java companion demonstrating all 4 OOP pillars | `cd java_companion && compile_and_run.bat` |

---

## 📁 Project Structure
```
shopping_system/
├── include/
│   ├── shop.h              <- Core C++ OOP domain classes (Units 1-4)
│   ├── demos.h             <- Syllabus unit demos & sample product catalog
│   └── httplib.h           <- cpp-httplib lightweight embedded HTTP server
├── web/
│   ├── index.html          <- Responsive dashboard (Inter font, dark glassmorphism)
│   ├── images/             <- Accurate product photos (T-Shirt, Rice, Olive Oil, Bat, Powerbank, etc.)
│   └── config.js           <- Backend API endpoint configuration
├── java_companion/         <- Separate Java OOP companion module
│   ├── src/                <- Java sources demonstrating Encapsulation, Inheritance, Polymorphism, Abstraction
│   ├── bin/                <- Compiled Java .class bytecode
│   ├── ecj.jar             <- Bundled portable Java compiler (zero-dependency fallback)
│   ├── compile_and_run.bat <- Automatic compile & execution script
│   └── README.md           <- Detailed Java OOP walkthrough
├── console/
│   └── console_main.cpp    <- C++ terminal menu program
├── server.cpp              <- Embedded C++ REST server exposing objects via JSON API
├── build.bat               <- Automated C++ build script with static linking
├── Makefile                <- Linux/macOS build configuration
└── README.md
```

---

## 🌟 Key Features & Improvements

### 1. Accurate Product Names & Matching Images
- Every single product in the catalog now has an authentic, dedicated image that visually matches its name:
  - **Cotton T-Shirt**: Crisp photo of a pure white cotton t-shirt (`images/tshirt.jpg`).
  - **Basmati Rice 5kg**: High-quality photo of premium Basmati rice bag (`images/rice.jpg`).
  - **Olive Oil 1L**: Glass bottle of extra virgin olive oil (`images/oliveoil.jpg`).
  - **Cricket Bat**: Authentic English willow cricket bat & leather ball (`images/cricketbat.jpg`).
  - **Powerbank**: Modern compact portable fast-charger (`images/powerbank.jpg`).
  - **Dell Laptop**, **Smartphone**, **Headphones**, **Denim Jacket**.
- **Resilient Fallback Pipeline**:
  1. High-resolution local image asset (`images/*.jpg`).
  2. Direct Unsplash CDN link fallback on image error.
  3. Inline SVG vector badge placeholder if offline, guaranteeing zero broken image icons anywhere.
- Consistent `aspect-ratio: 16/10` and `object-fit: cover` styling.

### 2. Functional Prime Membership Experience
- **Interactive Login Selection**: Clear toggle widget with distinct selected/deselected states, golden glow, and keyboard accessibility.
- **Genuine Backend Benefits (No Fake UI Elements)**:
  - **5% Exclusive Member Discount**: Calculated dynamically via `PrimeMember::calculateDiscount()`.
  - **Free Priority Delivery**: `PrimeMember::getDeliveryFee()` returns ₹0.00 (regular customers pay ₹50 under ₹1000).
  - **Loyalty Points Accumulation**: Prime members earn 5 loyalty points per ₹100 spent upon checkout (`PrimeMember::placeOrder()`).
  - **Session Persistence**: Points balance persists throughout the active session, displayed in the header badge (`⭐ XX pts`) and user profile.
  - **Clear Customer Distinction**: Regular customers receive standard delivery and earn 0 loyalty points.
  - **Clean Logout & Re-login**: Sessions reset cleanly upon sign-out.

### 3. Dual OOP Demonstration (C++ & Java)
All four pillars of Object-Oriented Programming are demonstrated meaningfully in both languages:

| OOP Concept | C++ Implementation (`shop.h`) | Java Companion (`java_companion/src/`) |
|---|---|---|
| **Encapsulation** | Protected fields with getters/setters; `reduceStock()` and `setPrice()` enforce invariants; constructor exception handling. | Private fields with validated accessors; defensive copying via `Collections.unmodifiableList()`. |
| **Inheritance** | Single, Multilevel (`Product` → `Electronics` → `Laptop`), Hierarchical (`Electronics`, `Clothing`, `Grocery`, `Sports`), and Multiple Inheritance (`PrimeMember : public Customer, public Loyalty`). | Hierarchical (`Product` → `Electronics`, `Clothing`, `Grocery`, `SportsEquipment`), Multilevel (`Product` → `Electronics` → `Laptop`), Single + Interface (`Customer` → `PrimeMember implements LoyaltyRewardable`). |
| **Polymorphism** | Dynamic dispatch via `virtual finalPrice()` and `Customer::placeOrder()`; function pointer strategies (`festiveDiscount`); operator overloading (`Cart::operator--`, `operator+`, `operator!`). | Dynamic method binding for `getFinalPrice()`, `placeOrder()`, and `calculateTierDiscount()`; compile-time method overloading (`Cart.addItem()`); functional strategy pattern (`DiscountStrategy`). |
| **Abstraction** | Abstract base class `Product` with pure virtual function `finalPrice() = 0`; class template `History<T, N>` and `Inventory<T>`. | Abstract class `Product` with abstract method `getFinalPrice()`; interface abstraction via `LoyaltyRewardable` and `@FunctionalInterface DiscountStrategy`. |

---

## 🛠️ How to Build and Run

### 1. C++ Web Server & Browser Dashboard
```cmd
cd shopping_system
.\build.bat
.\shop_server.exe
```
Open **http://localhost:8080** in any browser.

### 2. C++ Console Terminal Application
```cmd
cd shopping_system
.\build.bat
.\shop_console.exe
```

### 3. Java OOP Companion Demonstration
```cmd
cd shopping_system\java_companion
compile_and_run.bat
```
*(The batch file automatically uses `javac` if found in PATH, or the bundled zero-dependency portable compiler `ecj.jar` on any standard JRE.)*

### 4. Running the Comprehensive E2E Verification Suite
```cmd
cd shopping_system
python test_e2e.py
```
This automated suite verifies:
1. Product catalog, images, and dimensions for all 9 items.
2. Category filtering and Dell Laptop inclusion under Electronics.
3. Cart API math (`festive=1` vs `festive=0`, delivery fees, 5% Prime discount) matching checkout amount.
4. Prime loyalty points accumulation upon successful order.
5. Logout and same-customer login points restoration.
6. Server process shutdown and disk recovery from `data/customers.json`.
7. Regular customer isolation from Prime perks.
