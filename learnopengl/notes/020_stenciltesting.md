# Stencil Testing

Stencil testing happens before depth testing and is an optimization that allows fragments that don't need to be passed to the depth buffer to be culled.

```c++
glEnable(GL_STENCIL_TEST);
glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

// In glClear:
glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

glStencilMask(0xFF); // each bit is written to the stencil buffer as is, OR
glStencilMask(0x00); // each bit ends up as 0 in the stencil buffer (disabling writes)
```