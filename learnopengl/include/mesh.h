#ifndef MESH_H
#define MESH_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>
#include <string>

#include <shader.h>

#define MAX_BONE_INFLUENCE 4

struct Vertex
{
    glm::vec3 Position;
    glm::vec3 Normal;
    glm::vec2 TexCoords;
    glm::vec3 Tangent;
    glm::vec3 Bitangent;

    // bone indices which will influence this vertex
    int BoneIDs[MAX_BONE_INFLUENCE];

    // weights from each bone
    float Weights[MAX_BONE_INFLUENCE];
};

struct Texture
{
    unsigned int ID;
    std::string Type;
    std::string Path;
};

struct Mesh
{
public:
    // Mesh Data
    std::vector<Vertex> Vertices;
    std::vector<unsigned int> Indices;
    std::vector<Texture> Textures;

    unsigned int VAO; // Vertex Array Object made public for.....some reason...?

    Mesh(std::vector<Vertex> vertices,
         std::vector<unsigned int> indices,
         std::vector<Texture> textures)
    {
        this->Vertices = vertices;
        this->Indices = indices;
        this->Textures = textures;

        Setup();
    }

    void Draw(Shader &shader)
    {
        unsigned int diffuseIndex = 1;
        unsigned int specularIndex = 1;
        unsigned int normalIndex = 1;
        unsigned int heightIndex = 1;

        // Set the shader material texture names
        for (unsigned int i = 0; i < Textures.size(); ++i)
        {
            glActiveTexture(GL_TEXTURE0 + i); // Activate the proper texture unit before binding

            // retrieve the texture number (the N in diffuse_textureN)
            std::string number;
            std::string name = Textures[i].Type;

            if (name == "texture_diffuse")
            {
                number = std::to_string(diffuseIndex++);
            }
            else if (name == "texture_specular")
            {
                number = std::to_string(specularIndex++);
            }
            else if (name == "texture_normal")
            {
                number = std::to_string(normalIndex++);
            }
            else if (name == "texture_height")
            {
                number = std::to_string(heightIndex++);
            }

            shader.SetInt(("material." + name + number).c_str(), i);
            glBindTexture(GL_TEXTURE_2D, Textures[i].ID);
        }

        // Draw the mesh
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, static_cast<unsigned int>(Indices.size()), GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

        // It's good practice to set things back to defaults once configured
        glActiveTexture(GL_TEXTURE0);
    }

private:
    unsigned int VBO; // Vertex Buffer Object
    unsigned int EBO; // Element Buffer Object

    void Setup()
    {
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);

        // bind the vertex array object (VAO) first
        glBindVertexArray(VAO);

        // binds the VBO buffer id to the vertex buffer for access
        glBindBuffer(GL_ARRAY_BUFFER, VBO);

        // copy vertex data onto the vertex buffer's memory
        glBufferData(GL_ARRAY_BUFFER,
                     Vertices.size() * sizeof(Vertex),
                     &Vertices[0],
                     GL_STATIC_DRAW);

        // copy index data onto the element buffer's memory
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                     Indices.size() * sizeof(unsigned int),
                     &Indices[0],
                     GL_STATIC_DRAW);

        // tell OpenGL how it should interpret the vertex data (per vertex attribute)
        // glVertexAttribPointer parameters:
        // 1: which vertex attribute we want to configure (location = 0 in the shader)
        // 2: size of the vertex attribute (vec3 so it's composed of 3 values)
        // 3: type of the data
        // 4: do we want the data to be normalized?
        // 5: stride: space between consecutive vertex attributes
        // 6: offset of where the position data begins in the buffer (here, at the
        // start of the array)
        // position
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)0);
        // normal
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1,
                              3,
                              GL_FLOAT,
                              GL_FALSE,
                              sizeof(Vertex),
                              (void *)offsetof(Vertex, Normal));
        // texture coords
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2,
                              2,
                              GL_FLOAT,
                              GL_FALSE,
                              sizeof(Vertex),
                              (void *)offsetof(Vertex, TexCoords));

        // tangent
        glEnableVertexAttribArray(3);
        glVertexAttribPointer(3,
                              3,
                              GL_FLOAT,
                              GL_FALSE,
                              sizeof(Vertex),
                              (void *)offsetof(Vertex, Tangent));
        // bitangent
        glEnableVertexAttribArray(4);
        glVertexAttribPointer(4,
                              3,
                              GL_FLOAT,
                              GL_FALSE,
                              sizeof(Vertex),
                              (void *)offsetof(Vertex, Bitangent));
        // ids
        glEnableVertexAttribArray(5);
        glVertexAttribIPointer(5, 4, GL_INT, sizeof(Vertex), (void *)offsetof(Vertex, BoneIDs));

        // weights
        glEnableVertexAttribArray(6);
        glVertexAttribPointer(6,
                              4,
                              GL_FLOAT,
                              GL_FALSE,
                              sizeof(Vertex),
                              (void *)offsetof(Vertex, Weights));

        glBindVertexArray(0);
    }
};

#endif