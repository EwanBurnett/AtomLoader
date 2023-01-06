#ifndef ATOM_MODEL_H
#define ATOM_MODEL_H

#include "Types.h"
#include <vector>
#include <string>

namespace Atom {

    struct Mesh {
        uint64_t vertexCount;
        uint64_t faceCount;
        std::vector<Vector3f> positions;
        std::vector<Vector3f> texCoord;
        std::vector<Vector3f> normals;
        std::vector<Vector3f> tangents;
        std::vector<Vector3f> binormals;
        std::vector<uint32_t> indices;
    };

    /** 
    * \brief Runtime representation of the model data.
    * 
    */
    class Model {
    public:
        Model();
        Model(const Model& model);
        Model(Model&& model) noexcept;

        void ImportFromFile(const std::string& filePath);
        void LoadFromBinary(const std::string& imagePath);

    private:
        uint64_t m_HashID[4];
        uint64_t m_NumMeshes;
        std::vector<Mesh> m_Meshes;
    };
}

#endif