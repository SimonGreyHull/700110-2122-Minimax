#pragma region includes
/*
GLEW is the OpenGL Extension Wrangler Library which is a simple tool that helps C/C++ developers
initialize extensions and write portable applications.
http://glew.sourceforge.net/
*/
#include <GL/glew.h>

/*
Dear ImGui is a bloat-free graphical user interface library for C++.
https://github.com/ocornut/imgui

For imgui examples see https://github.com/ocornut/imgui/blob/v1.74/examples/example_glfw_opengl3/main.cpp
*/
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

/*
GLFW is an Open Source, multi-platform library for OpenGL, OpenGL ES and Vulkan development on the desktop. 
It provides a simple API for creating windows, contexts and surfaces, receiving input and events.
https://www.glfw.org/
*/
#include <GLFW/glfw3.h>

/*
OpenGL Mathematics (GLM) is a header only C++ mathematics library for graphics software based on the 
OpenGL Shading Language (GLSL) specifications.
https://github.com/g-truc/glm
*/
#include "glm/glm.hpp";
#include "glm/gtc/type_ptr.hpp";
#include "glm/gtc/matrix_transform.hpp";


#include <iostream>
#include <string>

#pragma endregion includes

#pragma region game logic

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

/*
Dirty static variables to avoid passing things around everywhere.
GENERALLY NOT GOOD PRACTICE!
*/

static int numberOfColumns = 3;
static int numberOfRows = 3;
static int numberOfPlayers = 2;
static int winCondition = 3;
static int currentPlayer = 0;

static int boardState[MAX_ROWS][MAX_COLUMNS];

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

/*
    Player object hold information about the player, such as the player ID, colour and whether they are AI or not
*/

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

static auto getBoardIndex(int screenWidth, int screenHeight, int xMousePosition, int yMousePosition) {
    struct boardIndex { int column; int row; };
    
    return boardIndex{ (int)(numberOfColumns * xMousePosition / screenWidth), 
    (int) (numberOfRows * (screenHeight - yMousePosition) / screenHeight) };
}


static void ResetGame()
{
    for (int rowIndex = 0; rowIndex < MAX_ROWS; rowIndex++)
    {
        for (int columnIndex = 0; columnIndex < MAX_COLUMNS; columnIndex++)
        {
            boardState[rowIndex][columnIndex] = 0;
        }
    }
}

static bool CheckForWin()
{
    /* This is suboptimal. 
    We should really only check around the space that has just been clicked,
    but instead we'll check every possible win on the current board!
    */

    for (int rowIndex = 0; rowIndex < numberOfRows; rowIndex++)
    {
        for (int colIndex = 0; colIndex < numberOfColumns; colIndex++)
        {
            if (boardState[rowIndex][colIndex] == 0)
            {
                continue;
            }

            // horizontal win --
            for (int winConIndex = 0; winConIndex < winCondition; winConIndex++)
            {

            }

            // vertical win |

            // diagonal win /

            // diagonal win \
        
        }
    }

    return false;
}

static void goToNextPlayer()
{
    currentPlayer++;
    if (currentPlayer == numberOfPlayers)
    {
        currentPlayer = 0;
    }
}

static bool TakeHumanTurn(int pRowIndex, int pColumnIndex)
{
    if (boardState[pRowIndex][pColumnIndex] != 0)
    {
        return false;
    }

    if (players[currentPlayer].isAI)
    {
        return false;
    }

    boardState[pRowIndex][pColumnIndex] = players[currentPlayer].ID();
    return true;
}

static void TakeAITurn()
{
    AddToLog("AI player " + std::to_string(players[currentPlayer].ID()) + " took turn");
}

#pragma endregion game logic


#pragma region window setup

