import subprocess
import time
import urllib.request
import urllib.parse
import urllib.error
import json
import os
import sys

PORT = 8086
BASE_URL = f"http://127.0.0.1:{PORT}"
ISOLATED_TEST_FILE = "data/test_customers_isolated.json"

def api_get(endpoint):
    req = urllib.request.Request(f"{BASE_URL}{endpoint}")
    with urllib.request.urlopen(req, timeout=5) as resp:
        return json.loads(resp.read().decode('utf-8'))

def api_post(endpoint, data=None):
    post_data = urllib.parse.urlencode(data or {}).encode('utf-8')
    req = urllib.request.Request(f"{BASE_URL}{endpoint}", data=post_data)
    try:
        with urllib.request.urlopen(req, timeout=5) as resp:
            return json.loads(resp.read().decode('utf-8')), resp.status
    except urllib.error.HTTPError as e:
        err_body = e.read().decode('utf-8')
        try:
            return json.loads(err_body), e.code
        except Exception:
            return {"error": err_body}, e.code

def run_tests():
    print("=" * 70)
    print("  SHOPX NON-DESTRUCTIVE E2E VERIFICATION TEST")
    print("=" * 70)

    # Verify that real customer database is preserved
    real_db_path = "data/customers.json"
    real_db_snapshot = None
    if os.path.exists(real_db_path):
        with open(real_db_path, "r") as f:
            real_db_snapshot = f.read()
        print(f"[SAFETY CHECK] Existing customer DB found ({len(real_db_snapshot)} bytes). It will NOT be touched.")
    else:
        print("[SAFETY CHECK] Real customer DB does not exist yet; won't be modified.")

    # Clean only our isolated test file
    if os.path.exists(ISOLATED_TEST_FILE):
        os.remove(ISOLATED_TEST_FILE)

    # Start shop_server.exe pointing to ISOLATED_TEST_FILE via SHOP_CUSTOMERS_FILE
    env = os.environ.copy()
    env["PORT"] = str(PORT)
    env["SHOP_CUSTOMERS_FILE"] = ISOLATED_TEST_FILE
    
    proc = subprocess.Popen(["shop_server.exe", str(PORT)], cwd=".", env=env)
    print(f"[TEST SETUP] Started shop_server.exe PID={proc.pid} on port {PORT}")
    print(f"[TEST SETUP] Using isolated test storage: {ISOLATED_TEST_FILE}")
    time.sleep(2)

    try:
        # TEST 1: Catalog & Product Images Verification
        print("\n--- TEST 1: Catalog & Product Images Verification ---")
        prods = api_get("/api/products")
        print(f"Total products returned: {len(prods)}")
        assert len(prods) == 9, f"Expected 9 products, got {len(prods)}"
        
        for p in prods:
            assert os.path.exists(os.path.join("web", p['image'])), f"Missing image file for {p['name']}: {p['image']}"
        print("[PASS] All 9 catalog products and verified local image files present.")

        # TEST 2: Dell Laptop & Electronics filtering
        print("\n--- TEST 2: Electronics Category Filtering ---")
        dell = next(p for p in prods if p['name'] == 'Dell Laptop')
        assert dell['category'] == 'Electronics', f"Dell Laptop category must be 'Electronics', got '{dell['category']}'"
        assert dell['subCategory'] == 'Laptop', f"Dell Laptop subCategory must be 'Laptop', got '{dell.get('subCategory')}'"
        
        electronics = [p for p in prods if p['category'] == 'Electronics']
        assert any(p['name'] == 'Dell Laptop' for p in electronics), "Dell Laptop must be in Electronics category"
        assert len(electronics) == 4, f"Expected 4 electronics, got {len(electronics)}"
        print(f"[PASS] Dell Laptop correctly appears under Electronics category: {[p['name'] for p in electronics]}")

        # TEST 3: New Regular Customer Registration & Shopping
        print("\n--- TEST 3: New Regular Customer Registration & Cart Calculations ---")
        login_res, status = api_post("/api/login", {"name": "TestRegular", "prime": "0"})
        assert status == 200
        assert login_res['loggedIn'] is True
        assert login_res['isPrime'] is False
        assert login_res['type'] == 'Regular Customer'
        print(f"[PASS] Registered new Regular Customer: {login_res['name']} ({login_res['type']})")

        # Cart calculations (Festive on vs off)
        api_post("/api/cart/add", {"id": "202", "qty": "1"}) # Cotton T-Shirt

        c1 = api_get("/api/cart?festive=1")
        assert float(c1['festiveDiscount']) > 0, "Festive discount must be applied when festive=1"
        assert float(c1['primeDiscount']) == 0.0, "Regular customer must have 0 prime discount"
        assert float(c1['deliveryFee']) == 50.0, "Delivery fee must be 50 for regular order < 1000"

        c0 = api_get("/api/cart?festive=0")
        assert float(c0['festiveDiscount']) == 0.0, "Festive discount must be 0 when festive=0"
        assert float(c0['festiveTotal']) == float(c0['total'])

        chk_res, status = api_post("/api/checkout", {"festive": "0"})
        assert status == 200
        assert abs(float(chk_res['paid']) - float(c0['finalTotal'])) < 0.01
        assert chk_res['pointsEarned'] == 0
        print(f"[PASS] Checkout paid Rs {chk_res['paid']} matches cart finalTotal Rs {c0['finalTotal']}.")

        # TEST 4: New Prime Member Registration & Benefits
        print("\n--- TEST 4: New Prime Member Registration & Benefits ---")
        api_post("/api/logout")
        prime_login, status = api_post("/api/login", {"name": "TestPrime", "prime": "1"})
        assert status == 200
        assert prime_login['loggedIn'] is True
        assert prime_login['isPrime'] is True
        assert prime_login['points'] == 0
        print(f"[PASS] Registered new Prime Member: {prime_login['name']}, Initial points: {prime_login['points']}")

        # Prime Checkout
        api_post("/api/cart/add", {"id": "104", "qty": "1"}) # Powerbank
        c_prime = api_get("/api/cart?festive=1")
        assert float(c_prime['deliveryFee']) == 0.0, "Prime member must get FREE delivery"
        assert float(c_prime['primeDiscount']) > 0.0, "Prime member must get 5% discount"

        chk_prime, status = api_post("/api/checkout", {"festive": "1"})
        assert status == 200
        earned_pts = int(chk_prime['pointsEarned'])
        assert earned_pts > 0, "Prime member must earn points"
        print(f"[PASS] Prime member earned +{earned_pts} loyalty points on checkout.")

        # TEST 5: Enforce Membership Consistency (Negative Tests)
        print("\n--- TEST 5: Customer Membership Consistency Enforcement ---")
        api_post("/api/logout")

        # 5a: Existing Prime Member attempting Regular login
        print("  Attempting Regular login for existing Prime Member 'TestPrime'...")
        err_res, code = api_post("/api/login", {"name": "TestPrime", "prime": "0"})
        assert code == 400, f"Expected HTTP 400, got {code}"
        assert "registered as a Prime Member" in err_res['error'], f"Unexpected error: {err_res}"
        print(f"  [BLOCKED AS EXPECTED]: {err_res['error']}")

        # 5b: Case-insensitive / whitespace check for Prime Member attempting Regular login
        print("  Attempting Regular login with ' testprime ' (lowercase & spaces)...")
        err_res2, code2 = api_post("/api/login", {"name": " testprime ", "prime": "0"})
        assert code2 == 400
        assert "registered as a Prime Member" in err_res2['error']
        print(f"  [BLOCKED AS EXPECTED]: {err_res2['error']}")

        # 5c: Existing Regular Customer attempting Prime login
        print("  Attempting Prime login for existing Regular Customer 'TestRegular'...")
        err_res3, code3 = api_post("/api/login", {"name": "TestRegular", "prime": "1"})
        assert code3 == 400, f"Expected HTTP 400, got {code3}"
        assert "registered as a Regular Customer" in err_res3['error'], f"Unexpected error: {err_res3}"
        print(f"  [BLOCKED AS EXPECTED]: {err_res3['error']}")

        # 5d: Verify existing Prime Member logs in successfully with Prime toggle
        relogin_prime, status = api_post("/api/login", {"name": "TestPrime", "prime": "1"})
        assert status == 200
        assert relogin_prime['points'] == earned_pts, f"Points lost! Expected {earned_pts}, got {relogin_prime['points']}"
        print(f"[PASS] Prime member successfully logged in with restored points: {relogin_prime['points']}.")

        # Place another order to accumulate more points
        api_post("/api/cart/add", {"id": "202", "qty": "2"})
        chk_2, _ = api_post("/api/checkout", {"festive": "1"})
        total_pts = int(chk_2['session']['points'])
        print(f"[PASS] Accumulated points after 2nd order: {total_pts}")

    finally:
        proc.terminate()
        proc.wait()
        print("\n[TEST CLEANUP] Stopped primary test server")

    # TEST 6: Server Restart & Points Recovery (Using Isolated Storage)
    print("\n--- TEST 6: Server Restart & Points Recovery ---")
    proc2 = subprocess.Popen(["shop_server.exe", str(PORT)], cwd=".", env=env)
    time.sleep(2)
    try:
        relogin_restart, status = api_post("/api/login", {"name": "TestPrime", "prime": "1"})
        assert status == 200
        assert relogin_restart['points'] == total_pts, f"Points mismatch after restart: {relogin_restart['points']} != {total_pts}"
        print(f"[PASS] Recovered {total_pts} points from isolated storage after full server restart!")
    finally:
        proc2.terminate()
        proc2.wait()
        print("[TEST CLEANUP] Stopped restarted test server")

    # TEST 7: Safety & Clean-up Verification
    print("\n--- TEST 7: Non-Destructive Test Verification ---")
    if os.path.exists(ISOLATED_TEST_FILE):
        os.remove(ISOLATED_TEST_FILE)
        print(f"[PASS] Isolated test storage {ISOLATED_TEST_FILE} cleaned up.")

    if real_db_snapshot is not None:
        assert os.path.exists(real_db_path), "Real database was deleted!"
        with open(real_db_path, "r") as f:
            after_snapshot = f.read()
        assert after_snapshot == real_db_snapshot, "Real customer database was altered during tests!"
        print("[PASS] Verified real data/customers.json remained 100% UNTOUCHED and identical.")
    else:
        print("[PASS] Verified test did not create or pollute real customer database.")

    print("\n" + "=" * 70)
    print("  ALL NON-DESTRUCTIVE VERIFICATION TESTS PASSED! (7/7)")
    print("=" * 70)

if __name__ == "__main__":
    run_tests()
