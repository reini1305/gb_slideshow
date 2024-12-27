#include <gbdk/platform.h>
#include <gb/drawing.h>
#include <stdint.h>

#include "slides.h"


#define CGB_BKG_PAL_0 0u
#define CGB_ONE_PAL   1u

uint8_t buttons, buttons_prev;
#define UPDATE_BUTTONS()            (buttons_prev = buttons, buttons = joypad())
#define BUTTON_TOGGLED(BUTTON_MASK) ((buttons & (~buttons_prev)) & (BUTTON_MASK))
#define BUTTON_PRESSED(BUTTON_MASK) (buttons & (BUTTON_MASK))
uint8_t current_slide_id = 0;
uint8_t current_text_id = 0;


void show_image(void){
    uint8_t image_id = slides[current_slide_id]->background_id;
    uint8_t current_bank = CURRENT_BANK;
    SWITCH_ROM(image_id / 2 + 1);
    draw_image(images[image_id]);
    if (_cpu == CGB_TYPE) {
        set_bkg_palette(BKGF_CGB_PAL0, CGB_ONE_PAL, palettes[image_id]);
    }
    SWITCH_ROM(current_bank);
}

void draw_text(void) {
    const slide* curr_slide = slides[current_slide_id];
    if (curr_slide->num_texts == 0) {
        return;
    }
    const text* curr_text = &curr_slide->texts[current_text_id];
    color(curr_text->color, curr_text->back, OR);
    if (curr_text->animated) {
        char* c = curr_text->t;
        uint8_t x = curr_text->x;
        uint8_t y = curr_text->y;
        while(*c) {
            gotogxy(x, y);
            gprintf("%c",*c);
            c++;
            x++;
            if (x==20) {
                x=0;
                y++;
            }
            vsync();
            vsync();
            vsync();
        }
    } else {
        gotogxy(curr_text->x, curr_text->y);
        gprint(curr_text->t);
    }
}

void load_next_text(void) {
    if (current_text_id < slides[current_slide_id]->num_texts - 1) {
        current_text_id++;
        draw_text();
    } else {
        if (current_slide_id == NUM_SLIDES - 1) {
            return;
        }
        current_text_id = 0;
        current_slide_id++;
        show_image();
        draw_text();
    }
}

void load_previous_text(void) {
    if (current_text_id > 0) {
        show_image();
        uint8_t i = current_text_id;
        for(current_text_id=0; current_text_id<i; current_text_id++){
            draw_text();
        }
    } else {
        if (current_slide_id == 0) {
            return;
        }
        current_slide_id--;
        show_image();
        for(current_text_id=0; current_text_id<slides[current_slide_id]->num_texts; current_text_id++){
            draw_text();
        }
    }
    current_text_id--;
}

void main(void)
{
    show_image();
    draw_text();
    SHOW_BKG;

    vsync();
    if (_cpu != CGB_TYPE) {
        BGP_REG = DMG_PALETTE(DMG_WHITE, DMG_LITE_GRAY, DMG_DARK_GRAY, DMG_BLACK);
    }
    while(1) {
        UPDATE_BUTTONS();
        if (BUTTON_TOGGLED(J_A)){
            load_next_text();
        }
        if (BUTTON_TOGGLED(J_B)){
            load_previous_text();
        }
        vsync();
    }
}
