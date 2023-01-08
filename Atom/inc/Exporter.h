#ifndef ATOM_EXPORTER_H
#define ATOM_EXPORTER_H

#include <string>

namespace Atom {

    class Model;

    class Exporter {
    public:
        static void ExportModelToImage(const Model& model, const std::string& exportPath);
    };
}

#endif