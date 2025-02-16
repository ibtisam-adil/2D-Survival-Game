#include "gui.h"

void gui_text(Rectangle rectangle, const char* text, Color color) {
    int text_width = MeasureText(text, FONT_SIZE);
    int text_x = static_cast<int>(rectangle.x + (rectangle.width - text_width) / 2);
    int text_y = static_cast<int>(rectangle.y + (rectangle.height - FONT_SIZE) / 2);

    DrawText(text, text_x, text_y, FONT_SIZE, color);
}

bool gui_button(Rectangle rectangle, const char* text) {
    bool clicked = false;
    bool hovered = false;

    if (CheckCollisionPointRec(GetMousePosition(), rectangle)) {
        hovered = true;

        if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
            clicked = true;
        }
    }

    Color background_color = hovered ? BUTTON_HOVERED_BACKGROUND_COLOR : BUTTON_BACKGROUND_COLOR;
    DrawRectangleRec(rectangle, background_color);
    gui_text(rectangle, text, BUTTON_TEXT_COLOR);

    return clicked;
}
