use axum::{extract::Json, http, response::IntoResponse};
use serde::Deserialize;

use crate::ffi::{
    find_og_wind, output_to_serializable, OgWindFinderInputFFI, OgWindFinderOutput, Wind, WindFFI,
};

#[derive(Deserialize)]
pub struct Payload {
    pub game: String,
    pub last_known_seed: Option<u32>,
    pub num_to_check: Option<u32>,
    pub winds: Vec<Wind>,
}

const VALID_GAMES: &[&str] = &["og_1.0", "og_1.1", "wsr"];

const MAX_NUM_TO_CHECK: u32 = 1000;

fn validate_payload(payload: &Payload) -> Result<(), String> {
    // make sure the game is valid
    if !VALID_GAMES.contains(&payload.game.as_str()) {
        return Err(format!("Invalid game: {}", payload.game));
    }

    let ver_1_0 = payload.game == "og_1.0";

    if payload.winds.len() > 9 {
        return Err("Too many winds provided".to_string());
    }

    if payload.winds.len() == 0 {
        return Err("No winds provided".to_string());
    }

    for wind in &payload.winds {
        if wind.direction > 9 || wind.direction == 8 {
            return Err(format!("Invalid wind direction {}", wind.direction));
        }
        if wind.speed > 17 || wind.direction == 16 {
            return Err(format!("Invalid wind speed {}", wind.speed));
        }
    }

    // make sure num_to_check is not too high
    if let Some(num_to_check) = payload.num_to_check {
        if num_to_check > MAX_NUM_TO_CHECK {
            return Err(format!(
                "num_to_check is too high, maximum is {}",
                MAX_NUM_TO_CHECK
            ));
        }
    }

    // make sure we didn't provide num_to_check without last_known_seed
    if payload.num_to_check.is_some() && !payload.last_known_seed.is_some() {
        return Err("num_to_check can only be provided with last_known_seed".to_string());
    }

    // // make sure we have enough information if last_known_seed is not provided
    // if !payload.last_known_seed.is_some() {
    //     // make sure we would receive back at most ~10 results
    //     let mut info_score = if ver_1_0 {
    //         2u64.pow(16) as f64
    //     } else {
    //         2u64.pow(32) as f64
    //     };

    //     for wind in &payload.winds {
    //         if wind.direction != 9 {
    //             info_score /= 8.0;
    //         }

    //         if wind.speed != 17 {
    //             info_score /= 16.0;
    //         }
    //     }

    //     if info_score > 5.0 {
    //         return Err("Not enough information provided".to_string());
    //     }

    //     // now make sure there is only one wildcard in the first 3

    //     let mut wildcard_count = 0;
    //     for wind in payload.winds.iter().take(3) {
    //         if wind.direction == 9 {
    //             wildcard_count += 1;
    //         }

    //         if wind.speed == 17 {
    //             wildcard_count += 1;
    //         }
    //     }

    //     if wildcard_count > 1 {
    //         return Err("Too many wildcards in the first 3 winds".to_string());
    //     }
    // }

    Ok(())
}

fn payload_to_settings(payload: &Payload) -> crate::ffi::OgWindFinderSettings {
    crate::ffi::OgWindFinderSettings {
        // convert game to its index in the VALID_GAMES array
        game: VALID_GAMES
            .iter()
            .position(|&g| g == payload.game)
            .unwrap_or(0) as u8,
        last_known_seed: payload.last_known_seed.map_or(-1, |s| s as i64),
        num_to_check: 1000,
    }
}

fn payload_to_ffi(payload: &Payload) -> OgWindFinderInputFFI {
    let mut winds = [WindFFI {
        mDirection: 0,
        mSpeed: 0,
    }; 21];

    // fill in with provided winds
    for (i, w) in payload.winds.iter().take(21).enumerate() {
        winds[i] = WindFFI {
            mDirection: w.direction,
            mSpeed: w.speed,
        };
    }

    // default value for remaining: wildcard
    for i in payload.winds.len()..21 {
        winds[i] = WindFFI {
            mDirection: 9,
            mSpeed: 17,
        };
    }

    OgWindFinderInputFFI { winds }
}

pub async fn find_og_wind_route(Json(payload): Json<Payload>) -> impl IntoResponse {
    match validate_payload(&payload) {
        Ok(_) => {}
        Err(e) => return (http::StatusCode::BAD_REQUEST, e).into_response(),
    }

    let input = payload_to_ffi(&payload);
    let settings = payload_to_settings(&payload);

    let output = find_og_wind(&input, &settings);

    if output.error.len() > 0 {
        return (http::StatusCode::INTERNAL_SERVER_ERROR, output.error).into_response();
    }

    let serializable_result: Vec<OgWindFinderOutput> = output_to_serializable(&output.seeds);

    return Json(serializable_result).into_response();
}
