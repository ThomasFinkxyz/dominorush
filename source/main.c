#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

// Include the main libnx system header, for Switch development
#include <switch.h>

const int blockdimension = 45; //size of the blocks 45 x 45 pixels
const int wtoboard = 415;  //x co ordinate where the board begins
const int rowtotal = 16;   // standard size of 16 rows 10 columns
const int coltotal = 10;

int field[16][10] = {{0,0,0,0,0,0,0,0,0,0},
					 {0,0,0,0,0,0,0,0,0,0},
					 {0,0,0,0,0,0,0,0,0,0},
					 {0,0,0,0,0,0,0,0,0,0},
					 {0,0,0,0,0,0,0,0,0,0},
					 {0,0,0,0,0,0,0,0,0,0},
				 	 {0,0,0,0,0,0,0,0,0,0},
				 	 {0,0,0,0,0,0,0,0,0,0},
				 	 {0,0,0,0,0,0,0,0,0,0},
				 	 {0,0,0,0,0,0,0,0,0,0},
				 	 {0,0,0,0,0,0,0,0,0,0},
				 	 {0,0,0,0,0,0,0,0,0,0},
				 	 {0,0,0,0,0,0,0,0,0,0},
				 	 {0,1,1,0,0,0,0,0,0,0},
				 	 {0,1,1,0,1,0,0,0,0,0},
				 	 {0,0,1,1,1,1,1,1,1,1}};


struct blockTetro{
	int shape[4][4];
	int row;   //row and col of top left block
	int col;
};

//struct blockTetro* newTetro(int r, int c){
	//struct blockTetro* blockpointer = (struct blockTetro*)malloc(sizeof(struct blockTetro));
//	block->row = r;
//	block->col = c;
//	int blockarray[4][4] = {{0,1,1,0},
//                  		{0,1,1,0},
//                  		{0,0,0,0},
//                  		{0,0,0,0}};
//	(*block).shape = blockarray;
//	struct blockTetro block = {{{0,1,1,0},
//                  		{0,1,1,0},
//                  		{0,0,0,0},
//                  		{0,0,0,0}},r,c};
//	blockpointer = &block;
//	return blockpointer;
//};

void drawBlock(SDL_Renderer *r, SDL_Texture *t,int y,int x){
	SDL_Rect destR;
	destR.y = y;
	destR.x = x;
	destR.w = blockdimension;   //size of blocks is 45 X 45
	destR.h = blockdimension;
	SDL_RenderCopy(r,t,NULL,&destR);
};

// Main program entrypoint
int main(int argc, char* argv[]){
	//initialize libraries
	SDL_Init(SDL_INIT_EVERYTHING);
	IMG_Init(IMG_INIT_PNG);
	romfsInit();
	TTF_Init();

	SDL_Window *window;
	SDL_Renderer *renderer;
	SDL_Surface *bgsurface;
	SDL_Texture *bgtexture;
	SDL_Texture *pbtexture;
	SDL_Surface *pbsurface;

	SDL_CreateWindowAndRenderer(0,0,SDL_WINDOW_FULLSCREEN_DESKTOP, &window, &renderer);
	bgsurface = IMG_Load("romfs:/img/background.png");
	bgtexture = SDL_CreateTextureFromSurface(renderer,bgsurface);
	SDL_FreeSurface(bgsurface);
	pbsurface = IMG_Load("romfs:/img/pinkblock.png");
	pbtexture = SDL_CreateTextureFromSurface(renderer,pbsurface);
	SDL_FreeSurface(pbsurface);

	struct blockTetro tetromino = {{{0,1,1,0},
                         			{0,1,1,0},
                         			{0,0,0,0},
                         			{0,0,0,0}} ,0,0};

    // Main loop
    while (appletMainLoop()){
        // Scan all the inputs. This should be done once for each frame
        hidScanInput();

        // hidKeysDown returns information about which buttons have been
        // just pressed in this frame compared to the previous one
        u64 kDown = hidKeysDown(CONTROLLER_P1_AUTO);

        if (kDown & KEY_PLUS)
            break; // break in order to return to hbmenu

		SDL_RenderClear(renderer);
		SDL_RenderCopy(renderer,bgtexture,NULL,NULL);
		for(int i =0; i<rowtotal;i++){
			for(int j = 0; j<coltotal;j++){
				if(field[i][j] != 0){
					drawBlock(renderer,pbtexture,i*blockdimension,j*blockdimension+wtoboard);
				}
			}
		}
		drawBlock(renderer,pbtexture,tetromino.row,tetromino.col+wtoboard);
		SDL_RenderPresent(renderer);

    }

	//shut everything down
	TTF_Quit();
	romfsExit();
	IMG_Quit();
	SDL_DestroyTexture(bgtexture);
	SDL_DestroyTexture(pbtexture);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
	//free(tetromino);
	return 0;
}
