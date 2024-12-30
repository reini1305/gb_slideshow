import json
import os

images = []

for file in os.listdir('res'):
    if file.endswith('png'):
        images.append(file.split('.')[0])

images_h = []
for image in images:
    images_h.append(f'#include <res/{image}.h>\n')
images_h.append(f'#define NUM_IMAGES {len(images)}\n')
images_h.append('const uint8_t* const images[NUM_IMAGES] = {\n')
for image in images:
    images_h.append(f'    {image}_tiles,\n')
images_h.append('};\n')
images_h.append('const palette_color_t* const palettes[NUM_IMAGES] = {\n')
for image in images:
    images_h.append(f'    {image}_palettes,\n')
images_h.append('};\n')

with open('src/images.h', 'w') as f:
    f.writelines(images_h)

with open('res/slides.json') as f:
    slides = json.load(f)

slides_h = []

slides_h.append('#include "images.h"\n')
slides_h.append('#include "types.h"\n')
slides_h.append('#include <gb/drawing.h>\n')
slides_h.append(f'#define NUM_SLIDES {len(slides)}\n')
for s_id, slide in enumerate(slides):
    slides_h.append(f'const slide slide_{s_id} = {{\n')
    slides_h.append(f'    .background_id={images.index(slide["background"])},\n')
    if slide["texts"]:
        if len(slide["texts"]) > 10:
            raise IndexError
        slides_h.append(f'    .texts={{\n')
        for t_id, text in enumerate(slide["texts"]):
            slides_h.append(f'        {{{text["x"]}, {text["y"]}, "{text["text"]}", {text["color"].upper()}, {text["back"].upper()}, {"TRUE" if text["animated"] else "FALSE"} }},\n')
        slides_h.append('    },\n')
    slides_h.append(f'    .num_texts={len(slide["texts"])}\n')
    slides_h.append('};\n')
slides_h.append('const slide* slides[NUM_SLIDES] = {\n')
for s_id, slide in enumerate(slides):
    slides_h.append(f'    &slide_{s_id},\n')
slides_h.append('};\n')

with open('src/slides.h', 'w') as f:
    f.writelines(slides_h)
