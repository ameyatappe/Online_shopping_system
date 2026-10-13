# Multi-stage Dockerfile for C++ Shopping System Web Service
FROM gcc:13-bookworm AS builder

WORKDIR /app

# Copy source code and dependencies
COPY Makefile server.cpp ./
COPY include/ ./include/
COPY console/ ./console/

# Build optimized binary
RUN g++ -std=c++17 -Wall -O2 -pthread server.cpp -o shop_server

# Minimal runtime stage
FROM debian:bookworm-slim

RUN apt-get update && apt-get install -y --no-install-recommends \
    ca-certificates \
    libstdc++6 \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /app

# Copy compiled executable
COPY --from=builder /app/shop_server /app/shop_server

# Copy static frontend assets (so backend can serve UI directly if opened)
COPY web/ /app/web/

# Cloud platforms pass the port in the PORT environment variable
ENV PORT=8080
EXPOSE 8080

CMD ["./shop_server"]
