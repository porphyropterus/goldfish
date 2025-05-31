fn main() {
    cxx_build::bridge("src/ffi.rs")
        .file("src/core/finders/finder_bridge.cpp")
        .flag_if_supported("-std=c++17")
        .include("src/core/finders")
        .include("src/core")
        .compile("finder_bridge");
}
