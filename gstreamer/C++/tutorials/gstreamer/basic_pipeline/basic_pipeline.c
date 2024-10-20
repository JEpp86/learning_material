#include <gst/gst.h>

int main(int argc, char *argv[]) {
    GstElement *pipeline, *source, *filter, *sink;
    GstBus *bus;
    GstMessage *msg;
    GstStateChangeReturn ret;

    // initalize
    gst_init(&argc, &argv);

    // create sink and source elements
    source = gst_element_factory_make("videotestsrc", "source");
    filter = gst_element_factory_make("videoconvert", "filter");
    sink = gst_element_factory_make("autovideosink", "sink");

    // create pipline
    pipeline = gst_pipeline_new("test-pipline");

    // build pipline
    gst_bin_add_many(GST_BIN(pipeline), source, filter, sink, NULL);
    if (gst_element_link(source, filter) != TRUE) {
        g_printerr("Could not link filter elements");
        gst_object_unref(pipeline);
        return -1;
    }
    if (gst_element_link(filter, sink) != TRUE) {
        g_printerr("Could not link sink elements");
        gst_object_unref(pipeline);
        return -1;
    }

    g_object_set(source, "pattern", 0, NULL);

    // start playing
    ret = gst_element_set_state(pipeline, GST_STATE_PLAYING);
    if (ret == GST_STATE_CHANGE_FAILURE) {
        g_printerr("set state playing failed. \n");
        gst_object_unref(pipeline);
        return -1;
    }

    // wait until error or EOS
    bus = gst_element_get_bus(pipeline);
    msg = gst_bus_timed_pop_filtered(bus, GST_CLOCK_TIME_NONE, GST_MESSAGE_ERROR | GST_MESSAGE_EOS);

    // parse messages
    if (msg != NULL) {
        GError *err;
        gchar *debug_info;

        switch (GST_MESSAGE_TYPE(msg)) {
        case GST_MESSAGE_ERROR:
            gst_message_parse_error(msg, &err, &debug_info);
            g_printerr("Error received from element %s: %s\n",
                       GST_OBJECT_NAME(msg->src), err->message);
            g_printerr("Debugging information: %s\n",
                       debug_info ? debug_info : "none");
            g_clear_error(&err);
            g_free(debug_info);
            break;
        case GST_MESSAGE_EOS:
            g_print("End-Of-Stream reached.\n");
            break;
        default:
            /* We should not reach here because we only asked for ERRORs and EOS */
            g_printerr("Unexpected message received.\n");
            break;
        }
        gst_message_unref(msg);
    }

    // free resouces
    gst_object_unref(bus);
    gst_element_set_state(pipeline, GST_STATE_NULL);
    gst_object_unref(pipeline);
    return 0;
}