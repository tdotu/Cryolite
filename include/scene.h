#ifndef SCENE_H
#define SCENE_H

#include <object.h>
#include <camera.h>
#include <List.h>
#include <light.h>

class Scene
{
    public:
        Scene(void);
        ~Scene(void);
        void addObject(Object *obj);
        int removeObject(Object *obj);
        void render(void);

        List<Object> *objectList;
        List<Camera> *Camlist;
        List<Lamp> *globalLamps;
        bool currentScene;
        GlobalLight *GlobalAmbience;

        int lasttick;
        int accuracy;
        float averageFPS;
        int handleCams(ListIterator<Camera> *c);

    private:
        void calculateFPS(void);

        void renderPolygones(Object *currentObject);
        void handleMotions(Object *currentObject);
        void handleRotations(Object *currentObject);
        int currenttick;
        int ticcount;
        int tickbundle;
        double sum;

};


#endif
