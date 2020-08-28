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

void fire_spread(struct texture tex, int wind)
{
        int index;
        for (int y = 1; y < tex.h; y++) {
                for (int x = 0; x < tex.w; x++) {
                        index = tex.mask[y*tex.w+x];
                        if (index == 0) {
                                tex.mask[(y-1)*tex.w+x] = 0;
                        } else {
                                int rnd = rand() % wind;
                                int dst = x - rnd + 1;
                                tex.mask[(y-1)*tex.w+dst] = index-(rnd & 1);
                        }
                }
        }
}

/* pixel colors */
struct color flame_colors[38];

void set_flame_colors()
{
        flame_colors[0] = color_rgb_int(0x07, 0x07, 0x07);
        flame_colors[1] = color_rgb_int(0x2f, 0x0f, 0x07);
        flame_colors[2] = color_rgb_int(0x1F,0x07,0x07);
        flame_colors[3] = color_rgb_int(0x2F,0x0F,0x07);
        flame_colors[4] = color_rgb_int(0x47,0x0F,0x07);
        flame_colors[5] = color_rgb_int(0x57,0x17,0x07);
        flame_colors[6] = color_rgb_int(0x67,0x1F,0x07);
        flame_colors[7] = color_rgb_int(0x77,0x1F,0x07);
        flame_colors[8] = color_rgb_int(0x8F,0x27,0x07);
        flame_colors[9] = color_rgb_int(0x9F,0x2F,0x07);
        flame_colors[10] = color_rgb_int(0xAF,0x3F,0x07);
        flame_colors[11] = color_rgb_int(0xBF,0x47,0x07);
        flame_colors[12] = color_rgb_int(0xC7,0x47,0x07);
        flame_colors[13] = color_rgb_int(0xDF,0x4F,0x07);
        flame_colors[14] = color_rgb_int(0xDF,0x57,0x07);
        flame_colors[15] = color_rgb_int(0xDF,0x57,0x07);
        flame_colors[16] = color_rgb_int(0xD7,0x5F,0x07);
        flame_colors[17] = color_rgb_int(0xD7,0x5F,0x07);
        flame_colors[18] = color_rgb_int(0xD7,0x67,0x0F);
        flame_colors[19] = color_rgb_int(0xCF,0x6F,0x0F);
        flame_colors[20] = color_rgb_int(0xCF,0x77,0x0F);
        flame_colors[21] = color_rgb_int(0xCF,0x7F,0x0F);
        flame_colors[22] = color_rgb_int(0xCF,0x87,0x17);
        flame_colors[23] = color_rgb_int(0xC7,0x87,0x17);
        flame_colors[24] = color_rgb_int(0xC7,0x8F,0x17);
        flame_colors[25] = color_rgb_int(0xC7,0x97,0x1F);
        flame_colors[26] = color_rgb_int(0xBF,0x9F,0x1F);
        flame_colors[27] = color_rgb_int(0xBF,0x9F,0x1F);
        flame_colors[28] = color_rgb_int(0xBF,0xA7,0x27);
        flame_colors[29] = color_rgb_int(0xBF,0xA7,0x27);
        flame_colors[30] = color_rgb_int(0xBF,0xAF,0x2F);
        flame_colors[31] = color_rgb_int(0xB7,0xAF,0x2F);
        flame_colors[32] = color_rgb_int(0xB7,0xB7,0x2F);
        flame_colors[33] = color_rgb_int(0xB7,0xB7,0x37);
        flame_colors[34] = color_rgb_int(0xCF,0xCF,0x6F);
        flame_colors[35] = color_rgb_int(0xDF,0xDF,0x9F);
        flame_colors[36] = color_rgb_int(0xEF,0xEF,0xC7);
        flame_colors[37] = color_rgb_int(0xFF,0xFF,0xFF);
}

void setup_palette(struct palette *pal)
{
        for (int x = 0; x < pal->size; x++) {
                palette_add_color(pal, flame_colors[x]);
                //struct color col = palette_get_by_index(pal, x);
                //printf("New color: %f, %f, %f\n", col.r, col.g, col.b);
        }
}

       

#define SRC_W 800
#define SRC_H 600
#define RES_W 200
#define RES_H 150

int main(int argc, char *argv[])
{
        arg_init(argc, argv);

        int wind = 3;
        char *wind_chk = arg_get(arg_check("--wind")+1);
        if (wind_chk) {
                wind = strtol(wind_chk, NULL, 10);
        }

        if (wind == 0 || wind == 1 || wind == -1) {wind = 3;}

        // init memory
        mem_init(100 * MEM_MEGABYTE);

        // init random number generator
        srand(0xdeadbeef);

        struct palette flames = palette(38);
        set_flame_colors();
        setup_palette(&flames);

        // set up texture
        struct texture tex = texture(RES_W, RES_H);
        tex.palette = flames;

        // set up display
        renderer_init("Flames", SRC_W, SRC_H, RES_W, RES_H);
        struct canvas display = renderer_get_window_canvas();
        struct color BLACK = color_rgb(0.0, 0.0, 0.0);
        canvas_fill(display, BLACK);
        renderer_update_display();

        // set bottom line of texture, this generates 'flames'
        for (int x = 0; x < tex.w; x++) {
                tex.mask[(tex.h-1)*tex.w+x] = 37;
        }

        int check = palette_check_color(flames, color_rgb_int(0xff,0xff,0xff));
        printf("Color check: %d\n", check);

        int running = 1;
        struct timer timer = timer_init(60);
        while(running) {

                fire_spread(tex, wind);

                texture_blit_to_canvas(tex, 0, 0, RES_W, RES_H, display, 0, 0, BLIT_ABS);
                renderer_update_display();

                running = check_quit();

                timer_tic(&timer);
        }

        mem_destroy();
        return 0;
}
