#include <found_libs.hpp>

const GLfloat SETTING_WIDTH = 300.0f, SETTING_HEIGHT = 500.0f;

// For text editor
char text_buffer[10] = "1.0";
GLint text_lenth = (int)strlen(text_buffer);
nk_flags edit_flags = NK_EDIT_FIELD | NK_EDIT_SELECTABLE | NK_EDIT_CLIPBOARD;

void ui_setting(struct nk_context *ctx, struct nk_colorf *bg_color, struct nk_colorf *lt_color, GLfloat *lt_power) {
  if (nk_begin(ctx, "Settings:", nk_rect(5.0f, 5.0f, SETTING_WIDTH, SETTING_HEIGHT), NK_WINDOW_TITLE | NK_WINDOW_MINIMIZABLE | NK_WINDOW_MOVABLE)) {
    // Light and background color
    // ------------------------------------------------------------------------
    nk_layout_row_dynamic(ctx, 20.0f, 1);
    nk_label(ctx, "Light & Background Color:", NK_TEXT_LEFT);

    nk_layout_row_dynamic(ctx, 100.0f, 2);
    *bg_color = nk_color_picker(ctx, *bg_color, NK_RGB);
    *lt_color = nk_color_picker(ctx, *lt_color, NK_RGB);

    // RGB sliders
    nk_layout_row_dynamic(ctx, 20.0f, 2);
    bg_color->r = nk_propertyf(ctx, "bg_R:", 0, bg_color->r, 1.0f, 0.01f, 0.005f);
    lt_color->r = nk_propertyf(ctx, "lt_R:", 0, lt_color->r, 1.0f, 0.01f, 0.005f);
    bg_color->b = nk_propertyf(ctx, "bg_B:", 0, bg_color->b, 1.0f, 0.01f, 0.005f);    
    lt_color->b = nk_propertyf(ctx, "lt_B:", 0, lt_color->b, 1.0f, 0.01f, 0.005f);    
    bg_color->g = nk_propertyf(ctx, "bg_G:", 0, bg_color->g, 1.0f, 0.01f, 0.005f);
    lt_color->g = nk_propertyf(ctx, "lt_G:", 0, lt_color->g, 1.0f, 0.01f, 0.005f);

    nk_layout_row_dynamic(ctx, 10.0f, 1);
    nk_layout_row_dynamic(ctx, 25.0f, 2);
    nk_label(ctx, "Light Power", NK_TEXT_LEFT);
    nk_edit_string(ctx, edit_flags, text_buffer, &text_lenth, 10, nk_filter_default);
    try {
      *lt_power = std::stof(text_buffer);
    } catch (const std::invalid_argument &e) {
      std::cout << "Invalid argument: " << e.what() << std::endl;
    } catch (const std::out_of_range &e) {
      std::cout << "Out of range: " << e.what() << std::endl;
    }
    // ------------------------------------------------------------------------
  }
  nk_end(ctx);
}
