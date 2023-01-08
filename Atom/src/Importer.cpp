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
        aiProcess_JoinIdenticalVertices |
        aiProcess_GenBoundingBoxes |
        aiProcess_MakeLeftHanded |
        aiProcess_FixInfacingNormals |
        aiProcess_SplitLargeMeshes |
        aiProcess_TransformUVCoords |
        aiProcess_CalcTangentSpace;

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
            Mesh& mesh = model.m_Meshes[i];
            auto& sceneMesh = scene->mMeshes[i];

            //Mesh Metadata
            mesh.vertexCount = sceneMesh->mNumVertices;
            mesh.faceCount = sceneMesh->mNumFaces;
            mesh.name = sceneMesh->mName.C_Str();

            //Mesh Positions
            if (sceneMesh->HasPositions()) {
                mesh.positions.resize(mesh.vertexCount);
                memcpy((float*)mesh.positions.data(), (float*)sceneMesh->mVertices, sceneMesh->mNumVertices * sizeof(Vector3f));
            }

            //Mesh Normals
            if (sceneMesh->HasNormals()) {
                mesh.normals.resize(mesh.vertexCount);
                memcpy((float*)mesh.normals.data(), (float*)sceneMesh->mNormals, sceneMesh->mNumVertices * sizeof(Vector3f));
            }

            //Mesh Tangents / Binormals
            if (sceneMesh->HasTangentsAndBitangents()) {
                mesh.binormals.resize(mesh.vertexCount);
                memcpy((float*)mesh.binormals.data(), (float*)sceneMesh->mBitangents, sceneMesh->mNumVertices * sizeof(Vector3f));

                mesh.tangents.resize(mesh.vertexCount);
                memcpy((float*)mesh.tangents.data(), (float*)sceneMesh->mTangents, sceneMesh->mNumVertices * sizeof(Vector3f));
            }

            //Mesh Texcoords
            mesh.texCoord.resize(sceneMesh->GetNumUVChannels());
            for (size_t i = 0; i < sceneMesh->GetNumUVChannels(); i++) {
                mesh.texCoord[i].resize(mesh.vertexCount);
                for (size_t j = 0; j < mesh.vertexCount; j++) {
                    const aiVector3D* uv = sceneMesh->mTextureCoords[i] + j;
                    mesh.texCoord[i][j] = { uv->x, uv->y };
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
        uint64_t strLen = 0;
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

    model.m_Meshes.resize(model.m_NumMeshes);

    //Per mesh
    for (size_t i = 0; i < model.m_NumMeshes; i++) {
        Mesh& mesh = model.m_Meshes[i];

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
        size_t numUVChannels;
        ReadData(inFile, fileOffset, 8, &numUVChannels);
        mesh.texCoord.resize(numUVChannels);

        for (auto i = 0; i < numUVChannels; i++) {
            size_t texCoordCount;
            ReadData(inFile, fileOffset, 8, &texCoordCount);
            mesh.texCoord[i].resize(texCoordCount);
            ReadData(inFile, fileOffset, sizeof(Vector2f) * texCoordCount, mesh.texCoord[i].data());
        }

        //VERTEX NORMALS
        size_t normalCount;
        ReadData(inFile, fileOffset, 8, &normalCount);
        mesh.normals.resize(normalCount);
        ReadData(inFile, fileOffset, sizeof(Vector3f) * normalCount, mesh.normals.data());

        //VERTEX TANGENTS
        size_t tangentCount;
        ReadData(inFile, fileOffset, 8, &tangentCount);
        mesh.tangents.resize(tangentCount);
        ReadData(inFile, fileOffset, sizeof(Vector3f) * tangentCount, mesh.tangents.data());

        //VERTEX BINORMALS
        size_t binormalCount;
        ReadData(inFile, fileOffset, 8, &binormalCount);
        mesh.binormals.resize(binormalCount);
        ReadData(inFile, fileOffset, sizeof(Vector3f) * binormalCount, mesh.binormals.data());

        //INDICES 
        mesh.indices.resize(mesh.faceCount * 3);
        ReadData(inFile, fileOffset, sizeof(uint32_t) * mesh.indices.size(), mesh.indices.data());


        //model.m_Meshes.push_back(mesh);
    }

    inFile.close();
}