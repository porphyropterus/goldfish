fn main() {
    cxx_build::bridge("src/ffi.rs")
        .file("src/core/finders/finder_bridge.cpp")
        .file("src/core/lib/RP/RPGolConfig.cpp")
        .file("src/core/lib/RP/RPUtlRandom.cpp")
        .file("src/core/lib/RP/RPGolWindSet.cpp")
        .file("src/core/util_zlib.cpp")
        .flag_if_supported("-std=c++17")
        .flag_if_supported("-O3")
        .include("src/core/finders")
        .include("src/core")
        .compile("finder_bridge");

    println!("cargo:rustc-link-lib=z");
}
