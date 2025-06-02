use axum::{extract::Json, response::IntoResponse};
use serde::Deserialize;

use crate::ffi::{
    find_og_wind, output_to_serializable, OgWindFinderInputFFI, OgWindFinderOutput, WindFFI,
};

#[derive(Deserialize)]
pub struct Payload {
    pub a: u32,
}

pub async fn find_og_wind_route(Json(payload): Json<Payload>) -> impl IntoResponse {
    println!("{}", payload.a);

    let input = OgWindFinderInputFFI {
        winds: [
            WindFFI {
                mDirection: 0,
                mSpeed: 0,
            },
            WindFFI {
                mDirection: 1,
                mSpeed: 1,
            },
            WindFFI {
                mDirection: 2,
                mSpeed: 2,
            },
            WindFFI {
                mDirection: 3,
                mSpeed: 3,
            },
            WindFFI {
                mDirection: 9,
                mSpeed: 17,
            },
            WindFFI {
                mDirection: 9,
                mSpeed: 17,
            },
            WindFFI {
                mDirection: 9,
                mSpeed: 17,
            },
            WindFFI {
                mDirection: 9,
                mSpeed: 17,
            },
            WindFFI {
                mDirection: 9,
                mSpeed: 17,
            },
        ],
    };

    let output = find_og_wind(&input);

    let serializable_result: Vec<OgWindFinderOutput> = output_to_serializable(&output);

    Json(serializable_result)
}
