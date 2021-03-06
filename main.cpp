//The headers
#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "SDL/SDL_ttf.h"
#include "SDL/SDL_mixer.h"
#include <string>
#include <iostream>
#include <sstream>
#include <fstream>
#include <ctime>
#include <cstdlib>
using namespace std;

//Screen attributes
const int SCREEN_WIDTH = 320;
const int SCREEN_HEIGHT = 240;
const int SCREEN_BPP = 32;
const int menuTextAmount = 4;

const int FRAMES_PER_SECOND=30;
//The surfaces
SDL_Surface *menuBackground = NULL;
SDL_Surface *background = NULL;
SDL_Surface *gameartImg = NULL;
SDL_Rect gameart[ 4 ];
SDL_Surface *gameboard = NULL;

SDL_Surface *menuMessage[menuTextAmount];
SDL_Surface *message = NULL;
SDL_Surface *messageFromOpponentDisplay = NULL;
SDL_Surface *messageWinsAndLoses = NULL;
string messageFromOpponent;
string menuMessageText[] = {"Press START to Play Game"," Press Y to View Credits", "Made by Nathan Hurde[theweirdn8]","www.PawByte.com"};
SDL_Surface *screen = NULL;

//The Game

//External Game Variables
const float gameVersion= 1.04;
string gameCaptionTitle = "PawByte - Tic Tac Toe";

//Map/Room Variables
int CURRENT_AREA = 0;
int MAIN_MENU = 0;
int GAME_CREDITS = 1;
int GAME_BOARD = 2;

int BOARD_PXWIDTH = 1;
int BOARD_PXHEIGHT =1;


/////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////
//Credits Variables

//Credits alpha

int creditsAlpha = SDL_ALPHA_OPAQUE;
string creditsCaptions[]={"Game Created By","Game Designers ","Game Programmers ","GCW-Zero Porting Team","Game Art","Game Music ","Thanks for Playing "};
int creditsMaxNumber[]={2,1,1,1,3,1,1};
string creditsNames[7][3]={
{
"Nathan Hurde(theweirdn8)","PawByte.com",""},
{"Nathan Hurde(theweirdn8)"," "," "},
{"Nathan Hurde(theweirdn8)"," "," "},
{"Zear"," "," "},
{
"Mathan Hurde(theweirdn8)",
"jimn346 from nationofdesign.com",
"Adzel from nationofdesign.com",
},
{"Kevin Macleod from incompetech.com"," "," "},
{"You are Awesome!"," ",""}
};

int creditsCaptionCounter=0;
int creditsNamesCounter=0;
int creditsTiming=10;
int creditsExtraLoopage=1;
int creditsECounter=0;
int creditsTimeCounter=0;


SDL_Surface *creditsCaptionDisplay = NULL;
SDL_Surface *creditsNameDisplay = NULL;


/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////



//Game Variables
int gameSquare[9];
int gameToken;
bool isPlayerTurn;
int turnNumber=0;
int gameDifficulty;
int gameRoom;
bool gameCompleted=false;
int playerWins=0;
int computerWins=0;
int currentSlot=0;
int nRanSquare=0;
int mouse_x=0, mouse_y=0;
int fakeTimer=0;
//The event structure
SDL_Event event;

//The font
TTF_Font *font = NULL;
TTF_Font *creditsFontTitle = NULL;

//The color of the font
SDL_Color textColor = { 0, 0, 0 };
SDL_Color redColor = {255, 0, 0};
SDL_Color whiteColor = {255, 255, 255};
//The music that will be played
Mix_Music *bgdMusic[3];

//The sound effects that will be used


ofstream myfile;

//messages from the PC

string opponentOpening[]={"Do not waste my time","Lets get this show on the road.","I wonder how I will beat you...",
"Lets go!", "Lets have a clean fight.", "This should be fun." ,"Try to beat me!", "Prepare to Lose","Let's give the crowd a show","My mission is to win","My code shall not fail me!"};

string opponentFirstMove[]={"There","YOLO!","Bring it on!","Feel the power!","9 squares, your blood, my code","Try to beat that","Ah, my 1st turn.","Time to start with a bang!","There you go.","Here we go..","I choose this one."};

string opponentBlock[]={"Beep Boop!","Nice try ole' sport.","My defenses are stronger than your will!","So predictable.","My A.I detected that.","I'm not falling for that","Nice try!","Try harder next time","Wham!","Haha.","Naughty, naughty."};

string opponentLose[]={"GG","I lost...", "Good job ole' sport!","Well played.","YOLO","A worthy opponent.","Good game.","Congratulations.","I let you win.","Come on, rematch!","Oh no!"};

