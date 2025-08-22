#[cxx::bridge]
mod ffi {
    #[derive(Clone, Copy)]
    struct WindFFI {
        mDirection: u32,
        mSpeed: i32,
    }

    struct OgWindFinderInputFFI {
        winds: [WindFFI; 21],
    }

    #[derive(Clone)]
    struct OgWindFinderOutputFFI {
        seed: u32,
        winds: [WindFFI; 21],
    }

    struct OgWindFinderOutputWithErrorFFI {
        seeds: Vec<OgWindFinderOutputFFI>,
        error: String,
    }

    struct OgWindFinderSettings {
        game: u8,
        last_known_seed: i64, // we need a representation for null (-1), so we need to use i64
        num_to_check: u32,
    }

    struct OgBlinkFinderInputFFI {
        blinks: Vec<u32>,
    }

    #[derive(Clone)]
    struct OgBlinkFinderOutputFFI {
        seed: u32,
        blinks: Vec<u32>,
    }

    #[derive(Clone)]
    struct ScoredOgBlinkFinderOutputFFI {
        score: u32,
        output: OgBlinkFinderOutputFFI,
    }

    struct ScoredOgBlinkFinderOutputWithErrorFFI {
        outputs: Vec<ScoredOgBlinkFinderOutputFFI>,
        error: String,
    }

    struct OgBlinkFinderSettings {
        last_known_seed: i64, // we need a representation for null (-1), so we need to use i64
        num_to_check: u32,
    }

    unsafe extern "C++" {
        include!("server/src/core/finders/finder_bridge.h");

        fn find_og_wind(
            input: &OgWindFinderInputFFI,
            settings: &OgWindFinderSettings,
        ) -> OgWindFinderOutputWithErrorFFI;

        fn find_og_blink(
            input: &OgBlinkFinderInputFFI,
            settings: &OgBlinkFinderSettings,
        ) -> ScoredOgBlinkFinderOutputWithErrorFFI;
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

#[derive(Serialize)]
pub struct OgBlinkFinderOutput {
    pub seed: u32,
    pub blinks: Vec<u32>,
}

#[derive(Serialize)]
pub struct ScoredOgBlinkFinderOutput {
    pub score: u32,
    pub output: OgBlinkFinderOutput,
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

impl From<crate::ffi::ScoredOgBlinkFinderOutputFFI> for ScoredOgBlinkFinderOutput {
    fn from(o: crate::ffi::ScoredOgBlinkFinderOutputFFI) -> Self {
        ScoredOgBlinkFinderOutput {
            score: o.score,
            output: OgBlinkFinderOutput {
                seed: o.output.seed,
                blinks: o.output.blinks.clone(),
            },
        }
    }
}

pub fn wind_output_to_serializable(vec: &Vec<OgWindFinderOutputFFI>) -> Vec<OgWindFinderOutput> {
    vec.iter().cloned().map(OgWindFinderOutput::from).collect()
}

pub fn blink_output_to_serializable(
    vec: &Vec<ScoredOgBlinkFinderOutputFFI>,
) -> Vec<ScoredOgBlinkFinderOutput> {
    vec.iter()
        .cloned()
        .map(ScoredOgBlinkFinderOutput::from)
        .collect()
}
