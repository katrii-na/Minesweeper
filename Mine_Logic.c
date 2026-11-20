/* MINES CALCULATIONS */

#include "Mine_Logic.h"


extern int Screenw;
extern int boardx;
extern int boardy;

extern double scale;

void pass(double m){
  
  boardx = 18;
  boardy = 21;
  scale = 32;

  boardx = (int)(boardx * m);
  boardy =(int)(boardy*m);
  scale = scale/m;
  
}

void floodFill(int x, int y, struct mine mineStates[128*128]){
  // Check if in bounds
  if(x >= 0 && y >= 0 && x < boardy && y < boardx){
      bool pass = mineStates[x*boardx+y].state == 10 && mineStates[x*boardx+y].hidden == true && mineStates[x*boardx+y].flagged == false;
      
      if(!mineStates[x*boardx+y].flagged)
      mineStates[x*boardx+y].hidden = false;
      
      if(pass){
      // Spread
       int dir[8][2] = {
    {-1, -1}, {0, -1}, {1, -1}, 
    {-1,  0},          {1,  0}, 
    {-1,  1}, {0,  1}, {1,  1} 
};
      for(int i = 0; i<8; i++){
      floodFill(x+dir[i][0], y+dir[i][1],mineStates);
      }
      
    }
  }
}

void tick(struct mine mineStates[128*128]){
  for(int x=0; x<boardy;x++){
    for(int y=0; y<boardx;y++){
      int count = 0;
      
      int dir[8][2] = {
    {-1, -1}, {0, -1}, {1, -1}, 
    {-1,  0},          {1,  0}, 
    {-1,  1}, {0,  1}, {1,  1} 
}; 
      for(int i = 0; i<8; i++){
        if(x+dir[i][0] >= 0 && x+dir[i][0] < boardy&&y+dir[i][1] >= 0 && y+dir[i][1] < boardx){
          if(mineStates[(x+dir[i][0])*boardx+y+dir[i][1]].state == 11) count++;
        }
      }
      if(count == 0) count = 10;
      
      if(mineStates[x*boardx+y].state == 10)
      mineStates[x*boardx+y].state = count;
    }
  }
}


void render9ScaleRotate(SDL_Renderer *render,SDL_Texture *Spritesheet,SDL_FRect *position, int deg,SDL_FRect gridTile ){
  /* ROTATE AND RENDER A 9 scale TEXTURE. */
  double s = Screenw*(2.4/704.0);
  double r = Screenw*(5/704.0);
  
  // Make a tempoary texture to render too
  SDL_Texture *temp = SDL_CreateTexture(render, 0, SDL_TEXTUREACCESS_TARGET,position->w,position->h);
  
  // Target it
  SDL_SetRenderTarget(render,temp);
  
  
  // Render grid to the tempoary texture
  SDL_FRect pos = {0,0,position->w,position->h};
  SDL_RenderTexture9Grid(render, Spritesheet,&gridTile,r,r,r,r,s,&pos);
  
  // back out of there girl
  SDL_SetRenderTarget(render,NULL);
  
  // Render the rotated
  SDL_RenderTextureRotated(render,temp,NULL,position,deg,NULL,SDL_FLIP_NONE);
  SDL_DestroyTexture(temp);
}