string opponentWin[]={"No autographs please.","Welcome to Bronze V Rank of TTT Players","Your best wasn't good enough","I win, as usual.","Good game.","Well played.","Rematch?","You were a good opponent.","GG","Well, that was fun.","I win!"};

string opponentTieGame[]={"Rematch?","Jeepers, a tie!","What?! My legendary skills failed me.","I'm tired with this nerd?!","This is worst than losing.","Ha ha, you tied to horribly programmed A.I.","That was sick!","Good game.","Lets go again.","Well thats unusual.","Cool, us legends tied."};

int oTextArraySize= 10;

string winsandLosesText = "";

int random(int low,int high)
{
    int randnum = (rand() % (high-low + 1)) + low;
    return randnum;
}


string floattostring(float in)
{
       ostringstream converter;
       converter << in;
       return converter.str();
}


string inttostring(int in)
{
       ostringstream converter;
       converter << in;
       return converter.str();
}


void resetCredits()
{
    creditsAlpha = SDL_ALPHA_OPAQUE;
    creditsCaptionCounter=0;
    creditsNamesCounter=0;
    creditsTiming=100;
    creditsExtraLoopage=10;
    creditsECounter=0;
    creditsTimeCounter=0;
}


void updateCreditsNameText()
{
    creditsCaptionDisplay = TTF_RenderText_Solid( creditsFontTitle ,creditsCaptions[creditsCaptionCounter].c_str(), whiteColor );
    creditsNameDisplay = TTF_RenderText_Solid( font,(creditsNames[creditsCaptionCounter][creditsNamesCounter]).c_str(), whiteColor );
    //Set surface alpha
    SDL_SetAlpha( creditsNameDisplay, SDL_SRCALPHA, creditsAlpha );
}

void updateOpponentText(string textInput)
{
    messageFromOpponent=textInput;
    messageFromOpponentDisplay = TTF_RenderText_Solid( font,messageFromOpponent.c_str(), whiteColor );
}


void updateWinsText()
{
    winsandLosesText.clear();
    winsandLosesText.append("Wins: ");
    winsandLosesText.append(inttostring(playerWins));
    winsandLosesText.append(" Loses: ");
    winsandLosesText.append(inttostring(computerWins));
    if( messageWinsAndLoses!=NULL)
    {
        SDL_FreeSurface(messageWinsAndLoses);
    }
    messageWinsAndLoses = TTF_RenderText_Solid( font,winsandLosesText.c_str(), redColor );
}

void startGame()
{
    for(int iLoop=0;iLoop<9;iLoop++)
    {
        gameSquare[iLoop]=-1;
    }
    if(random(0,100)>50)
    {
        isPlayerTurn=true;
    }
    else
    {
        isPlayerTurn=false;
    }
    gameCompleted=false;
    turnNumber=0;
    updateOpponentText(opponentOpening[random(0,oTextArraySize)]);
    updateWinsText();
}

void playBackgroundMusic()
{
    Mix_HaltMusic();
    Mix_PlayMusic( bgdMusic[CURRENT_AREA], -1 );
}

void goToMenu()
{
    startGame();
    CURRENT_AREA = MAIN_MENU;
    playBackgroundMusic();
}

SDL_Surface *load_image( std::string filename )
{
    //The image that's loaded
    SDL_Surface* loadedImage = NULL;

    //The optimized surface that will be used
    SDL_Surface* optimizedImage = NULL;

    //Load the image
    loadedImage = IMG_Load( filename.c_str() );

    //If the image loaded
    if( loadedImage != NULL )
    {
        //Create an optimized surface
        optimizedImage = SDL_DisplayFormat( loadedImage );

        //Free the old surface
        SDL_FreeSurface( loadedImage );

        //If the surface was optimized
        if( optimizedImage != NULL )
        {
            //Color key surface
            SDL_SetColorKey( optimizedImage, SDL_SRCCOLORKEY, SDL_MapRGB( optimizedImage->format, 255, 000, 255 ) );
        }
    }

    //Return the optimized surface
    return optimizedImage;
}


void apply_surface( int x, int y, SDL_Surface* source, SDL_Surface* destination, SDL_Rect* clip = NULL )
{
    //Holds offsets
    SDL_Rect offset;

    //Get offsets
    offset.x = x;
    offset.y = y;

    //Blit
    SDL_BlitSurface( source, clip, destination, &offset );
}


