#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb_image.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

#include <shader.h>
#include <camera.h>
#include <model.h>

// Function declerations
void ProcessInput(GLFWwindow *window);
void FrameBufferSizeCallback(GLFWwindow *window, int width, int height);
void MouseCallback(GLFWwindow *window, double xPosIn, double yPosIn);
void ScrollCallback(GLFWwindow *window, double xOffset, double yOffset);
unsigned int LoadTexture(const char *path);
unsigned int LoadCubemap(vector<std::string> faces);

// Settings
const unsigned int WINDOW_WIDTH = 1280;
const unsigned int WINDOW_HEIGHT = 720;

// Camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
bool isFirstMouseInput = true;
float lastMouseX = (float)WINDOW_WIDTH / 2.0f;
float lastMouseY = (float)WINDOW_HEIGHT / 2.0f;

// Timing
float deltaTime = 0.0f;     // Time between current frame and last frame
float lastFrameTime = 0.0f; // Time of last frame

int main()
{
    // Initialize and specify GLFW settings
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Create the window and set it to the current context
    GLFWwindow *window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    // Set callback functions
    glfwSetFramebufferSizeCallback(window, FrameBufferSizeCallback);
    glfwSetCursorPosCallback(window, MouseCallback);
    glfwSetScrollCallback(window, ScrollCallback);

    // Tell the window to disable the cursor
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // Load GLAD before using OpenGL functions
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // Configure global OpenGL state
    glEnable(GL_DEPTH_TEST);


    // Build and compile the shader program
    Shader shader("shaders/3.9.2.default.vs", "shaders/3.9.2.default.fs");
    Shader normalShader("shaders/3.9.2.normal_visualization.vs", 
                        "shaders/3.9.2.normal_visualization.fs", 
                        "shaders/3.9.2.normal_visualization.gs");


    stbi_set_flip_vertically_on_load(true);
    Model backpack("models/backpack/backpack.obj");

    // uncomment to enable wireframes
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    // Main Loop
    while (!glfwWindowShouldClose(window))
    {
        // Time
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrameTime;
        lastFrameTime = currentFrame;

        // Input Handling
        ProcessInput(window);

        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // configure transformation matrices
        glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)WINDOW_WIDTH / (float)WINDOW_HEIGHT, 1.0f, 100.0f);
        glm::mat4 view = camera.GetViewMatrix();;
        glm::mat4 model = glm::mat4(1.0f);
        shader.Use();
        shader.SetMat4x4("projection", projection);
        shader.SetMat4x4("view", view);
        shader.SetMat4x4("model", model);

        // draw model as usual
        backpack.Draw(shader);

        // then draw model with normal visualizing geometry shader
        normalShader.Use();
        normalShader.SetMat4x4("projection", projection);
        normalShader.SetMat4x4("view", view);
        normalShader.SetMat4x4("model", model);

        backpack.Draw(normalShader);

        // Swaps the 2d buffer that contains color values for each pixel
        glfwSwapBuffers(window);
        glfwPollEvents(); // Checks for events being triggered (input)
    }

    glfwTerminate(); // Cleanup GLFW resources
    return 0;
}

/// <summary>
/// Processes all GLFW Input
/// </summary>
void ProcessInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
    }

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    {
        camera.ProcessKeyboard(FORWARD, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    {
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    {
        camera.ProcessKeyboard(LEFT, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    {
        camera.ProcessKeyboard(RIGHT, deltaTime);
    }
}

/// <summary>
/// Called whenever the window size is changed.
/// </summary>
void FrameBufferSizeCallback(GLFWwindow *window, int width, int height)
{
    glViewport(0, 0, width, height);
}

/// <summary>
/// Called whenever the mouse moves in the window.
/// </summary>
void MouseCallback(GLFWwindow *window, double xPosIn, double yPosIn)
{
    float xPos = static_cast<float>(xPosIn);
    float yPos = static_cast<float>(yPosIn);

    if (isFirstMouseInput)
    {
        lastMouseX = xPos;
        lastMouseY = yPos;
        isFirstMouseInput = false;
    }

    float xOffset = (float)xPos - lastMouseX;
    float yOffset = lastMouseY - (float)yPos;
    lastMouseX = (float)xPos;
    lastMouseY = (float)yPos;

    camera.ProcessMouseMovement(xOffset, yOffset);
}

/// <summary>
/// Called whenever scrolling input is received from the mouse.
/// </summary>
void ScrollCallback(GLFWwindow *window, double xOffset, double yOffset)
{
    camera.ProcessMouseScroll(static_cast<float>(yOffset));
}

unsigned int LoadTexture(const char *path)
{
    unsigned int textureID;
    glGenTextures(1, &textureID);
    // load and generate the texture
    int width, height, numChannels;
    unsigned char *data = stbi_load(path, &width, &height, &numChannels, 0);
    if (data != nullptr)
    {
        GLenum format;
        if (numChannels == 1)
        {
            format = GL_RED;
        }
        else if (numChannels == 3)
        {
            format = GL_RGB;
        }
        else if (numChannels == 4)
        {
            format = GL_RGBA;
        }

        glBindTexture(GL_TEXTURE_2D, textureID);

        // glTexImage2D parameters:
        // 1: texture target, binds a texture on currently bound texture object at the same target
        // 2: specifies mipmap level if we want to set each mipmap manually
        // 3: what kind of format should we store the texture in?
        // 4: width of the texture
        // 5: height of the texture
        // 6: legacy stuff, should always be 0
        // 7: format of the image (again, RGB)
        // 8: data type of the image (we stored them as unsigned char *)
        // 9: the actual image data
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        // set the texture wrapping / filtering options
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    }
    else
    {
        std::cout << "Failed to load texture at path: " << path << std::endl;
    }

    stbi_image_free(data);

    return textureID;
}

// loads a cubemap texture from 6 individual texture faces
// order:
// +X (right)
// -X (left)
// +Y (top)
// -Y (bottom)
// +Z (front) 
// -Z (back)
// -------------------------------------------------------
unsigned int LoadCubemap(vector<std::string> faces)
{
    unsigned int textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

    int width, height, nrChannels;
    for (unsigned int i = 0; i < faces.size(); i++)
    {
        unsigned char *data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
        if (data)
        {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            stbi_image_free(data);
        }
        else
        {
            std::cout << "Cubemap texture failed to load at path: " << faces[i] << std::endl;
            stbi_image_free(data);
        }
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    return textureID;
}
