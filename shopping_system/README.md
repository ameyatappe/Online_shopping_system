# Online Shopping System — OOP in C++

A C++ project with **two front-ends sharing the same OOP core**:

| Version | What it is | Run it |
|---|---|---|
| **Web UI** | C++ server (`server.cpp`) + browser UI (`web/index.html`) | `make run` then open http://localhost:8080 |
| **Console** | Menu-driven terminal program | `make shop_console && ./shop_console` |

## Folder structure
```
shopping_system/
├── include/
│   ├── shop.h        <- ALL OOP classes (Units 1-4), tagged [U1:..] [U2:..] etc.
│   ├── demos.h       <- syllabus demo functions + sample data
│   └── httplib.h     <- cpp-httplib (header-only HTTP library, MIT licence)
├── server.cpp        <- web back-end: exposes the C++ objects as a small JSON API
├── web/index.html    <- the UI (HTML + CSS + JavaScript, no frameworks)
├── console/console_main.cpp
├── Makefile
└── README.md
```

## Build & run

**Linux / macOS** (needs g++ 7+ or clang):
```
make
./shop_server          # then open http://localhost:8080
```
**Windows (MinGW-w64 / MSYS2):**
```
g++ -std=c++17 server.cpp -o shop_server.exe -lws2_32 -lwsock32
shop_server.exe
```
Run from the project root so the server can find the `web/` folder. A different port: `./shop_server 9000`.

## How the UI is connected to the C++ code
The browser calls `/api/...`; `server.cpp` turns each call into a method call on the C++ objects.

| UI action | API | C++ code that runs |
|---|---|---|
| Log in (Prime / normal) | POST /api/login | `new PrimeMember(name)` or `new Customer(name)` stored in a `Customer*` |
| Browse / search | GET /api/products | `Inventory<Product>` + virtual `finalPrice()`, `category()` |
| Add to cart | POST /api/cart/add | `Cart::add()` → throws `OutOfStockException` |
| Remove last | POST /api/cart/undo | overloaded `Cart::operator--` |
| Checkout | POST /api/checkout | `Customer::placeOrder(cart, fnPointer)` (virtual; `PrimeMember` overrides) |
| Order history | GET /api/orders | `History<double,5>` template (non-type parameter) |
| Admin add product | POST /api/admin/product | `new Electronics/Clothing/Grocery(...)`, constructor exceptions |
| OOP Demo tab | GET /api/demo | `demoUnit1()..demoUnit4()` output captured from `cout` |

## Syllabus coverage
Search `shop.h` / `demos.h` for tags like `[U3: Virtual base class]` — every topic from Units 1–4 is marked.
(`throw()` specifications, `unexpected()` and template `export` were removed from modern C++; `noexcept` and `catch(...)` are used instead.)
