//#include "../../Core/Source/Core/Core.h"
#include <cstdint>
#include <sys/types.h>
#include <raylib.h>
#include <math.h>
#include <stdio.h>
#define SIZE(obj) sizeof(obj)/sizeof(obj[0])

typedef u_int16_t u16;
typedef u_int32_t u32;
typedef int16_t i16;
typedef int32_t i32;


float polynomialWave(float x, float a, float b, float c, float d, float e, float f, float g);

int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    const u16 screenWidth = 1600;
    const u16 screenHeight = 900;

    InitWindow(screenWidth, screenHeight, "raylib [core] Polynomial wave - window");

    SetTargetFPS(60);               
    //--------------------------------------------------------------------------------------
    const u16 wave = 400;
    float xRange = 4.0f; // from, to range
                         //
    
    const float zoomSpeed = 1.1f;

    float a = 0.0f, b = 0.0f, c = 0.0f, d = 0.0f, e = 0.0f, f = 0.0f, g = 1.0f;

    u16 dynamicCoefficient = 0;
    float adjustmentSpeed = 0.05f;

    char equationText[128] = { 0 };
    char buttonText[128] = { 0 };

    while (!WindowShouldClose())   
    {
        // Update
        //----------------------------------------------------------------------------------
        // TODO: Update your variables here
        //-------------------------------------------------------------------------- w--------

        // Draw
        //----------------------------------------------------------------------------------
        
        if(IsKeyPressed(KEY_UP)){
            xRange /= zoomSpeed;
        }
        if(IsKeyPressed(KEY_DOWN)){
            xRange *= zoomSpeed;
        }
            
        // constraint
        if(xRange < 1.0f) xRange = 0.1f;
        if(xRange > 50.0f) xRange = 50.0f;


        float step = xRange * 2 / wave;
    
            
        if(IsKeyPressed(KEY_RIGHT)) dynamicCoefficient = (dynamicCoefficient + 1) % 7;
        if(IsKeyPressed(KEY_LEFT)) dynamicCoefficient = (dynamicCoefficient + 6) % 7;
        
        u16 kp_add = 0;
        // Numerical keypad +
        if (IsKeyPressed(KEY_KP_ADD)){
            kp_add++;
            if(kp_add >= 6) kp_add = 6;
            if(dynamicCoefficient == 0) a += adjustmentSpeed; 
            else if(dynamicCoefficient == 1) b += adjustmentSpeed;
            else if(dynamicCoefficient == 2) c += adjustmentSpeed;
            else if(dynamicCoefficient == 3) d += adjustmentSpeed;
            else if(dynamicCoefficient == 4) e += adjustmentSpeed;
            else if(dynamicCoefficient == 5) f += adjustmentSpeed;
            else if(dynamicCoefficient == 6) g += adjustmentSpeed;
        }

        // Numerical keypad -
        if (IsKeyPressed(KEY_KP_SUBTRACT)){
            kp_add--;
            if(kp_add <= 0) kp_add = 0;
            if(dynamicCoefficient == 0) a -= adjustmentSpeed; 
            else if(dynamicCoefficient == 1) b -= adjustmentSpeed;
            else if(dynamicCoefficient == 2) c -= adjustmentSpeed;
            else if(dynamicCoefficient == 3) d -= adjustmentSpeed;
            else if(dynamicCoefficient == 4) e -= adjustmentSpeed;
            else if(dynamicCoefficient == 5) f -= adjustmentSpeed;
            else if(dynamicCoefficient == 6) g -= adjustmentSpeed;
        }
        // TODO: Refactor updates 
        // Update text
        //
        // TODO: editable text box for each Coefficient
        snprintf(equationText, sizeof(equationText), "y = %.2fx^6 + %.2fx^5 + %.2fx^4 + %.2fx^3 + %.2fx^2 + %.2fx + %.2f", a, b, c, d, e, f, g);
        snprintf(buttonText, sizeof(buttonText), "UP or DOWN Key: Zoom\nLEFT or RIGHT Key: change Coefficient");
        // Draw
        BeginDrawing();

        ClearBackground(BLACK);

        DrawLine(screenWidth / 2, 0, screenWidth / 2, screenHeight, GRAY);
        DrawLine(0, screenHeight / 2, screenWidth, screenHeight / 2, GRAY);
        
        DrawText("Y", screenWidth / 2 + 5, 5, 20, GRAY);
        DrawText("X", screenWidth - 20, screenHeight /2  + 5, 20, GRAY);

        // TODO: show axis number, following xRange 

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

        const char* names[] = { "a", "b", "c", "d", "e", "f", "g" };

        char status[64];

        snprintf(status, sizeof(status), "Adjusting: %s (keypad + or - to change)", names[dynamicCoefficient]);

        DrawText(status, 10, 40, 20, BLUE);
        DrawText(buttonText, 10, 60, 20, BLUE);

        EndDrawing();
    }
        //----------------------------------------------------------------------------------

    //--------------------------------------------------------------------------------------
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}
// Return
float polynomialWave(float x, float a, float b, float c, float d, float e, float f, float g){
    float y = a * powf(x, 6) + b * powf(x, 5) + c * powf(x, 4) + d * powf(x, 3) + e * powf(x, 2) + f * x + g;
    return y;
}

