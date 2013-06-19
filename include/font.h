#ifndef FONT_H
#define FONT_H

/*********************************************************
 * Font-Module                                           *
 * -----------                                           *
 *                                                       *
 * Peter trys to write nice code. Really.                *
 *                                                       *
 *********************************************************/

#include <SDL.h>
#include <SDL_ttf.h>
#include <GL/gl.h>
#include <GL/glu.h>

#include <types.h>
#include <List.h>

#define MAX_NAMELENGTH 20
#define STANDART_FCOLOR {1.0, 1.0, 1.0, 0.0}
#define STANDART_BGCOLOR {0.0, 0.0, 0.0, 0.0}



struct fontEntry
{
    char name[MAX_NAMELENGTH];
    int ptsize;
    TTF_Font *font;
};

struct patternFont
{
    List<struct patternEntry> *patterns;
    int numofPatterns;
    char name[MAX_NAMELENGTH];
};

struct patternEntry
{
  char character;
  SDL_Surface *pattern;
};


class Font
{
    public:
        Font();
        ~Font();

        bool loadTTF(char *font,char *name, int ptsize);
        bool unloadTTF(char *name);

        GLuint surftotex(SDL_Surface *surf);

        GLuint atotex(char *text, char *fontname);
        GLuint atotex(char *text, char *fontname, SDL_Color fontcolor);
        GLuint atotex(char *text, char *fontname, SDL_Color fontcolor, SDL_Color backgroundcolor);

        SDL_Surface *atosurf(char *text, char *fontname);
        SDL_Surface *atosurf(char *text, char *fontname, SDL_Color fontcolor);
        SDL_Surface *atosurf(char *text, char *fontname, SDL_Color fontcolor, SDL_Color backgroundcolor);



        List<struct fontEntry> *TrueTypeFonts;          // The list of loaded truetype-fonts
        List<struct patternFont> *PatternFonts;         // The list of loaded pattern-fonts
};

void TTFListCleaner(struct fontEntry *element);
void PatternFontListCleaner(struct patternFont *element);

#endif
