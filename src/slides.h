#include "images.h"
#include "types.h"
#include <gb/drawing.h>
#define NUM_SLIDES 3
const text text_0_0 = {6, 17, "This is a test", DKGREY, WHITE, FALSE };
const slide slide_0 = {
    .background_id=1,
    .texts={
        &text_0_0,
    },
    .num_texts=1
};
const text text_1_0 = {7, 4, "Hello", WHITE, BLACK, TRUE };
const text text_1_1 = {7, 5, "World", WHITE, BLACK, TRUE };
const slide slide_1 = {
    .background_id=3,
    .texts={
        &text_1_0,
        &text_1_1,
    },
    .num_texts=2
};
const slide slide_2 = {
    .background_id=0,
    .num_texts=0
};
const slide* slides[NUM_SLIDES] = {
    &slide_0,
    &slide_1,
    &slide_2,
};
