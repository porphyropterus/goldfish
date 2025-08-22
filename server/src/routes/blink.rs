use axum::{extract::Json, http, response::IntoResponse};
use serde::Deserialize;

use crate::ffi::{
    blink_output_to_serializable, find_og_blink, OgBlinkFinderInputFFI, OgBlinkFinderOutput,
    OgBlinkFinderSettings, ScoredOgBlinkFinderOutput, ScoredOgBlinkFinderOutputFFI,
};

#[derive(Deserialize)]
pub struct Payload {
    pub last_known_seed: Option<u32>,
    pub num_to_check: Option<u32>,
    pub blinks: Vec<u32>,
}

const MAX_NUM_TO_CHECK: u32 = 1000;

const VALID_BLINK_TIMES: &[u32] = &[10, 120, 180, 250, 260, 270, 280, 290, 300];

fn validate_payload(payload: &Payload) -> Result<(), String> {
    if payload.last_known_seed.is_none() && payload.blinks.len() < 10 {
        return Err("Not enough information provided".to_string());
    }

    for &blink in &payload.blinks {
        // -8 is for 8 extra frames between blinks
        if !VALID_BLINK_TIMES.contains(&blink) && !VALID_BLINK_TIMES.contains(&(blink - 8)) {
            return Err(format!("Invalid blink value {}", blink));
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

    Ok(())
}

fn payload_to_settings(payload: &Payload) -> OgBlinkFinderSettings {
    OgBlinkFinderSettings {
        last_known_seed: payload.last_known_seed.map_or(-1, |s| s as i64),
        num_to_check: payload.num_to_check.unwrap_or(1000),
    }
}

fn payload_to_ffi(payload: &Payload) -> OgBlinkFinderInputFFI {
    OgBlinkFinderInputFFI {
        blinks: payload.blinks.clone(),
    }
}

pub async fn find_seed_from_blink_route(Json(payload): Json<Payload>) -> impl IntoResponse {
    match validate_payload(&payload) {
        Ok(_) => {}
        Err(e) => return (http::StatusCode::BAD_REQUEST, e).into_response(),
    }

    let input = payload_to_ffi(&payload);
    let settings = payload_to_settings(&payload);

    let output = find_og_blink(&input, &settings);

    if output.error.len() > 0 {
        return (http::StatusCode::INTERNAL_SERVER_ERROR, output.error).into_response();
    }

    let serializable_result: Vec<ScoredOgBlinkFinderOutput> =
        blink_output_to_serializable(&output.outputs);

    return Json(serializable_result).into_response();
}
