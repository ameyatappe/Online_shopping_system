/*
 * ONLINE SHOPPING SYSTEM - WEB VERSION
 * C++ backend (all OOP logic lives in include/shop.h) + HTML/JS UI in web/
 * The UI talks to these C++ objects through a tiny embedded HTTP server.
 *
 * Build : make          Run : ./shop_server      Open : http://localhost:8080
 */
#include "include/httplib.h"
#include "include/demos.h"
#include <sstream>
#include <mutex>
#include <memory>
#include <filesystem>
#include <cstdlib>

using namespace std;
using namespace Shop;

/* ---------------- global application state (one shop, one shopper) ---------------- */
static Inventory<Product> inventory;
static unique_ptr<Customer> user;                // base pointer -> Customer / PrimeMember (polymorphism)
static Cart cart;
static mutex mtx;

/* ---------------- tiny JSON helpers ---------------- */
static string esc(const string& s) {
    string o;
    for (char c : s) {
        switch (c) {
            case '"': o += "\\\""; break;
            case '\\': o += "\\\\"; break;
            case '\n': o += "\\n"; break;
            case '\r': break;
            case '\t': o += "\\t"; break;
            default: o += c;
        }
    }
    return o;
}
static string q(const string& s) { return "\"" + esc(s) + "\""; }
static string num(double d) { ostringstream os; os << fixed << setprecision(2) << d; return os.str(); }

static string productJson(const Product* p) {
    return "{\"id\":" + to_string(p->getId()) + ",\"name\":" + q(p->getName()) +
           ",\"category\":" + q(p->category()) + ",\"price\":" + num(p->getPrice()) +
           ",\"finalPrice\":" + num(p->finalPrice()) + ",\"stock\":" + to_string(p->getStock()) + "}";
}
static string errJson(const string& m) { return "{\"error\":" + q(m) + "}"; }

static int toInt(const string& s, const string& field) {
    try { size_t pos; int v = stoi(s, &pos); if (pos != s.size()) throw 1; return v; }
    catch (...) { throw InvalidInputException(field + " must be a whole number"); }
}
static double toDouble(const string& s, const string& field) {
    try { size_t pos; double v = stod(s, &pos); if (pos != s.size()) throw 1; return v; }
    catch (...) { throw InvalidInputException(field + " must be a number"); }
}
static void needLogin() { if (!user) throw ShopException("Please log in first"); }

/* run a handler under the lock; map C++ exceptions -> JSON errors (Unit 4 in action) */
template <typename F>
static void handle(httplib::Response& res, F fn) {
    lock_guard<mutex> lock(mtx);
    try {
        res.set_content(fn(), "application/json");
    } catch (OutOfStockException& e) {
        res.status = 409; res.set_content(errJson(e.what()), "application/json");
    } catch (ShopException& e) {
        res.status = 400; res.set_content(errJson(e.what()), "application/json");
    } catch (exception& e) {
        res.status = 500; res.set_content(errJson(e.what()), "application/json");
    }
}

static string sessionJson() {
    if (!user) return "{\"loggedIn\":false}";
    int pts = 0;
    if (auto* pm = dynamic_cast<PrimeMember*>(user.get())) pts = pm->getPoints();   // RTTI downcast
    return "{\"loggedIn\":true,\"name\":" + q(user->getName()) + ",\"type\":" + q(user->type()) +
           ",\"id\":" + to_string(user->getId()) + ",\"points\":" + to_string(pts) +
           ",\"city\":" + q(user->getAddress().city) + "}";
}

static string cartJson() {
    string items = "[";
    for (int i = 0; i < cart.count(); i++) {
        Product* p = cart.itemAt(i);
        if (i) items += ",";
        items += "{\"index\":" + to_string(i) + ",\"id\":" + to_string(p->getId()) +
                 ",\"name\":" + q(p->getName()) + ",\"qty\":" + to_string(cart.qtyAt(i)) +
                 ",\"unit\":" + num(p->finalPrice()) +
                 ",\"line\":" + num(p->finalPrice() * cart.qtyAt(i)) + "}";
    }
    items += "]";
    double total = cart.total();
    return "{\"items\":" + items + ",\"total\":" + num(total) +
           ",\"festiveTotal\":" + num(festiveDiscount(total)) + "}";
}

