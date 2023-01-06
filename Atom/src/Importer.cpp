#include "Importer.h"
#include "Model.h"

#include <assert.h>

#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/Importer.hpp>

using namespace Atom;

Model& Atom::Importer::ImportModelFromFile(const std::string& filePath)
{
    Model model;
    
    //Parse the file format

    unsigned flags = aiProcess_Triangulate | aiProcess_CalcTangentSpace |
        aiProcess_JoinIdenticalVertices |
        aiProcess_GenBoundingBoxes |
        aiProcess_MakeLeftHanded |
        aiProcess_FixInfacingNormals | 
        aiProcess_SplitLargeMeshes |
        aiProcess_TransformUVCoords;

    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(filePath, flags);

    //Report if the import fails
    if (scene == nullptr) {
        //TODO: throw an error
        assert(false);
    }

    //Import each mesh
    if (scene->HasMeshes()) {
        model.m_Meshes.resize(scene->mNumMeshes);

        for (int i = 0; i < scene->mNumMeshes; i++) {
            Mesh mesh;
            auto& sceneMesh = scene->mMeshes[i];

            //Mesh Positions
            if (sceneMesh->HasPositions()) {
                mesh.positions.resize(sceneMesh->mNumVertices / (sizeof(Vector3f) / sizeof(float)));
                memcpy((float*)mesh.positions.data(), (float*)sceneMesh->mVertices, sceneMesh->mNumVertices * sizeof(float));
            }

            //Mesh Normals
            if (sceneMesh->HasNormals()) {
                mesh.normals.resize(sceneMesh->mNumVertices / (sizeof(Vector3f) / sizeof(float)));
                memcpy((float*)mesh.normals.data(), (float*)sceneMesh->mNormals, sceneMesh->mNumVertices * sizeof(float));
            }

            //Mesh Tangents / Binormals
            if (sceneMesh->HasTangentsAndBitangents()) {
                mesh.binormals.resize(sceneMesh->mNumVertices / (sizeof(Vector3f) / sizeof(float)));
                memcpy((float*)mesh.binormals.data(), (float*)sceneMesh->mBitangents, sceneMesh->mNumVertices * sizeof(float));

                mesh.tangents.resize(sceneMesh->mNumVertices / (sizeof(Vector3f) / sizeof(float)));
                memcpy((float*)mesh.tangents.data(), (float*)sceneMesh->mTangents, sceneMesh->mNumVertices * sizeof(float));
            }

            //Mesh Texcoords

            mesh.texCoord.resize(sceneMesh->mNumVertices);
            for (size_t i = 0; i < sceneMesh->GetNumUVChannels(); i++) {
                for (size_t j = 0; j < sceneMesh->mNumVertices; j++) {
                    const aiVector3D* uv = sceneMesh->mTextureCoords[i] + j;
                    mesh.texCoord[j] = { uv->x, uv->y };
                }
            }
            
            //Mesh Indices
            mesh.indices.resize(sceneMesh->mNumFaces * 3);

            for (size_t i = 0; i < sceneMesh->mNumFaces; i++) {
                aiFace* face = &sceneMesh->mFaces[i];
                for (size_t j = 0; j < face->mNumIndices; j++) {
                    mesh.indices[i + j] = face->mIndices[j];
                }
            }

            model.m_Meshes[i] = mesh;
        }
    }
    

    return model;
}

Model& Atom::Importer::ImportModelFromImage(const std::string& filePath)
{
    Model model;
    
    //Parse the file format


    return model;
}