void drawRectangle(int x1, int y1, int x2, int y2, int color)
{
    int xx=min(x1,x2);
    int yy=min(y1,y2);
    int w=max(x1,x2)-xx;
    int h=max(y1,y2)-yy;
    SDL_Rect rect = {xx,yy,w,h};
    SDL_FillRect(screen, &rect, color);
}


bool init()
{
    //Initialize all SDL subsystems
    if( SDL_Init( SDL_INIT_EVERYTHING ) == -1 )
    {
        return false;
    }

    //Set up the screen
    screen = SDL_SetVideoMode( SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP, SDL_SWSURFACE );

    //If there was an error in setting up the screen
    if( screen == NULL )
    {
        return false;
    }

    //Initialize SDL_ttf
    if( TTF_Init() == -1 )
    {
        return false;
    }

    //Initialize SDL_mixer
    if( Mix_OpenAudio( 22050, MIX_DEFAULT_FORMAT, 2, 4096 ) == -1 )
    {
        return false;
    }

    //Set the window caption
    gameCaptionTitle.append(" - Version - ");
    gameCaptionTitle.append(floattostring(gameVersion));
    SDL_WM_SetCaption( gameCaptionTitle.c_str(), NULL );

    //sets the background music to nulls/undeclared
    for(int iLoop=0;iLoop<3;iLoop++)
    {
        bgdMusic[iLoop]=NULL;
    }

    //If everything initialized fine
    return true;
}


bool load_files()
{
    //Load the background images
    menuBackground = load_image("resources/gfx/menu_background.png");
    background = load_image("resources/gfx/background.png");
    gameartImg = load_image("resources/gfx/gameart.png");
    gameboard = load_image("resources/gfx/gameboard.png");

    //Open the font(s)
    font = TTF_OpenFont( "resources/font/gamefont.ttf", 14 );
    creditsFontTitle = TTF_OpenFont( "resources/font/gamefont.ttf", 18 );

    //If there was a problem in loading the background
    if( background == NULL )
    {
        /*
        Commented out for GCW-Zero version
        myfile.open ("errors.txt");
        myfile << "Unable to load background.\n";
        myfile.close();
        */
        return false;
    }
    if(gameboard==NULL)
    {
        /*
        Commented out for GCW-Zero version
        myfile.open ("errors.txt");
        myfile << "Unable to load gameboard.\n";
        myfile.close();
        */
        return false;
    }
    else
    {
        BOARD_PXWIDTH=gameboard->w;
        BOARD_PXHEIGHT=gameboard->h;
    }
    if( gameartImg== NULL )
    {
        /*
        Commented out for GCW-Zero version
        myfile.open ("errors.txt");
        myfile << "Unable to game art! \n";
        myfile.close();
        */
        return false;
    }
    //If there was an error in loading the font
    if( font == NULL )
    {
        /*
        Commented out for GCW-Zero version
        myfile.open ("errors.txt");
        myfile << "Unable to load font\n";
        myfile.close();
        */
        return false;
    }

    bgdMusic[0] = Mix_LoadMUS( "resources/sfx/bckgdm0.ogg" );
    bgdMusic[1] = Mix_LoadMUS( "resources/sfx/bckgdm1.ogg" );
    bgdMusic[2] = Mix_LoadMUS( "resources/sfx/bckgdm2.ogg" );


    //If there was a problem loading the music
    for(int iBugMus=0;iBugMus<3;iBugMus++)
    {
        if(bgdMusic[0]==NULL)
        {
            /*
            Commented out for GCW-Zero version
            myfile.open ("errors.txt");
            myfile << "Unable to load music Index Number" << iBugMus << ".\n";
            myfile.close();
            */
            return false;
        }
    }

    //If everything loaded fine
    /*
        Commented out for GCW-Zero version
        myfile.open ("errors.txt");
        myfile << "Everything loaded successfully! \n";
        myfile.close();
    */
    return true;
}

void clean_up()
{
    //Free the surfaces
    SDL_FreeSurface( background );
    SDL_FreeSurface(menuBackground);
    SDL_FreeSurface( gameartImg );
    SDL_FreeSurface( gameboard);
    SDL_FreeSurface( message );
    for(int icleanUpMenu=0;icleanUpMenu<menuTextAmount;icleanUpMenu++)
    {
        SDL_FreeSurface(menuMessage[icleanUpMenu]);
    }
    SDL_FreeSurface( messageFromOpponentDisplay );
    SDL_FreeSurface( messageWinsAndLoses );

    //Free the sound effects

    //Free the music
    for(int iLoopM=0;iLoopM<3;iLoopM++)
    {
        Mix_FreeMusic(bgdMusic[iLoopM]);
    }
    //Close the font
    TTF_CloseFont( font );
    TTF_CloseFont( creditsFontTitle );

    //Quit SDL_mixer
    Mix_CloseAudio();

    //Quit SDL_ttf
    TTF_Quit();

    //Quit SDL
    SDL_Quit();
}


