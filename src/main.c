#include <libdragon.h>
#include <math.h>

int main(void) {
    display_init(RESOLUTION_320x240, DEPTH_16_BPP, 2, GAMMA_NONE, ANTIALIAS_RESAMPLE);
    dfs_init(DFS_DEFAULT_LOCATION);
    rdpq_init();
    joypad_init();

    sprite_t *skull = sprite_load("rom:/deadbeat.sprite");

    float pos_x = 160.0f;
    float pos_y = 120.0f;
    float scale = 1.0f;
    float time = 0.0f;

    while(1) {
        time += 0.05f;

        joypad_poll();
        joypad_inputs_t input = joypad_get_inputs(JOYPAD_PORT_1);
        joypad_buttons_t held = joypad_get_buttons_held(JOYPAD_PORT_1);

        // Analog stick moves sprite position
        pos_x += input.stick_x * 0.05f;
        pos_y -= input.stick_y * 0.05f; 

        // C-Buttons control manual zoom scale
        if (held.c_up)   scale += 0.02f;
        if (held.c_down) scale -= 0.02f;
        if (scale < 0.2f) scale = 0.2f;

        // B Button toggles rapid strobe glow
        uint8_t glow = 255;
        if (held.b) {
            glow = (uint8_t)(120 + 135 * sinf(time * 12.0f));
        }

        surface_t *disp = display_get();
        rdpq_attach(disp, NULL);
        rdpq_clear(RGBA32(0, 0, 0, 255));

        if (skull) {
            rdpq_mode_begin();
                rdpq_mode_blender(RDPQ_BLENDER_MULTIPLY);
                rdpq_mode_combiner(RDPQ_COMBINER1((TEX0, 0, PRIM, 0), (TEX0, 0, PRIM, 0)));
            rdpq_mode_end();

            rdpq_set_prim_color(RGBA32(glow, glow, 255, 255));

            // A Button toggles horizontal wave distortion
            float render_scale_x = scale;
            if (held.a) {
                render_scale_x += 0.15f * sinf(time * 5.0f);
            }

            rdpq_sprite_blit(skull, pos_x, pos_y, &(rdpq_blitparms_t){
                .cx = skull->width / 2.0f,
                .cy = skull->height / 2.0f,
                .scale_x = render_scale_x,
                .scale_y = scale,
            });
        }

        rdpq_detach_show();
    }
}