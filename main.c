#include<stdio.h>
#include<stdlib.h>

#include<SDL2/SDL.h>

#define WIDTH         320
#define HEIGHT        520
#define MAX_PATH_SIZE 20

int main(){ 
  SDL_Rect dest = {0};
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
  char dst[MAX_PATH_SIZE];
  SDL_Surface *numbers[9];
  SDL_Texture *numbers_texture[9];
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
  
  // Clearing the window with white color
  SDL_SetRenderDrawColor( renderer, 0xFF, 0xFF, 0xFF, 0xFF );
  SDL_RenderClear( renderer );
  SDL_RenderPresent( renderer );

  // Render the texture on screen
  //SDL_RenderCopy( renderer, pikachu_texture, NULL, NULL);
  //SDL_RenderPresent( renderer );
  for(int i=0; i<9; i++){
    SDL_RenderCopy(renderer, numbers_texture[i], NULL, &dest);
        dest.x += 32;
  }
  SDL_RenderPresent( renderer );
  
  int running = 1;
  while(running){
    SDL_Event event;
    while(SDL_PollEvent(&event)){
      if(event.type == SDL_QUIT){
	running = 0;
      }
    }
    SDL_Delay(100);
  }
  
  return 0;
}
