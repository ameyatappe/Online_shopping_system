/* CONSOLE VERSION - Online Shopping System (uses include/shop.h + demos.h) */
#include "../include/demos.h"

int readInt(const string& prompt) {
    int v;
    cout << prompt;
    while (!(cin >> v)) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "  Please enter a number: ";
    }
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    return v;
}

void adminAddProduct(Inventory<Product>& inv) {         // [U2: Dynamic initialization of object]
    cout << "Category (1=Electronics 2=Clothing 3=Grocery): ";
    int cat = readInt("");
    int id = readInt("Product ID: ");
    cout << "Name: ";
    string name; getline(cin, name);
    cout << "Price: ";
    double price; cin >> price; cin.ignore(numeric_limits<streamsize>::max(), '\n');
    int stock = readInt("Stock: ");
    if (inv.find(id)) throw InvalidInputException("Product ID already exists");
    Product* p = nullptr;
    switch (cat) {
        case ELECTRONICS: p = new Electronics(id, name, price, stock); break;
        case CLOTHING:    p = new Clothing(id, name, price, stock);    break;
        case GROCERY:     p = new Grocery(id, name, price, stock);     break;
        default: throw InvalidInputException("unknown category");
    }
    inv.add(p);
    cout << "  Product added. Total products now: " << Product::getCount() << "\n";
}

int main() {
    Inventory<Product> inventory;
    loadSampleData(inventory);

    cout << "Enter your name: ";
    string uname; getline(cin, uname);
    if (uname.empty()) uname = "Guest";
    cout << "Prime membership? (1=yes, 0=no): ";
    int prime = readInt("");

    Customer* user = prime ? new PrimeMember(uname) : new Customer(uname);   // [U3: base ptr to derived]
    Cart cart;

    int choice;
    do {
        Util::title("ONLINE SHOPPING SYSTEM  -  Welcome, " + uname);
        cout << " 1. View products        5. Checkout (festive 10% off)\n"
             << " 2. Search product       6. My profile & order history\n"
             << " 3. Add to cart          7. Admin: add product\n"
             << " 4. View / edit cart     8. Syllabus demo (Units 1-4)\n"
             << " 0. Exit\n";
        choice = readInt("Choice: ");
        try {
            switch (choice) {
            case 1:
                Util::line();
                inventory.showAll();
                stockReport(inventory);
                break;
            case 2: {
                cout << "Search by 1) ID or 2) Name: ";
                int m = readInt("");
                Product* p = nullptr;
                if (m == 1) p = inventory.find(readInt("ID: "));          // overloaded find(int)
                else { cout << "Name: "; string n; getline(cin, n); p = inventory.find(n); }  // find(string)
                if (p) p->display(); else cout << "  Not found.\n";
                break;
            }
            case 3: {
                int id = readInt("Product ID: ");
                Product* p = inventory.find(id);
                if (!p) throw InvalidInputException("no such product");
                int q = readInt("Quantity: ");
                cart.add(p, q);
                cout << "  Added to cart.\n";
                break;
            }
            case 4:
                cout << cart;
                if (cart.count() > 0) {
                    cout << "Remove last item? (1=yes 0=no): ";
                    if (readInt("") == 1) { --cart; cout << "  Removed.\n"; }
                }
                break;
            case 5:
                user->placeOrder(cart, festiveDiscount);                  // message passing + fn pointer
                break;
            case 6:
                user->show();
                static_cast<const Customer*>(user)->history();
                break;
            case 7:
                adminAddProduct(inventory);
                break;
            case 8:
                demoUnit1(); demoUnit2(); demoUnit3(); demoUnit4();
                break;
            case 0:
                cout << "Thank you for shopping!\n";
                break;
            default:
                cout << "  Invalid choice.\n";
            }
        }
        catch (OutOfStockException& e) { cout << "  [Stock] " << e.what() << "\n"; }
        catch (ShopException& e)       { cout << "  [Error] " << e.what() << "\n"; }
        catch (exception& e)           { cout << "  [System] " << e.what() << "\n"; }
    } while (choice != 0);

    delete user;     // virtual destructor in Person/Customer ensures proper cleanup
    return 0;
}
