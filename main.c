#include<stdio.h>
#include<stdlib.h>
#include "sudoku.c"

#include<SDL2/SDL.h>

#define WIDTH         288
#define HEIGHT        432
#define MAX_PATH_SIZE 20
#define BOARD_SIZE    9
#define SIZE          9

extern int board[BOARD_SIZE][BOARD_SIZE];

SDL_Surface *numbers[9];
SDL_Texture *numbers_texture[9];
SDL_Texture *qmark_texture;
/*

//sudoku problem
int board[9][9] = {
    {6,5,0,8,7,3,0,9,0},
    {0,0,3,2,5,0,0,0,8},
    {9,8,0,1,0,4,3,5,7},
    {1,0,5,0,0,0,0,0,0},
    {4,0,0,0,0,0,0,0,2},
    {0,0,0,0,0,0,5,0,3},
    {5,7,8,3,0,1,0,2,6},
    {2,0,0,0,4,8,9,0,0},
    {0,9,0,6,2,5,0,8,1}
};


 */

void DrawBoard(SDL_Renderer *renderer){
  SDL_Rect dest = {0};
  dest.w = 32;
  dest.h = 48;
  for(int i=0; i<BOARD_SIZE; i++){
    for(int j=0; j<BOARD_SIZE; j++){
      if(board[i][j] != 0){
	SDL_RenderCopy(renderer, numbers_texture[board[i][j]-1], NULL, &dest);
      }
      else{
	SDL_RenderCopy(renderer, qmark_texture,NULL,&dest);
      }
      dest.x += 32;
    }
    dest.x = 0;
    dest.y += 48;
  }
  SDL_RenderPresent(renderer);
}


//function to solve sudoku
//using backtracking
int solve_sudoku(SDL_Renderer *renderer)
{
  int row;
  int col;
  //if all cells are assigned then the sudoku is already solved
  //pass by reference because number_unassigned will change the values of row and col
  if(number_unassigned(&row, &col) == 0)
    return 1;
  int n,i;
  //number between 1 to 9
  for(i=1;i<=SIZE;i++)
    {
      //if we can assign i to the cell or not
      //the cell is board[row][col]
      if(is_safe(i, row, col))
        {
   	  SDL_Delay(500);
	  board[row][col] = i;
	  DrawBoard(renderer);
	  //backtracking
	  if(solve_sudoku(renderer))
	    return 1;
	  //if we can't proceed with this solution
	  //reassign the cell
	  board[row][col]=0;
	  DrawBoard(renderer);	  
        }
    }
  return 0;
}

int main(){
  char garbage;
  SDL_Rect dest = {0};
  SDL_Event event;
  dest.w = 32;
  dest.h = 48;
  if(SDL_Init(SDL_INIT_VIDEO)){
    fprintf(stderr,"ERROR: Could not inialize the Video!\n");
    exit(1);
  }

  SDL_Window *Window = SDL_CreateWindow("sudoku",
					0,
					0,
					WIDTH,
					HEIGHT,
				        0);

  if(Window == NULL){
    fprintf(stderr,"ERROR: Could not initialize a window\n");
    exit(1);
  }


  SDL_Renderer *renderer = SDL_CreateRenderer(Window,-1,SDL_RENDERER_ACCELERATED);

  if(renderer == NULL){
    fprintf(stderr,"ERROR: Could not create renderer\n");
    exit(1);
  }

  // loading the images
  SDL_Surface *pikachu = SDL_LoadBMP("./img/pikachu.bmp");
  if(!pikachu){
    fprintf(stderr,"ERROR: Failed to load the resources.\n");
    exit(1);
  }
  SDL_Surface *qmark = SDL_LoadBMP("./img/question_mark.bmp");
  char dst[MAX_PATH_SIZE];
  for(int i=1; i<10; i++){
    sprintf(dst,"./img/%d.bmp",i);
    numbers[i-1] = SDL_LoadBMP(dst);
    if(!numbers[i-1]){
      fprintf(stderr,"%s",SDL_GetError());
      exit(1);
    }
    numbers_texture[i-1] = SDL_CreateTextureFromSurface( renderer, numbers[i-1]);
    if(numbers_texture[i-1] == NULL){
      fprintf(stderr,"%s",SDL_GetError());
      exit(1);
    }
  }
  
  
  // Converting the surface to textures
  SDL_Texture *pikachu_texture =  SDL_CreateTextureFromSurface( renderer, pikachu);
  for(int i=0; i<9; i++){
    numbers_texture[i] = SDL_CreateTextureFromSurface(renderer , numbers[i]);
  }
  qmark_texture = SDL_CreateTextureFromSurface( renderer, qmark);
  // Render the texture on screen
  SDL_RenderCopy( renderer, pikachu_texture, NULL, NULL);
  SDL_RenderPresent( renderer );
  
  // Clearing the window with white color
  SDL_SetRenderDrawColor( renderer, 0x00, 0x18, 0x18, 0x18 );
  SDL_RenderClear( renderer );
  //SDL_RenderPresent( renderer );

  // Render the texture on screen
  //SDL_RenderCopy( renderer, pikachu_texture, NULL, NULL);
  //SDL_RenderPresent( renderer );

  
  DrawBoard(renderer);
  solve_sudoku(renderer);  
  int running = 1;
  while(running){
    
    while(SDL_PollEvent(&event)){
      if(event.type == SDL_QUIT){
	running = 0;
      }
    }
    SDL_Delay(100);
  }

  return 0;
}
