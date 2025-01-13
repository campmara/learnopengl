# Depth Testing

The __Depth Buffer__ is similar to the color buffer in that it stores information per-fragment and has the same width and height as the color buffer. The windowing system automatically creates the depth buffer and stores depth values as 16, 24, or 32 bit floats. Most systems have a precision of 24 bits.

*Depth Testing* is when OpenGL tests the depth value of a fragment against the content of the depth buffer. A test is run and if it passes, the fragment gets rendered and the depth buffer gets updated with the new depth value. Otherwise, it gets discarded.

Depth testing is done in screen space, after the fragment shader and stencil test have both run!

Screen space coordinates can be accessed via the gl_FragCoord variable in the fragment shader. This variable, on top of storing x and y coordinates in screen space (0,0 being the bottom left corner), also stores a z value which is the depth value of the fragment.

Enabling and using the depth buffer:
```c++
glEnable(GL_DEPTH_TEST);

// in the main loop:
glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

// if you want to perform depth testing and discarding, but not update the depth buffer, use this:
glDepthMask(GL_FALSE);
```

## The Depth Test Function

We can modify what kind of depth test OpenGL performs per-fragment, with `glDepthFunc(value)`. The different comparisons we can offer this function are as follows:

| Function          | Description                                                                              |
| ----------------- | ---------------------------------------------------------------------------------------- |
| GL_ALWAYS         | The depth test always passes.                                                            |
| GL_NEVER          | The depth test never passes.                                                             |
| GL_LESS (default) | Passes if the fragment's depth value is less than the stored depth value.                |
| GL_EQUAL          | Passes if the fragment's depth value is equal to the stored depth value.                 |
| GL_LEQUAL         | Passes if the fragment's depth value is less than or equal to the stored depth value.    |
| GL_GREATER        | Passes if the fragment's depth value is greater than the stored depth value.             |
| GL_NOTEQUAL       | Passes if the fragment's depth value is not equal to the stored depth value.             |
| GL_GEQUAL         | Passes if the fragment's depth value is greater than or equal to the stored depth value. |

## Depth Value Precision

Depth values are a number between 0.0 and 1.0. These values are basically constrained to the view frustum's near and far planes. The following equation transforms a z-value to a depth value:

> Fdepth = (z - near) / (far - near)

However, this is not normally the equation that we use when we perform this sort of calculation. Because of projection properties, we have to use a non-linear equation like so:

> Fdepth = ((1/z) - (1/near)) / ((1/far) - (1/near))

I suspect that an orthographic projection mode would use the linear equation! It's the perspective projection that forces us to use a nonlinear equation.

## Z-Fighting

When two triangles or planes are so closely aligned that the depth buffer lacks the precision to test the fragments properly, you get what is called *z-fighting*. It looks like the shapes are fighting over who gets to be on top.

You can't completely prevent z-fighting from happening, but you can mitigate it in the following ways.
* Never place objects too close to each other.
* Set the near plane as far as possible. Precision is much higher the closer we are to the near plane, so having it be farther away means you will get less precision errors. Find a good balance between clipping and near plane distance in your game.
* Use a higher precision depth buffer! (This costs you some performance, though).