// Curious Orbital Toy
// Malhar Palkar
#include <curious-orbital-toy.h>

#include <stdlib.h>

// Graphics objects with multiple uses
static const sfColor dsp_bodyColor = { .a = 100, .b = 0, .r = 255, .b = 0 };
static sfShader *dsp_shader;
static const char *dsp_shader_fragstr = "void main() { gl_FragColor = vec4(1,0,0,1); }";
static sfRenderStates dsp_rState;
static sfCircleShape *cs_bodies[COT_BODYCOUNT];

void cot_dsp_draw(sfRenderWindow *pWindow, body_t *pScene)
{
    // Loop through each body
    for (size_t i = 0; i < COT_BODYCOUNT; i++)
    {
        // Create circle to represent body
        sfCircleShape_setRadius(cs_bodies[i], (float)pScene[i].mass);
        sfCircleShape_setFillColor(cs_bodies[i], dsp_bodyColor);
        sfCircleShape_setPosition(cs_bodies[i], pScene[i].position);

        // Draw the body
        sfRenderWindow_drawCircleShape(pWindow, cs_bodies[i], &dsp_rState);
    }
}

void cot_dsp_init()
{
    // Create shaders
    dsp_shader = sfShader_createFromMemory(NULL, NULL, dsp_shader_fragstr);
    dsp_rState.shader = dsp_shader;
    dsp_rState.blendMode = sfBlendAlpha;
    dsp_rState.texture = NULL;
    dsp_rState.transform = sfTransform_Identity;

    // Create bodies
    for (size_t i = 0; i < COT_BODYCOUNT; i++)
        cs_bodies[i] = sfCircleShape_create();
}

void cot_dsp_destroy()
{
    // Destroy circle shapes
    for (size_t i = 0; i < COT_BODYCOUNT; i++)
        sfCircleShape_destroy(cs_bodies[i]);

    // Destroy shaders
    sfShader_destroy(dsp_shader);
}