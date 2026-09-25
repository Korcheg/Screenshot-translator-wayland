#include "../headers/buttons.h"


CircleButton create_circle_button(float cx, float cy, float radius) {
    return (CircleButton){
        .center_x = cx,
        .center_y = cy,
        .radius = radius,
        .radius_sq = radius * radius
    };
}

bool is_inside_circle(const CircleButton *btn, float mouse_x, float mouse_y) {
    float dx = mouse_x - btn->center_x;
    float dy = mouse_y - btn->center_y;

    // Квадрат гіпотенузи
    return (dx * dx + dy * dy) <= btn->radius_sq;
}

void circle_button_draw_hitbox(const CircleButton *btn, 
                             uint32_t *pixels, int buf_w, int buf_h, 
                             int thickness, uint32_t color) {
    if (!pixels || buf_w <= 0 || buf_h <= 0 || !btn) return;

    float r_outer = btn->radius + (thickness / 2.0f);
    float r_inner = btn->radius - (thickness / 2.0f);
    if (r_inner < 0) r_inner = 0;

    int r_outer_sq = r_outer * r_outer;
    int r_inner_sq = r_inner * r_inner;

    int min_x = (int)(btn->center_x - r_outer);
    int max_x = (int)(btn->center_x + r_outer);
    int min_y = (int)(btn->center_y - r_outer);
    int max_y = (int)(btn->center_y + r_outer);

    if (min_x < 0) min_x = 0;
    if (max_x >= buf_w) max_x = buf_w - 1;
    if (min_y < 0) min_y = 0;
    if (max_y >= buf_h) max_y = buf_h - 1;

    for (int y = min_y; y <= max_y; y++) {
        // Зміщення рядка для uint32_t* вираховується в елементах (пікселях)
        int row = y * buf_w; 
        float dy = y - btn->center_y;

        for (int x = min_x; x <= max_x; x++) {
            float dx = x - btn->center_x;
            float dist_sq = dx * dx + dy * dy;

            if (dist_sq >= r_inner_sq && dist_sq <= r_outer_sq) {
                pixels[row + x] = color;
            }
        }
    }
}
