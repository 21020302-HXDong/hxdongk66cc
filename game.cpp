#include <bits/stdc++.h>
#include "SDL.h"
#include "Game.h"
using namespace std;
Game::Game(){
    for (int i=0;i<=GRID_WIDTH-1;i++){
        for (int j=0;j<=GRID_HEIGHT-1;j++){
            grid[i][j]=Block::empty;
        }
    }
    srand (static_cast<unsigned int>(time(0)));
}
void Game::ReplaceFood (){
    int x,y;
    while (true){
        x=rand()%GRID_WIDTH;
        y=rand()%GRID_HEIGHT;
        if (grid[x][y]==Block::empty){
            grid[x][y]=Block::food;
            food.x=x;
            food.y=y;
            break;
        }
    }
}
void Game::GameLoop (){
    Uint32 before,second=SDL_GetTicks(),after;
    int frame_time;
    int frames=0;
    while (running){
        before=SDL_GetTicks();
        PollEvents ();
        Update ();
        Render ();
        frames++;
        after=SDL_GetTicks();
        frame_time=after-before;
        if (after-second>=1000){
            fps=frames;
            frames=0;
            second=after;
            UpdateWindowTitle ();
        }
        if (FRAME_RATE>frame_time){
            SDL_Delay (FRAME_RATE-frame_time);
        }
    }
}
void Game::PollEvents (){
    SDL_Event e;
    while (SDL_PollEvent(&e)){
        if (e.type==SDL_QUIT){
            running=false;
        }
        else if (e.type==SDL_KEYDOWN){
            switch (e.key.keysym.sym){
                case SDLK_UP:
                    if (last_huong!=Move::xuong || size==1)
                        huong=Move::len;
                    break;
                case SDLK_DOWN:
                    if (last_huong!=Move::len || size==1)
                        huong=Move::xuong;
                    break;
                case SDLK_LEFT:
                    if (last_huong!=Move::phai || size==1)
                        huong=Move::trai;
                    break;
                case SDLK_RIGHT:
                    if (last_huong!=Move::trai || size==1)
                        huong=Move::phai;
                    break;
            }
        }
    }
}
void Game::Update (){
    if (!cs){
        return;
    }
    switch (huong){
        case Move::len:
            pos.y-=td;
            break;
        case Move::xuong:
            pos.y+=td;
            break;
        case Move::trai:
            pos.x-=td;
            break;
        case Move::phai:
            pos.x+=td;
            break;
    }
    // Thay doi chieu dai
    if (pos.x<0) pos.x=GRID_WIDTH-1;
    else if (pos.x>GRID_WIDTH-1) pos.x=0;
    if (pos.y<0) pos.y=GRID_HEIGHT-1;
    else if (pos.y>GRID_HEIGHT-1) pos.y=0;
    // Toa do vi tri dau moi
    int new_x=static_cast<int>(pos.x);
    int new_y=static_cast<int>(pos.y);
    // Kiem tra neu vi tri dau ran thay doi
    if (new_x!=dau.x || new_y!=dau.y){
        last_huong=huong;
        // Neu chieu dai tang => Them 1 node moi
        if (growing>0){
            size++;
            than.push_back(dau);
            growing--;
            grid[dau.x][dau.y]=Block::than;
        }
        else{
            SDL_Point free=dau;
            vector<SDL_Point>::reverse_iterator rit=than.rbegin();
            for (;rit!=than.rend();rit++){
                grid[free.x][free.y]=Block::than;
                swap(*rit,free);
            }
            grid[free.x][free.y]=Block::empty;
        }
    }
    dau.x=new_x;
    dau.y=new_y;
    Block&next=grid[dau.x][dau.y];
    // Kiem tra xem co food tai cho dang xet khong
    if (next==Block::food){
        score++;
        ReplaceFood ();
        GrowBody (1);
    }
    // Neu bi chet, dam vao than minh
    else if (next==Block::than){
        cs=false;
    }
    next=Block::dau;
}
void Game::GrowBody (int sl){
    growing+=sl;
}
void Game::UpdateWindowTitle (){
    string title = "Snake game C++, Score: "+to_string(score)+" FPS: " + to_string(fps);
    SDL_SetWindowTitle(window,title.c_str());
}
void Game::Render (){
    SDL_Rect block;
    block.w=SCREEN_WIDTH/GRID_WIDTH;
    block.h=SCREEN_WIDTH/GRID_HEIGHT;
    // Xoa man hinh
    SDL_SetRenderDrawColor (renderer,30,30,30,255);
    SDL_RenderClear (renderer);
    // Render food
    SDL_SetRenderDrawColor (renderer,255,204,0,255);
    block.x=food.x*block.w;
    block.y=food.y*block.h;
    SDL_RenderFillRect (renderer,&block);
    // Render than ran
    SDL_SetRenderDrawColor (renderer,255,255,255,255);
    for (SDL_Point&point:than){
        block.x=point.x*block.w;
        block.y=point.y*block.h;
        SDL_RenderFillRect (renderer,&block);
    }
    // Render dau ran
    block.x=dau.x*block.w;
    block.y=dau.y*block.h;
    if (cs) SDL_SetRenderDrawColor (renderer,0,122,204,255);
    else SDL_SetRenderDrawColor (renderer,255,0,0,255);
    SDL_RenderFillRect (renderer,&block);
    // Update Screen
    SDL_RenderPresent (renderer);
}
void Game::Run(){
    // Khoi tao SDL
    if (SDL_Init (SDL_INIT_VIDEO)<0){
        cout<<"Error! SDL_Error: "<<SDL_GetError ()<<endl;
        exit (1);
    }
    // Tao cua so
    window=SDL_CreateWindow ("Snake Game",SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,SCREEN_WIDTH,SCREEN_HEIGHT,SDL_WINDOW_SHOWN);
    if (window==NULL){
        cout<<"Error! SDL_Error: "<<SDL_GetError ()<<endl;
        exit (1);
    }
    // Renderer
    renderer=SDL_CreateRenderer (window,-1,SDL_RENDERER_ACCELERATED);
    if (renderer==NULL){
        cout<<"Error! SDL_Error: " << SDL_GetError ()<<endl;
        exit (1);
    }
    cs=true;
    running=true;
    ReplaceFood ();
    GameLoop ();
}
void Game::Close (){
    SDL_DestroyWindow (window);
    SDL_Quit ();
}
