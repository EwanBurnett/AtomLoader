#ifndef ATOM_IMPORTER_H
#define ATOM_IMPORTER_H

#include <string>

namespace Atom {
    class Model;

    class Importer {
    public:
        static Model& ImportModelFromFile(const std::string& filePath);

        static Model& ImportModelFromImage(const std::string& filepath);
    };
}

#endif