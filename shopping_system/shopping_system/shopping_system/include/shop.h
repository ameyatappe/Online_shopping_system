#pragma once
/*
 * ===================================================================
 *  PROJECT : ONLINE SHOPPING SYSTEM
 *  SUBJECT : Object Oriented Programming in C++
 *  Compile : g++ -std=c++17 -Wall online_shopping_system.cpp -o shop
 *  Run     : ./shop
 *
 *  Every syllabus topic is tagged in comments as  [U1: topic] etc.
 *  so you can point to it during your viva / demonstration.
 * ===================================================================
 */
#include <iostream>
#include <string>
#include <vector>
#include <iomanip>
#include <limits>
#include <exception>
#include <stdexcept>
using namespace std;

/* ------------------------------------------------------------------
 * UNIT 1 : Namespaces
 * ------------------------------------------------------------------ */
namespace Util {                                        // [U1: Namespace]
    void line(char c = '-', int n = 70) { cout << string(n, c) << "\n"; }
    void title(const string& t) { line('='); cout << "  " << t << "\n"; line('='); }
}

namespace Shop {

enum Category { ELECTRONICS = 1, CLOTHING, GROCERY };   // [U1: Enumeration]

struct Address {                                        // [U1: Structure]
    string city;
    int pin;
};

inline double gst(double amount, double rate = 0.18) {  // [U2: inline function, default arg]
    return amount * rate;
}

// [U2: Function pointers] - discount strategies
double noDiscount(double p)      { return p; }
double festiveDiscount(double p) { return p * 0.90; }   // 10% off

/* ------------------------------------------------------------------
 * UNIT 4 : User-defined exceptions + exceptions & inheritance
 * ------------------------------------------------------------------ */
class ShopException : public exception {
protected:
    string msg;
public:
    explicit ShopException(const string& m) : msg(m) {}
    const char* what() const noexcept override { return msg.c_str(); }
};
class OutOfStockException : public ShopException {
public:
    explicit OutOfStockException(const string& item)
        : ShopException("Out of stock / insufficient quantity: " + item) {}
};
class InvalidInputException : public ShopException {
public:
    explicit InvalidInputException(const string& m)
        : ShopException("Invalid input: " + m) {}
};

/* ------------------------------------------------------------------
 * UNIT 4 : TEMPLATES
 * ------------------------------------------------------------------ */
template <typename T> T maxOf(T a, T b)      { return (a > b) ? a : b; }      // function template
template <typename T> T maxOf(T a, T b, T c) { return maxOf(maxOf(a, b), c); } // overloaded template

// Class template with a NON-TYPE parameter N : remembers last N values
template <typename T, int N>
class History {
    T data[N];
    int n;
public:
    History() : n(0) {}
    void add(const T& v) {
        if (n == N) { for (int i = 1; i < N; i++) data[i - 1] = data[i]; n--; }
        data[n++] = v;
    }
    int size() const { return n; }
    T at(int i) const { return data[i]; }
    void show() const {
        if (n == 0) { cout << "  (no orders yet)\n"; return; }
        for (int i = 0; i < n; i++)
            cout << "  Order " << i + 1 << " : Rs " << fixed << setprecision(2) << data[i] << "\n";
    }
};

/* ------------------------------------------------------------------
 * PRODUCT HIERARCHY
 * Abstract class, encapsulation, static members, constructors,
 * destructor, virtual functions, hierarchical + multilevel inheritance
 * ------------------------------------------------------------------ */
class Product {                                         // [U3: Abstract class]
protected:                                              // [U3: Protected members]
    int id;
    string name;
    double price;
    int stock;
private:                                                // [U1: Access specifiers / info hiding]
    static int count;                                   // [U2: Static data member]
public:
    Product() : id(0), name("N/A"), price(0), stock(0) { ++count; }        // default ctor
    Product(int i, const string& n, double p, int s = 10)                  // [U2: ctor w/ default args]
        : id(i), name(n), price(p), stock(s) {
        if (p < 0) throw InvalidInputException("price cannot be negative"); // [U4: ctor + exception]
        ++count;
    }
    Product(const Product& o)                                              // copy ctor
        : id(o.id), name(o.name), price(o.price), stock(o.stock) { ++count; }
    virtual ~Product() { --count; }                     // [U3: Virtual destructor]

