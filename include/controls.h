#ifndef CONTROLS_H
#define CONTROLS_H

#include <SDL.h>
#include <camera.h>



class Controls
{
    public:
        Controls(SDL* window);

        void controls_handler(Camera *cam);

        static void endprogramm(SDL_Event *e);
        static void haldeKeydown(SDL_Event *e);
        static void haldeKeyup(SDL_Event *e);
        static void haldeMouse(SDL_Event *e);
        static void toggle_printFPS(SDL_Event *e);
        static void moveCube(SDL_Event *e);
        static void rotateCube(SDL_Event *e);

    private:
        void rotation_handler(Camera *cam);
        void move_handler(Camera *cam);


        static bool right;
        static bool left;
        static bool up;
        static bool down;

        static bool move_right;
        static bool move_left;
        static bool move_foreward;
        static bool move_backward;

        static bool already_initialized;
};



#endif
