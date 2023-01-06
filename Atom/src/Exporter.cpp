#include "Exporter.h"
#include "Model.h"
#include <fstream>

using namespace Atom;

const uint32_t ATOM_VERSION = 1;

template<typename T>
void WriteData(std::fstream& outFile, T pData, uint64_t numBytes) {
    outFile.write((char*)&pData, numBytes);
}

template<typename T>
void WriteDataPtr(std::fstream& outFile, T* pData, uint64_t numBytes) {
    outFile.write((char*)pData, numBytes);
}

void Atom::Exporter::ExportModelToImage(const Model& model, const std::string& exportPath)
{
    std::fstream outFile(exportPath.c_str(), std::ios::out | std::ios::binary);

    
    //Serialize the model, and write out each element. 
    WriteData(outFile, ATOM_VERSION, 4); //VERSION [4]
    //SOURCE NAME 
    WriteData(outFile, model.m_SourcePath.length(), 8);
    WriteDataPtr(outFile, model.m_SourcePath.data(), model.m_SourcePath.length() * sizeof(char));
    
    //HASHED ID
    WriteData(outFile, model.m_HashID, 32);

    //MESH COUNT
    WriteData(outFile, model.m_NumMeshes, 8);

    //Write each mesh
    for (auto& mesh : model.m_Meshes) {
        //NAME
        WriteData(outFile, mesh.name.length(), 8);
        WriteDataPtr(outFile, mesh.name.data(), mesh.name.length() * sizeof(char));

        //VERTEX COUNT
        WriteData(outFile, mesh.vertexCount, 8);

        //FACE COUNT
        WriteData(outFile, mesh.faceCount, 8);

        //VERTEX POSITIONS
        WriteDataPtr(outFile, mesh.positions.data(), mesh.vertexCount * sizeof(Vector3f));

        //TEXTURE COORDINATES
        WriteData(outFile, mesh.texCoord.size(), 8);
        WriteDataPtr(outFile, mesh.texCoord.data(), mesh.texCoord.size() * sizeof(Vector3f)); //TODO: Write Vector2f texcoords

        //VERTEX NORMALS
        WriteDataPtr(outFile, mesh.normals.data(), mesh.vertexCount * sizeof(Vector3f));

        //VERTEX TANGENTS
        WriteDataPtr(outFile, mesh.tangents.data(), mesh.vertexCount * sizeof(Vector3f));

        //VERTEX BINORMALS
        WriteDataPtr(outFile, mesh.binormals.data(), mesh.vertexCount * sizeof(Vector3f));

        //INDICES
        WriteDataPtr(outFile, mesh.indices.data(), mesh.indices.size());


       

    }


    
}
