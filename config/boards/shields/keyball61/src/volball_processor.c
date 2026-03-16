#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/input/input.h>
#include <zephyr/logging/log.h>

LOG_MODULE_REGISTER(zmk_volball, CONFIG_ZMK_LOG_LEVEL);

/*
 * STARTER SKELETON ONLY
 *
 * This file is intentionally a scaffold, not a guaranteed drop-in final module.
 * It captures the logic for a custom ZMK processor that:
 *   - only acts on one layer,
 *   - converts vertical trackball movement into volume key taps,
 *   - swallows normal pointer movement on that layer.
 *
 * The parts marked TODO are the pieces most likely to need adjustment to match
 * your exact ZMK version, input-listener wiring, and consumer-key injection API.
 */

static int volume_accumulator;

/* TODO: Replace with the correct include/symbol for your ZMK version. */
extern bool zmk_keymap_layer_active(uint8_t layer);

static bool volball_layer_active(int layer) {
    return zmk_keymap_layer_active((uint8_t)layer);
}

/*
 * TODO: Implement these using the actual ZMK API you decide to use for
 * injecting consumer key taps. The most likely options are:
 *   - invoke a behavior binding for C_VOL_UP / C_VOL_DN
 *   - or synthesize the matching HID consumer report through ZMK internals
 */
static void tap_consumer_volume_up(void) {
    /* TODO */
}

static void tap_consumer_volume_down(void) {
    /* TODO */
}

static void handle_delta_y(int dy, int threshold, bool invert) {
    if (invert) {
        dy = -dy;
    }

    volume_accumulator += dy;

    while (volume_accumulator >= threshold) {
        tap_consumer_volume_down();
        volume_accumulator -= threshold;
    }

    while (volume_accumulator <= -threshold) {
        tap_consumer_volume_up();
        volume_accumulator += threshold;
    }
}

/*
 * TODO: Adapt this to the exact ZMK input processor registration API and event
 * dispatch signature used by your current ZMK version.
 */
int volball_process_event(const struct device *dev,
                          struct input_event *evt,
                          int layer,
                          int threshold,
                          bool invert) {
    ARG_UNUSED(dev);

    if (!volball_layer_active(layer)) {
        return 0;
    }

    if (evt->type == INPUT_EV_REL && evt->code == INPUT_REL_Y) {
        handle_delta_y(evt->value, threshold, invert);

        /* Swallow normal Y motion while volume mode is active. */
        evt->value = 0;
        return 0;
    }

    if (evt->type == INPUT_EV_REL && evt->code == INPUT_REL_X) {
        /* Also swallow X so the cursor does not drift sideways in volume mode. */
        evt->value = 0;
        return 0;
    }

    return 0;
}