int getWinningSquare(int squareBoxType)
{
    //diagnol line victories
    //the first diagnol line check
    if((gameSquare[4]==squareBoxType)&&(gameSquare[8]==squareBoxType))
    {
        if(gameSquare[0]<0)
        {
            return 0;
        }
    }
    if((gameSquare[0]==squareBoxType)&&(gameSquare[8]==squareBoxType))
    {
        if(gameSquare[4]<0)
        {
            return 4;
        }
    }
    if((gameSquare[0]==squareBoxType)&&(gameSquare[4]==squareBoxType))
    {
        if(gameSquare[8]<0)
        {
            return 8;
        }
    }

    //the second diagnol line check
    if((gameSquare[4]==squareBoxType)&&(gameSquare[6]==squareBoxType))
    {
        if(gameSquare[2]<0)
        {
            return 2;
        }
    }
    if((gameSquare[2]==squareBoxType)&&(gameSquare[6]==squareBoxType))
    {
        if(gameSquare[4]<0)
        {
            return 4;
        }
    }
    if((gameSquare[2]==squareBoxType)&&(gameSquare[4]==squareBoxType))
    {
        if(gameSquare[6]<0)
        {
            return 6;
        }
    }
    //row checking
    for(int i=0;i<9;i++)
    {
        //first row check
        if((i==0)||(i==3)||(i==6))
        {
            if((gameSquare[i+1]==squareBoxType)&&(gameSquare[i+2]==squareBoxType))
            {
                if(gameSquare[i]<0)
                {
                    return i;
                }
            }
        }
        if((i==1)||(i==4)||(i==7))
        {
            //second row check
            if((gameSquare[i-1]==squareBoxType)&&(gameSquare[i+1]==squareBoxType))
            {
                if(gameSquare[i]<0)
                {
                    return i;
                }
            }
        }
        if((i==2)||(i==5)||(i==8))
        {
            //third row check
            if((gameSquare[i-2]==squareBoxType)&&(gameSquare[i-1]==squareBoxType))
            {
                if(gameSquare[i]<0)
                {
                    return i;
                }
            }
        }
        if(i<3)
        {
            //first column check
            if((gameSquare[i+3]==squareBoxType)&&(gameSquare[i+6]==squareBoxType))
            {
                if(gameSquare[i]<0)
                {
                    return i;
                }
            }
        }
        if((i>=3)&&(i<=5))
        {
            //second column check
            if((gameSquare[i-3]==squareBoxType)&&(gameSquare[i+3]==squareBoxType))
            {
                if(gameSquare[i]<0)
                {
                    return i;
                }
            }
        }
        if(i>=6)
        {
            //third column check
            if((gameSquare[i-3]==squareBoxType)&&(gameSquare[i-6]==squareBoxType))
            {
                if(gameSquare[i]<0)
                {
                    return i;
                }
            }
        }
    }
    return -1;
}


bool getVictory(int squareBoxType)
{
    //diagnol line victories
    //the first diagnol line check
    if((gameSquare[4]==squareBoxType)&&(gameSquare[8]==squareBoxType))
    {
        if(gameSquare[0]==squareBoxType)
        {
            return true;
        }
    }
    if((gameSquare[0]==squareBoxType)&&(gameSquare[8]==squareBoxType))
    {
        if(gameSquare[4]==squareBoxType)
        {
            return true;
        }
    }
    if((gameSquare[0]==squareBoxType)&&(gameSquare[4]==squareBoxType))
    {
        if(gameSquare[8]==squareBoxType)
        {
            return true;
        }
    }

    //the second diagnol line check
    if((gameSquare[4]==squareBoxType)&&(gameSquare[6]==squareBoxType))
    {
        if(gameSquare[2]==squareBoxType)
        {
            return true;
        }
    }
    if((gameSquare[2]==squareBoxType)&&(gameSquare[6]==squareBoxType))
    {
        if(gameSquare[4]==squareBoxType)
        {
            return true;
        }
    }
    if((gameSquare[2]==squareBoxType)&&(gameSquare[4]==squareBoxType))
    {
        if(gameSquare[6]==squareBoxType)
        {
            return true;
        }
    }
    //row checking
    for(int i=0;i<9;i++)
    {
        //first row check
        if((i==0)||(i==3)||(i==6))
        {
            if((gameSquare[i+1]==squareBoxType)&&(gameSquare[i+2]==squareBoxType))
            {
                if(gameSquare[i]==squareBoxType)
                {
                    return true;
                }
            }
        }
        if((i==1)||(i==4)||(i==7))
        {
            //second row check
            if((gameSquare[i-1]==squareBoxType)&&(gameSquare[i+1]==squareBoxType))
            {
                if(gameSquare[i]==squareBoxType)
                {
                    return true;
                }
            }
        }
        if((i==2)||(i==5)||(i==8))
        {
            //third row check
            if((gameSquare[i-2]==squareBoxType)&&(gameSquare[i-1]==squareBoxType))
            {
                if(gameSquare[i]==squareBoxType)
                {
                    return true;
                }
            }
        }
        if(i<3)
        {
            //first column check
            if((gameSquare[i+3]==squareBoxType)&&(gameSquare[i+6]==squareBoxType))
            {
                if(gameSquare[i]==squareBoxType)
                {
                    return true;
                }
            }
        }
        if((i>=3)&&(i<=5))
        {
            //second column check
            if((gameSquare[i-3]==squareBoxType)&&(gameSquare[i+3]==squareBoxType))
            {
                if(gameSquare[i]==squareBoxType)
                {
                    return true;
                }
            }
        }
        if(i>=6)
        {
            //third column check
            if((gameSquare[i-3]==squareBoxType)&&(gameSquare[i-6]==squareBoxType))
            {
                if(gameSquare[i]==squareBoxType)
                {
                    return true;
                }
            }
        }
    }
    return false;
}

