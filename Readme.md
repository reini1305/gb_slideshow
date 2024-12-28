
### Create Slide Shows as a Gameboy ROM

Images placed in the res/ folder will be automatically converted into C code and compiled into the viewer.

![slideshow in action](slideshow.gif)

Slides can be defined as a json, referencing the images as background. Text can be overlayed with configurable colors and with or without a typewriter effect:

~~~json
{
        "background": "xt",
        "texts": [
            {
                "x": 7,
                "y": 4,
                "animated": true,
                "text": "Hello",
                "color": "white",
                "back": "black"
            },
            {
                "x": 7,
                "y": 5,
                "animated": true,
                "text": "World",
                "color": "white",
                "back": "black"
            }
        ]
    },
    {
        "background": "fesch",
        "texts": [ ]
    }
~~~

Pressing the A or B button goes to the next/previous text/slide. START to return to first slide.

Pixel art originally by RodrixAP under Creative Commons Attribution 2.0 Generic (CC BY 2.0)

https://www.flickr.com/photos/rodrixap/10591266994/in/album-72157637154901153/

TODOs:
-
- blank screen (press start?)
- blank screen at the end
- sprites for overlays?