    virtual double finalPrice() const = 0;              // [U3: Pure virtual function]
    virtual void display() const {                      // [U3: Virtual function]
        cout << left << setw(4) << id << setw(18) << name
             << "Rs " << setw(9) << fixed << setprecision(2) << price
             << "Stock:" << setw(4) << stock;
    }
    virtual string category() const { return "Product"; }
    string tag() const { return "Product"; }            // NON-virtual -> static binding demo

    int getId() const { return id; }                    // [U1: Data abstraction - getters]
    const string& getName() const { return name; }
    double getPrice() const { return price; }
    int getStock() const { return stock; }
    void setPrice(double p) {
        if (p < 0) throw InvalidInputException("negative price");
        price = p;
    }
    Product& rename(const string& n) { this->name = n; return *this; }    // [U2: this pointer]
    void reduceStock(int q) {
        if (q <= 0) throw InvalidInputException("quantity must be > 0");
        if (q > stock) throw OutOfStockException(name);
        stock -= q;
    }
    static int getCount() { return count; }             // [U2: Static member function]
};
int Product::count = 0;

class Electronics : public Product {                    // [U3: Single / Hierarchical inheritance]
protected:
    int warranty;
public:
    Electronics(int i, const string& n, double p, int s = 10, int w = 1)
        : Product(i, n, p, s), warranty(w) {}
    double finalPrice() const override { return price + gst(price); }     // 18% GST
    void display() const override {
        Product::display();
        cout << " [Electronics " << warranty << "yr] Final: Rs " << finalPrice() << "\n";
    }
    string category() const override { return "Electronics"; }
    string tag() const { return "Electronics"; }        // hides base (static binding)
};

class Laptop : public Electronics {                     // [U3: Multilevel inheritance]
public:
    Laptop(int i, const string& n, double p, int s = 5) : Electronics(i, n, p, s, 2) {}
    double finalPrice() const override {                // [U3: Overriding member function]
        return Electronics::finalPrice() + price * 0.02;                  // + 2% insurance
    }
};

class Clothing : public Product {                       // [U3: Hierarchical inheritance]
    string size;
public:
    Clothing(int i, const string& n, double p, int s = 20, const string& sz = "M")
        : Product(i, n, p, s), size(sz) {}
    string category() const override { return "Clothing"; }
    double finalPrice() const override { return price + gst(price, 0.05); } // 5% GST
    void display() const override {
        Product::display();
        cout << " [Clothing " << size << "] Final: Rs " << finalPrice() << "\n";
    }
};

class Grocery : public Product {
public:
    Grocery(int i, const string& n, double p, int s = 50) : Product(i, n, p, s) {}
    string category() const override { return "Grocery"; }
    double finalPrice() const override { return price + gst(price, 0.05); }
    void display() const override {
        Product::display();
        cout << " [Grocery] Final: Rs " << finalPrice() << "\n";
    }
};

/* ------------------------------------------------------------------
 * INVENTORY : class template that owns product pointers
 * [U2: new/delete, arrays of pointers, friend, function overloading]
 * ------------------------------------------------------------------ */
template <typename T>
class Inventory {
    vector<T*> items;
public:
    Inventory() {}
    Inventory(const Inventory&) = delete;               // owns raw pointers: no copies
    Inventory& operator=(const Inventory&) = delete;
    ~Inventory() { for (T* p : items) delete p; }       // [U2: delete]

    void add(T* p) { items.push_back(p); }
    int size() const { return (int)items.size(); }
    T* at(int i) const { return items[i]; }