void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
    ImGuiIO& io = ImGui::GetIO();
    if (io.WantCaptureMouse)
    {
        return;
    }

    if (players[currentPlayer].isAI)
    {
        return;
    }

    if (!(button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS))
    {
        return;
    }

    double xPosition, yPosition;
    int windowWidth, windowHeight;
    int closestRowIndex, closestColumnIndex;
    glfwGetWindowSize(window, &windowWidth, &windowHeight);
    glfwGetCursorPos(window, &xPosition, &yPosition);
    auto result = getBoardIndex(windowWidth, windowHeight, xPosition, yPosition);

    if (boardState[result.row][result.column] != 0)
    {
        return;
    }

    boardState[result.row][result.column] = players[currentPlayer].ID();

    CheckForWin();
    goToNextPlayer();
}

GLFWwindow* setupWindow()
{
    GLFWwindow* window;

    const char* glsl_version = "#version 130";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

    /* Initialize the library */
    if (!glfwInit())
        return 0;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 480, "Hello, GLFW!", NULL, NULL);

    if (!window)
    {
        glfwTerminate();
        return 0;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui::StyleColorsDark();

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);

    glfwSetMouseButtonCallback(window, mouseButtonCallback);

    return window;
}

void cleanUp(GLFWwindow * window)
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();
}

#pragma endregion window setup

#pragma openGL code


static GLuint setupOpenGL()
{
    // load vertex buffer data to graphics card
    GLuint bufferID;
    glGenBuffers(1, &bufferID);
    glBindBuffer(GL_ARRAY_BUFFER, bufferID);
    float vertices[8] = {
        -0.5f, -0.5f, 
        0.5f, -0.5f,
        0.5f, 0.5f,
        -0.5f, 0.5f
    };
    glBufferData(GL_ARRAY_BUFFER, 8 * sizeof(float), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, false, 2 * sizeof(float), 0);
    glEnableVertexAttribArray(0);
    
    // write, compile and link some shaders
    std::string vertexShader =
        "#version 330 core\n"
        "layout(location = 0) in vec2 iPosition;\n"
        "uniform vec2 uTranslation = vec2(0.0, 0.0);\n"
        "uniform mat4 uProjection;\n"
        "void main(){\n"
        "   gl_Position = uProjection * vec4((iPosition + uTranslation).xy, 0.0, 1.0);\n"
        "}\n";

    std::string fragmentShader =
        "#version 330 core\n"
        "layout(location = 0) out vec4 oColour;\n"
        "uniform vec4 uColour = vec4(1.0, 0.0, 1.0, 1.0);\n"
        "void main(){\n"
        "   oColour = uColour;\n"
        "}\n";
    
    GLuint vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
    const char* shader = vertexShader.c_str();
    glShaderSource(vertexShaderID, 1, &shader, nullptr);
    glCompileShader(vertexShaderID);

    int result;
    glGetShaderiv(vertexShaderID, GL_COMPILE_STATUS, &result);

    GLuint fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
    shader = fragmentShader.c_str();
    glShaderSource(fragmentShaderID, 1, &shader, nullptr);
    glCompileShader(fragmentShaderID);

    GLuint shaderProgramID = glCreateProgram();

    glAttachShader(shaderProgramID, vertexShaderID);
    glAttachShader(shaderProgramID, fragmentShaderID);
    glLinkProgram(shaderProgramID);
    glValidateProgram(shaderProgramID);
    glUseProgram(shaderProgramID);

    glDeleteShader(vertexShaderID);
    glDeleteShader(fragmentShaderID);

    return shaderProgramID;
}

#pragma endregion openGL code

