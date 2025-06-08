use axum::{routing::post, Router};
use tower_http::cors::{Any, CorsLayer};
mod routes {
    pub mod wind2seed;
}

mod ffi;

#[tokio::main]
async fn main() {
    // allow all cors origins
    let cors = CorsLayer::new()
        .allow_origin(Any)
        .allow_methods(Any)
        .allow_headers(Any);

    // build our application with a single route
    let app = Router::new()
        .route("/wind2seed", post(routes::wind2seed::find_og_wind_route))
        .layer(cors);

    // run our app with hyper, listening globally on port 3000
    let listener = tokio::net::TcpListener::bind("0.0.0.0:3000").await.unwrap();
    axum::serve(listener, app).await.unwrap();
}
