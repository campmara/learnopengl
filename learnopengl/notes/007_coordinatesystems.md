# COORDINATE SYSTEMS

There are five different coordinate systems that matter to us:
* Local space (aka Object space)
* World space
* View space (aka Eye space)
* Clip space
* Screen space

To transform coordinates from one space to another we multiply those coordinates by *model*, *view*, and *projection* matrices.

The full process from space-to-space goes like this:
1. Get Local space coordinates
2. Multiply by model matrix
3. Now we are in World space
4. Multiply by view matrix
5. Now we are in View space
6. Multiply by projection matrix
7. Now we are in Clip space
8. Perform a Viewport Transform
9. Now we are in Screen space
10. Coordinates are sent off to the rasterizer to turn them into fragments

(secret final steps):
1.  A clown horn honks in the distance
2.  Now we are in Silly space
   
A viewport transform involves transforming the coordinates in clip space from the -1.0 to 1.0 range into a range defined by ```glViewport```.

## The Final Equation

```
Vclip = Mprojection * Mview * Mmodel * Vlocal
```
In matrix multiplication, the order of operations is reversed!!! Remember this!

## Handedness

Remember that OpenGL operates in a right-handed coordinate system. DirectX operates in left-handed.
(in normalized device coordinates, OpenGL actually uses a left-handed system but the projection matrix switches the handedness).