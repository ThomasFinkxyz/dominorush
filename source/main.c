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
const int tetrohw = 4;
int fallms = 500;
typedef enum {left,right}leftorright;

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

int blockshape [4][4] = {{0,1,1,0},
						 {0,1,1,0},
     					 {0,0,0,0},
     					 {0,0,0,0}};

int TstraightUp [4][4] = {{0,0,0,0},
					 	  {0,1,1,1},
					 	  {0,0,1,0},
					 	  {0,0,0,0}};

int TrightTilt [4][4] = {{0,0,1,0},
					 	 {0,1,1,0},
					 	 {0,0,1,0},
					 	 {0,0,0,0}};

int TupsideDown [4][4] = {{0,0,1,0},
					 	  {0,1,1,1},
					 	  {0,0,0,0},
					 	  {0,0,0,0}};

int TleftTilt [4][4] = {{0,0,1,0},
					 	{0,0,1,1},
					 	{0,0,1,0},
					 	{0,0,0,0}};

int IstraightUp [4][4] = {{0,0,1,0},
						 {0,0,1,0},
						 {0,0,1,0},
						 {0,0,1,0}};

int Isideways [4][4] = {{0,0,0,0},
					    {0,0,0,0},
						{1,1,1,1},
						{0,0,0,0}};

int JstraightUp [4][4] = {{0,0,1,0},
						  {0,0,1,0},
						  {0,1,1,0},
						  {0,0,0,0}};

int JrightTilt [4][4] = {{0,1,0,0},
						 {0,1,1,1},
						 {0,0,0,0},
						 {0,0,0,0}};

int JupsideDown [4][4] = {{0,0,1,1},
						  {0,0,1,0},
						  {0,0,1,0},
						  {0,0,0,0}};

int JleftTilt [4][4] = {{0,0,0,0},
						 {0,1,1,1},
						 {0,0,0,1},
						 {0,0,0,0}};

int LstraightUp [4][4] = {{0,0,1,0},
						  {0,0,1,0},
						  {0,0,1,1},
						  {0,0,0,0}};

int LrightTilt [4][4] = {{0,0,0,0},
						 {0,1,1,1},
						 {0,1,0,0},
						 {0,0,0,0}};

int LupsideDown [4][4] = {{0,1,1,0},
						  {0,0,1,0},
						  {0,0,1,0},
						  {0,0,0,0}};

int LleftTilt [4][4] = {{0,0,0,1},
						{0,1,1,1},
						{0,0,0,0},
						{0,0,0,0}};

int SstraightUp [4][4] = {{0,0,1,0},
						  {0,0,1,1},
						  {0,0,0,1},
						  {0,0,0,0}};

int Ssideways [4][4] = {{0,0,0,0},
						{0,0,1,1},
						{0,1,1,0},
						{0,0,0,0}};

int ZstraightUp [4][4] = {{0,0,0,1},
						  {0,0,1,1},
						  {0,0,1,0},
						  {0,0,0,0}};

int ZstraightSideways [4][4] = {{0,0,0,0},
						  		{0,0,0,0},
						  		{0,1,1,0},
						  		{0,0,1,1}};

struct tetro{
	int (*shape) [4][4];
	int row;   //row and col of top left block
	int col;
	int potentialrow; //collision checking
	int potentialcol;
	enum {o,t,i,l,j,s,z}type;
};

struct tetro* newTetro(int r, int c, int ty){
	struct tetro* tetropointer = (struct tetro*)malloc(sizeof(struct tetro));
	tetropointer->row = r;
	tetropointer->col = c;
	tetropointer->type = ty;
	switch(tetropointer->type){
		case o:
			tetropointer->shape = &blockshape;
			break;
		case i:
			tetropointer->shape = &IstraightUp;
			break;
		default:
			tetropointer->shape = &TstraightUp;
	}
	return tetropointer;
};

