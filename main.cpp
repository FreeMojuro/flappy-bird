#include <raylib.h>


constexpr int window_width{1200};
constexpr int window_height{900};
constexpr Vector2 origin{0, 0};

constexpr int min_colon_size{100};
constexpr int max_colon_size{600};

int score{0};

class Bird{
    public:

    Texture2D birdTexture;
    float scale{0.06f};

    Bird(){birdTexture = LoadTexture("flappy.png");}
    ~Bird(){UnloadTexture(birdTexture);}
    
    Vector2 position{150,200};
    double velocity{1};
    bool iter{false};
    
    void Draw()const{
        Rectangle source{0.0f, 0.0f, (float)birdTexture.width, (float)birdTexture.height};
        Rectangle dest{position.x, position.y, birdTexture.width * scale, birdTexture.height * scale};
        DrawTexturePro(birdTexture, source, dest, origin, 0.0f, WHITE);
    }

    void Update(){
        if(iter){
        velocity += 0.8;
        position.y+=velocity;
        iter=false;}
        else{iter=true;}

        if(IsKeyPressed(KEY_SPACE)){position.y-=100; velocity=1;}

    }
};


int GenerateColonHeight(){
    int height = GetRandomValue(min_colon_size, max_colon_size);
    return height;

}



class Colonpair{
    public:

    int upper_colon_x{1000};
    int upper_colon_y{0};
    int upper_colon_width{100};
    int upper_colon_height{GenerateColonHeight()};

    int GapSize{GetRandomValue(200, 500)};

    int lower_colon_x{1000};
    int lower_colon_y{GapSize + upper_colon_height};
    int lower_colon_width{100};
    int lower_colon_height{window_height-lower_colon_y};

    void Draw(){
        DrawRectangle(upper_colon_x, upper_colon_y, upper_colon_width, upper_colon_height, WHITE);
        DrawRectangle(lower_colon_x, lower_colon_y, lower_colon_width, lower_colon_height, WHITE);
    }

    void Reset(){
        lower_colon_x = window_width;
        upper_colon_x = window_width;

        upper_colon_height = GenerateColonHeight();
        GapSize = GetRandomValue(200, 500);
        lower_colon_y = GapSize + upper_colon_height;
        lower_colon_height = window_height - lower_colon_y;
    }

    void Update(){
        lower_colon_x -= 10;
        upper_colon_x -= 10;
        if(lower_colon_x == 0){Reset();score++;}
    }


};

bool checkcollision(const Bird& bird, const Colonpair& colon){
    
    Rectangle birdRec{
        bird.position.x, 
        bird.position.y, 
        (float)bird.birdTexture.width * bird.scale, 
        (float)bird.birdTexture.height * bird.scale
    };

    
    Rectangle upperColonRec{
        (float)colon.upper_colon_x, 
        (float)colon.upper_colon_y, 
        (float)colon.upper_colon_width, 
        (float)colon.upper_colon_height
    };

    Rectangle lowerColonRec{
        (float)colon.lower_colon_x, 
        (float)colon.lower_colon_y, 
        (float)colon.lower_colon_width, 
        (float)colon.lower_colon_height
    };

    
    if (CheckCollisionRecs(birdRec, upperColonRec) || CheckCollisionRecs(birdRec, lowerColonRec)) {
        return true;
    }

    
    if (bird.position.y < 0 || bird.position.y + birdRec.height > window_height) {
        return true;
    }

    return false; 
}


int main(){
    InitWindow(window_width, window_height, "game");
    SetTargetFPS(60);
    Bird bird{};
    Colonpair colons{};

    while(!WindowShouldClose()){
        BeginDrawing();
        bird.Update();
        colons.Update();
       

        ClearBackground(BLACK);
        bird.Draw();
        colons.Draw();
        

        if(checkcollision(bird, colons)){
            break;
        }

        DrawText(TextFormat("%i", score), 20, 20, 50, WHITE);
        EndDrawing();
    }
    CloseWindow();
    return 0;

}
