# 700110-2122-Minimax

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

Run the code, explore the interface and play the game. You can do this in groups if you like. It is important to try to build a community amongst students on the module!

You will be building on this code to complete the application and create AI players to play against.

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

There is also a log facility to get (a limited amount of) feedback from the application - similar to how you would do in the console. This could be improved by creating a more object oriented solution.

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

## 3. Fix the win conditions

When playing the game you might have noticed that sometimes a player does not win when they should.

The CheckForWin() method checks the entire board to try to find a winning player - but at the momemnt it only checks for horizontal wins. The code that does this is below.

```cpp
    // horizontal win --
    bool isWin;
    for (int rowIndex = 0; rowIndex < MAX_ROWS; rowIndex++)
    {
        for (int colIndex = 0; colIndex <= MAX_COLUMNS - winCondition; colIndex++)
        {
            if (boardState[rowIndex][colIndex] == 0)
            {
                continue;
            }

            isWin = true;
            for (int winConIndex = 0; winConIndex < winCondition - 1; winConIndex++)
            {
                if(boardState[rowIndex][colIndex + winConIndex] != boardState[rowIndex][colIndex + winConIndex + 1])
                {
                    isWin = false;
                    break;
                }
            }

            if (isWin) { return boardState[rowIndex][colIndex];; }
        }
    }
```

The nested loop iterates over every possible element in the 2 dimensional board state that could be the left most element on a winning row, so for example in a 3x3 board when three in a row is required to win only the elements on the left most column are checked.

```cpp
    for (int rowIndex = 0; rowIndex < MAX_ROWS; rowIndex++)
    {
        for (int colIndex = 0; colIndex <= MAX_COLUMNS - winCondition; colIndex++)
        {
            // other code
        }
    }
```

For each of these elements first a check is performed to see it the element is 0. If it is then no player has played in this spot and so this is not part of a winning row. Next it is first assumed that the element at [rowIndex][colIndex] win will be found, then the elements to the right are checked to see if they are the same.

```cpp

        if (boardState[rowIndex][colIndex] == 0)
        {
            continue;
        }

```

Next, we assume that the current element is the first element of a horizontal winning row by setting isWin to true - then we look at the elements to the right to see if this is the case. If we find that this is not a winning row we set isWin to false, and break out of the rest of the loop.

```cpp
        isWin = true;
        for (int winConIndex = 0; winConIndex < winCondition - 1; winConIndex++)
        {
            if(boardState[rowIndex][colIndex + winConIndex] != boardState[rowIndex][colIndex + winConIndex + 1])
            {
                isWin = false;
                break;
            }
        }
```

If isWin is still true a win is found, and so the method returns the corresponding boardState element which is the ID of the player who has won.

```cpp
    if (isWin) { return boardState[rowIndex][colIndex]; }
```

### Know your next commit!

Remaining focused and making regular commits to source control is a habit that is difficult to build. In order to try and cultivate that we will promote an approach of knowing what your next commit will be. In this case, it will be to add a vertical win condition. To do this, in the CheckForWin method, after we check for horizontal wins, but before we return 0 add a loop that will loop through all of the columns.

```cpp
    for (int colIndex = 0; colIndex < MAX_COLUMNS; colIndex++)
    {
        // more code to be added here
    }
```

Inside that loop add a second loop, that will iterate over any row that can be the top most element of a vertical win.

```cpp

    for (int rowIndex = 0; rowIndex <= MAX_ROWS - winCondition; rowIndex++)
    {
        // more code to be added here
    }

```

Check to see if the element is 0. If it is then no player has played in this spot and so this is not part of a winning row.

```cpp
    if (boardState[rowIndex][colIndex] == 0)
    {
        continue;
    }
```

Next, assume that the current element is the top most element of a vertical winning column by setting isWin to true - then we look at the elements below to see if this is the case. If we find that this is not a winning column we set isWin to false, and break out of the rest of the loop.

```cpp
    isWin = true;
    for (int winConIndex = 0; winConIndex < winCondition - 1; winConIndex++)
    {
        if (boardState[rowIndex + winConIndex][colIndex] != boardState[rowIndex + winConIndex + 1][colIndex])
        {
            isWin = false;
            break;
        }
    }
```
If isWin is still true a win is found, and so the method returns the corresponding boardState element which is the ID of the player who has won.
```cpp
    if (isWin) { return boardState[rowIndex][colIndex]; }
```

### Test and commit your code to source control

Test your code is working as expected, then commit your code with an appropriate commit message like "Added vertical win condition"

### Know your next commit!

The next piece of functionality we need to add is the diagonal win condition going from the top left to the bottom right.

Replicate the previous steps, however you will have to think carefully about which "starting element" cells to check, and which other elements you need to check to determine a win.

### Test and commit your code to source control

Test your code is working as expected, then commit your code with an appropriate commit message like "Added diagonal \ win condition"

### Know your next commit!

The next piece of functionality we need to add is the diagonal win condition going from the top right to the bottom left.

### Test and commit your code to source control

Test your code is working as expected, then commit your code with an appropriate commit message like "Added diagonal / win condition"

Again replicate the previous steps, however you will have to think carefully about which "starting element" cells to check, and which other elements you need to check to determine a win.

### Know your next commit!

Add a tied game condition. You can do this by checking to see if there are any unoccupied spaces remaining. If there is a draw add a message to the log using the AddLogEntry method.

### Test and commit your code to source control

Test your code is working as expected, then commit your code with an appropriate commit message like "Added code to detect a tied game"

## 4. Add AI players

The data has been set up to enable a player to be controlled by a human or an AI. The next task is to toggle between the two. The purpose of this part of the lab is to give you some experience of ImGui.

### Know your next commit!

```cpp
    
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

In the renderImGui method

### Test and commit your code to source control

Test your code is working as expected, then commit your code with an appropriate commit message like "Added ability to toggle AI players to GUI"

## 5. Write a basic AI player

### Know your next commit!

### Commit your code to source control with an appropriate message like ""

## 6. Write a minimax AI player

A minimax

Initially, you 

you will need to add

https://www.youtube.com/watch?v=trKjYdBASyQ

Unfortunately, writing the code for your AI player was probably more fun than writing

### Know your next commit!

### Commit your code to source control with an appropriate message like ""

## 7. Add more AI players

In our game you can add more players. Adapt your

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
### Know your next commit!

### Commit your code to source control with an appropriate message like ""
