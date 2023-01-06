#ifndef ATOM_IMPORTER_H
#define ATOM_IMPORTER_H

#include <string>

namespace Atom {
    class Model;

    class Importer {
    public:
        static void ImportModelFromFile(Model& model, const std::string& filePath);

        static void ImportModelFromImage(Model& model, const std::string& filepath);
    };
}

#endif