void renderBorders(SDL_Renderer *render,SDL_Texture *Spritesheet,SDL_FRect gridTile ){
  double s = Screenw*(2.4/704.0);
  double r = Screenw*(5/704.0);
  /* VANITY, KEEPS MAIN CLEAN */
  SDL_FRect pos;
  //bg
  pos = (SDL_FRect){.x=0,.y=0,.w=Screenw,.h=Screenw};
  SDL_RenderTexture9Grid(render, Spritesheet,&gridTile,r,r,r,r,s,&pos);
  double scaled = scale;
  //inlet
  pos = (SDL_FRect){.x=Screenw*(11/704.0),.y=Screenw*(106/704.0),.w=Screenw*((scaled*boardy+10)/704.0),.h=Screenw*((scaled*boardx+12)/704.0)};
  render9ScaleRotate(render, Spritesheet, &pos,180,gridTile);
  
  pos = (SDL_FRect){.x=Screenw*(11/704.0),.y=Screenw*(11/704.0),.w=Screenw*((32*21+10)/704.0),.h=Screenw*(86/704.0)};
  render9ScaleRotate(render, Spritesheet, &pos,180,gridTile);
  //outlet
  
  pos = (SDL_FRect){.x=Screenw*(27/704.0),.y=Screenw*(19/704.0),.w=Screenw*((32*3+9)/704.0),.h=Screenw*((69)/704.0)};
  render9ScaleRotate(render, Spritesheet, &pos,0,gridTile);
  
  pos = (SDL_FRect){.x=Screenw*(570/704.0),.y=Screenw*(19/704.0),.w=Screenw*((32*3+9)/704.0),.h=Screenw*((69)/704.0)};
  render9ScaleRotate(render, Spritesheet, &pos,0,gridTile);
  
  
  
  
}
void renderGrid(SDL_Renderer *render,SDL_Texture* Spritesheet, int mines[128][128],struct mine minestates[boardy*boardx]){
  /* RENDERS BOARD, KEEPS MAIN CLEAN */
  double s = Screenw*(2.4/704.0);
  double r = Screenw*(5/704.0);
  SDL_FRect num[15];
  SDL_FRect highlightOverlay = {.x=0,.y=85,.w=15.7,.h=15.7};
  
  num[0] = (SDL_FRect){.x=0,.y=51.1,.w=16,.h=16}; //Default
  for(int i=1;i<10;i++){
    num[i] = (SDL_FRect){.x=17*(i-1),.y=68,.w=16,.h=16};
  }// 1-9 = num
  num[10] = (SDL_FRect){.x=17,.y=51.1,.w=15.7,.h=15.7};
  num[11] = (SDL_FRect){.x=17*5,.y=51.1,.w=15.7,.h=15.7};
  num[12] = (SDL_FRect){.x=17*6,.y=51.1,.w=15.7,.h=15.7};
  num[13] = (SDL_FRect){.x=17*2,.y=51.1,.w=15.7,.h=15.7};
  num[14] = (SDL_FRect){.x=17*7,.y=51.1,.w=15.7,.h=15.7};
    // RENDER
  for(int row = 0; row < boardy; row++){
    for(int col = 0; col < boardx; col++){
    
      int scaled = Screenw*((scale)/704.0);
      SDL_FRect pos ={
      .x=Screenw*(16/704.0)+(scaled*row),.
      y=Screenw*(112/704.0)+(scaled*col),
      .w=scaled,
      .h=scaled};
      
     if(minestates[row*boardx+col].flagged == true)
      SDL_RenderTexture(render, Spritesheet,&num[13],&pos);
      
     else if(minestates[row*boardx+col].hidden != true)
      SDL_RenderTexture(render, Spritesheet,&num[minestates[row*boardx+col].state],&pos);
      
      else
        SDL_RenderTexture(render, Spritesheet,&num[0],&pos);
      
      if(minestates[row*boardx+col].highlight){
        int alpha = 167;
        SDL_SetTextureAlphaMod(Spritesheet,alpha);
        
        SDL_RenderTexture(render, Spritesheet,&highlightOverlay,&pos);
        minestates[row*boardx+col].highlight = false;
        
        alpha = 255;
        SDL_SetTextureAlphaMod(Spritesheet,alpha);
      }
    }
  }
}

void renderClock(SDL_Renderer *render,SDL_Texture* Spritesheet, int num,int x,int y){
    
    SDL_FRect times[10];
    times[0] = (SDL_FRect){.x=14*9,.y=0,.w=13,.h=23};
    for(int i=1;i<10;i++){
      times[i] = (SDL_FRect){.x=14*(i-1),.y=0,.w=13,.h=23};
    }
    
    if(num > 999) num = 999; //CAP
    
    SDL_FRect pos;
    
    pos =(SDL_FRect){.x=Screenw*(x/704.0),.y=Screenw*(y/704.0),.w=Screenw*(32/704.0),.h=Screenw*(56/704.0)};
    SDL_RenderTexture(render, Spritesheet,&times[num/100],&pos);
    
    pos =(SDL_FRect){.x=Screenw*((x+31)/704.0),.y=Screenw*(y/704.0),.w=Screenw*(32/704.0),.h=Screenw*(56/704.0)};
    SDL_RenderTexture(render, Spritesheet,&times[(num/10)%10],&pos);
    
    pos =(SDL_FRect){.x=Screenw*((x+62)/704.0),.y=Screenw*(y/704.0),.w=Screenw*(32/704.0),.h=Screenw*(56/704.0)};
    SDL_RenderTexture(render, Spritesheet,&times[num%10],&pos);
    
}
