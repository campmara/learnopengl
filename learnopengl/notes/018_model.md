# Model

Assimp has a helpful `Assimp::Importer::ReadFile()` function for reading the contents of a model file. It has a series of helpful bit flags for its second parameter such as:

`aiProcess_Triangulate`: Tell Assimp that if the model does not entirely consist of triangles, it should transform the primitive shapes into triangles first.

`aiProcess_FlipUVs`: Flips the texture coordinates on the y axis (remember that images in OpenGL are reversed around the y axis!!)

`aiProcess_GenNormals`: creates normal vectors for each vertex if the model doesn't contain normal vectors.

`aiProcess_SplitLargeMeshes`: splits large meshes into smaller sub-meshes which is useful if your rendering has a maximum number of vertices allowed and can only process smaller meshes.

`aiProcess_OptimizeMeshes`: does the reverse by trying to join several meshes into one larger mesh, reducing drawing calls for optimization.
