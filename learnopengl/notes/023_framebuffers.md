# Framebuffers

A framebuffer is a combination of multiple buffers that get stored in GPU memory (Depth and Stencil buffers are an example of these buffers that get stored).

Using more than the default framebuffer that GLSL provides allows one to do interesting rendering effects, like mirrors or post-processing effects.

Framebuffers are created by doing the following:
```c++
unsigned int fbo;
glGenFrameBuffers(1, &fbo);

// Then, the buffer must be binded in the render loop before we perform the operations used by that buffer
glBindFramebuffer(GL_FRAMEBUFFER, fbo);
```

Framebuffers have a certain set of requirements in order to be considered complete:
* At least one buffer must be attached (color, depth, or stencil buffer)
* There should be at least one color attachment
* All attachments should be complete (memory is reserved)
* Each buffer should have the same number of samples

After binding, since we are not drawing to the default framebuffer, we're doing what is called *offscreen rendering*. In order for the effects of other framebuffers to be drawn to the main screen, we must finally bind the default frame buffer at the end of our render frame.

```c++
// At the end, after other framebuffers have been bound and drawn to:
glBindFramebuffer(GL_FRAMEBUFFER, 0);
```

## Texture Attachments

When you attach a texture to a framebuffer, all rendering commands you do after will write to the texture as if it were a color/depth or stencil buffer. The advantage of this is that you can then take that rendered image and read & manipulate it in shaders to get all sorts of effects.

```c++
// Creating a texture for a framebuffer is similar to creating any other texture.
unsigned int texture;
glGenTextures(1, &texture);
glBindTexture(GL_TEXTURE_2D, texture);

glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, WINDOW_WIDTH, WINDOW_HEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);

glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
```

If you want to render the whole screen to a smaller size texture you have to call glViewport again before rendering to the framebuffer with the new dimensions.

Once the texture is created, it just needs to be attached to the framebuffer:

```c++
glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture, 0);
```

The parameters of this function are as follows:
* `target`: Framebuffer type we're targeting (draw, read or both)
* `attachment`: Type of attachment.
* `textarget`: Type of texture you want to attach.
* `texture`: Actual texture to attach.
* `level`: The mipmap level.

## Renderbuffer Object Attachments

These are a type of framebuffer that cannot be directly read from. This gives it a performance bonus over regular framebuffers.

```c++
unsigned int rbo;
glGenRenderbuffers(1, &rbo);

// Similarly, we bind before performing operations on the buffer:
glBindRenderBuffer(GL_RENDERBUFFER, rbo);

// Creating a depth and stencil renderbuffer object is done with the following:
glRenderBufferStorage(GL_RENDERBUFER, GL_DEPTH24_STENCIL8, WINDOW_WIDTH, WINDOW_HEIGHT);

// Finally, this must be attached:
glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFER, rbo);
```

