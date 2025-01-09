#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb_image.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

#include <shader.h>
#include <camera.h>

// Function declerations
void ProcessInput(GLFWwindow *window);
void FrameBufferSizeCallback(GLFWwindow *window, int width, int height);
void MouseCallback(GLFWwindow *window, double xPosIn, double yPosIn);
void ScrollCallback(GLFWwindow *window, double xOffset, double yOffset);
unsigned int LoadTexture(const char *path);

// Settings
const unsigned int WINDOW_WIDTH = 800;
const unsigned int WINDOW_HEIGHT = 600;

// Camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
bool isFirstMouseInput = true;
float lastMouseX = (float)WINDOW_WIDTH / 2.0f;
float lastMouseY = (float)WINDOW_HEIGHT / 2.0f;

// Timing
float deltaTime = 0.0f;     // Time between current frame and last frame
float lastFrameTime = 0.0f; // Time of last frame

// lighting
glm::vec3 lightPos(1.2f, 1.0f, 2.0f);

int main()
{
    // Initialize and specify GLFW settings
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Create the window and set it to the current context
    GLFWwindow *window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
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
    Shader lightingShader("shaders/2.6.1.multiplelights.vs", "shaders/2.6.1.multiplelights.fs");
    Shader lightCubeShader("shaders/2.3.1.light_cube.vs", "shaders/2.3.1.light_cube.fs");

    // Set up vertex data
    float vertices[] = {
        -0.5f, -0.5f, -0.5f, 0.0f,  0.0f,  -1.0f, 0.0f,  0.0f,  0.5f,  -0.5f, -0.5f, 0.0f,
        0.0f,  -1.0f, 1.0f,  0.0f,  0.5f,  0.5f,  -0.5f, 0.0f,  0.0f,  -1.0f, 1.0f,  1.0f,
        0.5f,  0.5f,  -0.5f, 0.0f,  0.0f,  -1.0f, 1.0f,  1.0f,  -0.5f, 0.5f,  -0.5f, 0.0f,
        0.0f,  -1.0f, 0.0f,  1.0f,  -0.5f, -0.5f, -0.5f, 0.0f,  0.0f,  -1.0f, 0.0f,  0.0f,

        -0.5f, -0.5f, 0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,  0.5f,  -0.5f, 0.5f,  0.0f,
        0.0f,  1.0f,  1.0f,  0.0f,  0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
        0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,  -0.5f, 0.5f,  0.5f,  0.0f,
        0.0f,  1.0f,  0.0f,  1.0f,  -0.5f, -0.5f, 0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,

        -0.5f, 0.5f,  0.5f,  -1.0f, 0.0f,  0.0f,  1.0f,  0.0f,  -0.5f, 0.5f,  -0.5f, -1.0f,
        0.0f,  0.0f,  1.0f,  1.0f,  -0.5f, -0.5f, -0.5f, -1.0f, 0.0f,  0.0f,  0.0f,  1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f, 0.0f,  0.0f,  0.0f,  1.0f,  -0.5f, -0.5f, 0.5f,  -1.0f,
        0.0f,  0.0f,  0.0f,  0.0f,  -0.5f, 0.5f,  0.5f,  -1.0f, 0.0f,  0.0f,  1.0f,  0.0f,

        0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,  0.5f,  0.5f,  -0.5f, 1.0f,
        0.0f,  0.0f,  1.0f,  1.0f,  0.5f,  -0.5f, -0.5f, 1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
        0.5f,  -0.5f, -0.5f, 1.0f,  0.0f,  0.0f,  0.0f,  1.0f,  0.5f,  -0.5f, 0.5f,  1.0f,
        0.0f,  0.0f,  0.0f,  0.0f,  0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

        -0.5f, -0.5f, -0.5f, 0.0f,  -1.0f, 0.0f,  0.0f,  1.0f,  0.5f,  -0.5f, -0.5f, 0.0f,
        -1.0f, 0.0f,  1.0f,  1.0f,  0.5f,  -0.5f, 0.5f,  0.0f,  -1.0f, 0.0f,  1.0f,  0.0f,
        0.5f,  -0.5f, 0.5f,  0.0f,  -1.0f, 0.0f,  1.0f,  0.0f,  -0.5f, -0.5f, 0.5f,  0.0f,
        -1.0f, 0.0f,  0.0f,  0.0f,  -0.5f, -0.5f, -0.5f, 0.0f,  -1.0f, 0.0f,  0.0f,  1.0f,

        -0.5f, 0.5f,  -0.5f, 0.0f,  1.0f,  0.0f,  0.0f,  1.0f,  0.5f,  0.5f,  -0.5f, 0.0f,
        1.0f,  0.0f,  1.0f,  1.0f,  0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
        0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,  -0.5f, 0.5f,  0.5f,  0.0f,
        1.0f,  0.0f,  0.0f,  0.0f,  -0.5f, 0.5f,  -0.5f, 0.0f,  1.0f,  0.0f,  0.0f,  1.0f
    };

    unsigned int indices[] = {
        // note that we start from 0!
        0, 1, 3, // first triangle
        1, 2, 3  // second triangle
    };

    glm::vec3 cubePositions[] = { glm::vec3(0.0f, 0.0f, 0.0f),    glm::vec3(2.0f, 5.0f, -15.0f),
                                  glm::vec3(-1.5f, -2.2f, -2.5f), glm::vec3(-3.8f, -2.0f, -12.3f),
                                  glm::vec3(2.4f, -0.4f, -3.5f),  glm::vec3(-1.7f, 3.0f, -7.5f),
                                  glm::vec3(1.3f, -2.0f, -2.5f),  glm::vec3(1.5f, 2.0f, -2.5f),
                                  glm::vec3(1.5f, 0.2f, -1.5f),   glm::vec3(-1.3f, 1.0f, -1.5f) };

    glm::vec3 pointLightPositions[] = { glm::vec3(0.7f, 0.2f, 2.0f),
                                        glm::vec3(2.3f, -3.3f, -4.0f),
                                        glm::vec3(-4.0f, 2.0f, -12.0f),
                                        glm::vec3(0.0f, 0.0f, -3.0f) };

    // * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
    // Setup the vertex buffer
    // * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *

    unsigned int VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    // bind the vertex array object (VAO) first
    glBindVertexArray(VAO);

    // binds the VBO buffer id to the vertex buffer for access
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    // copies vertex data onto the vertex buffer's memory
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // tell OpenGL how it should interpret the vertex data (per vertex attribute)
    // glVertexAttribPointer parameters:
    // 1: which vertex attribute we want to configure (location = 0 in the shader)
    // 2: size of the vertex attribute (vec3 so it's composed of 3 values)
    // 3: type of the data
    // 4: do we want the data to be normalized?
    // 5: stride: space between consecutive vertex attributes
    // 6: offset of where the position data begins in the buffer (here, at the
    // start of the array)
    // position
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);
    // normal
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    // texture coords
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    // second, configure the light's VAO (VBO stays the same; the vertices are the same for the
    // light object which is also a 3D cube)
    unsigned int lightCubeVAO;
    glGenVertexArrays(1, &lightCubeVAO);
    glBindVertexArray(lightCubeVAO);

    // we only need to bind to the VBO (to link it with glVertexAttribPointer), no need to fill it;
    // the VBO's data already contains all we need (it's already bound, but we do it again for
    // educational purposes)
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);

    // * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
    // Set up the textures
    // * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *

    unsigned int diffuseMap = LoadTexture("textures/container2.png");
    unsigned int specularMap = LoadTexture("textures/container2_specular.png");

    // note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex
    // attribute's bound vertex buffer object so afterwards we can safely unbind
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // remember: do NOT unbind the EBO while a VAO is active as the bound element buffer object IS
    // stored in the VAO; keep the EBO bound.
    // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but
    // this rarely happens. Modifying other VAOs requires a call to glBindVertexArray anyways so we
    // generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
    // glBindVertexArray(0);

    // uncomment this call to draw in wireframe polygons.
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    lightingShader.Use();
    lightingShader.SetInt("material.diffuse", 0);
    lightingShader.SetInt("material.specular", 1);

    // Main Loop
    while (!glfwWindowShouldClose(window))
    {
        // Time
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrameTime;
        lastFrameTime = currentFrame;

        // Input Handling
        ProcessInput(window);

        // Rendering Commands
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        lightPos.x = sin(currentFrame) * 2.0f;
        lightPos.z = cos(currentFrame) * 2.0f;

        // be sure to activate shader when setting uniforms / drawing objects
        lightingShader.Use();
        lightingShader.SetVec3("viewPos", camera.Position);

        // material properties
        lightingShader.SetFloat("material.shininess", 32.0f);

        // DirectionalLight properties
        lightingShader.SetVec3("directionalLight.direction", -0.2f, -1.0f, -0.3f);
        lightingShader.SetVec3("directionalLight.ambient", 0.05f, 0.05f, 0.05f);
        lightingShader.SetVec3("directionalLight.diffuse", 0.4f, 0.4f, 0.4f);
        lightingShader.SetVec3("directionalLight.specular", 0.5f, 0.5f, 0.5f);

        // PointLight properties
        lightingShader.SetVec3("pointLights[0].position", pointLightPositions[0]);
        lightingShader.SetVec3("pointLights[0].ambient", 0.05f, 0.05f, 0.05f);
        lightingShader.SetVec3("pointLights[0].diffuse", 0.8f, 0.8f, 0.8f);
        lightingShader.SetVec3("pointLights[0].specular", 1.0f, 1.0f, 1.0f);
        lightingShader.SetFloat("pointLights[0].constant", 1.0f);
        lightingShader.SetFloat("pointLights[0].linear", 0.09f);
        lightingShader.SetFloat("pointLights[0].quadratic", 0.032f);
        
        lightingShader.SetVec3("pointLights[1].position", pointLightPositions[1]);
        lightingShader.SetVec3("pointLights[1].ambient", 0.05f, 0.05f, 0.05f);
        lightingShader.SetVec3("pointLights[1].diffuse", 0.8f, 0.8f, 0.8f);
        lightingShader.SetVec3("pointLights[1].specular", 1.0f, 1.0f, 1.0f);
        lightingShader.SetFloat("pointLights[1].constant", 1.0f);
        lightingShader.SetFloat("pointLights[1].linear", 0.09f);
        lightingShader.SetFloat("pointLights[1].quadratic", 0.032f);

        lightingShader.SetVec3("pointLights[2].position", pointLightPositions[2]);
        lightingShader.SetVec3("pointLights[2].ambient", 0.05f, 0.05f, 0.05f);
        lightingShader.SetVec3("pointLights[2].diffuse", 0.8f, 0.8f, 0.8f);
        lightingShader.SetVec3("pointLights[2].specular", 1.0f, 1.0f, 1.0f);
        lightingShader.SetFloat("pointLights[2].constant", 1.0f);
        lightingShader.SetFloat("pointLights[2].linear", 0.09f);
        lightingShader.SetFloat("pointLights[2].quadratic", 0.032f);

        lightingShader.SetVec3("pointLights[3].position", pointLightPositions[3]);
        lightingShader.SetVec3("pointLights[3].ambient", 0.05f, 0.05f, 0.05f);
        lightingShader.SetVec3("pointLights[3].diffuse", 0.8f, 0.8f, 0.8f);
        lightingShader.SetVec3("pointLights[3].specular", 1.0f, 1.0f, 1.0f);
        lightingShader.SetFloat("pointLights[3].constant", 1.0f);
        lightingShader.SetFloat("pointLights[3].linear", 0.09f);
        lightingShader.SetFloat("pointLights[3].quadratic", 0.032f);

        // SpotLight properties
        lightingShader.SetVec3("spotLight.position", camera.Position);
        lightingShader.SetVec3("spotLight.direction", camera.Front);
        lightingShader.SetFloat("spotLight.cutOff", glm::cos(glm::radians(12.5f)));
        lightingShader.SetFloat("spotLight.outerCutOff", glm::cos(glm::radians(15.0f)));  
        lightingShader.SetVec3("spotLight.ambient", 0.0f, 0.0f, 0.0f);
        lightingShader.SetVec3("spotLight.diffuse", 1.0f, 1.0f, 1.0f);
        lightingShader.SetVec3("spotLight.specular", 1.0f, 1.0f, 1.0f);
        lightingShader.SetFloat("spotLight.constant", 1.0f);
        lightingShader.SetFloat("spotLight.linear", 0.09f);
        lightingShader.SetFloat("spotLight.quadratic", 0.032f);

        // view / projection transformations
        glm::mat4 projection = glm::perspective(glm::radians(camera.FoV),
                                                (float)WINDOW_WIDTH / (float)WINDOW_HEIGHT,
                                                0.1f,
                                                100.0f);
        glm::mat4 view = camera.GetViewMatrix();
        lightingShader.SetMat4x4("projection", projection);
        lightingShader.SetMat4x4("view", view);

        // world transformation
        glm::mat4 model = glm::mat4(1.0f);
        lightingShader.SetMat4x4("model", model);

        // bind diffuse map
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, diffuseMap);

        // bind specular map
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, specularMap);

        // render the cube
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        for (unsigned int i = 0; i < 10; ++i)
        {
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, cubePositions[i]);
            float angle = 20.0f * i;
            model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
            lightingShader.SetMat4x4("model", model);

            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        // also draw the lamp objects
        lightCubeShader.Use();
        lightCubeShader.SetVec3("lightColor", 1.0f, 1.0f, 1.0f);
        lightCubeShader.SetMat4x4("projection", projection);
        lightCubeShader.SetMat4x4("view", view);

        for (unsigned int i = 0; i < 4; ++i)
        {
            model = glm::mat4(1.0f);
            model = glm::translate(model, pointLightPositions[i]);
            model = glm::scale(model, glm::vec3(0.2f)); // a smaller cube
            lightCubeShader.SetMat4x4("model", model);
            glBindVertexArray(lightCubeVAO);
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        // Swaps the 2d buffer that contains color values for each pixel
        glfwSwapBuffers(window);
        glfwPollEvents(); // Checks for events being triggered (input)
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteVertexArrays(1, &lightCubeVAO);
    glDeleteBuffers(1, &VBO);

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
    stbi_set_flip_vertically_on_load(true); // only need to do this once
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
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    }
    else
    {
        std::cout << "Failed to load texture at path: " << path << std::endl;
    }

    stbi_image_free(data);

    return textureID;
}