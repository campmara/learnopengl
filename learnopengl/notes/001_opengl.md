# OPENGL

what follows are notes from the first introductory section of learnopengl.com
* this will be targeting opengl 3.3
* opengl uses 'extensions' to signify feature support
  * we may need them throughout the course
* opengl is a state machine:
  * contains a collection of variables that signify the current state
  * referred to as the 'context'
  * state-changing and state-using functions, know the difference!
  * object-based

using objects:

```c++
// The State of OpenGL
struct OpenGL_Context {
  	...
  	object_name* object_Window_Target;
  	...  	
};

// create object
unsigned int objectId = 0;
glGenObject(1, &objectId);
// bind/assign object to context
glBindObject(GL_WINDOW_TARGET, objectId);
// set options of object currently bound to GL_WINDOW_TARGET
glSetObjectOption(GL_WINDOW_TARGET, GL_OPTION_WINDOW_WIDTH,  800);
glSetObjectOption(GL_WINDOW_TARGET, GL_OPTION_WINDOW_HEIGHT, 600);
// set context target back to default
glBindObject(GL_WINDOW_TARGET, 0);
```

1. create an object and store a reference to it as an id
2. bind using the id
3. bind object to the target location of the context using id
4. unbind the object by setting id of the previously bound target to 0
5. options are stored in the object referenced by the id, all we have to do is rebind to get that back