    T* find(int id) const {                             // [U3: Function overloading]
        for (T* p : items) if (p->getId() == id) return p;
        return nullptr;
    }
    T* find(const string& name) const {
        for (T* p : items) if (p->getName() == name) return p;
        return nullptr;
    }
    void showAll() const {
        for (T* p : items) p->display();                // [U3: runtime polymorphism]
    }
    template <typename U>                               // [U4: template friend generic function]
    friend void stockReport(const Inventory<U>& inv);
};

template <typename U>
void stockReport(const Inventory<U>& inv) {
    int total = 0;
    for (U* p : inv.items) total += p->getStock();      // accesses private member
    cout << "  Distinct products: " << inv.items.size()
         << " | Total units in stock: " << total << "\n";
}

/* ------------------------------------------------------------------
 * PEOPLE : virtual base class, multiple + hybrid inheritance
 * ------------------------------------------------------------------ */
class Person {
protected:
    string name;
public:
    explicit Person(const string& n = "Guest") : name(n) {}
    virtual ~Person() {}
    const string& getName() const { return name; }
};

class Loyalty {
protected:
    int points;
public:
    Loyalty() : points(0) {}
    int getPoints() const { return points; }
    void show() const { cout << "  Loyalty points: " << points << "\n"; }
};

class Cart;                                             // forward declaration

class Customer : virtual public Person {                // [U3: Virtual base class]
protected:
    Address addr;
    History<double, 5> orders;                          // [U4: Non-type template param]
    static int nextId;
    int custId;
public:
    Customer(const string& n = "Guest", const Address& a = Address{"Pune", 411001})
        : Person(n), addr(a), custId(nextId++) {}
    virtual ~Customer() {}
    virtual double placeOrder(Cart& cart, double (*disc)(double));   // [U1: Message passing]
    virtual string type() const { return "Regular Customer"; }
    void show() const {
        cout << "  " << name << " (ID " << custId << ", " << addr.city << "-" << addr.pin
             << ") - " << type() << "\n";
    }
    const History<double, 5>& getOrders() const { return orders; }
    int getId() const { return custId; }
    const Address& getAddress() const { return addr; }
    void history() const { cout << "Order history of " << name << ":\n"; orders.show(); }
};
int Customer::nextId = 1001;

class Employee : virtual public Person {
public:
    explicit Employee(const string& n = "Staff") : Person(n) {}
    void show() const { cout << "  Employee: " << name << "\n"; }
};

class PrimeMember : public Customer, public Loyalty {   // [U3: Multiple inheritance]
public:
    explicit PrimeMember(const string& n) : Person(n), Customer(n) {}
    string type() const override { return "Prime Member"; }
    double placeOrder(Cart& cart, double (*disc)(double)) override;
    void show() const {
        Customer::show();                               // [U3: Ambiguity resolved with ::]
        Loyalty::show();
    }
};

class Reseller : public Customer, public Employee {     // [U3: Hybrid inheritance (diamond via virtual Person)]
public:
    explicit Reseller(const string& n) : Person(n), Customer(n), Employee(n) {}
    string type() const override { return "Reseller"; }
};

/* ------------------------------------------------------------------
 * CART : operator overloading, friend function, arrays of pointers
 * ------------------------------------------------------------------ */
class Cart {
    static const int MAX = 10;
    Product* items[MAX];                                // [U2: Array of pointers]
    int qty[MAX];
    int n;
public:
    Cart() : n(0) {}
    void add(Product* p, int q = 1) {
        if (n >= MAX) throw ShopException("Cart is full");
        if (q <= 0) throw InvalidInputException("quantity must be > 0");
        if (q > p->getStock()) throw OutOfStockException(p->getName());
        items[n] = p; qty[n] = q; ++n;
    }
    int count() const { return n; }
    double total() const {
        double t = 0;
        for (int i = 0; i < n; i++) t += items[i]->finalPrice() * qty[i];   // dynamic binding
        return t;
    }
    void commit() {                                     // reduces stock after payment
        for (int i = 0; i < n; i++) items[i]->reduceStock(qty[i]);
        n = 0;
    }
    void clear() { n = 0; }
    Product* itemAt(int i) const { return items[i]; }
    int qtyAt(int i) const { return qty[i]; }
    void removeAt(int i) { if (i<0||i>=n) throw InvalidInputException("bad cart index"); for(int k=i+1;k<n;k++){items[k-1]=items[k];qty[k-1]=qty[k];} --n; }

