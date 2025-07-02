# Face Culling

We can check the winding order of a triangle in order to determine whether or noth that triangle faces towards or away from the camera. We can then cull these triangles that are back-facing (clockwise winding order).

We enable face culling by calling the following:
```c++
glEnable(GL_CULL_FACE);
glCullFace(GL_BACK);
glFrontFace(GL_CW);
```