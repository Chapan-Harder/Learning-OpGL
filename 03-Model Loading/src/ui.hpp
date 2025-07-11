#pragma once
#include <found_libs.hpp>

#define WidthNuklear 320.0f
#define HightNuklear 450.0f

void ui_app(struct nk_context *ctx, struct nk_colorf* bg_color, bool *bg_color_option, bool *wireframe_mode) {
  if (nk_begin(ctx, "Menu Option", nk_rect(0, 0, WidthNuklear, HightNuklear), NK_WINDOW_TITLE | NK_WINDOW_MINIMIZABLE | NK_WINDOW_MOVABLE)) {
    nk_layout_row_dynamic(ctx, 10.0f, 1);
    nk_label(ctx, "--------", NK_TEXT_CENTERED);
    
    // Wireframe mode toggle
    nk_layout_row_dynamic(ctx, 25.0f, 1);
    *wireframe_mode = nk_check_label(ctx, "Wireframe Mode", *wireframe_mode);

    nk_layout_row_dynamic(ctx, 10.0f, 1);
    nk_label(ctx, "--------", NK_TEXT_CENTERED);
    // Checkbox for -> bg_color
    // ------------------------------------------------------------------------
    nk_layout_row_dynamic(ctx, 10.0f,  1);
    *bg_color_option = nk_check_label(ctx, "Edit the background color.", *bg_color_option);

    if (*bg_color_option) {
      nk_layout_row_dynamic(ctx, 70.0f,  1);
      nk_label(ctx, "Choose Background Color:", NK_TEXT_LEFT);
    
      // Color picker widget
      nk_layout_row_dynamic(ctx, 120.0f, 1);
      *bg_color = nk_color_picker(ctx, *bg_color, NK_RGBA);

      // RGB sliders
      nk_layout_row_dynamic(ctx, 25.0f, 1);
      bg_color->r = nk_propertyf(ctx, "Red:", 0, bg_color->r, 1.0f, 0.01f, 0.005f);
      bg_color->g = nk_propertyf(ctx, "Green:", 0, bg_color->g, 1.0f, 0.01f, 0.005f);
      bg_color->b = nk_propertyf(ctx, "Blue:", 0, bg_color->b, 1.0f, 0.01f, 0.005f);
    }
    // ------------------------------------------------------------------------
  }
  nk_end(ctx);
}
