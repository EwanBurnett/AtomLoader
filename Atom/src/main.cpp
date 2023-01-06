#include <cstdio>
#include "Model.h"
#include "Exporter.h"

int main(){
    printf("\nHello, World!");

    Atom::Model model;

    model.ImportFromFile("../Assets/cube.obj");
    model.LoadFromBinary("");

    Atom::Exporter::ExportModelToImage(model, "Exports/");
}