bool boardIsFull()
{
    for(int iLoop=0;iLoop<9;iLoop++)
    {
        if(gameSquare[iLoop]==-1)
        {
            return false;
        }
    }
    return true;

}

void checkForCatGame()
{
    if(boardIsFull()==true)
    {
        if((getVictory(0)==false)&&(getVictory(1)==false))
        {
            //game tied
            gameCompleted=true;
            updateOpponentText(opponentTieGame[random(0,oTextArraySize)]);
            isPlayerTurn=true;
        }
    }

}

bool mouseBetween(double x1, double y1, double x2,double y2)
{
    if((mouse_x>=x1)&&(mouse_x<=x2))
    {
        if((mouse_y>=y1)&&(mouse_y<=y2))
        {
            return true;
        }
        else
        {
            return false;
        }
    }
    else
    {
        return false;
    }
}


void playerPickSlot()
{
    if(gameSquare[currentSlot]==-1)
    {
        gameSquare[currentSlot]=0;
        isPlayerTurn=false;
        if(turnNumber==0)
        {
            updateOpponentText(opponentFirstMove[random(0,oTextArraySize)]);
        }
        else
        {
            updateOpponentText(" ");
        }
        if(getVictory(0))
        {
            gameCompleted=true;
            updateOpponentText(opponentLose[random(0,oTextArraySize)]);
            playerWins+=1;
            isPlayerTurn=true;
        }
        else
        {
            checkForCatGame();
            turnNumber+=1;
        }
    }
}


void getGameCreditsInput()
{
    //If a key was pressed
    if( event.type == SDL_KEYUP )
    {
        switch(event.key.keysym.sym)
        {
            case SDLK_ESCAPE:
            case SDLK_LALT:
                goToMenu();
            break;

            default:
            //    goToMenu();
            break;
        }
    }
}


bool getGameMenuInput(bool quit)
{
    //If a key was pressed
    if( event.type == SDL_KEYUP )
    {
        switch(event.key.keysym.sym)
        {
            case SDLK_SPACE:
                startGame();
                resetCredits();
                CURRENT_AREA = GAME_CREDITS;
                playBackgroundMusic();
                quit=false;
            break;

            case SDLK_RETURN:
            case SDLK_x:
            case SDLK_v:
            case SDLK_LCTRL:
                startGame();
                CURRENT_AREA = GAME_BOARD;
                playBackgroundMusic();
                quit= false;
            break;

            case SDLK_ESCAPE:
                quit= true;
            break;

            default:
                quit= false;
            break;
        }
    }
    //If the user has Xed out the window
    if( event.type == SDL_QUIT )
    {
        //Quit the program
        quit = true;
    }
    return quit;
}

