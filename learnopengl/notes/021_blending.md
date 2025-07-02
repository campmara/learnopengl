# Blending

Blending in OpenGL works by alpha testing the A component of a texture's color data. If it's less than a certain threshold, we discard the fragment.

One note is that we needed to set the texture wrap property of the texture to `GL_CLAMP_TO_EDGE` in the `LoadTexture` function in order for edges to look correct when alpha tested. 

This method of discarding fragments works for just superimposing images on top of each other, but in order to achieve true blending we have to tell OpenGL to enable it:

```c++
glEnable(GL_BLEND);
```

Blending works with the following equation:

```
Cresult = Csrc * Fsrc + Cdest * Fdest
```
Values:
* Csrc: Source color vector, output from the fragment shader.
* Cdest: Destination color vector, what is currently stored in the color buffer.
* Fsrc: Source factor value, sets the impact of the alpha value on the source color.
* Fdest: The dest factor value, which sets the impact of the alpha value on the dest color.

What do we set the F-values to, then?

With a red square (solid) and a transparent green square:
       (1.0, 0.0, 0.0, 1.0)     (0.0, 1.0, 0.0, 0.6)

Fsrc = source color alpha value (0.6)
Fdest = "remainder" alpha value (1.0 - 0.6 = 0.4)

The way we tell OpenGL to use factors like this is through the `glBlendFunc` function:

```c++
glBlendFunc(GLenum sfactor, GLenum dfactor);
```

Here are the GLenum options for this function (NOTE: The constant color vector can be set via the `glBlendColor` function):
* `GL_ZERO`: Factor is 0
* `GL_ONE`: Factor is 1
* `GL_SRC_COLOR`: Factor is equal to the src color vector (Csrc)
* `GL_ONE_MINUS_SRC_COLOR`: Factor is 1 minus the src color vector
* `GL_DST_COLOR`: Factor is equal to the dest color vector (Cdest)
* `GL_ONE_MINUS_DST_COLOR`: Factor is 1 minus the dest color vector
* `GL_SRC_ALPHA`: Factor is equal to the alpha component of the src color vector
* `GL_ONE_MINUS_SRC_ALPHA`: Factor is 1 minus the alpha component of the src color vector
* `GL_DST_ALPHA`: Factor is the alpha component of the dest color vector
* `GL_ONE_MINUS_DST_ALPHA`: Factor is 1 minus the alpha component of the dest color vector
* `GL_CONSTANT_COLOR`: Factor is the constant color vector
* `GL_ONE_MINUS_CONSTANT_COLOR`: Factor is 1 minus the constant color vector
* `GL_CONSTANT_ALPHA`: Factor is the alpha component of the constant color vector
* `GL_ONE_MINUS_CONSTANT_ALPHA`: Factor is 1 minus the alpha component of the constant color vector

So we'd probably want to set the following values to get the blending we want:
```c++
glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
```

We can also set different options for the RGB and A channels individually with `glBlendFuncSeparate`:
```c++
glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ZERO);
```

For more control, we can set `glBlendEquation(GLenum mode)` to change the operator between the src and dest parts of the equation. These are the options:
* `GL_FUNC_ADD`: Adds both colors (src + dst)
* `GL_FUNC_SUBTRACT`: Subtracts both colors (src - dst)
* `GL_FUNC_REVERSE_SUBTRACT`: Subtracts both colors in reverse order (dst - src)
* `GL_MIN`: Takes component-wise minimum of both colors (min(dst, src))
* `GL_MAX`: Takes component-wise maximum of both colors (max(dst, src))

## Sorting

Because the depth test doesn't automatically solve blending for us, we have to sort blended elements from furthest to nearest and draw them accordingly.

The resulting procedure becomes the following:
1. Draw all opaque objects first.
2. Sort the transparent objects.
3. Draw all the transparent objects in sorted order.

