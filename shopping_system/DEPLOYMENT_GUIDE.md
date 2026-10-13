# Quick Live Deployment Guide — Online Shopping System (C++ & Web)

This guide takes your **Online Shopping System — OOP in C++** live on the internet with a public HTTPS URL that you can share with college faculty and judges. Once deployed, it runs **24/7 in the cloud** — your laptop can be completely shut down!

---

## Architecture Overview

```
               [ Faculty / Judge Browser ]
                           │
             ┌─────────────┴─────────────┐
             ▼                           ▼
   [ Frontend: Vercel ]       [ Backend: Render ]
   • web/index.html           • server.cpp (C++17)
   • Product images           • include/shop.h (OOP core)
   • vercel.json rewrite      • Docker / Debian Container
             │                           │
             └──────► /api/... ──────────┘
```

- **Frontend (Vercel)**: Fast global CDN serving HTML, CSS, JavaScript, and product images.
- **Backend (Render)**: Persistent cloud web service compiling and running your native C++ server.
- **Dual Access**: You can access the project via your **Vercel URL** (e.g. `https://shopx.vercel.app`) OR directly via your **Render URL** (e.g. `https://shopx-backend.onrender.com`) since the C++ server also serves the UI!

---

## Step 1: Push Code to GitHub (2 minutes)

1. Open **[GitHub.com](https://github.com)** and create a new repository (e.g., `online-shopping-cpp`).
2. Keep it **Public** (required for free Render tier) and do not check "Add README".
3. In PowerShell or Command Prompt inside `d:\Downloads\shopping_system`, run:

```powershell
# Set branch to main
git branch -M main

# Link your GitHub repository (replace with your repo URL)
git remote add origin https://github.com/YOUR_USERNAME/online-shopping-cpp.git

# Push the code
git push -u origin main
```

*(All deployment configurations, Dockerfile, render.yaml, and vercel.json are already committed in git for you!)*

---

## Step 2: Deploy C++ Backend on Render (3 minutes)

1. Go to **[Render.com](https://render.com)** and sign in with GitHub.
2. Click **New +** → **Web Service**.
3. Choose **Build and deploy from a Git repository** and select `online-shopping-cpp`.
4. Configure the service:
   - **Name**: `shopx-backend` (or your choice)
   - **Language / Environment**: `Docker` *(Recommended)*
     - *Dockerfile Path*: `./Dockerfile` (Render detects it automatically)
   - *(Alternative Native Linux setup)*:
     - Environment: `Native / C++`
     - Build Command: `g++ -std=c++17 -Wall -O2 -pthread server.cpp -o shop_server`
     - Start Command: `./shop_server`
   - **Instance Type**: `Free`
5. Click **Create Web Service**.
6. Render will build the C++ server using GCC 13 and deploy it.
7. Once the status shows **Live**, copy your public URL:
   - Example: `https://shopx-backend.onrender.com`
8. Verify it by visiting:
   - `https://shopx-backend.onrender.com/health` → returns `{"status":"ok"}`
   - `https://shopx-backend.onrender.com/api/products` → returns the sample C++ products JSON!

> **Note on Render Free Tier**: When inactive for 15 minutes, Render spins down the service. When a visitor opens the website, it automatically wakes up in ~30–50 seconds. A status indicator and ping tool have been added to the header so you can demonstrate this smoothly to judges.

---

## Step 3: Deploy Frontend on Vercel (2 minutes)

1. Go to **[Vercel.com](https://vercel.com)** and sign in with GitHub.
2. Click **Add New…** → **Project**.
3. Select your `online-shopping-cpp` repository.
4. Settings:
   - **Framework Preset**: `Other`
   - **Root Directory**: Leave as `./` (or select `web`)
5. Click **Deploy**.
6. Within 20 seconds, your site is live at:
   - Example: `https://online-shopping-cpp.vercel.app`

---

## Step 4: Link Frontend to Backend (30 seconds)

You have **3 simple ways** to link the frontend to your Render backend:

### Option A: Via the Web Interface (Instant — No Git Push)
1. Open your live Vercel website in any browser.
2. In the top navigation header, click the **🌐 Backend** button.
3. Paste your Render URL (e.g. `https://shopx-backend.onrender.com`).
4. Click **Test Ping** (verifies latency and product count).
5. Click **Save & Connect**.
   - Your preference is saved in your browser's `localStorage` and will immediately load the live C++ data!

### Option B: Via `config.js`
In `web/config.js`, update:
```javascript
window.SHOP_CONFIG = {
  BACKEND_URL: "https://shopx-backend.onrender.com"
};
```
Commit and push: `git commit -am "Set live backend URL" ; git push`. Vercel will redeploy automatically in seconds.

### Option C: Via `vercel.json` (Vercel Edge Proxy)
In `vercel.json`, replace `YOUR_BACKEND_URL` with your Render subdomain:
```json
{
  "source": "/api/(.*)",
  "destination": "https://shopx-backend.onrender.com/api/$1"
}
```
Commit and push: `git commit -am "Set Vercel reverse proxy destination" ; git push`.

---

## Features to Demonstrate to Faculty / Judges

| Feature | Where in UI | Syllabus OOP Concept |
|---|---|---|
| **Browse Products & Categories** | Shop tab | `Inventory<Product>` template, inheritance (`Electronics`, `Clothing`, `Grocery`) |
| **Dynamic Tax Calculation** | Product cards | Virtual function `finalPrice()` overridden by each derived class |
| **Login as Normal vs Prime** | Sign In Modal (toggle ⭐) | Polymorphism: `Customer*` pointing to `Customer` or `PrimeMember` |
| **Loyalty Points & Bonus** | Header profile badge | Downcasting via `dynamic_cast<PrimeMember*>` & RTTI |
| **Add to Cart & Out of Stock** | Cart button | `OutOfStockException` user-defined exception class |
| **Undo Last Cart Item** | Cart sidebar `↩ Remove last` | Overloaded unary operator `--cart` |
| **Festive Discount Checkout** | Checkout button | Function pointers (`festiveDiscount` vs `noDiscount`) |
| **Order History** | My Orders tab | Class template with non-type parameter `History<double, 5>` |
| **Admin Add Product** | Admin tab | Dynamic memory allocation (`new`), constructor validation exceptions |
| **Run Syllabus OOP Demo** | OOP Demo tab | Redirects C++ `std::cout` stream to capture Units 1–4 outputs |
| **Backend Latency & Status** | Header `Backend` pill | Live HTTP/JSON API communication over HTTPS |

---

## Data Persistence Note for Evaluation
- Data (inventory stock, active session, carts, orders) is maintained **in memory** inside the C++ server process (`Inventory<Product>`, `unique_ptr<Customer>`, `Cart`).
- This satisfies typical OOP laboratory syllabus requirements without requiring an external SQL/NoSQL database.
- If the Render free container restarts or cold-boots after 15 minutes of inactivity, data resets to default sample items (`loadSampleData()`).
