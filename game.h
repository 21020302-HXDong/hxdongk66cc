#pragma once
#include <bits/stdc++.h>
#include "SDL.h"
#include "SDL_image.h"
class Game{
public:
    Game ();
    void Run ();
private:
    bool running=false,cs=false;
    int fps=0;

    static const int FRAME_RATE=1000/60;
    static const int SCREEN_WIDTH=640;
    static const int SCREEN_HEIGHT=640;
    static const int GRID_WIDTH=32;
    static const int GRID_HEIGHT=32;

    SDL_Window*window=NULL;
    SDL_Renderer*renderer=NULL;

    enum class Block {dau,than,food,empty};
    enum class Move {len,xuong,trai,phai};
    Move last_huong=Move::len;
    Move huong=Move::len;

    struct {
        float x=GRID_WIDTH/2;
        float y=GRID_HEIGHT/2;
    } pos;

    SDL_Point dau={
        static_cast<int>(pos.x),
        static_cast<int>(pos.y),
    };
    SDL_Point food;
    std::vector<SDL_Point> than;
    Block grid[GRID_WIDTH][GRID_HEIGHT];

    float td=0.3f;
    int growing=0,score=0,size=1;

    void ReplaceFood ();
    void GrowBody (int sl);
    void UpdateWindowTitle ();
    void GameLoop ();
    void Render ();
    void Update ();
    void PollEvents ();
    void Close ();
};
