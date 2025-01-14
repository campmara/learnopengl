# GETTING STARTED SECTION REVIEW

* OpenGL: a formal specification of a graphics API that defines the layout and output of each function.
* GLAD: an extension loading library that loads and sets all OpenGL's function pointers for us so we can use all (modern) OpenGL's functions.
* Viewport: the 2D window region where we render to.
* Graphics Pipeline: the entire process vertices have to walk through before ending up as one or more pixels on the screen.
* Shader: a small program that runs on the graphics card. Several stages of the graphics pipeline can use user-made shaders to replace existing functionality.
* Vertex: a collection of data that represent a single point.
* Normalized Device Coordinates: the coordinate system your vertices end up in after perspective division is performed on clip coordinates. All vertex positions in NDC between -1.0 and 1.0 will not be discarded or clipped and end up visible.
* Vertex Buffer Object: a buffer object that allocates memory on the GPU and stores all the vertex data there for the graphics card to use.
* Vertex Array Object: stores buffer and vertex attribute state information.
* Element Buffer Object: a buffer object that stores indices on the GPU for indexed drawing.
* Uniform: a special type of GLSL variable that is global (each shader in a shader program can access this uniform variable) and only has to be set once.
* Texture: a special type of image used in shaders and usually wrapped around objects, giving the illusion an object is extremely detailed.
* Texture Wrapping: defines the mode that specifies how OpenGL should sample textures when texture coordinates are outside the range: (0, 1).
* Texture Filtering: defines the mode that specifies how OpenGL should sample the texture when there are several texels (texture pixels) to choose from. This usually occurs when a texture is magnified.
* Mipmaps: stored smaller versions of a texture where the appropriate sized version is chosen based on the distance to the viewer.
stb_image: image loading library.
* Texture Units: allows for multiple textures on a single shader program by binding multiple textures, each to a different texture unit.
* Vector: a mathematical entity that defines directions and/or positions in any dimension.
* Matrix: a rectangular array of mathematical expressions with useful transformation properties.
* GLM: a mathematics library tailored for OpenGL.
* Local Space: the space an object begins in. All coordinates relative to an object's origin.
* World Space: all coordinates relative to a global origin.
* View Space: all coordinates as viewed from a camera's perspective.
* Clip Space: all coordinates as viewed from the camera's perspective but with projection applied. This is the space the vertex coordinates should end up in, as output of the vertex shader. OpenGL does the rest (clipping/perspective division).
* Screen Space: all coordinates as viewed from the screen. Coordinates range from 0 to screen width/height.
* LookAt: a special type of view matrix that creates a coordinate system where all coordinates are rotated and translated in such a way that the user is looking at a given target from a given position.
* Euler Angles: defined as yaw, pitch and roll that allow us to form any 3D direction vector from these 3 values.