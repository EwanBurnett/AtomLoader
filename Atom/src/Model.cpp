#include "Model.h"
#include "Importer.h"

using namespace Atom;


Atom::Model::Model()
{
    m_HashID[0] = 0;
    m_HashID[1] = 0;
    m_HashID[2] = 0;
    m_HashID[3] = 0;

    m_NumMeshes = 0;
}

//Atom::Model::Model(const Model& model)
//{
    
//}

Atom::Model::Model(Model&& model) noexcept
{
    m_HashID[0] = model.m_HashID[0];
    m_HashID[1] = model.m_HashID[1];
    m_HashID[2] = model.m_HashID[2];
    m_HashID[3] = model.m_HashID[3];
    m_NumMeshes = model.m_NumMeshes;
    m_SourcePath = model.m_SourcePath;
    m_Meshes = std::move(model.m_Meshes);
}

void Atom::Model::ImportFromFile(const std::string& filePath)
{
    //Invoke the Importer for this file
    Importer::ImportModelFromFile(*this, filePath);
}


void Atom::Model::LoadFromBinary(const std::string& imagePath)
{
    //Invoke the importer for this image
    Importer::ImportModelFromImage(*this, imagePath);
}

