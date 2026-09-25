#ifndef CIRCLE_BUTTON_H
#define CIRCLE_BUTTON_H

#include <stdbool.h>
#include <stdint.h>

typedef struct {
    float center_x;
    float center_y;
    float radius;
    float radius_sq; 
} CircleButton;


CircleButton create_circle_button(float cx, float cy, float radius);
bool is_inside_circle(const CircleButton *btn, float mouse_x, float mouse_y);
void circle_button_draw_hitbox(const CircleButton *btn, 
                             uint32_t *buffer, int buf_w, int buf_h, 
                             int thickness, uint32_t color);

#endif // CIRCLE_BUTTON_H
