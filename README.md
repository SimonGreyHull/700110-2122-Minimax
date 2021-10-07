# 7001100-Minimax

## Introduction

The purpose of this lab is to introduce you to the tools and libraries that we will use in this module. We will also be writing an AI to play TicTacToe using the Minimax algorithm.

We will be using OpenGL to deal with a lot of the rendering for the labs and coursework. You do not need to have a comprehensive understanding of OpenGL and all the graphics you need will be given to you in an easily understandable and reusable way.

GLEW is the OpenGL Extension Wrangler Library which is a simple tool that helps C/C++ developers initialize extensions and write portable applications.
You can find out more about GLEW at http://glew.sourceforge.net/

Our window is provided using GLFW. GLFW is an Open Source, multi-platform library for OpenGL, OpenGL ES and Vulkan development on the desktop. It provides a simple API for creating windows, contexts and surfaces, receiving input and events.
You can find out more about GLFW at https://www.glfw.org/

Our user interface is provided using Dear ImGui. Dear ImGui is a bloat-free graphical user interface library for C++. For the coursework you may want to create your own custom user interfaces, so it is worth trying to understand how Dear ImGui works.
You can find out more about Dear ImGui at https://github.com/ocornut/imgui

A lot of the functionality for TicTacToe is already provided. To begin with check out and compile the code. You should see a semi-functional game of TicTacToe.

## 1. Explore the existing functionality

## 2. Look at the data

An important part of programming is understanding what information is important and how it is stored. In the code there is a region called data. The first part of this code includes a section of constants. They are appropriately named, and so are hopefully self explanatory. Mostly they set limits for other variables.

```cpp

/*
    Constant values to avoid magic numbers
*/

const int MIN_COLUMNS = 3;
const int MAX_COLUMNS = 5;
const int MIN_ROWS = 3;
const int MAX_ROWS = 5;
const int MIN_PLAYERS = 2;
const int MAX_PLAYERS = 5;
const int MIN_WIN_CONDITION = 3;
const int MAX_WIN_CONDITION = 5;

```

Next there are some static variables. These are effectively global variables that can be accessed from anywhere. This is generally not good practice. They have appropriate variable names and so should be self-explanatory. They allow us to change various things about the game, such as the number of columns, or the number of tiles in a row required to win, or the index of the current player.

The board state itself is represented as two dimensional array of integers. In this array a value of 0 means a space is currently unoccupied. Otherwise the ID of the specific player is used (more on the player later).

There is also a log made up of strings that is displayed in the Gui to give feedback.

```cpp

static int numberOfColumns = 3;
static int numberOfRows = 3;
static int numberOfPlayers = 2;
static int winCondition = 3;

static int currentPlayer = 0;

static int boardState[MAX_ROWS][MAX_COLUMNS];

```

The game will allow for multiple players that can be played using a human controller or an AI. To allow for this a player class is created that stores a colour used to represent the player in the game and the GUI. An ID which identifies the player in the game board and a boolean value to denote if the player is an AI player or not.

Up to 5 players are stored in an array of players.

```cpp
class Player
{
private:
    float _colour[4];
    unsigned short _ID;
public:
    bool isAI;
    Player(unsigned short pID, ImVec4 pColour) : _ID(pID), isAI(false) {
        _colour[0] = pColour.x;
        _colour[1] = pColour.y;
        _colour[2] = pColour.z;
        _colour[3] = pColour.w;
    }
    unsigned short ID() { return _ID; }
    float * Colour() { return _colour; }
};


static Player players[] = {
    { 1, ImVec4(1.0, 0.0, 0.0, 1.0) },
    { 2, ImVec4(0.0, 1.0, 0.0, 1.0) },
    { 3, ImVec4(0.0, 0.0, 1.0, 1.0) },
    { 4, ImVec4(1.0, 1.0, 0.0, 1.0) },
    { 5, ImVec4(1.0, 0.0, 1.0, 1.0) }, };
```

There is also a log facility . This could be improved by creating a more object oriented solution.

```cpp

const int LOG_LENGTH = 8;
static std::string guiLog[] = { "", "", "", "", "", "", "", "" };

static void AddToLog(std::string pLogEntry)
{
    for (int i = 1; i < LOG_LENGTH; i++)
    {
        guiLog[i - 1] = guiLog[i];
    }
    guiLog[LOG_LENGTH - 1] = pLogEntry;
}

static void DisplayLog()
{
    ImGui::Text("Game Log");
    for (int i = 0; i < LOG_LENGTH; i++)
    {
        ImGui::Text(guiLog[i].c_str());
    }
}

```


## 3. Fix the win condition

## 4. Write a random AI player

## 5. Write a minimax AI player

## 6. Add more AI players

```cpp
            std::string label = "";
            if (players[i].isAI)
            {
                label = "AI Player " + std::to_string(i + 1);
                ImGui::Checkbox(label.c_str(), &players[i].isAI);
                if (currentPlayer == i)
                {
                    ImGui::SameLine();
                    if (ImGui::Button("Take Turn"))
                    {
                        TakeAITurn();
                        CheckForWin();
                        goToNextPlayer();
                    }
                }
            }
            else
            {
               label = "Human Player " + std::to_string(i + 1);
               ImGui::Checkbox(label.c_str(), &players[i].isAI);
               if (currentPlayer == i)
               {
                   ImGui::SameLine();
                   ImGui::Text("Your Turn");
               }
            }
```
