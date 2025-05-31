#[cxx::bridge]
mod ffi {
    #[derive(Clone)]
    struct WindFFI {
        mDirection: u32,
        mSpeed: i32,
    }

    struct OgWindFinderInputFFI {
        winds: [WindFFI; 9],
    }

    #[derive(Clone)]
    struct OgWindFinderOutputFFI {
        seed: u32,
        winds: [WindFFI; 9],
    }

    unsafe extern "C++" {
        include!("server/src/core/finders/finder_bridge.h");

        fn find_og_wind(input: &OgWindFinderInputFFI) -> &CxxVector<OgWindFinderOutputFFI>;
    }
}

pub use ffi::*;

use serde::Serialize;

#[derive(Serialize)]
pub struct OgWindFinderOutput {
    pub seed: u32,
    pub winds: Vec<Wind>,
}

#[derive(Serialize)]
pub struct Wind {
    pub direction: u32,
    pub speed: i32,
}

impl From<crate::ffi::WindFFI> for Wind {
    fn from(w: crate::ffi::WindFFI) -> Self {
        Wind {
            direction: w.mDirection,
            speed: w.mSpeed,
        }
    }
}

impl From<crate::ffi::OgWindFinderOutputFFI> for OgWindFinderOutput {
    fn from(o: crate::ffi::OgWindFinderOutputFFI) -> Self {
        OgWindFinderOutput {
            seed: o.seed,
            winds: o.winds.iter().cloned().map(Wind::from).collect(),
        }
    }
}

pub fn output_to_serializable(
    cxx_vec: &cxx::CxxVector<ffi::OgWindFinderOutputFFI>,
) -> Vec<OgWindFinderOutput> {
    cxx_vec
        .iter()
        .cloned()
        .map(OgWindFinderOutput::from)
        .collect()
}
