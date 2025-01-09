# Mesh

In this chapter, we mostly set up a Mesh class that would handle opengl calls internally, and set up a few structs to aid in this more easily.

```c++
struct Vertex {
    glm::vec3 Position;
    glm::vec3 Normal;
    glm::vec2 TexCoords;
};

struct Texture {
    unsigned int id;
    string type;
};

class Mesh {
    public:
        // mesh data
        vector<Vertex>       vertices;
        vector<unsigned int> indices;
        vector<Texture>      textures;

        Mesh(vector<Vertex> vertices, vector<unsigned int> indices, vector<Texture> textures);
        void Draw(Shader &shader);
    private:
        //  render data
        unsigned int VAO, VBO, EBO;

        void setupMesh();
};  
```

## The `offsetof` Macro

`offsetof(STRUCT_NAME, STRUCT_MEMBER_VARIABLE)` will tell you the byte offset of that particular member variable in that particular struct. Super useful!!!