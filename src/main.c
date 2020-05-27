
#include <raylib.h>
#include <math.h>

#include "level.h"
#include "draw.h"
#include "state.h"

int main(int argc, char const *argv[]){

    // Initialization
    const int screen_width = 800;
    const int screen_height = 600;

    InitWindow(screen_width,screen_height,"Presente - the game");
    SetTargetFPS(60);

    // Initialize level and fill randomly
    level *lvl = level_new(50,40);
    level_fill_random(lvl,6);

    // Initialize state (and add enemies)
    state *sta = state_new();
    state_populate_random(lvl,sta,40);

    // Load music
    InitAudioDevice();
    Music battle = LoadMusicStream("./resources/Into-Battle_v001.mp3");
    /* Music by Eric Matyas   www.soundimage.org*/
    PlayMusicStream(battle); 
    bool pause = false;

    // Main loop
    while(!WindowShouldClose()){
        // Music control
        UpdateMusicStream(battle);

        if (IsKeyPressed(KEY_SPACE)){
            StopMusicStream(battle);
            PlayMusicStream(battle);
        }

        // Pause/Resume music playing
        if (IsKeyPressed(KEY_P)){
            pause = !pause;

            if (pause) PauseMusicStream(battle);
            else ResumeMusicStream(battle);
        }


        // Update input depending if keys are pressed or not
        sta->button_state[0] = IsKeyDown(KEY_D);
        sta->button_state[1] = IsKeyDown(KEY_W);
        sta->button_state[2] = IsKeyDown(KEY_A);
        sta->button_state[3] = IsKeyDown(KEY_S);
        sta->button_state[4] = IsMouseButtonDown(MOUSE_LEFT_BUTTON);
        // Compute the angle the player is looking at from the mouse's position relative to the screen's center
        float mouse_y = GetMouseY()-GetScreenHeight()/2;
        float mouse_x = GetMouseX()-GetScreenWidth()/2;
        sta->aim_angle = atan2(-mouse_y,mouse_x);

        // Update the state
        state_update(lvl,sta);

        // Drawing
        BeginDrawing();

            ClearBackground(RAYWHITE);

            draw_state(lvl, sta);

            DrawText("Presente profe!\nPress P to pause/resume music\nPress Space to replay music\nPress Esc to exit",50,50,16,LIGHTGRAY);
            DrawText("Music by Eric Matyas\nwww.soundimage.org",550,50,16,LIGHTGRAY);

        EndDrawing();

    }

    // Unload music
    UnloadMusicStream(battle);
    CloseAudioDevice();

    // Closer window
    CloseWindow();

    // Free memory
    state_free(sta);
    level_free(lvl);

    return 0;
}
