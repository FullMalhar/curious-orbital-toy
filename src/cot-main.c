// Curious Orbital Toy
// Malhar Palkar
#include <curious-orbital-toy.h>

#include <time.h>

int main(int argc, char **argv)
{
    clock_t ct_start = 0, ct_end = 0;
    float dt = 0.0f;
    sfContextSettings sfCS;
    sfVideoMode sfVM;
    sfRenderWindow *pWindow;

    // Array of orbital bodies
    body_t scene[COT_BODYCOUNT];
    scene[0].mass = 20.0f;
    scene[0].position.x = 100.0f;
    scene[0].position.y = 100.0f;
    scene[0].velocity.x = 0.01f;
    scene[0].velocity.y = 0.01f;
    scene[1].mass = 10.0f;
    scene[1].position.x = 200.0f;
    scene[1].position.y = 200.0f;
    scene[1].velocity.x = 1.0f;
    scene[1].velocity.y = 1.0f;


    // Create window
    sfCS.depthBits = 0;
    sfCS.majorVersion = 4;
    sfCS.minorVersion = 5;
    sfCS.sRgbCapable = sfTrue;
    sfCS.stencilBits = 0;
    sfVM.height = 600;
    sfVM.width = 800;
    pWindow = sfRenderWindow_create(sfVM, "Curious Orbital Toy", sfDefaultStyle, &sfCS);

    // Initialise display objects
    cot_dsp_init();

    // Program loop
    while (sfRenderWindow_isOpen(pWindow))
    {
        sfEvent sEv;

        // Check for events
        while (sfRenderWindow_pollEvent(pWindow, &sEv))
        {
            if (sEv.type == sfEvtClosed)
            {
                sfRenderWindow_close(pWindow);
                cot_dsp_destroy();
                return 0;
            }
        }

        // Calculate delta time
        if (ct_start > 0)
        {
            ct_end = clock();
            dt = ((float)ct_end - (float)ct_start) / (float)CLOCKS_PER_SEC;
        }
        ct_start = clock();

        // Update the scene (physics engine)
        cot_phy_update(scene, dt);

        // Clear before drawing
        sfRenderWindow_clear(pWindow, sfBlack);

        // Draw every body
        cot_dsp_draw(pWindow, scene);

        // Write buffer to window
        sfRenderWindow_display(pWindow);
    }

    return 0;
}