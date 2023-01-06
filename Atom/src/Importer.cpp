#include "Importer.h"
#include "Model.h"

#include <assert.h>

#include <fstream>

#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/Importer.hpp>

using namespace Atom;

void Atom::Importer::ImportModelFromFile(Model& model, const std::string& filePath)
{    
    //Parse the file format

    unsigned flags = aiProcess_Triangulate | 
        aiProcess_GenNormals | 
        aiProcess_GenUVCoords |
        aiProcess_CalcTangentSpace |
        aiProcess_JoinIdenticalVertices |
        aiProcess_GenBoundingBoxes |
        aiProcess_MakeLeftHanded |
        aiProcess_FixInfacingNormals | 
        aiProcess_SplitLargeMeshes |
        aiProcess_TransformUVCoords ; 

    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(filePath, flags);

    //Report if the import fails
    if (scene == nullptr) {
        //TODO: throw an error
        assert(false);
    }

    model.m_SourcePath = scene->GetShortFilename(filePath.c_str());

    //Import each mesh
    if (scene->HasMeshes()) {
        model.m_Meshes.resize(scene->mNumMeshes);
        model.m_NumMeshes = scene->mNumMeshes;

        for (int i = 0; i < scene->mNumMeshes; i++) {
            Mesh mesh;
            auto& sceneMesh = scene->mMeshes[i];

            //Mesh Metadata
            mesh.vertexCount = sceneMesh->mNumVertices / (sizeof(Vector3f) / sizeof(float));
            mesh.faceCount = sceneMesh->mNumFaces;
            mesh.name = sceneMesh->mName.C_Str();

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

}

template<typename T>
T ReadData(std::ifstream& inFile, uint64_t& offset, uint64_t numBytes, T* pData = nullptr) {
    if (pData != nullptr) {
        //Load the data directly into the supplied pointer
        inFile.seekg(offset);
        offset += numBytes;
        inFile.read((char*)pData, numBytes);
        return *pData;
    }
}

void Atom::Importer::ImportModelFromImage(Model& model, const std::string& filePath)
{
    //Parse the file format
    
    size_t fileOffset = 0;
    std::ifstream inFile(filePath.c_str(), std::ios::binary | std::ios::in);

    auto ReadString = [&](std::string& str) {
        uint64_t strLen;
        ReadData(inFile, fileOffset, 8, &strLen);

        str.resize(strLen);
        ReadData(inFile, fileOffset, strLen, str.data());
    };

    //VERSION
    unsigned version;
    ReadData(inFile, fileOffset, 4, &version);

    //SOURCE FILE
    ReadString(model.m_SourcePath);

    //HASH ID
    ReadData(inFile, fileOffset, 32, model.m_HashID);

    //MESH COUNT
    ReadData(inFile, fileOffset, 8, &model.m_NumMeshes);

    //Per mesh
    for (size_t i = 0; i < model.m_NumMeshes; i++) {
        Mesh mesh;

        //MESH NAME
        ReadString(mesh.name);

        //VERTEX COUNT
        ReadData(inFile, fileOffset, 8, &mesh.vertexCount);

        //FACE COUNT
        ReadData(inFile, fileOffset, 8, &mesh.faceCount);

        //VERTEX POSITIONS
        mesh.positions.resize(mesh.vertexCount);
        ReadData(inFile, fileOffset, sizeof(Vector3f) * mesh.vertexCount, mesh.positions.data());

        //TEXTURE COORDINATES 
        size_t texCoordCount;
        ReadData(inFile, fileOffset, 8, &texCoordCount);
        mesh.texCoord.resize(texCoordCount);
        ReadData(inFile, fileOffset, sizeof(Vector3f) * texCoordCount, mesh.texCoord.data());

        //VERTEX NORMALS
        mesh.normals.resize(mesh.vertexCount);
        ReadData(inFile, fileOffset, sizeof(Vector3f) * mesh.vertexCount, mesh.normals.data());

        //VERTEX TANGENTS
        mesh.tangents.resize(mesh.vertexCount);
        ReadData(inFile, fileOffset, sizeof(Vector3f) * mesh.vertexCount, mesh.tangents.data());

        //VERTEX BINORMALS
        mesh.binormals.resize(mesh.vertexCount);
        ReadData(inFile, fileOffset, sizeof(Vector3f) * mesh.vertexCount, mesh.binormals.data());


        //INDICES 
        mesh.indices.resize(mesh.faceCount * 3);
        ReadData(inFile, fileOffset, sizeof(uint32_t), mesh.indices.data());


        model.m_Meshes.push_back(mesh);
    }

}