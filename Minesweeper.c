#include "Mine_Logic.h"
int Screenw = 704;
int Screenh = 704;

double m = 1;

int boardx = 18;
int boardy = 21;

double scale = 32;

int main(){
  srand(time(NULL));
  
  SDL_Init(SDL_INIT_VIDEO);
  
  SDL_Event event;
  SDL_Time currentTicks;
  SDL_Time previousTicks = currentTicks;
  
  SDL_Window* window = SDL_CreateWindow("Minesweeper", 704,704,SDL_WINDOW_RESIZABLE);
  SDL_Renderer* render = SDL_CreateRenderer(window,0);
  
  SDL_SetWindowMaximumSize(window,1028,1028);
  
  int x = 0, y = 0;
  
  float temp1, temp2;
  SDL_GetMouseState(&temp1, &temp2);
  
  int mousex = temp1-16;
  int mousey= temp2-112;
  
  
  int time = 0;
  
  // MINES COUNT
  int totalMines = 99;
  int nonMinesCount = 0;
  int numFlags = 0;
  
  int flags = totalMines;
  
  bool running = true;
  bool first = true, firstclick = true;
  
  bool alive = true;
  bool hovering = false;
  
  int mines[128][128];
  struct mine mineStates[128*128];
  
  bool events[3] = {false,false,false};
  
   for(int row = 0; row < boardy; row++){
    for(int col = 0; col < boardx; col++){
      mineStates[row*boardx+col].state=10;
      mineStates[row*boardx+col].hidden = true;
      mineStates[row*boardx+col].flagged = false;
    }}
    
    
  // SPRITESHEET //
  
  // Find image path!
  char imagePath[128];
  char titlePath[128];
  char easyPath[128];
  char medPath[128];
  char hardPath[128];
  
  snprintf(imagePath, sizeof(imagePath),"%s/MineSheet.png",       SDL_GetBasePath());
  snprintf(titlePath, sizeof(titlePath),"%s/Title.png", SDL_GetBasePath());
  snprintf(easyPath, sizeof(easyPath),"%s/Easy.png", SDL_GetBasePath());
  snprintf(medPath, sizeof(medPath),"%s/Med.png", SDL_GetBasePath());
  snprintf(hardPath, sizeof(hardPath),"%s/Hard.png", SDL_GetBasePath());
  SDL_Surface* temp = IMG_Load(imagePath);
  SDL_Texture* Spritesheet = SDL_CreateTextureFromSurface(render, temp);
  
  temp = IMG_Load(titlePath);
  SDL_Texture* Title = SDL_CreateTextureFromSurface(render, temp);
  
  temp = IMG_Load(easyPath);
  SDL_Texture* Easy = SDL_CreateTextureFromSurface(render, temp);
  
  temp = IMG_Load(medPath);
  SDL_Texture* Medium = SDL_CreateTextureFromSurface(render, temp);
  
  temp = IMG_Load(hardPath);
  SDL_Texture* Hard = SDL_CreateTextureFromSurface(render, temp);
  // SLICING SPRITESHEET //
  
  SDL_FRect gridTile = {.x=0,.y=51.1,.w=15.7,.h=15.7};
  SDL_FRect face = {.x=0,.y=24,.w=26,.h=26};
  SDL_FRect facePress = {.x=27,.y=24,.w=26,.h=26};
  SDL_FRect faceWin = {.x=27*3,.y=24,.w=26,.h=26};
  SDL_FRect faceLose = {.x=27*4,.y=24,.w=26,.h=26};
  bool title = true;
  
  
  while(running){
  
    
    SDL_GetCurrentRenderOutputSize(render, &Screenw, &Screenh);
      
    SDL_Event event;
    SDL_GetCurrentTime(&currentTicks);
    
    // INPUT
    while(SDL_PollEvent(&event)){
    
      if(event.type == SDL_EVENT_QUIT){
          running = false;
          SDL_Quit();
        }
    if (event.type == SDL_EVENT_WINDOW_RESIZED) {
    
      SDL_GetCurrentRenderOutputSize(render, &Screenw, &Screenh);
      SDL_SetWindowSize(window, Screenw, Screenw);
}
      //MOUSE
      
      for(int i=0; i<3;i++){
        events[i] = false;
      }
      
      if (event.type == SDL_EVENT_MOUSE_BUTTON_DOWN) {
        if(event.button.button == SDL_BUTTON_LEFT)
        events[0] = true;
        else if(event.button.button == SDL_BUTTON_RIGHT)
        events[1] = true;
      }
  }
  
  
    //                     TITLE                      //
    
  if(title){
    pass(1);
    
    SDL_GetMouseState(&temp1, &temp2);
    
    mousex = temp1;
    mousey = temp2;
    
    
    SDL_SetRenderDrawColor(render,192,192,192,0);
    SDL_RenderClear(render);
    
    SDL_FRect pos;
    
    
    renderBorders(render,Spritesheet,gridTile);
    
    pos =(SDL_FRect){.x=Screenw*((704/2.0-420/2)/704.0),.y=Screenw*(32/704.0),.w=Screenw*(420/704.0),.h=Screenw*(48/704.0)};
    SDL_RenderTexture(render,Title,NULL, &pos);
    
    pos =(SDL_FRect){.x=Screenw*((704/2.0-420/2)/704.0),.y=Screenw*(225/704.0),.w=Screenw*(420/704.0),.h=Screenw*(92/704.0)};
    bool easy = (mousex>pos.x && mousey>pos.y && mousex < pos.x+pos.w  && mousey < pos.y+pos.h );
    render9ScaleRotate(render,Spritesheet,&pos,0,gridTile);
    

    
    pos =(SDL_FRect){.x=Screenw*((704/2.0-420/2)/704.0),.y=Screenw*((375+92/4)/704.0),.w=Screenw*(420/704.0),.h=Screenw*(92/704.0)};
    bool medium = (mousex>pos.x && mousey>pos.y && mousex < pos.x+pos.w  && mousey < pos.y+pos.h );
    render9ScaleRotate(render,Spritesheet,&pos,0,gridTile);
   
    
    pos =(SDL_FRect){.x=Screenw*((704/2.0-420/2)/704.0),.y=Screenw*((375+200)/704.0),.w=Screenw*(420/704.0),.h=Screenw*(92/704.0)};
    bool hard = (mousex>pos.x && mousey>pos.y && mousex < pos.x+pos.w  && mousey < pos.y+pos.h );
    render9ScaleRotate(render,Spritesheet,&pos,0,gridTile);
    
        pos =(SDL_FRect){.x=Screenw*((704/2.0-420/2+115)/704.0),.y=Screenw*((225+(92/4))/704.0),.w=Screenw*(210/704.0),.h=Screenw*(48/704.0)};
    SDL_RenderTexture(render,Easy,NULL, &pos);
    
     pos =(SDL_FRect){.x=Screenw*((704/2.0-520/2+115)/704.0),.y=Screenw*(((375+92/4)+(92/4))/704.0),.w=Screenw*((210+80)/704.0),.h=Screenw*(48/704.0)};
    SDL_RenderTexture(render,Medium,NULL, &pos);
    
    pos =(SDL_FRect){.x=Screenw*((704/2.0-420/2+115)/704.0),.y=Screenw*(((375+200)+(92/4))/704.0),.w=Screenw*(210/704.0),.h=Screenw*(48/704.0)};
    SDL_RenderTexture(render,Hard,NULL, &pos);
    
    
    SDL_RenderPresent(render);
    
    
    if(easy && events[0]){
      title = false;
      
      totalMines = 25;
      flags = totalMines;
      pass(0.64);
  
      for(int i=0; i<3;i++){
        events[i] = false;
      }
      }
   else if(medium && events[0]){
      title = false;
      
      totalMines = 50;
      flags = totalMines;
      pass(1);
      
      for(int i=0; i<3;i++){
        events[i] = false;
      }
      }
      else if(hard && events[0]){
      title = false;
      
      totalMines = 150;
      flags = totalMines;
      pass(1.369);
      
      for(int i=0; i<3;i++){
        events[i] = false;
      }
      }
  }
  else{
  
    //                     MAIN LOOP                    //

    SDL_GetMouseState(&temp1, &temp2);
    
    mousex = temp1 - Screenw*(16/704.0);;
    mousey = temp2 - Screenw*(112/704.0);;

    hovering = (mousex>Screenw*((704/2-48)/704.0) && mousex <Screenw*((704/2+16)/704.0) && mousey > Screenw*((-88)/704.0)  && mousey < Screenw*((-24)/704.0) );
int scaled = Screenw*(scale/704.0);
    
    int gridX = mousex / scaled;
    int gridY = mousey / scaled;
    if(scaled > 0 &&
    gridX >= 0 && gridX < boardy && gridY >= 0 && gridY < boardx || hovering){
        if(!hovering && alive)
        mineStates[(mousex/(scaled))*boardx+(mousey/(scaled))].highlight = true;
        // On click
        
        if(events[0]){
          
          //GENERATE
          if(hovering){
            title = true;
            alive = true;
            firstclick = true;
            flags = totalMines;
            time = 0;
            time = 0;
            nonMinesCount = 0;
            for(int row = 0; row < boardy; row++){
              for(int col = 0; col < boardx; col++){
                mineStates[row*boardx+col].state=10;
                mineStates[row*boardx+col].hidden = true;
                mineStates[row*boardx+col].flagged = false;
              }}
          }
          else if(firstclick){
            do{
              for(int row = 0; row < boardy; row++){
                for(int col = 0; col < boardx; col++){
                  mineStates[row*boardx+col].state=10;
                  mineStates[row*boardx+col].hidden = true;
                }}
    
              numFlags = 0;
              while(numFlags < totalMines){
                int ranx = (int)(rand()%boardy), rany = (int)(rand()%boardx);
                if(mineStates[ranx*boardx+rany].state != 11){
                  mineStates[ranx*boardx+rany].state = 11;
                  numFlags++;
                }
              }
              
              tick(mineStates);
              
            } while((mineStates[(mousex/scaled)*boardx+(mousey/scaled)].state != 10));
          }
          if(!hovering && alive){
          firstclick = false;
          
          // Every click
         
          if(alive)
          floodFill(mousex/scaled,mousey/scaled,mineStates);
          if(mineStates[(mousex/scaled)*boardx+(mousey/scaled)].state == 11 && !mineStates[(mousex/scaled)*boardx+(mousey/scaled)].flagged){
            //Lose like the fat loser chud u r NOOOB
            alive = false;
            mineStates[(mousex/scaled)*boardx+(mousey/scaled)].state = 12;
             for(int x=0; x<boardy;x++){
             for(int y=0; y<boardx;y++){
             
             if(mineStates[(x)*boardx+(y)].flagged == true && mineStates[(x)*boardx+(y)].state != 11){
                    mineStates[(x)*boardx+(y)].state = 14;
                    mineStates[(x)*boardx+(y)].hidden = false;
                    mineStates[(x)*boardx+(y)].flagged = false;
                    }
                if(mineStates[(x)*boardx+(y)].state == 11){
                  mineStates[(x)*boardx+(y)].hidden = false;
                  mineStates[(x)*boardx+(y)].flagged = false;
                  }
             }
            }
           }
           }
          }
         
        else if (events[1]){
        events[1] = false;
        if(mineStates[(mousex/scaled)*boardx+(mousey/scaled)].hidden == true){
          //switch
          if(mineStates[(mousex/scaled)*boardx+(mousey/scaled)].flagged ){
            mineStates[(mousex/scaled)*boardx+(mousey/scaled)].flagged = false;
            flags++;
          }
          else if (flags > 0){
            flags--;
            mineStates[(mousex/scaled)*boardx+(mousey/scaled)].flagged = true;
            }
          }
        }
        
    }
    
    
    nonMinesCount = 0;
    for(int x=0; x<boardy;x++){
     for(int y=0; y<boardx;y++){
        if(mineStates[(x)*boardx+(y)].state != 11 && mineStates[(x)*boardx+(y)].hidden == true)
          nonMinesCount++;
     }
     }
             
    // Once per second
    if(currentTicks/1000000000.0L - previousTicks/1000000000.0L > 1){
      previousTicks = currentTicks;
      if(alive && !firstclick)
      time++;
    }
    
    // RENDERING //
    SDL_SetRenderDrawColor(render, 0, 0, 0, 255);
    SDL_RenderClear(render);
    
    renderBorders(render,Spritesheet,gridTile);
    renderGrid(render,Spritesheet,mines,mineStates);
    
    renderClock(render, Spritesheet, time,575,25);
    renderClock(render, Spritesheet, flags,32,25);
    
    // FACE
    SDL_FRect pos ={.x=Screenw*((704/2-32)/704.0),.y=Screenw*((22)/704.0),.w=Screenw*((64)/704.0),.h=Screenw*((64)/704.0)};
    if(hovering)
    SDL_RenderTexture(render,Spritesheet,&facePress,&pos);
    
    else if(nonMinesCount == 0 && !firstclick){
    SDL_RenderTexture(render,Spritesheet,&faceWin,&pos);
    alive = false;
    }
    
    else
    SDL_RenderTexture(render,Spritesheet,&face,&pos);
    
    
    SDL_RenderPresent(render);
    first = false;
    events[0] = false;
  }
  }
  return 0;
}
