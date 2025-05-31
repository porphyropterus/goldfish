use axum::{response::IntoResponse, Json};

use crate::ffi::{
    find_og_wind, output_to_serializable, OgWindFinderInputFFI, OgWindFinderOutput, WindFFI,
};

pub async fn find_og_wind_route() -> impl IntoResponse {
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
            WindFFI {
                mDirection: 9,
                mSpeed: 17,
            },
        ],
    };

    let result = find_og_wind(&input);

    let serializable_result: Vec<OgWindFinderOutput> = output_to_serializable(&result);

    Json(serializable_result)
}
