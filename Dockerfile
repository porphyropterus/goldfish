# Use the official Rust image with build tools and C++ toolchain
FROM rust:1.76 as builder

RUN apt-get update && apt-get install -y \
    cmake \
    build-essential \
    zlib1g-dev

WORKDIR /app

COPY . .

RUN cargo build --release

FROM debian:bookworm-slim

RUN apt-get update && apt-get install -y zlib1g && rm -rf /var/lib/apt/lists/*

WORKDIR /app

COPY --from=builder /app/server/target/release/server /app/server

EXPOSE 3000

CMD ["/app/server"]