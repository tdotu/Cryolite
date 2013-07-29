#include <texture.h>


using namespace std;



Texture::Texture()
{
    initTexture();
}


Texture::Texture(Texture *templateTexture)
{
    initTexture();

    nr = templateTexture->nr;
    strncpy(file, templateTexture->file, MAX_IMAGE_FILENAME_LENGTH);
}


Texture::Texture(const char *filename)
{
    initTexture();

    strncpy(file, filename, MAX_IMAGE_FILENAME_LENGTH);

    nr = loadTexture(filename);
}


Texture::~Texture()
{
    glDeleteTextures(1, &nr);
}


void Texture::initTexture()
{
    nr = 0;
    memset(file, '\0', sizeof(file));
}


GLuint Texture::loadTexture(const char *filename)
{
    GLint nOfColors = 0;
    GLenum texture_format = 0;
    SDL_Surface *SDL_Texture = IMG_Load(filename);
    GLuint GL_Texture = 0;

    if(SDL_Texture == NULL)
    {
        printf("textur %s not found\n",filename);
        return 0;
    }


    nOfColors = SDL_Texture->format->BytesPerPixel;

    if (nOfColors == 4)
    {
        if (SDL_Texture->format->Rmask == 0x000000ff)
            texture_format = GL_RGBA;
        else
            texture_format = GL_BGRA;
    }

    else if (nOfColors == 3)
    {
        if (SDL_Texture->format->Rmask == 0x000000ff)
            texture_format = GL_RGB;
        else
            texture_format = GL_BGR;
    }
    else
    {
        printf("warning: %s is not truecolor.. this will probably break\n", filename);
    }

    glEnable(GL_TEXTURE_2D);
    glGenTextures( 1, &GL_Texture );
    glActiveTextureARB(GL_Texture);




    glBindTexture( GL_TEXTURE_2D, GL_Texture );
    glTexImage2D( GL_TEXTURE_2D, 0, nOfColors, SDL_Texture->w, SDL_Texture->h, 0,texture_format, GL_UNSIGNED_BYTE, SDL_Texture->pixels );

    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );


    cout << "texur " << GL_Texture << " " << filename << " loaded" << endl;

    return GL_Texture;
}
