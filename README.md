# Goldfish

A REST API for predicting wind in golf for games in the Wii Sports series.

## Demo

https://goldfish-demo.vercel.app
https://github.com/vncz14/goldfish-demo

## Precompute files

### Download

`og_wind_precompute.bin` (8.2 GB): https://app.drime.cloud/drive/s/Ymin69vHRnQxMpyt9fHOgCRTYradFX

### Generation

The code for generating the precompute files are also available in this repository under [`server/src/core/generators`](server/src/core/generators).

```sh
cd server/src/core
cmake -S . -B build
cmake --build build
./build/generator
```

## Running

### Development

```sh
cd server
export OG_WIND_PRECOMPUTE_PATH=/path/to/og_wind_precompute.bin
cargo run
```

### Production

```sh
sudo docker build -t goldfish .
sudo docker run -p [PORT]:3000 -v /path/to/og_wind_precompute.bin:/og_wind_precompute.bin goldfish
```
## Structure of the project

-   [server](server/): Rust code that implements the REST API using [axum](https://github.com/tokio-rs/axum). Interfaces with the `core` module using [cxx](https://github.com/dtolnay/cxx).
-   [server/src/core](server/src/core/): C++ code that implements the core logic.
    -   [/generators](server/src/core/generators/): Generating precompute files
    -   [/finders](server/src/core/finders/): Finding RNG seeds that match a partially known wind. Partially based on [kiwi515/GolfAtHome](https://github.com/kiwi515/GolfAtHome).

## Special thanks

-   [kiwi515](https://github.com/kiwi515/)
    -   for [GolfAtHome](https://github.com/kiwi515/GolfAtHome)
    -   for an initial attempt at a web-based wind bruteforcer that I took some ideas from

