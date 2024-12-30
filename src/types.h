#pragma once

typedef struct Text {
    uint8_t x;
    uint8_t y;
    const char* t;
    uint8_t color;
    uint8_t back;
    uint8_t animated;
} text;

typedef struct Slide {
    uint8_t background_id;
    const text texts[10];
    int8_t num_texts;
} slide;