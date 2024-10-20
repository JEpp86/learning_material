use gstreamer as gst;

use gst::prelude::*;

fn main() {
    gst::init().unwrap();
    println!("Hello, gstreamer!");

    let uri = "https://gstreamer.freedesktop.org/data/media/sintel_trailer-480p.webm";
    let pipeline = gst::parse::launch(&format!("playbin uri={uri}")).unwrap();

    // Start playing pipeline
    pipeline
        .set_state(gst::State::Playing)
        .expect("unable to set pipline state");

    // wait until error or EOS
    let bus = pipeline.bus().unwrap();
    // need to do the loop in rust
    for msg in bus.iter_timed(gst::ClockTime::NONE) {
        use gst::MessageView;

        match msg.view() {
            MessageView::Eos(..) => break,
            MessageView::Error(err) => {
                println!(
                    "Error from {:?}: {} ({:?})",
                    err.src().map(|s| s.path_string()),
                    err.error(),
                    err.debug()
                );
                break;
            }
            _ => (),
        }
    }

    // shutdown pipeline
    pipeline
        .set_state(gst::State::Null)
        .expect("Unable to set pipline state to NULL");
}
