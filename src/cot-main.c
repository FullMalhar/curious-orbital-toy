// Curious Orbital Toy
// Malhar Palkar
#include <curious-orbital-toy.h>

#include <SFML/Window.h>

// Array of orbital bodies
body_t scene[COT_BODYCOUNT];

int main(int argc, char **argv)
{
    sfContextSettings sfCS;
    sfVideoMode sfVM;
    sfWindow *pWindow;

    sfVM.height = 600;
    sfVM.width = 800;

    pWindow = sfWindow_create(sfVM, "Curious Orbital Toy", sfDefaultStyle, &sfCS);

    while (sfWindow_isOpen(pWindow))
    {
        sfEvent sEv;

        while (sfWindow_pollEvent(pWindow, &sEv))
        {
            if (sEv.type == sfEvtClosed)
                sfWindow_close(pWindow);
        }
    }

    return 0;
}