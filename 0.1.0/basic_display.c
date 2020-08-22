#include <stdio.h>
#include <stdlib.h>     // for strtol
#include <unistd.h>     // for sleep

#include <smallengine/se_sys.h>
#include <smallengine/se_maths.h>
#include <smallengine/se_graphics.h>

/*
 * basic_display
 * a test program for smallengine 0.1.0, it creates a window and displays a
 * colour for the number of seconds given as an argument, if no argument is
 * given, it will display for 5 seconds
 */

int main(int argc, char **argv)
{
        mem_init(50 * MEM_MEGABYTE);
        int disp_time = 5;

        if (argc >= 2) {
                disp_time = strtol(argv[1], NULL, 10);
                if (disp_time <= 0 || disp_time > 100) {
                        disp_time = 5;
                }
        }

        printf("Display time: %d\n", disp_time);

        struct color red = color_rgb_int(200, 16, 46);

        renderer_init("Basic Display", 640, 320, 640, 320);
        struct canvas canvas = renderer_get_window_canvas();
        canvas_fill(canvas, red);

        renderer_update_display();

        sleep(disp_time);

        renderer_quit();
        
        mem_destroy();
}
