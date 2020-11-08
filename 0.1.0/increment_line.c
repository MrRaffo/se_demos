#include <stdio.h>
#include <stdlib.h>

#include <smallengine/se_sys.h>
#include <smallengine/se_maths.h>
#include <smallengine/se_graphics.h>

#include <SDL2/SDL.h>

int check_quit()
{
        SDL_Event e;

        while (SDL_PollEvent(&e)) {
                if (e.type == SDL_QUIT) {
                        return 0;
                } else if (e.type == SDL_KEYDOWN) {
                        
                        switch (e.key.keysym.sym) {
                        case SDLK_ESCAPE:
                                return 0;
                                break;
                        default:
                                break;
                        }
                }
        }

        return 1;
}

void incremental_line(struct canvas c, int startx, int starty, int endx, int endy, struct color color)
{
        int x;        // x-coord counter
        double dx, dy, m, y;

        dy = endy - starty;
        dx = endx - startx;
        m = dy/dx;
        y = (double)starty;

        for (x = startx; x < endx; x++) {
                canvas_write_pixel(c, x, (int)y, color, 0);
                y = y + m;
        }

        return;
}

#define SRC_W 800
#define SRC_H 600
#define RES_W 200
#define RES_H 150

int main(int argc, char *argv[])
{
        arg_init(argc, argv);

        // allocate memory
        mem_init(100 * MEM_MEGABYTE);

        // start and end points of the line
        int sx = 50, sy = 40, ex = 150, ey = 110; 

        char *startx = arg_get(arg_check("--startx") + 1);
        char *starty = arg_get(arg_check("--starty") + 1);
        char *endx = arg_get(arg_check("--endx") + 1);
        char *endy = arg_get(arg_check("--endy") + 1);

        printf("ARG CHECK: %d\n", arg_check("--startx"));
        printf("TEST: %s\n", startx);

        if (startx) { 
                sx = atoi(startx);
        }

        if (starty) { 
                sy = atoi(starty);
        }

        if (endx) { 
                ex = atoi(endx);
        }         

        if (endy) { 
                ey = atoi(endy);
        }

        // initialise render target
        renderer_init("Incremental Scan-conversion Line", SRC_W, SRC_H, RES_W, RES_H);
        struct canvas display = renderer_get_window_canvas();
        struct color black = color_rgb(0.0, 0.0, 0.0);
        struct color white = color_rgb(1.0, 1.0, 1.0);
        canvas_fill(display, black);
        renderer_update_display();

        // 60 fps loop
        int running = 1;
        struct timer timer = timer_init(60);
        while(running) {
                
                incremental_line(display, sx, sy, ex, ey, white);
                renderer_update_display();

                running = check_quit();

                timer_tic(&timer);
        }

        mem_destroy();

        return 0;
}
                