int main(void)
{
    GLFWwindow * window = setupWindow(); 

    if (glewInit() != GLEW_OK)
    {
        std::cout << "Error initialising GLEW" << std::endl;
    }

    int shaderProgramID = setupOpenGL();

    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        ImGui::Begin("Hello, IMGUI!");                          // Create a window called "Hello, world!" and append into it.
        
        ImGuiStyle& style = ImGui::GetStyle();
        style.Colors[ImGuiCol_FrameBg] = ImVec4(0.16f, 0.29f, 0.48f, 0.54f);

        ImGui::Text("Game Log");
        for (int i = 0; i < LOG_LENGTH; i++)
        {
            ImGui::Text(guiLog[i].c_str());
        }

        if (ImGui::SliderInt("Players", &numberOfPlayers, MIN_PLAYERS, MAX_PLAYERS))
        {
            ResetGame();
        }
        ImGui::Text("AI or Human Players");
        for (int i = 0; i < numberOfPlayers; i++)
        {
            style.Colors[ImGuiCol_FrameBg].x = players[i].Colour()[0];
            style.Colors[ImGuiCol_FrameBg].y = players[i].Colour()[1];
            style.Colors[ImGuiCol_FrameBg].z = players[i].Colour()[2];
            style.Colors[ImGuiCol_FrameBg].w = players[i].Colour()[3];

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

        }

        style.Colors[ImGuiCol_FrameBg] = ImVec4(0.16f, 0.29f, 0.48f, 0.54f);
        if(ImGui::SliderInt("Columns", &numberOfColumns, MIN_COLUMNS, MAX_COLUMNS))
        {
            ResetGame();
        }

        if (ImGui::SliderInt("Rows", &numberOfRows, MIN_ROWS, MAX_ROWS))
        {
            ResetGame();
        }

        if (ImGui::SliderInt("Win Condition", &winCondition, MIN_WIN_CONDITION, MAX_WIN_CONDITION))
        {
            ResetGame();
        }

        if (ImGui::Button("Reset Game"))
        {
            ResetGame();
        }

        ImGui::End();

        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);
        
        GLint colourLocation = glGetUniformLocation(shaderProgramID, "uColour");
        GLint translationLocation = glGetUniformLocation(shaderProgramID, "uTranslation");
        GLint projectionLocation = glGetUniformLocation(shaderProgramID, "uProjection");

        glm::vec2 currentTranslation(0.6f, 0.6f);
        glm::vec2 xTranslation(1.2f, 0.0f);
        glm::vec2 yTranslation(0.0f, 1.2f);
        glm::vec2 xReset(-1.2f * numberOfColumns, 0.f);

        glm::mat4 projection = glm::ortho(0.0f, 1.2f * numberOfColumns, 0.0f, 1.2f * numberOfRows, -1.f, 1.f);

        glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, glm::value_ptr(projection));

        float black[4] = { 0.f, 0.f, 0.f, 1.f };

        double xPosition, yPosition;
        int windowWidth, windowHeight;
        int closestRowIndex, closestColumnIndex;
        glfwGetWindowSize(window, &windowWidth, &windowHeight);
        glfwGetCursorPos(window, &xPosition, &yPosition);
        auto result = getBoardIndex(windowWidth, windowHeight, xPosition, yPosition);

        closestRowIndex = result.row;
        closestColumnIndex = result.column;

        for (int rowIndex = 0; rowIndex < numberOfRows; rowIndex++)
        {
            for (int columnIndex = 0; columnIndex < numberOfColumns; columnIndex++)
            {
                if (boardState[rowIndex][columnIndex] == 0)
                {
                    if (!(rowIndex == closestRowIndex && columnIndex == closestColumnIndex))
                    {
                        glUniform4fv(colourLocation, 1, black);
                    }
                    else
                    {
                        glUniform4fv(colourLocation, 1, players[currentPlayer].Colour());
                    }
                }
                else
                {
                    int playerIndex = 0;
                    for (; playerIndex < numberOfPlayers; playerIndex++)
                    {
                        if (boardState[rowIndex][columnIndex] == players[playerIndex].ID())
                        {
                            break;
                        }
                    }
                    glUniform4fv(colourLocation, 1, players[playerIndex].Colour());
                }
                glUniform2fv(translationLocation, 1, glm::value_ptr(currentTranslation));
                glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
                currentTranslation += xTranslation;
            }
            currentTranslation += xReset;
            currentTranslation += yTranslation;
        }

        ImGui::Render();
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
        
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    cleanUp(window);

    return 0;
}