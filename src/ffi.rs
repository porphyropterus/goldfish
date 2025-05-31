#[cxx::bridge]
mod ffi {
    struct Wind {
        mDirection: u32,
        mSpeed: i32,
    }

    struct OgWindFinderInputFFI {
        winds: Vec<Wind>,
    }

    struct OgWindFinderOutputFFI {
        seed: u32,
        winds: Vec<Wind>,
    }

    unsafe extern "C++" {
        include!("finder_bridge.h");

        fn find_og_wind(input: &OgWindFinderInputFFI) -> Vec<OgWindFinderOutputFFI>;
    }
}

pub use ffi::*;
