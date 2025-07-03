# Advanced Data

There are a few alternative approaches to managing buffers. At its core, a buffer is an object that manages GPU memory. We give meaning to a buffer when we bind it to a *buffer target*. OpenGL will process buffers differently based on what target is assigned to each one.

So far, we have been filling buffer memory with `glBufferData`. If we pass null into this function, it only allocates memory and doesn't fill it. It's useful for when we want to reserve a specific amount of memory at one point and fill it later.

Instead of using that function to fill the entire buffer, we can also use `glBufferSubData` to fill specific parts of the buffer. Note that the buffer should have enough allocated memory first, so `glBufferData` must be called at some point before you fill sub-regions.

```c++
glBufferSubData(GL_ARRAY_BUFFER, 24, sizeof(data), &data); // Range: [24, 24 + sizeof(data)]
```

Another method for getting data into a buffer is to ask for a pointer to the buffer's memory and use that. `glMapBuffer` accomplishes this for us.

Here's an example:

```c++
float data[] = {
  0.5f, 1.0f, -0.35f
  [...]
};
glBindBuffer(GL_ARRAY_BUFFER, buffer);
// get pointer
void *ptr = glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
// now copy data into memory
memcpy(ptr, data, sizeof(data));
// make sure to tell OpenGL we're done with the pointer
glUnmapBuffer(GL_ARRAY_BUFFER);
```

Note that after calling glMapBuffer we have to call `glUnmapBuffer` to tell OpenGL we're done mapping memory to the buffer.

## Batching Vertex Attributes

When you load vertex data from a file, the arrays of different attributes are usually not interleaved, and so you have to often perform a costly step in your program to do so. With `glBufferSubData` we can take the batched data directly and fill the buffer with it.

```c++
float positions[] = { ... };
float normals[] = { ... };
float tex[] = { ... };
// fill buffer
glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(positions), &positions);
glBufferSubData(GL_ARRAY_BUFFER, sizeof(positions), sizeof(normals), &normals);
glBufferSubData(GL_ARRAY_BUFFER, sizeof(positions) + sizeof(normals), sizeof(tex), &tex);

// We then have to update the vertex attribute pointers to reflect this change.
glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);  
glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)(sizeof(positions)));  
glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)(sizeof(positions) + sizeof(normals)));  
```

Note that the stride parameter is equal to the size of the vertex attribute, since the next attribute can be found directly after its 3 (or 2) components.

## Copying Buffers

Once you're past the step of filling buffers with data, you may want to share that data with other buffers or copy the content of one buffer to another. This can be done with the `glCopyBufferSubData` function.

```c++
void glCopyBufferSubData(GLenum readtarget, GLenum writetarget, GLintptr readoffset,
                         GLintptr writeoffset, GLsizeiptr size);
```

Because sometimes multiple buffers might use the same buffer target, OpenGL offers `GL_COPY_READ_BUFFER` and `GL_COPY_WRITE_BUFFER` as extra targets for this operation specifically.

```c++
glBindBuffer(GL_COPY_READ_BUFFER, vbo1);
glBindBuffer(GL_COPY_WRITE_BUFFER, vbo2);
glCopyBufferSubData(GL_COPY_READ_BUFFER, GL_COPY_WRITE_BUFFER, 0, 0, 8 * sizeof(float));

// We can also do this with just GL_COPY_WRITE_BUFER:
float vertexData[] = { ... };
glBindBuffer(GL_ARRAY_BUFFER, vbo1);
glBindBuffer(GL_COPY_WRITE_BUFFER, vbo2);
glCopyBufferSubData(GL_ARRAY_BUFFER, GL_COPY_WRITE_BUFFER, 0, 0, 8 * sizeof(float));  
```