    // [U3: Unary operator overloading] prefix -- removes the last item
    Cart& operator--() { if (n > 0) --n; return *this; }
    // [U3: Unary operator overloading] ! tests emptiness
    bool operator!() const { return n == 0; }
    // [U3: Binary operator overloading] merge two carts
    Cart operator+(const Cart& o) const {
        Cart r = *this;
        for (int i = 0; i < o.n; i++) r.add(o.items[i], o.qty[i]);
        return r;
    }
    friend ostream& operator<<(ostream& os, const Cart& c);   // [U2: friend function]
};

ostream& operator<<(ostream& os, const Cart& c) {
    if (c.n == 0) return os << "  (cart is empty)\n";
    for (int i = 0; i < c.n; i++)
        os << "  " << left << setw(18) << c.items[i]->getName() << " x" << setw(3) << c.qty[i]
           << " = Rs " << fixed << setprecision(2) << c.items[i]->finalPrice() * c.qty[i] << "\n";
    os << "  Cart total (incl. tax): Rs " << c.total() << "\n";
    return os;
}

/* ------------------------------------------------------------------
 * ORDER : friend class + friend function
 * ------------------------------------------------------------------ */
class Order {
    int orderNo;
    double amount;
    static int counter;
public:
    Order(double a) : orderNo(++counter), amount(a) {}
    static int lastOrderNo() { return counter; }
    friend class Auditor;                               // [U3: Friend class]
    friend void printReceipt(const Order& o, const string& cust);  // [U2: Friend function]
};
int Order::counter = 0;

void printReceipt(const Order& o, const string& cust) {
    Util::line('*', 40);
    cout << "  RECEIPT  #" << o.orderNo << "\n  Customer : " << cust
         << "\n  Amount   : Rs " << fixed << setprecision(2) << o.amount << "\n";
    Util::line('*', 40);
}
class Auditor {
public:
    void audit(const Order& o) const {
        cout << "  [Auditor] Order #" << o.orderNo << " verified, amount Rs " << o.amount << "\n";
    }
};

/* Customer::placeOrder - the "message" sent to a customer object */
double Customer::placeOrder(Cart& cart, double (*disc)(double)) {
    if (!cart) throw ShopException("Cannot place order: cart is empty");  // uses operator!
    double payable = disc(cart.total());                // [U2: function pointer call]
    cart.commit();
    orders.add(payable);
    Order o(payable);
    printReceipt(o, name);
    Auditor().audit(o);
    return payable;
}
double PrimeMember::placeOrder(Cart& cart, double (*disc)(double)) {
    double base = cart.total();
    double paid = Customer::placeOrder(cart, disc);
    points += (int)(base / 100) * 2;                    // 2 points per Rs 100
    cout << "  Prime bonus: earned " << (int)(base / 100) * 2 << " points!\n";
    return paid;
}

/* ------------------------------------------------------------------
 * UNIT 4 helpers : exception demos
 * ------------------------------------------------------------------ */
double splitBill(double total, int people) {            // divide-by-zero demo
    if (people == 0) throw runtime_error("Cannot split bill among 0 people (divide by zero)");
    return total / people;
}
void legacySafe() noexcept {                            // [U4: exception specification (modern form)]
    cout << "  legacySafe(): guaranteed not to throw (noexcept)\n";
}
// NOTE: dynamic specs like  void f() throw(int);  were removed in C++17.
//       std::unexpected() was removed too; today an escaping exception calls std::terminate.

class Guard {                                           // [U4: Destructor + stack unwinding]
    string n;
public:
    explicit Guard(const string& s) : n(s) { cout << "  Guard(" << n << ") acquired\n"; }
    ~Guard() { cout << "  ~Guard(" << n << ") released during unwinding\n"; }
};
void failingFunction() {
    Guard g1("db-connection");
    Guard g2("payment-gateway");
    throw ShopException("Payment gateway timeout");
}
void rethrowDemo() {
    try { failingFunction(); }
    catch (ShopException& e) { cout << "  rethrowDemo caught: " << e.what() << " -> rethrowing\n"; throw; }
}

}   // namespace Shop