void drawBlock(SDL_Renderer *r, SDL_Texture *t,int y,int x){
	SDL_Rect destR;
	destR.y = y;
	destR.x = x;
	destR.w = blockdimension;   //size of blocks is 45 X 45
	destR.h = blockdimension;
	SDL_RenderCopy(r,t,NULL,&destR);
};

bool tetroDown(struct tetro * tetromino){
	tetromino->potentialrow = tetromino->row+1;
	tetromino->potentialcol = tetromino->col;
	bool cantmove = false;
	if(tetromino->potentialrow > 15){
		cantmove = true;
	}
	if(!cantmove){
		for(int i = 0; i<tetrohw;i++){
			if(cantmove)
				break;
			for(int j = 0; j<tetrohw;j++){
				if(blockshape[i][j] != 0){
					if(field[i+tetromino->potentialrow][j+tetromino->potentialcol] != 0){
						cantmove = true;
						break;
					}
				}
			}
		}
	}
	if(cantmove){
		for(int i = 0; i<tetrohw;i++){
			for(int j = 0; j<tetrohw;j++){
				if(blockshape[i][j] != 0){
					if(i+tetromino->row < 16 && j+tetromino->col <10){
					field[i+tetromino->row][j+tetromino->col] = blockshape[i][j];
					}
				}
			}
		}
		return true;
	} else{
		tetromino->row = tetromino->potentialrow;
		tetromino->col = tetromino->potentialcol;
	return false;
	}
}

void tetroLeftRight(struct tetro* tetromino, leftorright direction){
	switch(direction){
		case left:
			tetromino->potentialcol = tetromino->col-1;
			if(tetromino->potentialcol >= 0)
				tetromino->col = tetromino->potentialcol;
			break;
		case right:
			tetromino->potentialcol = tetromino->col+1;
			if(tetromino->potentialcol < 9)
				tetromino->col = tetromino->potentialcol;
			break;
	}
}

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
	struct tetro* tetroptr;
	tetroptr = newTetro(0,4,1);



	Uint32 currentMs = SDL_GetTicks();
	//Uint32 lastMs = currentMs;
	//float delta = 0.0;
	//float fps = 0.0;
	Uint32 lastFell = SDL_GetTicks();
    // Main loop

	while (appletMainLoop()){
		// Scan all the inputs. This should be done once for each frame
		currentMs = SDL_GetTicks();
		//delta = (currentMs-lastMs)/1000;
		//fps = delta*1000;
		//lastMs = currentMs;
		if(currentMs - lastFell > fallms){
			bool hitground = tetroDown(tetroptr);
			lastFell = SDL_GetTicks();
			if(hitground){
				free(tetroptr);
				tetroptr = newTetro(3,3,1);
			}
		}
        hidScanInput();

        // hidKeysDown returns information about which buttons have been
        // just pressed in this frame compared to the previous one
        u64 kDown = hidKeysDown(CONTROLLER_P1_AUTO);

        if (kDown & KEY_PLUS)
            break; // break in order to return to hbmenu

		switch(kDown){
			case KEY_LSTICK_RIGHT:
				tetroLeftRight(tetroptr,right);
				break;
			case KEY_LSTICK_LEFT:
				tetroLeftRight(tetroptr,left);
				break;
		}

		SDL_RenderClear(renderer);
		SDL_RenderCopy(renderer,bgtexture,NULL,NULL);
		for(int i =0; i<rowtotal;i++){                 //Draw grounded blocks
			for(int j = 0; j<coltotal;j++){
				if(field[i][j] != 0){
					drawBlock(renderer,pbtexture,i*blockdimension,j*blockdimension+wtoboard);
				}
			}
		}
		for(int i = 0; i<tetrohw;i++){                   //Draw the currently falling tetromino
			for(int j = 0; j<tetrohw;j++){
				if(blockshape[i][j] != 0){
					drawBlock(renderer,pbtexture,(i+tetroptr->row)*blockdimension,(j+tetroptr->col)*blockdimension+wtoboard);
				}
			}
		}
		//drawBlock(renderer,pbtexture,tetromino.row,tetromino.col+wtoboard);
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
	free(tetroptr);
	return 0;
}
