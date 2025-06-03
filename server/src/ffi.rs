#[cxx::bridge]
mod ffi {
    #[derive(Clone, Copy)]
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

    struct OgWindFinderSettings {
        last_known_seed: i64, // we need a representation for null (-1), so we need to use i64
        num_to_check: u32,
    }

    unsafe extern "C++" {
        include!("server/src/core/finders/finder_bridge.h");

        fn find_og_wind(
            input: &OgWindFinderInputFFI,
            settings: &OgWindFinderSettings,
        ) -> Vec<OgWindFinderOutputFFI>;
    }
}

pub use ffi::*;

use serde::{Deserialize, Serialize};

#[derive(Serialize)]
pub struct OgWindFinderOutput {
    pub seed: u32,
    pub winds: Vec<Wind>,
}

#[derive(Serialize, Deserialize)]
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

pub fn output_to_serializable(vec: &Vec<OgWindFinderOutputFFI>) -> Vec<OgWindFinderOutput> {
    vec.iter().cloned().map(OgWindFinderOutput::from).collect()
}
