#include "include/controls.h"
#include "include/sdl.h"
#include "include/general_def.h"
#include "include/object.h"
#include <math.h>

#define ANGLE 1
#define SPEED 10

bool right=false;
bool left=false;
bool up = false;
bool down = false;
bool move_right=false;
bool move_left=false;
bool move_foreward=false;
bool move_backward=false;

float move_direction_foreward=(float)SPEED/100;
float move_direction_left=0;

float position_x = STARTING_X;
float position_y = STARTING_Y;
float position_z = STARTING_Z;

extern bool printFPS;

void endprogramm(SDL_Event *event)
{
    exit(0);
}

void INIT_Controls(SDL* window)
{
    window->addEvent(SDL_QUIT,endprogramm);
    window->addEvent(SDL_KEYDOWN,haldeKeydown);
    window->addEvent(SDL_KEYUP,haldeKeyup);
    SDL_ShowCursor(SDL_DISABLE); //mausanzeige deaktivieren
    SDL_WM_GrabInput( SDL_GRAB_ON );// Maus einfangen
    window->addEvent(SDL_MOUSEMOTION,haldeMouse);
    window->addEvent(SDL_KEYDOWN,toggle_printFPS);
    window->addEvent(SDL_KEYDOWN,moveCube);
}

void haldeKeydown(SDL_Event *e)
{
    switch(e->key.keysym.sym)
    {
        case SDLK_ESCAPE:   // Exit programm if Escape is pressed.
            exit(0);
            break;
        case SDLK_w:
            move_foreward=true;
            break;
        case SDLK_s:
            move_backward=true;
            break;
        case SDLK_a:
            move_left=true;
            break;
        case SDLK_d:
            move_right=true;
            break;
        default:
            break;

    }

}


void haldeKeyup(SDL_Event *e)
{
    switch(e->key.keysym.sym)
    {
        case SDLK_w:
            move_foreward=false;
            break;
        case SDLK_s:
            move_backward=false;
            break;
        case SDLK_a:
            move_left=false;
            break;
        case SDLK_d:
            move_right=false;
            break;
        default:
            break;
    }
}

void haldeMouse(SDL_Event *e)
{
    if(e->motion.xrel>0)
    {
        right=true;
    }
    else
    {
        left=true;
    }
/*
    if(e->motion.yrel>0)
    {
        up = true;
    }
    else
    {
        down = true;
    }
*/
}

void move_camera(GLfloat move_x, GLfloat move_y, GLfloat move_z)
{
    position_x += move_x;
    position_y += move_y;
    position_z += move_z;

    glTranslatef(move_x, move_y, move_z);
}

void rotate_correctly(GLfloat angle, GLfloat rotate_x, GLfloat rotate_y, GLfloat rotate_z)
{
    glTranslatef(-position_x, -position_y, -position_z);
    glRotatef(angle, rotate_x, rotate_y, rotate_z);
    glTranslatef(position_x, position_y, position_z);
}

void rotation_handler(){    // Rotates the object if a key is pressed.
    if(right)
    {
        rotate_correctly(ANGLE,0,1,0);

        if(move_direction_foreward <= 0)
            move_direction_left += (float)1/((9000/SPEED)*(1.0f/ANGLE));
        else
            move_direction_left -=  (float)1/((9000/SPEED)*(1.0f/ANGLE));

        if(move_direction_left <= 0)
            move_direction_foreward -= (float)1/((9000/SPEED)*(1.0f/ANGLE));
        else
            move_direction_foreward += (float)1/((9000/SPEED)*(1.0f/ANGLE));

        right = false;

    }

    if(left)
    {
        rotate_correctly(-ANGLE,0,1,0);

        if(move_direction_foreward >= 0)
            move_direction_left += (float)1/((9000/SPEED)*(1.0f/ANGLE));
        else
            move_direction_left -=  (float)1/((9000/SPEED)*(1.0f/ANGLE));

        if(move_direction_left >= 0)
            move_direction_foreward -= (float)1/((9000/SPEED)*(1.0f/ANGLE));
        else
            move_direction_foreward += (float)1/((9000/SPEED)*(1.0f/ANGLE));

        left = false;
    }

}

void move_handler(){        // Moves the object if a key is pressed
    if(move_right)
        move_camera(-move_direction_foreward,0,move_direction_left);
    if(move_left)
        move_camera(move_direction_foreward,0,-move_direction_left);
    if(move_foreward)
        move_camera(move_direction_left,0,move_direction_foreward);
    if(move_backward)
        move_camera(-move_direction_left,0,-move_direction_foreward);
}

void toggle_printFPS(SDL_Event *e)
{
    if(e->key.keysym.sym == SDLK_f)
    {
        if(printFPS)
            printFPS = false;
        else
            printFPS = true;
    }

    return;
}
extern Object *iccube;

void moveCube(SDL_Event *e)
{
    if(e->key.keysym.sym == SDLK_e)
    {
        struct vector3D direction = {0,0,30};
        iccube->moveObject(1,0.002,direction);
    }

    return;
}
