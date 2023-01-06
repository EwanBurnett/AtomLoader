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

Atom::Model::Model(const Model& model)
{

}

Atom::Model::Model(Model&& model) noexcept
{

}

void Atom::Model::ImportFromFile(const std::string& filePath)
{
    //Invoke the Importer for this file
    Importer::ImportModelFromFile(filePath);
}


void Atom::Model::LoadFromBinary(const std::string& imagePath)
{
    //Invoke the importer for this image
    Importer::ImportModelFromImage(imagePath);
}