void getPlayerInput()
{

    //If a key was pressed
    if( event.type == SDL_KEYUP )
    {
        switch(event.key.keysym.sym)
        {
            case SDLK_LEFT:
            case SDLK_TAB:
                if(currentSlot>0)
                {
                    currentSlot-=1;
                }
            break;

            case SDLK_RIGHT:
            case SDLK_BACKSPACE:
                if(currentSlot<8)
                {
                    currentSlot+=1;
                }
            break;

            case SDLK_UP:
            case SDLK_w:
                if(currentSlot>=3)
                {
                    currentSlot-=3;
                }
            break;

            case SDLK_DOWN:
            case SDLK_s:
                if(currentSlot<=5)
                {
                    currentSlot+=3;
                }
            break;

            case SDLK_LCTRL:
            case SDLK_LSHIFT:
            case SDLK_x:
                playerPickSlot();
            break;

            case SDLK_ESCAPE:
                goToMenu();
            break;

            default:

            break;
        }
    }
    int ipjValue = 0;
    //If a mouse button was released
    if( event.type == SDL_MOUSEMOTION )
    {
        for(int iLoop=0;iLoop<3;iLoop+=1)
        {
            for(int jLoop=0;jLoop<3;jLoop+=1)
            {
                mouse_x=event.motion.x;
                mouse_y=event.motion.y;
                if(mouseBetween(SCREEN_WIDTH/2 -BOARD_PXWIDTH/3+(iLoop*(BOARD_PXWIDTH/3)), SCREEN_HEIGHT/2-BOARD_PXHEIGHT/3+(jLoop*(BOARD_PXHEIGHT/3)),SCREEN_WIDTH/2 +(iLoop*(BOARD_PXWIDTH/3)),SCREEN_HEIGHT/2 +(jLoop*(BOARD_PXHEIGHT/3))))
                {
                    ipjValue=iLoop+(jLoop*3);
                    currentSlot=ipjValue;
                }
            }
        }
    }
    if(event.type==SDL_MOUSEBUTTONUP)
    {
        //If the left mouse button was pressed
        if( event.button.button == SDL_BUTTON_LEFT )
        {
            playerPickSlot();
        }
    }
}


void getPlayerGameDoneInput()
{
    //If the left mouse button was pressed
    if(event.type == SDL_MOUSEBUTTONUP)
    {
        if( event.button.button == SDL_BUTTON_LEFT )
        {
            startGame();
        }
    }
    //If a key was pressed
    if( event.type == SDL_KEYUP )
    {
        switch(event.key.keysym.sym)
        {
            case SDLK_RETURN:
            case SDLK_SPACE:
            case SDLK_x:
            case SDLK_y:
                startGame();
            break;
            case SDLK_ESCAPE:
                goToMenu();
            break;

            default:

            break;
        }
    }
    //If the user has Xed out the window
    if( event.type == SDL_QUIT )
    {
        //Quit the program
        goToMenu();
    }
    fakeTimer=0;
}



void computerAi()
{
    int possibleSquare;
    int iiiLoop=0;
    if(isPlayerTurn==false)
    {
        possibleSquare=getWinningSquare(1);
        //if the computer is not about to win
        if(possibleSquare<0)
        {
            //select a diffrent square
            //if the player is about to win
            possibleSquare=getWinningSquare(0);
            if(possibleSquare>0)
            {
                gameSquare[possibleSquare]=1;
                if(getVictory(1))
                {
                    gameCompleted=true;
                    updateOpponentText(opponentBlock[random(0,oTextArraySize)]+" "+opponentWin[random(0,oTextArraySize)]);
                    computerWins+=1;
                    isPlayerTurn=true;
                }
                else
                {

                    updateOpponentText(opponentBlock[random(0,oTextArraySize)]);
                    //checks if the game is over...
                    checkForCatGame();
                }
                //the computer blocks the player's winning square
                isPlayerTurn=true;
            }
            else
            {
                if(random(0,100)<33)
                {
                    //selects the 1st open square, starting from the top of board
                    for(iiiLoop=0;iiiLoop<9;iiiLoop++)
                    {
                        if(gameSquare[iiiLoop]==-1)
                        {
                            gameSquare[iiiLoop]=1;
                            isPlayerTurn=true;
                            //checks if the game is over...
                            checkForCatGame();
                            return;
                        }
                    }
                }
                else
                {
                    if(random(0,100)<50)
                    {
                        //choose the middle square
                        if(gameSquare[4]==-1)
                        {
                            gameSquare[4]=1;
                            isPlayerTurn=true;
                            //checks if the game is over...
                            checkForCatGame();
                            return;
                        }
                    }
                    else
                    {
                        //picks a random square
                        nRanSquare=random(0,9);
                        if(gameSquare[nRanSquare]==-1)
                        {
                            gameSquare[nRanSquare]=1;
                            isPlayerTurn=true;
                            //checks if the game is over...
                            checkForCatGame();
                            return;
                        }
                        else
                        {
                            //selects the 1st open square, starting from the bottom of board
                            for(iiiLoop=8;iiiLoop>0;iiiLoop--)
                            {
                                if(gameSquare[iiiLoop]==-1)
                                {
                                    gameSquare[iiiLoop]=1;
                                    isPlayerTurn=true;
                                    //checks if the game is over...
                                    checkForCatGame();
                                    return;
                                }
                            }
                        }
                    }
                }
            }
        }
        else
        {
            if(gameSquare[possibleSquare]<0)
            {
                gameSquare[possibleSquare]=1;
                //the computer wins by picking the winning square
                gameCompleted=true;
                updateOpponentText(opponentWin[random(0,oTextArraySize)]);
                computerWins+=1;
                isPlayerTurn=true;
            }
        }
    }
}


