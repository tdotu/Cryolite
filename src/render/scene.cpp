#include <GL/gl.h>
#include <GL/glu.h>
#include <math.h>
#include <SDL.h>
#include <general_def.h>
#include <scene.h>
#include <object.h>


Scene::Scene()
{
    this->objectList = new List<Object>;
    this->Camlist = new List<Camera>;
    this->GlobalAmbience = NULL;
    currenttick = 0;
    ticcount = 0;
    tickbundle = 3;
    sum = 0;
    lasttick = SDL_GetTicks();
    accuracy = 60;
    averageFPS = 60;

}

Scene::~Scene()
{
    this->objectList->ListSetFirst();
    while(!this->objectList->ListIsLast())
    {
        this->objectList->ListRemove();
        this->objectList->ListNext();
    }
    delete this->objectList;
}


void Scene::addObject(Object *obj)
{
    this->objectList->ListPushFront(obj);
}

int Scene::removeObject(char *name)
{
    int delObj = 0;
    this->objectList->ListSetFirst();
    while(!this->objectList->ListIsLast())
    {
        Object *currentObject = (Object *)this->objectList->ListGetCurrent();
        if(!strncmp(currentObject->objectname,name,20))
        {
            this->objectList->ListRemove();
            delObj++;
        }
        this->objectList->ListNext();
    }
    return delObj;
}

int Scene::removeObject(Object *obj)
{
    int delObj = 0;
    this->objectList->ListSetFirst();
    while(!this->objectList->ListIsLast())
    {
        if(this->objectList->ListGetCurrent() == obj)
        {
            this->objectList->ListRemove();
            delObj++;
        }
        this->objectList->ListNext();
    }
    return delObj;
}


void Scene::render()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    if(GlobalAmbience && !currentScene)
        GlobalAmbience->activateLight();

    else if(!GlobalAmbience)
    {
        GLboolean activlight;
        glGetBooleanv(GL_LIGHTING, &activlight);
        if(activlight)
            glDisable(GL_LIGHTING);
    }


    calculateFPS();
    this->Camlist->ListSetFirst();
    while(handleCams())
    {
        if(!objectList->ListIsEmpty())
        {
            glMatrixMode(GL_MODELVIEW);
            objectList->ListSetFirst();

            while(!objectList->ListIsLast())
            {
                Object *currentObject = (Object *)objectList->ListGetCurrent();
                if(!currentObject->vertices->ListIsEmpty())
                {
                    glPushMatrix();

                        glEnable(GL_TEXTURE_2D);

                        if(currentObject->ObjectMaterial)
                            glBindTexture( GL_TEXTURE_2D, currentObject->ObjectMaterial->textureGL);
                        else
                            glBindTexture( GL_TEXTURE_2D, 0);

                        //Modify Model position
                        handleMotions(currentObject);
                        handleRotations(currentObject);

                        //Modify Model Matrix
                        glRotatef(currentObject->Angle, currentObject->rotationAxis.x, currentObject->rotationAxis.y, currentObject->rotationAxis.z);
                        glTranslatef(currentObject->position.x,currentObject->position.y,currentObject->position.z);
                        glScalef(currentObject->scale.x,currentObject->scale.y,currentObject->scale.z);

                        //Render Polyganes
                        if(!currentObject->polygones->ListIsEmpty())
                            renderPolygones(currentObject);


                    glPopMatrix();
                }

                objectList->ListNext();
           }
        }
    }

}



void Scene::handleMotions(Object *currentObject)
{
    if(currentObject->Am || currentObject->V0m)
    {
        vector direction = vector(currentObject->Dm);
        float acc = currentObject->Am;
        if(currentObject->position.y > -2)
        {
            acc = len( (direction * acc) + vector(0,-10,0) );
            direction = unify( (direction * acc) + vector(0,-10,0) );
        }

        else
        {
            currentObject->position.y = -2;
        }

        currentObject->V0m +=  currentObject->Am* ((SDL_GetTicks()-currentObject->Tms)/1000);
        double MPF = (double)currentObject->V0m/averageFPS;
        currentObject->position = (currentObject->position) + (direction * MPF);
    }
}

void Scene::handleRotations(Object *currentObject)
{
    if(currentObject->remeaningAngle*currentObject->remAngleSing>0)
    {
        double APF = (double)(currentObject->rotationVelocity+currentObject->rotationAcceleration*(SDL_GetTicks()-currentObject->startRotationTime))/averageFPS;
        currentObject->remeaningAngle -=APF;
        currentObject->Angle += APF;
    }
    else if(currentObject->rotationVelocity)
    {
        currentObject->Angle = currentObject->destAngle;
        currentObject->rotationVelocity = 0;
        currentObject->rotationAcceleration = 0;
        currentObject->remeaningAngle = 0;
        printf("Angle: %f\n",currentObject->Angle);
    }
}

int Scene::handleCams(void)
{

    if(!Camlist->ListIsEmpty() && !Camlist->ListIsLast())
    {
        Camera *currentCam = Camlist->ListGetCurrent();
        Camlist->ListNext();

        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();

        glViewport(currentCam->x,currentCam->y,currentCam->width,currentCam->height);
        gluPerspective(currentCam->fov, currentCam->width/currentCam->height, currentCam->nearClip, currentCam->farClip );

        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        gluLookAt(currentCam->position.x,currentCam->position.y,currentCam->position.z,currentCam->lookingDirection.x+currentCam->position.x,currentCam->lookingDirection.y+currentCam->position.y,currentCam->lookingDirection.z+currentCam->position.z,0.0,1.0,0.0);

        return true;
    }

    return false;
}


void Scene::renderPolygones(Object *currentObject)
{

        currentObject->polygones->ListSetFirst();

        while(!currentObject->polygones->ListIsLast())
        {
            Polygone *currentPolygone = (Polygone *)currentObject->polygones->ListGetCurrent();

            glBegin( GL_POLYGON );

                for(int i=0;i < currentPolygone->getVertexAmount();i++)
                {
                    if(currentObject->ObjectMaterial && currentObject->ObjectMaterial->textureGL && currentPolygone->getTexVertex(i) != NULL)
                            glTexCoord2f( currentPolygone->getTexVertex(i)->x, currentPolygone->getTexVertex(i)->y );

                    if(currentPolygone->getObjVertex(i) != NULL)
                        glVertex3f( currentPolygone->getObjVertex(i)->x, currentPolygone->getObjVertex(i)->y, currentPolygone->getObjVertex(i)->z);
                }

            glEnd();

            currentObject->polygones->ListNext();
        }
}

void Scene::calculateFPS(void)
{
    currenttick = SDL_GetTicks();
    if(ticcount==tickbundle)
    {
        averageFPS = sum/tickbundle;
        sum = 0;
        ticcount = 0;
        tickbundle = accuracy;
    }
    else
    {
        if(currenttick-lasttick > 0)
            sum +=(1000.0/(float)(currenttick-lasttick));
            ticcount++;
    }
    lasttick = currenttick;
}