int main(int argc, char** argv) {
    int port = 8080;
    if (const char* env_p = getenv("PORT")) {
        port = atoi(env_p);
    }
    if (argc > 1) {
        port = atoi(argv[1]);
    }
    loadSampleData(inventory);

    httplib::Server svr;

    /* ---------- CORS & preflight options handler ---------- */
    svr.set_pre_routing_handler([](const httplib::Request& req, httplib::Response& res) {
        res.set_header("Access-Control-Allow-Origin", "*");
        res.set_header("Access-Control-Allow-Methods", "GET, POST, OPTIONS");
        res.set_header("Access-Control-Allow-Headers", "Content-Type, Authorization, X-Requested-With, Accept");
        if (req.method == "OPTIONS") {
            res.status = 204;
            return httplib::Server::HandlerResponse::Handled;
        }
        return httplib::Server::HandlerResponse::Unhandled;
    });

    /* ---------- health check endpoints ---------- */
    svr.Get("/health", [](const httplib::Request&, httplib::Response& res) {
        res.set_header("Access-Control-Allow-Origin", "*");
        res.set_content("{\"status\":\"ok\"}", "application/json");
    });
    svr.Get("/api/health", [](const httplib::Request&, httplib::Response& res) {
        res.set_header("Access-Control-Allow-Origin", "*");
        res.set_content("{\"status\":\"ok\"}", "application/json");
    });

    /* static UI (if present) */
    string webDir = filesystem::exists("web") ? "web" : (filesystem::exists("../web") ? "../web" : "");
    if (!webDir.empty()) {
        svr.set_mount_point("/", webDir);
    } else {
        svr.Get("/", [](const httplib::Request&, httplib::Response& res) {
            res.set_header("Access-Control-Allow-Origin", "*");
            res.set_content("{\"status\":\"ok\",\"service\":\"Online Shopping System C++ Backend\"}", "application/json");
        });
    }

    /* ---------- products ---------- */
    svr.Get("/api/products", [](const httplib::Request& req, httplib::Response& res) {
        handle(res, [&] {
            string term = req.get_param_value("q");
            string out = "[";
            bool first = true;
            for (int i = 0; i < inventory.size(); i++) {
                Product* p = inventory.at(i);
                if (!term.empty()) {                                  // simple case-insensitive search
                    string a = p->getName(), b = term;
                    for (auto& c : a) c = tolower(c);
                    for (auto& c : b) c = tolower(c);
                    if (a.find(b) == string::npos) continue;
                }
                if (!first) out += ",";
                first = false;
                out += productJson(p);
            }
            return out + "]";
        });
    });

    svr.Get("/api/stats", [](const httplib::Request&, httplib::Response& res) {
        handle(res, [&] {
            int units = 0;
            for (int i = 0; i < inventory.size(); i++) units += inventory.at(i)->getStock();
            return "{\"products\":" + to_string(Product::getCount()) + ",\"units\":" + to_string(units) + "}";
        });
    });

    /* ---------- session ---------- */
    svr.Post("/api/login", [](const httplib::Request& req, httplib::Response& res) {
        handle(res, [&] {
            string name = req.get_param_value("name");
            if (name.empty()) throw InvalidInputException("name is required");
            bool prime = req.get_param_value("prime") == "1";
            cart.clear();
            if (prime) user.reset(new PrimeMember(name));             // Customer* -> PrimeMember
            else       user.reset(new Customer(name));
            return sessionJson();
        });
    });
    svr.Get("/api/session", [](const httplib::Request&, httplib::Response& res) {
        handle(res, [&] { return sessionJson(); });
    });
    svr.Post("/api/logout", [](const httplib::Request&, httplib::Response& res) {
        handle(res, [&] { cart.clear(); user.reset(); return string("{\"ok\":true}"); });
    });

    /* ---------- cart ---------- */
    svr.Get("/api/cart", [](const httplib::Request&, httplib::Response& res) {
        handle(res, [&] { return cartJson(); });
    });
    svr.Post("/api/cart/add", [](const httplib::Request& req, httplib::Response& res) {
        handle(res, [&] {
            needLogin();
            int id = toInt(req.get_param_value("id"), "id");
            int qty = toInt(req.get_param_value("qty"), "quantity");
            Product* p = inventory.find(id);                          // overloaded find(int)
            if (!p) throw InvalidInputException("no such product");
            cart.add(p, qty);
            return cartJson();
        });
    });
    svr.Post("/api/cart/remove", [](const httplib::Request& req, httplib::Response& res) {
        handle(res, [&] {
            cart.removeAt(toInt(req.get_param_value("index"), "index"));
            return cartJson();
        });
    });
    svr.Post("/api/cart/undo", [](const httplib::Request&, httplib::Response& res) {
        handle(res, [&] { --cart; return cartJson(); });              // overloaded unary operator--
    });

    /* ---------- checkout / orders ---------- */
    svr.Post("/api/checkout", [](const httplib::Request& req, httplib::Response& res) {
        handle(res, [&] {
            needLogin();
            bool festive = req.get_param_value("festive") == "1";
            double (*disc)(double) = festive ? festiveDiscount : noDiscount;   // function pointer
            double paid = user->placeOrder(cart, disc);               // message passing, dynamic binding
            return "{\"orderNo\":" + to_string(Order::lastOrderNo()) + ",\"paid\":" + num(paid) +
                   ",\"session\":" + sessionJson() + "}";
        });
    });
    svr.Get("/api/orders", [](const httplib::Request&, httplib::Response& res) {
        handle(res, [&] {
            needLogin();
            const auto& h = user->getOrders();
            string out = "[";
            for (int i = 0; i < h.size(); i++) { if (i) out += ","; out += num(h.at(i)); }
            return out + "]";
        });
    });

    /* ---------- admin ---------- */
    svr.Post("/api/admin/product", [](const httplib::Request& req, httplib::Response& res) {
        handle(res, [&] {
            int cat = toInt(req.get_param_value("category"), "category");
            int id = toInt(req.get_param_value("id"), "id");
            string name = req.get_param_value("name");
            double price = toDouble(req.get_param_value("price"), "price");
            int stock = toInt(req.get_param_value("stock"), "stock");
            if (name.empty()) throw InvalidInputException("name is required");
            if (inventory.find(id)) throw InvalidInputException("product ID already exists");
            if (stock < 0) throw InvalidInputException("stock cannot be negative");
            Product* p = nullptr;                                     // dynamic initialisation
            switch (cat) {
                case ELECTRONICS: p = new Electronics(id, name, price, stock); break;
                case CLOTHING:    p = new Clothing(id, name, price, stock);    break;
                case GROCERY:     p = new Grocery(id, name, price, stock);     break;
                default: throw InvalidInputException("unknown category");
            }
            inventory.add(p);
            return productJson(p);
        });
    });

    /* ---------- syllabus demo: captures what the C++ demos print ---------- */
    svr.Get("/api/demo", [](const httplib::Request&, httplib::Response& res) {
        handle(res, [&] {
            ostringstream buf;
            streambuf* old = cout.rdbuf(buf.rdbuf());                 // redirect cout
            demoUnit1(); demoUnit2(); demoUnit3(); demoUnit4();
            cout.rdbuf(old);
            return "{\"output\":" + q(buf.str()) + "}";
        });
    });

    cout << "Online Shopping System running on port " << port << " (0.0.0.0:" << port << ")\n(Ctrl+C to stop)\n";
    if (!svr.listen("0.0.0.0", port)) { cerr << "Could not bind port " << port << "\n"; return 1; }
    return 0;
}
