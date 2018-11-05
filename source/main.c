#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <switch.h>
#include <string.h>
#include <stdlib.h>


int SnekY = 30, SnekX = 30, direction = 4, R = 0, SnackX = 0, SnackY = 0, score = -1, Timer = 35500000, cursor = 4;
bool debug = false, Slowmode = true, Dummy = true, Death = true, Dead = false;

int main(int argc, char **argv)
{
    gfxInitDefault();

    DeadJ:
    consoleInit(NULL);
    while(Dead){
    printf("\x1b[1;1H\x1b[31mYOU DIED!\n\x1b[32mFinal Score: %d\n\nPress A to continue", score);
    hidScanInput();
    u64 kDown = hidKeysDown(CONTROLLER_P1_AUTO);

    if (kDown & KEY_A) break;

    gfxFlushBuffers();
    gfxSwapBuffers();
    gfxWaitForVsync();
    }

    Dead = false;
    SnekY = 30;
    SnekX = 30;
    Timer = 35500000;
    cursor = 4;
    score = -1;


    consoleInit(NULL);
    while(Dummy){
    hidScanInput();
    u64 kDown = hidKeysDown(CONTROLLER_P1_AUTO);

    printf("\x1b[36m\x1b[%d;1H>", cursor);

    if (kDown & KEY_DUP) cursor = cursor - 1, consoleInit(NULL);
    if (kDown & KEY_DDOWN) cursor = cursor + 1, consoleInit(NULL);

    if (cursor < 4) cursor = 4;
    if (cursor > 7) cursor = 7;

    if (kDown & KEY_A && cursor == 4) break;

    if (kDown & KEY_A && debug == true && cursor == 5){ debug = false; goto SkipBut; }
    if (kDown & KEY_A && debug == false && cursor == 5) debug = true;

    if (kDown & KEY_A && Death == true && cursor == 6){ Death = false; goto SkipBut; }
    if (kDown & KEY_A && Death == false && cursor == 6) Death = true;

    if (kDown & KEY_A && Slowmode == true && cursor == 7){ Slowmode = false; goto SkipBut; }
    if (kDown & KEY_A && Slowmode == false && cursor == 7) Slowmode = true;

    SkipBut:

    printf("\x1b[32m\x1b[1;1H--- SnakeGame by Such Meme, Many Skill ---");
    printf("\x1b[4;3H\x1b[32mStart!");

    if (debug) printf("\x1b[5;3H\x1b[33mDebug mode \x1b[32mON ");
    if (debug == false) printf("\x1b[5;3H\x1b[33mDebug mode \x1b[31mOFF");

    if (Death) printf("\x1b[6;3H\x1b[31mDeaths \x1b[32mON ");
    if (Death == false) printf("\x1b[6;3H\x1b[31mDeaths \x1b[31mOFF");

    if (Slowmode == false) printf("\x1b[7;3H\x1b[34mFastmode \x1b[32mON ");
    if (Slowmode) printf("\x1b[7;3H\x1b[34mFastmode \x1b[31mOFF");

    printf("\x1b[0m\x1b[9;1HPress + to exit");
    if (kDown & KEY_PLUS) goto End;


    gfxFlushBuffers();
    gfxSwapBuffers();
    gfxWaitForVsync();
    }


    consoleInit(NULL);
    void gensquare(){
        R = rand();
        R = R/25000000;
        SnackX = R;
        R = rand();
        R = R/50000000;
        SnackY = R;
        score = score+1;
        if (SnackY < 1) SnackY = 15;
        if (SnackX > 79) SnackX = 50;
        if (SnackX < 1) SnackX = 30;
        if (Slowmode == true) Timer = Timer - 500000;
         }

    gensquare();

    while(appletMainLoop())
    {
        hidScanInput();
        u64 kDown = hidKeysDown(CONTROLLER_P1_AUTO);

        printf("\b ");

        if (kDown & KEY_DUP) direction = 1;
        if (kDown & KEY_DDOWN) direction = 2;
        if (kDown & KEY_DLEFT) direction = 3;
        if (kDown & KEY_DRIGHT) direction = 4;
        if (direction == 1) SnekY = SnekY-1;
        if (direction == 2) SnekY = SnekY+1;
        if (direction == 3) SnekX = SnekX-1;
        if (direction == 4) SnekX = SnekX+1;

        if (Death == false){
        
        if (SnekY == 0) SnekY = 1;
        if (SnekY == 45) SnekY = 44;

        if (SnekX == 0) SnekX = 1;
        if (SnekX == 80) SnekX = 79; }

        if (SnekX < 1 || SnekX > 80 || SnekY < 1 || SnekY > 45 && Death == true){ Dead = true; goto DeadJ; }


        if (debug == true){
        printf("\x1b[1;69H%d  ", SnekX);
        printf("\x1b[2;69H%d  ", SnekY);
        printf("\x1b[3;69H%d  ", SnackX);
        printf("\x1b[4;69H%d  ", SnackY);
        printf("\x1b[4;69H%d  ", Timer); }

        if (SnekX == SnackX && SnekY == SnackY) gensquare();
        printf("\x1b[32m\x1b[44;60HScore: %d", score);

        //44 lines, 81 coloms
        printf("\x1b[32m\x1b[%d;%dH0", SnackY, SnackX);
        printf("\x1b[31m\x1b[%d;%dH8", SnekY, SnekX);

        if (kDown & KEY_PLUS) goto DeadJ;
        gfxFlushBuffers();
        gfxSwapBuffers();
        gfxWaitForVsync();
        if (Timer < 1) Slowmode = false;
        if (Slowmode == true) svcSleepThread(Timer);
    }

    End:

    gfxExit();
    return 0;
}

