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

## Stencil Functions

Similar to the depth test function, we can specify when a stencil test will pass or fail and how it should affect the stencil buffer. For this, we use `glStencilFunc` and `glStencilOp`.

`glStencilFunc`:
* GLenum func param sets the function. GL_NEVER, GL_LESS, GL_LEQUAL, GL_GREATER, GL_GEQUAL, GL_EQUAL, GL_NOTEQUAL and GL_ALWAYS are the options.
* ref specifies the reference value for the stencil test. Content in the stencil buffer is compared to this buffer.
* mask specifies a mask that is ANDed with the ref value and the stored stencil value before the test comparest them.

`glStencilOp`:
* sfail: action to take if the test fails.
* dpfail: action to take if the test passes but the depth test fails.
* dppass: action to take if both the stencil and depth test pass.
* For each of the parameters, we have the following options:
  * GL_KEEP: keep the current stencil value
  * GL_ZERO: set to 0
  * GL_REPLACE: replace the stencil value with the ref value set in glStencilFunc
  * GL_INCR: value is increased by 1 if it is lower than the max value
  * GL_INCR_WRAP: same as GL_INCR, but wraps back to 0 after max value
  * GL_DECR: decreased by 1 if it is higher than the min value
  * GL_DECR_WRAP: same as GL_DECR but wraps to the max value if it goes lower than 0
  * GL_INVERT: Bitwise inverts the value.
* Default is set to GL_KEEP, GL_KEEP, GL_KEEP.

