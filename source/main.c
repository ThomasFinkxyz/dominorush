#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <time.h>

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
				 	 {0,0,0,0,0,0,0,0,0,0},
				 	 {0,0,0,0,0,0,0,0,0,0},
				 	 {0,0,0,0,0,0,0,0,0,0}};

int blockshape [4][4] = {{0,2,2,0},    // 1 type of O square block. 2 makes them red.
						 {0,2,2,0},
     					 {0,0,0,0},
     					 {0,0,0,0}};

int TstraightUp [4][4] = {{0,0,0,0},  // 4 types of T block. 1 makes them pink.
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

int IstraightUp [4][4] = {{0,0,3,0},        //Two type of I straight block. 3 makes them green.
						 {0,0,3,0},
						 {0,0,3,0},
						 {0,0,3,0}};

int Isideways [4][4] = {{0,0,0,0},
					    {0,0,0,0},
						{3,3,3,3},
						{0,0,0,0}};

int JstraightUp [4][4] = {{0,0,4,0},        //Four types of J and L. 4 makes J light blue. 5 makes L orange.
						  {0,0,4,0},
						  {0,4,4,0},
						  {0,0,0,0}};

int JrightTilt [4][4] = {{0,4,0,0},
						 {0,4,4,4},
						 {0,0,0,0},
						 {0,0,0,0}};

int JupsideDown [4][4] = {{0,0,4,4},
						  {0,0,4,0},
						  {0,0,4,0},
						  {0,0,0,0}};

int JleftTilt [4][4] = {{0,0,0,0},
						 {0,4,4,4},
						 {0,0,0,4},
						 {0,0,0,0}};

int LstraightUp [4][4] = {{0,0,5,0},
						  {0,0,5,0},
						  {0,0,5,5},
						  {0,0,0,0}};

int LrightTilt [4][4] = {{0,0,0,0},
						 {0,5,5,5},
						 {0,5,0,0},
						 {0,0,0,0}};

int LupsideDown [4][4] = {{0,5,5,0},
						  {0,0,5,0},
						  {0,0,5,0},
						  {0,0,0,0}};

int LleftTilt [4][4] = {{0,0,0,5},
						{0,5,5,5},
						{0,0,0,0},
						{0,0,0,0}};

int SstraightUp [4][4] = {{0,0,6,0},                   //2 types of S and Z. 6 makes S yellow. 7 makes Z purple.
						  {0,0,6,6},
						  {0,0,0,6},
						  {0,0,0,0}};

int Ssideways [4][4] = {{0,0,0,0},
						{0,0,6,6},
						{0,6,6,0},
						{0,0,0,0}};

int ZstraightUp [4][4] = {{0,0,0,7},
						  {0,0,7,7},
						  {0,0,7,0},
						  {0,0,0,0}};

int Zsideways [4][4] = {{0,0,0,0},
						  		{0,0,0,0},
						  		{0,7,7,0},
						  		{0,0,7,7}};

struct tetro{
	int (*shape) [4][4];
	int row;   //row and col of top left block
	int col;
	int potentialrow; //collision checking
	int potentialcol;
	enum {O,T,I,L,J,S,Z}type;
	int rotnumber;
};

struct tetro* newTetro(int r, int c, int ty){
	struct tetro* tetropointer = (struct tetro*)malloc(sizeof(struct tetro));
	tetropointer->row = r;
	tetropointer->col = c;
	tetropointer->rotnumber = 1;
	tetropointer->type = ty;
	switch(tetropointer->type){
		case O:
			tetropointer->shape = &blockshape;
			break;
		case I:
			tetropointer->shape = &IstraightUp;
			break;
		case T:
			tetropointer->shape = &TstraightUp;
			break;
		case L:
			tetropointer->shape = &LstraightUp;
			break;
		case J:
			tetropointer->shape = &JstraightUp;
			break;
		case S:
			tetropointer->shape =&SstraightUp;
			break;
		case Z:
			tetropointer->shape = &ZstraightUp;
			break;
		default:
			tetropointer->shape = &LstraightUp;
			break;

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

void drawText(SDL_Renderer *renderer, TTF_Font *font, int x, int y, SDL_Color color, const char *text)
{
	SDL_Surface *surface = TTF_RenderText_Blended_Wrapped(font, text, color, 1280);
	SDL_SetSurfaceAlphaMod(surface, color.a);
	SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
	SDL_FreeSurface(surface);

	SDL_Rect position;
	position.x = x; position.y = y;
	SDL_QueryTexture(texture, NULL, NULL, &position.w, &position.h);
	SDL_RenderCopy(renderer, texture, NULL, &position);
	SDL_DestroyTexture(texture);
}

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
				if((*tetromino->shape)[i][j] != 0){
					if(tetromino->potentialrow+i > 15){
						cantmove = true;
						break;
					}
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
				if((*tetromino->shape)[i][j] != 0){
					if(i+tetromino->row < 16 && j+tetromino->col <10){
					field[i+tetromino->row][j+tetromino->col] = (*tetromino->shape)[i][j];
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
	bool notallowed = false;
	switch(direction){
		case left:
			tetromino->potentialcol = tetromino->col-1;
			for(int i = 0; i<tetrohw;i++){
				if(notallowed)
					break;
				for(int j = 0; j<tetrohw;j++){
					if((*tetromino->shape)[i][j] != 0){
						if(tetromino->potentialcol+j < 0){
							//Out of bounds
							notallowed = true;
							break;
						}
						else if(field[tetromino->row+i][tetromino->potentialcol+j] != 0){
							//Space taken
							notallowed = true;
							break;
						}
					}
				}
			}
			if(!notallowed)
				tetromino->col = tetromino->potentialcol;
			break;
		case right:
			tetromino->potentialcol = tetromino->col+1;
			for(int i = 0; i<tetrohw;i++){
				if(notallowed)
					break;
				for(int j = 0; j<tetrohw;j++){
					if((*tetromino->shape)[i][j] != 0){
						if(tetromino->potentialcol+j >= coltotal){
							//Out of bounds
							notallowed = true;
							break;
						}
						else if(field[tetromino->row+i][tetromino->potentialcol+j] != 0){
							//Space taken
							notallowed = true;
							break;
						}
					}
				}
			}
			if(!notallowed)
				tetromino->col = tetromino->potentialcol;
			break;
	}
}

bool allowedToRotate(int row, int col, int shape[4][4]){
	for(int i = 0; i<tetrohw; i++){
		for(int j = 0; j<tetrohw; j++){
			if(shape[i][j] != 0){
				if(i+row < 0 || i+row > 15 || j+col < 0 || j+row >9){
					//out of bounds
				   return false;
			  	}
				if(field[row+i][col+j] != 0){
					//blocks blocking you
					return false;
				}
			}
		}
	}
	return true;
}

void tetroRotate(struct tetro* tetromino){
	tetromino->rotnumber++;
	bool rotated = false;
	switch(tetromino->type){
		case O:
			break;
		case I:
			switch(tetromino->rotnumber){
				case 2:
					if(allowedToRotate(tetromino->row,tetromino->col,Isideways)){
						tetromino->shape = &Isideways;
						rotated = true;
					}
					break;
				default:
					if(allowedToRotate(tetromino->row,tetromino->col,IstraightUp)){
						tetromino->rotnumber = 1;
						tetromino->shape = &IstraightUp;
						rotated = true;
					}
					break;
			}
			break;
		case T:
			switch(tetromino->rotnumber){
				case 2:
					if(allowedToRotate(tetromino->row,tetromino->col,TrightTilt)){
						tetromino->shape = &TrightTilt;
						rotated = true;
					}
					break;
				case 3:
					if(allowedToRotate(tetromino->row,tetromino->col,TrightTilt)){
						tetromino->shape = &TupsideDown;
						rotated = true;
					}
					break;
				case 4:
					if(allowedToRotate(tetromino->row,tetromino->col,TleftTilt)){
						tetromino->shape = &TleftTilt;
						rotated = true;
					}
					break;
				default:
					if(allowedToRotate(tetromino->row,tetromino->col,TstraightUp)){
						tetromino->rotnumber = 1;
						tetromino->shape = &TstraightUp;
						rotated = true;
					}
					break;
			}
			break;
		case L:
			switch(tetromino->rotnumber){
				case 2:
					if(allowedToRotate(tetromino->row,tetromino->col,LrightTilt)){
						tetromino->shape = &LrightTilt;
						rotated = true;
					}
					break;
				case 3:
					if(allowedToRotate(tetromino->row,tetromino->col,LupsideDown)){
						tetromino->shape = &LupsideDown;
						rotated = true;
					}
					break;
				case 4:
					if(allowedToRotate(tetromino->row,tetromino->col,LleftTilt)){
						tetromino->shape = &LleftTilt;
						rotated = true;
					}
					break;
				default:
					if(allowedToRotate(tetromino->row,tetromino->col,LstraightUp)){
						tetromino->rotnumber = 1;
						tetromino->shape = &LstraightUp;
						rotated = true;
					}
					break;
			}
			break;
		case J:
			switch(tetromino->rotnumber){
				case 2:
					if(allowedToRotate(tetromino->row,tetromino->col,JrightTilt)){
						tetromino->shape = &JrightTilt;
						rotated = true;
					}
					break;
				case 3:
					if(allowedToRotate(tetromino->row,tetromino->col,JupsideDown)){
						tetromino->shape = &JupsideDown;
						rotated = true;
					}
					break;
				case 4:
					if(allowedToRotate(tetromino->row,tetromino->col,JleftTilt)){
						tetromino->shape = &JleftTilt;
						rotated = true;
					}
					break;
				default:
					if(allowedToRotate(tetromino->row,tetromino->col,JstraightUp)){
						tetromino->rotnumber = 1;
						tetromino->shape = &JstraightUp;
						rotated = true;
					}
					break;
			}
			break;
		case S:
			switch(tetromino->rotnumber){
				case 2:
					if(allowedToRotate(tetromino->row,tetromino->col,Ssideways)){
						tetromino->shape = &Ssideways;
						rotated = true;
					}
					break;
				default:
					if(allowedToRotate(tetromino->row,tetromino->col,SstraightUp)){
						tetromino->rotnumber = 1;
						tetromino->shape =&SstraightUp;
						rotated = true;
					}
					break;
			}
			break;
		case Z:
			switch(tetromino->rotnumber){
				case 2:
					if(allowedToRotate(tetromino->row,tetromino->col,Zsideways)){
						tetromino->shape = &Zsideways;
						rotated = true;
					}
					break;
				default:
					if(allowedToRotate(tetromino->row,tetromino->col,ZstraightUp)){
						tetromino->rotnumber = 1;
						tetromino->shape = &ZstraightUp;
						rotated = true;
					}
					break;
			}
			break;
	}
	if(!rotated)
		tetromino->rotnumber--;
}



int rng(int lower, int upper){
	return (rand() % (upper-lower + 1)) + lower; // % (upper-lower + 1) + lower is apparently what you need to do to get random number in a range in C.
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
	SDL_Texture *blocktextures[7];
	SDL_Surface *blocksurface;

	SDL_CreateWindowAndRenderer(0,0,SDL_WINDOW_FULLSCREEN_DESKTOP, &window, &renderer);
	bgsurface = IMG_Load("romfs:/img/background.png");
	bgtexture = SDL_CreateTextureFromSurface(renderer,bgsurface);
	SDL_FreeSurface(bgsurface);

	//load all the block textures into an array for later use.

	blocksurface = IMG_Load("romfs:/img/pinkblock.png");
	blocktextures[0] = SDL_CreateTextureFromSurface(renderer,blocksurface);
	SDL_FreeSurface(blocksurface);

	blocksurface = IMG_Load("romfs:/img/redblock.png");
	blocktextures[1] = SDL_CreateTextureFromSurface(renderer,blocksurface);
	SDL_FreeSurface(blocksurface);

	blocksurface = IMG_Load("romfs:/img/greenblock.png");
	blocktextures[2] = SDL_CreateTextureFromSurface(renderer,blocksurface);
	SDL_FreeSurface(blocksurface);

	blocksurface = IMG_Load("romfs:/img/blueblock.png");
	blocktextures[3] = SDL_CreateTextureFromSurface(renderer,blocksurface);
	SDL_FreeSurface(blocksurface);

	blocksurface = IMG_Load("romfs:/img/orangeblock.png");
	blocktextures[4] = SDL_CreateTextureFromSurface(renderer,blocksurface);
	SDL_FreeSurface(blocksurface);

	blocksurface = IMG_Load("romfs:/img/yellowblock.png");
	blocktextures[5] = SDL_CreateTextureFromSurface(renderer,blocksurface);
	SDL_FreeSurface(blocksurface);

	blocksurface = IMG_Load("romfs:/img/purpleblock.png");
	blocktextures[6] = SDL_CreateTextureFromSurface(renderer,blocksurface);
	SDL_FreeSurface(blocksurface);
	char score[4];
	SDL_Color color = {255,255,255,255};
	TTF_Font *Roboto_80 = TTF_OpenFont("romfs:Roboto-Regular.ttf", 80);
	int textureindex;

	srand(time(0));
	int random;
	random = rng(0,6);
	struct tetro* tetroptr;
	tetroptr = newTetro(0,0,random);



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
				random = rng(0,6);
				tetroptr = newTetro(0,0,random);
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
			case KEY_B:
				tetroRotate(tetroptr);
				break;
		}

		SDL_RenderClear(renderer);
		SDL_RenderCopy(renderer,bgtexture,NULL,NULL);
		for(int i =0; i<rowtotal;i++){                 //Draw grounded blocks
			for(int j = 0; j<coltotal;j++){
				if(field[i][j] != 0){
					textureindex = field[i][j] -1;
					drawBlock(renderer,blocktextures[textureindex],i*blockdimension,j*blockdimension+wtoboard);
				}
			}
		}
		for(int i = 0; i<tetrohw;i++){                   //Draw the currently falling tetromino
			for(int j = 0; j<tetrohw;j++){
				if((*tetroptr->shape)[i][j] != 0){
					textureindex = (*tetroptr->shape)[i][j] -1;
					drawBlock(renderer,blocktextures[textureindex],(i+tetroptr->row)*blockdimension,(j+tetroptr->col)*blockdimension+wtoboard);
				}
			}
		}
		//drawBlock(renderer,pbtexture,tetromino.row,tetromino.col+wtoboard);
		sprintf(score,"%d",tetroptr->rotnumber);
		drawText(renderer, Roboto_80, 0, 0, color, score);
		SDL_RenderPresent(renderer);

    }

	//shut everything down
	TTF_Quit();
	romfsExit();
	IMG_Quit();
	SDL_DestroyTexture(bgtexture);
	for(int i =0; i<7;i++){
		SDL_DestroyTexture(blocktextures[i]);
	}
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
	free(tetroptr);
	return 0;
}
