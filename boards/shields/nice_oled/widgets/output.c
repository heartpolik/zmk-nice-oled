#include "output.h"
// #include "../assets/custom_fonts.h"
#include <fonts.h>
#include <zephyr/kernel.h>

LV_IMG_DECLARE(bt_no_signal);
LV_IMG_DECLARE(bt_unbonded);
LV_IMG_DECLARE(bt);
LV_IMG_DECLARE(usb);

#if !IS_ENABLED(CONFIG_ZMK_SPLIT) || IS_ENABLED(CONFIG_ZMK_SPLIT_ROLE_CENTRAL)
static void draw_usb_connected(lv_layer_t *layer) {
    lv_draw_image_dsc_t img_dsc;
    lv_draw_image_dsc_init(&img_dsc);

    lv_area_t coords = {
        CONFIG_NICE_OLED_WIDGET_OUTPUT_USB_CUSTOM_X,
        CONFIG_NICE_OLED_WIDGET_OUTPUT_USB_CUSTOM_Y,
        CONFIG_NICE_OLED_WIDGET_OUTPUT_USB_CUSTOM_X + usb.header.w - 1,
        CONFIG_NICE_OLED_WIDGET_OUTPUT_USB_CUSTOM_Y + usb.header.h - 1
    };
    lv_draw_image(layer, &img_dsc, &coords);
}

static void draw_ble_unbonded(lv_layer_t *layer) {
    lv_draw_image_dsc_t img_dsc;
    lv_draw_image_dsc_init(&img_dsc);

    lv_area_t coords = {
        CONFIG_NICE_OLED_WIDGET_OUTPUT_BT_UNBONDED_CUSTOM_X,
        CONFIG_NICE_OLED_WIDGET_OUTPUT_BT_UNBONDED_CUSTOM_Y,
        CONFIG_NICE_OLED_WIDGET_OUTPUT_BT_UNBONDED_CUSTOM_X + bt_unbonded.header.w - 1,
        CONFIG_NICE_OLED_WIDGET_OUTPUT_BT_UNBONDED_CUSTOM_Y + bt_unbonded.header.h - 1
    };
    lv_draw_image(layer, &img_dsc, &coords);
}
#endif

static void draw_ble_disconnected(lv_layer_t *layer) {
    lv_draw_image_dsc_t img_dsc;
    lv_draw_image_dsc_init(&img_dsc);

    lv_area_t coords = {
        CONFIG_NICE_OLED_WIDGET_OUTPUT_BT_CUSTOM_X,
        CONFIG_NICE_OLED_WIDGET_OUTPUT_BT_CUSTOM_Y,
        CONFIG_NICE_OLED_WIDGET_OUTPUT_BT_CUSTOM_X + bt_no_signal.header.w - 1,
        CONFIG_NICE_OLED_WIDGET_OUTPUT_BT_CUSTOM_Y + bt_no_signal.header.h - 1
    };
    lv_draw_image(layer, &img_dsc, &coords);
}

static void draw_ble_connected(lv_layer_t *layer) {
    lv_draw_image_dsc_t img_dsc;
    lv_draw_image_dsc_init(&img_dsc);

    lv_area_t coords = {
        CONFIG_NICE_OLED_WIDGET_OUTPUT_BT_CUSTOM_X,
        CONFIG_NICE_OLED_WIDGET_OUTPUT_BT_CUSTOM_Y,
        CONFIG_NICE_OLED_WIDGET_OUTPUT_BT_CUSTOM_X + bt.header.w - 1,
        CONFIG_NICE_OLED_WIDGET_OUTPUT_BT_CUSTOM_Y + bt.header.h - 1
    };
    lv_draw_image(layer, &img_dsc, &coords);
}

void draw_output_status(lv_layer_t *layer, const struct status_state *state) {
    lv_layer_t layer;
    lv_canvas_init_layer(canvas, &layer);
#if IS_ENABLED(CONFIG_NICE_EPAPER_ON) &&                                                           \
    !IS_ENABLED(CONFIG_NICE_OLED_WIDGET_CENTRAL_SHOW_BATTERY_PERIPHERAL_ALL)
    lv_draw_label_dsc_t label_dsc;
    init_label_dsc(&label_dsc, LVGL_FOREGROUND, &pixel_operator_mono_16, LV_TEXT_ALIGN_LEFT);
    lv_area_t sig_coords = {0, 1, 24, 1 + lv_font_get_line_height(label_dsc.font) - 1};
    lv_draw_label(&layer, &label_dsc, &sig_coords);

#if IS_ENABLED(CONFIG_NICE_OLED_WIDGET_OUTPUT_BACKGROUND)
    lv_draw_rect_dsc_t rect_white_dsc;
    init_rect_dsc(&rect_white_dsc, LVGL_FOREGROUND);
    lv_area_t rect_area = {43, 0, 66, 14};
    lv_draw_rect(&layer, &rect_white_dsc, &rect_area);
#endif

#else

#if IS_ENABLED(CONFIG_NICE_OLED_WIDGET_OUTPUT_BACKGROUND)
    lv_draw_rect_dsc_t rect_white_dsc;
    init_rect_dsc(&rect_white_dsc, LVGL_FOREGROUND);
    lv_area_t rect_area = {-3, 32, 20, 46};
    lv_draw_rect(&layer, &rect_white_dsc, &rect_area);
#endif

#endif // CONFIG_NICE_EPAPER_ON

#if !IS_ENABLED(CONFIG_ZMK_SPLIT) || IS_ENABLED(CONFIG_ZMK_SPLIT_ROLE_CENTRAL)
    switch (state->selected_endpoint.transport) {
    case ZMK_TRANSPORT_USB:
        draw_usb_connected(&layer);
        break;

    case ZMK_TRANSPORT_BLE:
        if (state->active_profile_bonded) {
            if (state->active_profile_connected) {
                draw_ble_connected(&layer);
            } else {
                draw_ble_disconnected(&layer);
            }
        } else {
            draw_ble_unbonded(&layer);
        }
        break;
    }
#else
    if (state->connected) {
        draw_ble_connected(&layer);
    } else {
        draw_ble_disconnected(&layer);
    }
#endif
    lv_canvas_finish_layer(canvas, &layer);
}
