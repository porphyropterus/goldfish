FROM rust:slim as builder

RUN apt-get update && apt-get install -y \
    cmake \
    build-essential \
    zlib1g-dev

WORKDIR /app

COPY server/. .

RUN cargo build --release

FROM debian:bookworm-slim

RUN apt-get update && apt-get install -y zlib1g && rm -rf /var/lib/apt/lists/*

WORKDIR /app

COPY --from=builder /app/target/release/server /app/server

EXPOSE 3000

CMD ["/app/server"]
