#ifndef CRYOLITE_SDL_H
#define CRYOLITE_SDL_H
#include <SDL.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include "types.h"
#include "list.h"

struct eventHandler
{
    uint8_t event;
    void (*handle)(SDL_Event *event);
};





class SDL
{
    public:
        SDL(int width, int height, int flags, const char* caption);
        ~SDL();
        void addEvent(uint8_t event, void (*handle)(SDL_Event *event));
        int  removeEvent(uint8_t event, void (*handle)(SDL_Event *event));
        void pollEvents();

    private:
        struct List *events;
        SDL_Surface *screen;
        bool lock;


};


#endif