void drawCredits()
{
    //Fill the screen white
    SDL_FillRect( screen, &screen->clip_rect, SDL_MapRGB( screen->format, 0xFF, 0xFF, 0xFF ) );
    apply_surface( 0, 0, menuBackground, screen );

    //Apply the credits
    apply_surface( ( SCREEN_WIDTH - creditsCaptionDisplay->w ) / 2, 100, creditsCaptionDisplay, screen );
    apply_surface( ( SCREEN_WIDTH - creditsNameDisplay->w ) / 2, 150, creditsNameDisplay, screen );
}


void drawEverything()
{
    int ipjValue=0;
    //Fill the screen white
    SDL_FillRect( screen, &screen->clip_rect, SDL_MapRGB( screen->format, 255, 25, 255 ) );
    //Apply the background
    apply_surface( 0, 0, background, screen );
    int boardStartX=00+SCREEN_WIDTH/2 -gameboard->w/2;
    int boardStartY=16+SCREEN_HEIGHT/2 -gameboard->h/2;
    apply_surface(boardStartX, boardStartY, gameboard, screen );
    for(int iLoop=0;iLoop<3;iLoop+=1)
    {
        for(int jLoop=0;jLoop<3;jLoop+=1)
        {
            ipjValue=iLoop+(jLoop*3);
            gameToken=gameSquare[ipjValue];
            if(currentSlot==ipjValue)
            {
                drawRectangle(boardStartX+( iLoop *(BOARD_PXWIDTH/3)), boardStartY+(jLoop*(BOARD_PXHEIGHT/3)),
                              boardStartX +((iLoop+1)*(BOARD_PXWIDTH/3)),boardStartY +( (jLoop+1)*(BOARD_PXHEIGHT/3)),SDL_MapRGB( screen->format,  32,32,32) );
            }
            if((gameToken>=0)&&(gameToken<2))
            {
                apply_surface(boardStartX+(iLoop*(BOARD_PXWIDTH/3)), SCREEN_HEIGHT/2-BOARD_PXHEIGHT/3+(jLoop*(BOARD_PXHEIGHT/3)), gameartImg, screen, &gameart[ gameToken ] );
            }
        }
    }
    apply_surface( ( SCREEN_WIDTH - messageWinsAndLoses ->w-20 ),SCREEN_HEIGHT*1/9, messageWinsAndLoses , screen );
    apply_surface( ( SCREEN_WIDTH - messageFromOpponentDisplay->w ) / 2,SCREEN_HEIGHT*2/9, messageFromOpponentDisplay, screen );
    apply_surface( ( SCREEN_WIDTH - message->w ) / 2, SCREEN_HEIGHT*8/9, message, screen );
}


void drawGameMenu()
{
    int imenu;
    //Fill the screen white
    SDL_FillRect( screen, &screen->clip_rect, SDL_MapRGB( screen->format, 0xFF, 0xFF, 0xFF ) );
    //Apply the background
    apply_surface( 0, 0, menuBackground, screen );
    for(imenu=0;imenu<menuTextAmount;imenu++)
    {
        apply_surface( ( SCREEN_WIDTH - menuMessage[imenu]->w ) / 2, SCREEN_HEIGHT/3+(imenu*SCREEN_HEIGHT/6), menuMessage[imenu], screen );
    }
}


