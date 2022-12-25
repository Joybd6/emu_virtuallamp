#include <iostream>
#include <string>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "io.h"

const int s_width=400;
const int s_height=500;
const char app_name[]="Virtual Lamp For EMU8086";
const std::string image_path="lamp.png";
const uint16_t port=0x0FAF;



SDL_Window *gwindow=NULL;
SDL_Texture *gtexture=NULL;
SDL_Renderer *grender=NULL;
SDL_Surface *gsurface=NULL;

SDL_Texture *loadTexture(const std::string &path)
{
    SDL_Surface *surface=NULL;
    surface=IMG_Load(image_path.c_str());
    if(surface==NULL)
    {
        printf("Image Loading Failed: %s",IMG_GetError());
        return NULL;
    }

    SDL_Texture *texture=NULL;
    texture=SDL_CreateTextureFromSurface(grender,surface);
    SDL_FreeSurface(surface);
    return texture;
}

bool initializAll()
{
    if(SDL_Init(SDL_INIT_VIDEO)<0)
    {
        return false;
    }

    gwindow=SDL_CreateWindow(app_name,SDL_WINDOWPOS_UNDEFINED,SDL_WINDOWPOS_UNDEFINED,s_width,s_height,SDL_WINDOW_SHOWN);
    
    if(gwindow==NULL)
    return false;

    if(!(IMG_Init(IMG_INIT_PNG)&IMG_INIT_PNG))
    {
        return false;
    }
    grender=SDL_CreateRenderer(gwindow,-1,SDL_RENDERER_ACCELERATED);

    if(grender==NULL)
    return false;

    return true;
}

int main(int argc, char *args[])
{
    int read;
    initializAll();
    SDL_Rect rect{0,0,s_width,s_height};
    SDL_Rect *current=NULL;
    SDL_Rect clip1{0,0,388,680};
    SDL_Rect clip2{447,0,835,680};
    gtexture=loadTexture(image_path);

    SDL_Event e;
    bool quit=false;
    while(!quit)
    {
        while(SDL_PollEvent(&e)!=0)
        {
            if(e.type==SDL_QUIT)
            {
                quit=true;
            }
        }
        read=READ_IO_BYTE(port);
        current=read==0?&clip1:&clip2;
        SDL_SetRenderDrawColor(grender,255,255,235,0);
        SDL_RenderClear(grender);
        SDL_RenderCopy(grender,gtexture,current,&rect);
        SDL_RenderPresent(grender);
    }
    SDL_DestroyRenderer(grender);
    SDL_Quit();
    IMG_Quit();
    
    
    return 0;
}