# HELLO WINDOW

GLAD:
* location of most opengl functions is not known at compile time, thus a 'loader library' is required

render loop:
```c++
while(!glfwWindowShouldClose(window))
{
    glfwSwapBuffers(window);
    glfwPollEvents();    
}
```