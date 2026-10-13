#pragma once
// Syllabus demos + sample data (shared by console and web versions)
#include "shop.h"
using namespace Shop;
// [U2: Passing pointers to functions]
inline void swapPrices(double* a, double* b) { double t = *a; *a = *b; *b = t; }
inline void applyHike(Product* p, double percent) { p->setPrice(p->getPrice() * (1 + percent / 100)); }
/* ------------------------------------------------------------------
 * SYLLABUS DEMO (menu option 8) - one function per unit
 * ------------------------------------------------------------------ */
inline void demoUnit1() {
    Util::title("UNIT 1 : Fundamentals of OOP");
    Electronics e(1, "Demo Phone", 10000);              // object created from class
    cout << "Object 'e' created. Message passing: e.finalPrice() -> Rs " << e.finalPrice() << "\n";
    cout << "Encapsulation: price is protected; only setPrice()/getPrice() are public.\n";
    Category c = CLOTHING;
    cout << "Enum Category value for CLOTHING = " << c << "\n";
    Address a{"Mumbai", 400001};
    cout << "Structure Address: " << a.city << " - " << a.pin << "\n";
    string s = "Online"; s += " Shopping";
    cout << "String: " << s << " (length " << s.length() << ")\n";
    int arr[3] = {10, 20, 30}; int sum = 0;
    for (int i = 0; i < 3; i++) sum += arr[i];
    cout << "Array sum = " << sum << "\n";
    Util::line();
}

inline void demoUnit2() {
    Util::title("UNIT 2 : Functions & Pointers");
    cout << "inline gst(1000) = " << gst(1000) << "   (default rate 18%)\n";
    {
        Product* p = new Grocery(50, "Rice 5kg", 400);  // [U2: Dynamic initialization via new]
        cout << "Product::getCount() (static fn) = " << Product::getCount() << "\n";
        p->rename("Basmati Rice 5kg").rename("Basmati Rice 5kg (Premium)");  // this pointer chaining
        cout << "After rename chain (this pointer): " << p->getName() << "\n";
        delete p;                                       // destructor runs
        cout << "After delete, count = " << Product::getCount() << "\n";
    }
    double x = 100, y = 200;
    swapPrices(&x, &y);
    cout << "swapPrices via pointers: x=" << x << " y=" << y << "\n";
    Product* lap = new Laptop(60, "Test Laptop", 50000);
    Product** pp = &lap;                                // [U2: Pointer to pointer]
    cout << "Pointer to pointer: (*pp)->getName() = " << (*pp)->getName() << "\n";
    applyHike(lap, 10);
    cout << "Price after 10% hike (pointer passed to fn): Rs " << lap->getPrice() << "\n";
    double (*fp)(double) = festiveDiscount;             // function pointer
    cout << "Function pointer festiveDiscount(1000) = " << fp(1000) << "\n";
    delete lap;
    Util::line();
}

inline void demoUnit3() {
    Util::title("UNIT 3 : Inheritance & Polymorphism");
    Product* list[3] = { new Laptop(1, "Laptop", 1000), new Clothing(2, "Shirt", 1000),
                         new Grocery(3, "Atta", 1000) };
    cout << "Runtime polymorphism (base ptr -> derived objects):\n";
    for (int i = 0; i < 3; i++)
        cout << "  " << list[i]->getName() << " finalPrice = Rs " << list[i]->finalPrice() << "\n";
    Electronics* e = new Laptop(4, "Gaming Laptop", 1000);
    Product* base = e;
    cout << "Static binding  (non-virtual tag): via Product* = " << base->tag()
         << ", via Electronics* = " << e->tag() << "\n";
    cout << "Dynamic binding (virtual finalPrice): " << base->finalPrice() << "\n";
    delete base;                                        // virtual destructor -> correct cleanup
    for (int i = 0; i < 3; i++) delete list[i];

    PrimeMember pm("Asha");
    cout << "Multiple inheritance + ambiguity resolution:\n";
    pm.show();
    Reseller r("Ravi");
    cout << "Hybrid inheritance (single shared Person via virtual base): " << r.Customer::getName()
         << " / " << r.Employee::getName() << "\n";
    Customer* cp = &pm;                                 // base pointer -> derived
    cout << "Customer* pointing to PrimeMember -> type() = " << cp->type() << "\n";

    cout << "Operator overloading on Cart:\n";
    Laptop l(5, "Lap", 1000); Grocery g(6, "Tea", 100);
    Cart c1, c2; c1.add(&l, 1); c2.add(&g, 2);
    Cart c3 = c1 + c2;                                  // binary +
    cout << c3;
    --c3;                                               // unary --
    cout << "After --c3 (removed last item):\n" << c3;
    cout << "Empty? " << (!c3 ? "yes" : "no") << "\n";
    Util::line();
}

inline void demoUnit4() {
    Util::title("UNIT 4 : Exception Handling & Templates");
    cout << "1) Divide by zero:\n";
    try { splitBill(900, 0); }
    catch (const runtime_error& e) { cout << "  Caught: " << e.what() << "\n"; }
    cout << "   splitBill(900,3) = " << splitBill(900, 3) << "\n";

    cout << "2) Multiple catch + user-defined exceptions:\n";
    for (int k = 0; k < 3; k++) {
        try {
            if (k == 0) throw OutOfStockException("Phone");
            if (k == 1) throw InvalidInputException("negative quantity");
            throw 42;
        }
        catch (OutOfStockException& e) { cout << "  [OutOfStock] " << e.what() << "\n"; }
        catch (ShopException& e)       { cout << "  [ShopException base] " << e.what() << "\n"; }
        catch (...)                    { cout << "  [catch-all] unexpected exception processed\n"; }
    }

    cout << "3) Constructor throws exception:\n";
    try { Electronics bad(9, "Bad", -5); }
    catch (ShopException& e) { cout << "  Constructor failed: " << e.what() << "\n"; }

    cout << "4) Rethrow + destructors during stack unwinding:\n";
    try { rethrowDemo(); }
    catch (exception& e) { cout << "  main-level catch: " << e.what() << "\n"; }

    cout << "5) Exception specification (noexcept):\n";
    legacySafe();

    cout << "6) Function templates (overloaded):\n";
    cout << "  maxOf(3,9)=" << maxOf(3, 9) << "  maxOf(2.5,1.5)=" << maxOf(2.5, 1.5)
         << "  maxOf(4,8,6)=" << maxOf(4, 8, 6) << "\n";

    cout << "7) Class template with non-type parameter History<double,3>:\n";
    History<double, 3> h;
    h.add(100); h.add(200); h.add(300); h.add(400);     // oldest dropped
    h.show();

    cout << "8) Class template Inventory<Product> + template friend stockReport():\n";
    Inventory<Product> inv;
    inv.add(new Grocery(1, "Sugar", 45, 100));
    inv.add(new Clothing(2, "Jeans", 1500, 10));
    stockReport(inv);
    Util::line();
}

inline void loadSampleData(Inventory<Product>& inv) {
    inv.add(new Laptop(101, "Dell Laptop", 55000, 5));
    inv.add(new Electronics(102, "Smartphone", 20000, 10, 1));
    inv.add(new Electronics(103, "Headphones", 2500, 25, 1));
    inv.add(new Clothing(201, "Denim Jacket", 2200, 15, "L"));
    inv.add(new Clothing(202, "Cotton T-Shirt", 600, 40, "M"));
    inv.add(new Grocery(301, "Basmati Rice 5kg", 450, 60));
    inv.add(new Grocery(302, "Olive Oil 1L", 700, 30));
}
