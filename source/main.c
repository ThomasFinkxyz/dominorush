#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

// Include the main libnx system header, for Switch development
#include <switch.h>

int[][] field = [[0,0,0,0,0,0,0,0,0,0],
				 [0,0,0,0,0,0,0,0,0,0],
				 [0,0,0,0,0,0,0,0,0,0],
				 [0,0,0,0,0,0,0,0,0,0],
				 [0,0,0,0,0,0,0,0,0,0],
				 [0,0,0,0,0,0,0,0,0,0],
				 [0,0,0,0,0,0,0,0,0,0],
				 [0,0,0,0,0,0,0,0,0,0],
				 [0,0,0,0,0,0,0,0,0,0],
				 [0,0,0,0,0,0,0,0,0,0],
				 [0,0,0,0,0,0,0,0,0,0],
				 [0,0,0,0,0,0,0,0,0,0],
				 [0,0,0,0,0,0,0,0,0,0],
				 [0,0,0,0,0,0,0,0,0,0],
				 [0,0,0,0,0,0,0,0,0,0],
				 [0,0,0,0,0,0,0,0,0,0]];

struct blockTetro{
	int[][] shape = [[0,1,1,0],
					 [0,1,1,0],
					 [0,0,0,0],
					 [0,0,0,0]]
	int row;   //row and col of top left block
	int col;
}

// Main program entrypoint
int main(int argc, char* argv[])
{
	//initialize libraries
	SDL_Init(SDL_INIT_EVERYTHING);
	IMG_Init(IMG_INIT_PNG);
	romfsInit();
	TTF_Init();

	SDL_Window *window;
	SDL_Renderer *renderer;
	SDL_Surface *bgsurface;
	SDL_Texture *bgtexture;

	SDL_CreateWindowAndRenderer(0,0,SDL_WINDOW_FULLSCREEN_DESKTOP, &window, &renderer);
	bgsurface = IMG_Load("romfs:/img/background.png");
	bgtexture = SDL_CreateTextureFromSurface(renderer,bgsurface);
	SDL_FreeSurface(bgsurface);

    // Main loop
    while (appletMainLoop())
    {
        // Scan all the inputs. This should be done once for each frame
        hidScanInput();

        // hidKeysDown returns information about which buttons have been
        // just pressed in this frame compared to the previous one
        u64 kDown = hidKeysDown(CONTROLLER_P1_AUTO);

        if (kDown & KEY_PLUS)
            break; // break in order to return to hbmenu
		SDL_RenderClear(renderer);
		SDL_RenderCopy(renderer,bgtexture,NULL,NULL);
		SDL_RenderPresent(renderer);

    }

	//shut everything down
	TTF_Quit();
	romfsExit();
	IMG_Quit();
	SDL_DestroyTexture(bgtexture);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
	return 0;
}
