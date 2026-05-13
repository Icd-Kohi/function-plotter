/**********************************************************************
 * @            App.cpp
 * @            Self contained function visualizer app
 *
 * @            1.0.0
 *
 * @            Unlicense license
 **********************************************************************/

#include <cstdint>
#include <sys/types.h>
#include <raylib.h>
#include <math.h>
#include <stdio.h>
#define SIZE(obj) sizeof(obj)/sizeof(obj[0])

typedef u_int8_t u8;
typedef u_int16_t u16;
typedef u_int32_t u32;
typedef int8_t i8;
typedef int16_t i16;
typedef int32_t i32;

#define MAX_INPUT   4

float polynomialWave(float x, float a, float b, float c, float d, float e, float f, float g);

/*
 * TODO: Modularize code,
 *       Test suite,
 *       More features,
 *       Better README.
 */

int main(void){
    // TODO: Better variable namings
    const char* names[] = { "a", "b", "c", "d", "e", "f", "g" };

    char status[64];

    //--------------------------------------------------------------------------------------
    // Initialization
    //--------------------------------------------------------------------------------------
    const u16 screenWidth = 1600;
    const u16 screenHeight = 900;

    InitWindow(screenWidth, screenHeight, "raylib [core] Polynomial wave - window");

    SetTargetFPS(60);               
    //--------------------------------------------------------------------------------------
    
    const u16 wave = 400;
    float xRange = 4.0f; // from, to range
    
    const float zoomSpeed = 1.1f;

    float a = 0.0f, b = 0.0f, c = 0.0f, d = 0.0f, e = 0.0f, f = 0.0f, g = 1.0f;

    u16 dynamic_coefficient = 0;
    float adjustmentSpeed = 0.05f;

    char equationText[128] = { 0 };
    char buttonText[128] = { 0 };
    
    bool space_pressed = false; 
    bool show_text = false;
    char text_delimiter[MAX_INPUT + 1] = "\0";
    u8 letterCount = 0;

    // textBox
    Rectangle text_box =  { screenWidth / 2.0f - 550, 180, 250, 50 };
    bool mouse_on_text = false;
    u8 frames_counter = 0;

    // ------ Update text --------
    char text[64] = { 0 };
    
    // TODO: Refactor updates and drawings
    while (!WindowShouldClose())   
    {
        //----------------------------------------------------------------------------------
        // Update
        //----------------------------------------------------------------------------------
        
        // Mouse collision in text_box
        if (CheckCollisionPointRec(GetMousePosition(), text_box)){
            mouse_on_text = true;
        } else mouse_on_text = false;

        // Function zoom
        if(IsKeyPressed(KEY_UP)){
            xRange /= zoomSpeed;
        }
        if(IsKeyPressed(KEY_DOWN)){
            xRange *= zoomSpeed;
        }

        // Zoom constraint
        if(xRange < 1.0f) xRange = 0.1f;
        if(xRange > 50.0f) xRange = 50.0f;


        float step = xRange * 2 / wave;


        if(IsKeyPressed(KEY_RIGHT)) dynamic_coefficient = (dynamic_coefficient + 1) % 7;
        if(IsKeyPressed(KEY_LEFT)) dynamic_coefficient = (dynamic_coefficient + 6) % 7;

        u16 kp_add = 0;

        // Numerical keypad + ADD {DEFAULT VALUE} to the current selected coefficient
        if (IsKeyPressed(KEY_KP_ADD)){
            kp_add++;
            if(kp_add >= 6) kp_add = 6;
            if(dynamic_coefficient == 0) a += adjustmentSpeed; 
            else if(dynamic_coefficient == 1) b += adjustmentSpeed;
            else if(dynamic_coefficient == 2) c += adjustmentSpeed;
            else if(dynamic_coefficient == 3) d += adjustmentSpeed;
            else if(dynamic_coefficient == 4) e += adjustmentSpeed;
            else if(dynamic_coefficient == 5) f += adjustmentSpeed;
            else if(dynamic_coefficient == 6) g += adjustmentSpeed;
        }

        // Numerical keypad - SUBTRACT {DEFAULT VALUE} from the current selected coefficient
        if (IsKeyPressed(KEY_KP_SUBTRACT)){
            kp_add--;
            if(kp_add <= 0) kp_add = 0;
            if(dynamic_coefficient == 0) a -= adjustmentSpeed; 
            else if(dynamic_coefficient == 1) b -= adjustmentSpeed;
            else if(dynamic_coefficient == 2) c -= adjustmentSpeed;
            else if(dynamic_coefficient == 3) d -= adjustmentSpeed;
            else if(dynamic_coefficient == 4) e -= adjustmentSpeed;
            else if(dynamic_coefficient == 5) f -= adjustmentSpeed;
            else if(dynamic_coefficient == 6) g -= adjustmentSpeed;
        }

        // Reset to {DEFAULT VALUE} by pressing [R] key
        if(IsKeyPressed(KEY_R)){
            a = 0.0;
            b = 0.0;
            c = 0.0;
            d = 0.0;
            e = 0.0;
            f = 0.0;
            g = 1.0;
            xRange = 4.0f; 
        }

        // Invokes the text_box by pressing [SPACE] key
        if(IsKeyPressed(KEY_SPACE)) {
            space_pressed = true;
            show_text = true;
        }

        // Update `number_key` when hovering `text_box`.
        if(mouse_on_text) {
            i8 number_key = GetCharPressed();

            while(number_key > 0){
                if((number_key >= 48) && (number_key <= 57) && (letterCount < MAX_INPUT)){
                    text_delimiter[letterCount] = (char)number_key;
                    text_delimiter[letterCount + 1] = '\0';
                    letterCount++;
                }
                number_key = GetCharPressed();
            }

            if(IsKeyPressed(KEY_BACKSPACE)){
                letterCount--;
                if(letterCount < 0) letterCount = 0;
                text_delimiter[letterCount] = '\0';
            } 

            // TODO: Handle user float inputs,
            //       LOG current function after each modification.
            if(IsKeyPressed(KEY_ENTER)) {
                i16 new_value = 0;
                new_value = atoi(text_delimiter);
                if(dynamic_coefficient == 0) a = new_value; 
                else if(dynamic_coefficient == 1) b = new_value;
                else if(dynamic_coefficient == 2) c = new_value;
                else if(dynamic_coefficient == 3) d = new_value;
                else if(dynamic_coefficient == 4) e = new_value;
                else if(dynamic_coefficient == 5) f = new_value;
                else if(dynamic_coefficient == 6) g = new_value;
                number_key = 0;
                space_pressed = false;
                show_text = false;
            }

        } else {
            SetMouseCursor(MOUSE_CURSOR_DEFAULT);
        }

        // Cursor blinking
        if (mouse_on_text) {
            frames_counter++;
        } else frames_counter = 0;
        
        // Write text to given arrays
        snprintf(equationText, sizeof(equationText), "y = %.2fx^6 + %.2fx^5 + %.2fx^4 + %.2fx^3 + %.2fx^2 + %.2fx + %.2f", a, b, c, d, e, f, g);
        snprintf(buttonText, sizeof(buttonText), "UP or DOWN Key: Zoom\nLEFT or RIGHT Key: change Coefficient");

        //--------------------------------------------------------------------------------------
        // Draw
        //--------------------------------------------------------------------------------------
        BeginDrawing();

        ClearBackground(BLACK);

        // Rectangle box
        snprintf(text, sizeof(text), "Modifying coefficient %s", names[dynamic_coefficient]);
        if(space_pressed){
            show_text = true;
            DrawRectangleRec(text_box, LIGHTGRAY);
            if (mouse_on_text) DrawRectangleLines((int)text_box.x, (int)text_box.y, (int)text_box.width, (int)text_box.height, RED);
            else DrawRectangleLines((int)text_box.x, (int)text_box.y, (int)text_box.width, (int)text_box.height, DARKGRAY);

            DrawText(text_delimiter, (int)text_box.x + 5, (int)text_box.y + 8, 40, MAROON);

            DrawText(TextFormat("INPUT NUMBERS: %i/%i", letterCount, MAX_INPUT), 250, 250, 20, DARKGRAY);

            if(mouse_on_text){
                if(show_text) {
                    DrawText(text, 250, 150, 20, PINK);
                }
                if(letterCount < MAX_INPUT) {
                    if (((frames_counter/20)%2) == 0) DrawText("_", (int)text_box.x + 8 + MeasureText(text_delimiter, 40), (int)text_box.y + 12, 40, MAROON);
                }else {
                    DrawText("Maximum number reached", 250, 270, 20, RED);
                }
            }else {
                show_text = false; 
                DrawText("Hover box to edit", 250, 150, 20, DARKGRAY);
            }
        }else {
            DrawText("Press [space] change value ", 180, 140, 20, GRAY);
        }

        // Graph
        DrawLine(screenWidth / 2, 0, screenWidth / 2, screenHeight, GRAY);
        DrawLine(0, screenHeight / 2, screenWidth, screenHeight / 2, GRAY);

        DrawText("Y", screenWidth / 2 + 5, 5, 20, GRAY);
        DrawText("X", screenWidth - 20, screenHeight /2  + 5, 20, GRAY);

        // TODO: show axis values, `xRange` tied.

        for(size_t i = 0; i < wave - 1; ++i){
            float x1 = -xRange + i * step;
            float x2 = x1 + step; 

            float y1 = polynomialWave(x1, a, b, c, d, e, f, g);
            float y2 = polynomialWave(x2, a, b, c, d, e, f, g);

            Vector2 start = {(float) screenWidth / 2 + x1 * (screenWidth / ( 2 * xRange)),
                             (float) screenHeight / 2 - y1 * (screenHeight / ( 2 * xRange))};
            Vector2 end =   {(float) screenWidth / 2 + x2 * (screenWidth / ( 2 * xRange)),
                             (float) screenHeight / 2 - y2 * (screenHeight / ( 2 * xRange))};

            DrawLineEx(start,end, 2.0f, WHITE);
        }

        DrawText(equationText, 10, 10, 20, YELLOW);


        snprintf(status, sizeof(status), "Adjusting: %s (keypad + or - to change)", names[dynamic_coefficient]);
        DrawText(status, 10, 40, 20, BLUE);
        DrawText(buttonText, 10, 60, 20, BLUE);
        DrawText("Press [R] to reset values", 10, 100, 20, BLUE);



        EndDrawing();
    }

    //--------------------------------------------------------------------------------------
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}

float polynomialWave(float x, float a, float b, float c, float d, float e, float f, float g){
    float y = a * powf(x, 6) + b * powf(x, 5) + c * powf(x, 4) + d * powf(x, 3) + e * powf(x, 2) + f * x + g;
    return y;
}

