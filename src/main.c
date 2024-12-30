#include <gbdk/platform.h>
#include <gb/drawing.h>
#include <stdint.h>

#include "slides.h"

#define REMEMBER_SLIDE  // Comment if you want to start slideshow from beginning
#define CGB_BKG_PAL_0 0u
#define CGB_ONE_PAL   1u

uint8_t buttons, buttons_prev;
#define UPDATE_BUTTONS()            (buttons_prev = buttons, buttons = joypad())
#define BUTTON_TOGGLED(BUTTON_MASK) ((buttons & (~buttons_prev)) & (BUTTON_MASK))
#define BUTTON_PRESSED(BUTTON_MASK) (buttons & (BUTTON_MASK))
extern uint8_t current_slide_id;
extern uint8_t checksum;
int8_t current_text_id = 0;


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

void draw_text(BOOLEAN ignore_animated) {
    const slide* curr_slide = slides[current_slide_id];
    if (curr_slide->num_texts == 0) {
        return;
    }
    const text* curr_text = &curr_slide->texts[current_text_id];
    color(curr_text->color, curr_text->back, OR);
    if (curr_text->animated && !ignore_animated) {
        char* c = curr_text->t;
        uint8_t x = curr_text->x;
        uint8_t y = curr_text->y;
        while(*c) {
            gotogxy(x, y);
            gprintf("%c", *c);
            c++;
            x++;
            if (x==20) {
                x=curr_text->x;
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
        draw_text(FALSE);
    } else {
        if (current_slide_id == NUM_SLIDES - 1) {
            return;
        }
        current_text_id = 0;
        current_slide_id++;
        show_image();
        draw_text(FALSE);
    }
}

void load_previous_text(void) {
    if (current_text_id > 0) {
        show_image();
        int8_t i = current_text_id;
        for(current_text_id=0; current_text_id<i; current_text_id++){
            draw_text(TRUE);
        }
    } else {
        if (current_slide_id == 0) {
            return;
        }
        current_slide_id--;
        show_image();
        for(current_text_id=0; current_text_id<slides[current_slide_id]->num_texts; current_text_id++){
            draw_text(TRUE);
        }
    }
    current_text_id--;
    if (current_text_id < 0) {
        current_text_id = 0;
    }
}

void main(void)
{
    ENABLE_RAM;
    SWITCH_RAM(0);
#ifdef REMEMBER_SLIDE
    if (checksum != NUM_SLIDES) {
        current_slide_id=0;
        checksum = NUM_SLIDES;
    }
#else
    current_slide_id = 0;
#endif
    show_image();
    draw_text(FALSE);
    SHOW_BKG;

    vsync();
    if (_cpu != CGB_TYPE) {
        BGP_REG = DMG_PALETTE(DMG_WHITE, DMG_LITE_GRAY, DMG_DARK_GRAY, DMG_BLACK);
    }
    while(1) {
        UPDATE_BUTTONS();
        if (BUTTON_TOGGLED(J_A | J_RIGHT)){
            load_next_text();
        }
        if (BUTTON_TOGGLED(J_B | J_LEFT)){
            load_previous_text();
        }
        if (BUTTON_TOGGLED(J_START)){
            current_slide_id = 0;
            current_text_id = 0;
            show_image();
            draw_text(FALSE);
        }
        vsync();
    }
}