bool creditsLoop(bool quit)
{
    //While there's events to handle
    while( SDL_PollEvent( &event ) )
    {
        //If a key was pressed
        if( event.type == SDL_KEYUP )
        {
            goToMenu();
        }
        //If the user has Xed out the window
        if( event.type == SDL_QUIT )
        {
            goToMenu();
        }
    }
    //Processing
    /*
    creditsECounter++;
    if(creditsECounter>=creditsExtraLoopage)
    {
        creditsTimeCounter++;
        creditsECounter=0;
    }
    creditsTimeCounter++;
    if(creditsTimeCounter>=creditsTiming)
    {
        creditsECounter=0;
        creditsAlpha-=5;
        //creditsTimeCounter=0;
    }
    */
    creditsAlpha-=2;
    if(creditsAlpha<=0)
    {
        creditsECounter=0;
        creditsNamesCounter++;
        creditsTimeCounter=0;
        creditsAlpha = SDL_ALPHA_OPAQUE;
    }
    if(creditsNamesCounter>=creditsMaxNumber[creditsCaptionCounter])
    {
        creditsCaptionCounter++;
        creditsNamesCounter=0;
    }
    if(creditsCaptionCounter>=5)
    {
        goToMenu();
    }

    //drawing
    updateCreditsNameText();
    //Update the screen
    drawCredits();
    return quit;
}


bool mainGameLoop(bool quit)
{
    if((boardIsFull()==false)&&(gameCompleted==false))
    {
        //create a buffer
        //Show the message on the screen
        //Render the text
        //put message in buffer
        //Show the message on the screen
        char buffer[ 32 ];
        sprintf( buffer, "Position: %d", currentSlot+1 );
        message = TTF_RenderText_Solid( font, buffer, textColor );
        if(isPlayerTurn)
        {
            //While there's events to handle
            while( SDL_PollEvent( &event ) )
            {
                getPlayerInput();
                //If the user has Xed out the window
                if( event.type == SDL_QUIT )
                {
                    goToMenu();
                }
            }
        }
        else
        {
            computerAi();
        }
        //Update the screen
        drawEverything();
    }
    else
    {
        updateWinsText();
        message = TTF_RenderText_Solid( font," Press START to Begin a New Game!", textColor );
        //While there's events to handle
        while( SDL_PollEvent( &event ) )
        {
            getPlayerGameDoneInput();
        }
        //Update the screen
        drawEverything();
    }
    return quit;
}

bool mainMenuLoop(bool quit)
{
    //While there's events to handle
    while( SDL_PollEvent( &event ) )
    {
        quit=getGameMenuInput(quit);
        //If the user has Xed out the window
        if( event.type == SDL_QUIT )
        {
            //Quit the program
            quit = true;
        }
    }
    //Update the screen
    drawGameMenu();
    return quit;
}


int main( int argc, char* args[] )
{
    //Quit flag
    bool quit = false;

    //Initialize
    if( init() == false )
    {
        return 1;
    }
    SDL_ShowCursor(0);
    //Load the files
    if( load_files() == false )
    {
        return 1;
    }
    int startTicks;
    playBackgroundMusic();
    //gameart range for the X
    gameart[ 0 ].x = 0;
    gameart[ 0 ].y = 0;
    gameart[ 0 ].w = 32;
    gameart[ 0 ].h = 32;

    //gameart range for the O
    gameart[ 1 ].x = 32;
    gameart[ 1 ].y = 0;
    gameart[ 1 ].w = 64;
    gameart[ 1 ].h = 32;

    //gameart range for the You Win
    gameart[ 2 ].x = 0;
    gameart[ 2 ].y = 32;
    gameart[ 2 ].w = 32;
    gameart[ 2 ].h = 64;

    //gameart range for the You Lose
    gameart[ 3 ].x = 32;
    gameart[ 3 ].y = 32;
    gameart[ 3 ].w = 64;
    gameart[ 3 ].h = 64;

    for(int imenu=0;imenu<menuTextAmount;imenu++)
    {
        menuMessage[imenu]= TTF_RenderText_Solid( font,(menuMessageText[imenu]).c_str(), whiteColor );
    }
    //Apply the background
    apply_surface( 0, 0, background, screen );
    startGame();
    //While the user hasn't quit
    while( quit == false )
    {
        startTicks = SDL_GetTicks();
        switch(CURRENT_AREA)
        {
            case 0:
                quit=mainMenuLoop(quit);
            break;

            case 1:
                quit=creditsLoop(quit);
            break;

            case 2:
                quit=mainGameLoop(quit);
            break;

            default:
                quit=true;
            break;
        }
        if( SDL_Flip( screen ) == -1 )
        {
            return 1;
        }
        if(  SDL_GetTicks()-startTicks < 1000 / FRAMES_PER_SECOND  )
        {
             //Sleep the remaining frame time
             SDL_Delay( ( 1000 / FRAMES_PER_SECOND ) - (SDL_GetTicks()-startTicks) );
        }
    }
    //Free surfaces, fonts and sounds
    //then quit SDL_mixer, SDL_ttf and SDL
    clean_up();
    return 0;
}
