#include <cstdio>
#include "Model.h"
#include "Exporter.h"

int main(){
    printf("\nHello, World!");

    {
        Atom::Model model;

        model.ImportFromFile("..\\Assets/HT-SF_medium.FBX");
        //model.LoadFromBinary("");

        Atom::Exporter::ExportModelToImage(model, "cube.bin");
    }
    {
        Atom::Model model;
        model.LoadFromBinary("cube.bin");
    }
}
