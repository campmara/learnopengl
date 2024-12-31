# TEXTURES

Every vertex should carry texture coordinates with it in the vertex data. This coordinate is a 0-1 clamped set of float values, x and y.

We have to tell OpenGL how to sample the textures.

## Wrapping
* GL_REPEAT: The default behavior for textures. Repeats the texture image.
* GL_MIRRORED_REPEAT: Same as GL_REPEAT but mirrors the image with each repeat.
* GL_CLAMP_TO_EDGE: Clamps the coordinates between 0 and 1. The result is that higher coordinates become clamped to the edge, resulting in a stretched edge pattern.
* GL_CLAMP_TO_BORDER: Coordinates outside the range are now given a user-specified border color.

These options can be set per-coordinate axis (s, t, (and r if you're using 3d textures)) with
```c++
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
```

## Filtering

Texel: Texture Pixel

* GL_NEAREST (nearest-neighbor filtering)
* GL_LINEAR (linear filtering)

You can set filtering for *magnifying* or *minifying* operations (when the texture is scaled up or down, respectively).
```c++
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
```

## Mipmaps

OpenGL can generate mipmaps for all your textures with ```glGenerateMipmap``` after generating a texture. You don't want to do this every frame, it's slow, so usually an engine does it on texture import.

You can change the filter between mipmap levels:
* GL_NEAREST_MIPMAP_NEAREST: takes the nearest mipmap to match the pixel size and uses nearest neighbor interpolation for texture sampling.
* GL_LINEAR_MIPMAP_NEAREST: takes the nearest mipmap level and samples that level using linear interpolation.
* GL_NEAREST_MIPMAP_LINEAR: linearly interpolates between the two mipmaps that most closely match the size of a pixel and samples the interpolated level via nearest neighbor interpolation.
* GL_LINEAR_MIPMAP_LINEAR: linearly interpolates between the two closest mipmaps and samples the interpolated level via linear interpolation.

This can be set with:
```c++
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
```

## Texture Units

We can assign multiple textures to the same object with
```c++
glActiveTexture(GL_TEXTURE0); // goes from GL_TEXTURE0 to GL_TEXTURE15
```