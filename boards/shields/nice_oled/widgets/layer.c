#include "layer.h"
#include <fonts.h>
#include <zephyr/kernel.h>

void draw_layer_status(lv_obj_t *canvas, const struct status_state *state) {
    lv_layer_t layer;
    lv_canvas_init_layer(canvas, &layer);
    lv_draw_label_dsc_t label_dsc;
#if IS_ENABLED(CONFIG_NICE_EPAPER_ON)
    init_label_dsc(&label_dsc, LVGL_FOREGROUND, &pixel_operator_mono_16, LV_TEXT_ALIGN_CENTER);
#else
    init_label_dsc(&label_dsc, LVGL_FOREGROUND, &pixel_operator_mono_16, LV_TEXT_ALIGN_LEFT);
#endif // CONFIG_NICE_EPAPER_ON

    char text[10] = {};

    if (state->layer_label == NULL) {
        sprintf(text, "Layer %i", state->layer_index);
    } else {
        strncpy(text, state->layer_label, 9);
        to_uppercase(text);
    }

#if IS_ENABLED(CONFIG_NICE_OLED_WIDGET_RESPONSIVE_BONGO_CAT)
    lv_canvas_fill_bg(canvas, LVGL_BACKGROUND, LV_OPA_COVER);
#endif
    lv_area_t coords = {
        CONFIG_NICE_OLED_WIDGET_LAYER_CUSTOM_X,
        CONFIG_NICE_OLED_WIDGET_LAYER_CUSTOM_Y,
        CONFIG_NICE_OLED_WIDGET_LAYER_CUSTOM_X + 68 - 1,
        CONFIG_NICE_OLED_WIDGET_LAYER_CUSTOM_Y + lv_font_get_line_height(label_dsc.font) - 1
    };
    lv_draw_label(&layer, &label_dsc, &coords);
    lv_canvas_finish_layer(canvas, &layer);
}
