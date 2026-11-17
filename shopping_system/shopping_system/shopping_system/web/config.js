/**
 * ShopX Frontend Runtime Configuration
 *
 * If you deploy the frontend and backend on different URLs:
 * Set BACKEND_URL to your Render backend (e.g. "https://your-service.onrender.com").
 *
 * If left empty (""), the app will use:
 * 1. Runtime override saved via the UI ("Backend URL" button in header)
 * 2. Vercel reverse-proxy rewrites (/api/... -> Render backend)
 * 3. Relative path (when running locally or served directly from the C++ server)
 */
window.SHOP_CONFIG = {
  BACKEND_URL: ""
};
