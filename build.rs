fn main() {
    cxx_build::bridge("src/ffi.rs")
        .file("core/src/finders/finder_bridge.cpp")
        .flag_if_supported("-std=c++17")
        .include("../core/src/finders")
        .compile("finder_bridge");
}
