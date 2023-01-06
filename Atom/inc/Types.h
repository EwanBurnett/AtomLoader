#ifndef ATOM_TYPES_H
#define ATOM_TYPES_H

namespace Atom {

    struct Vector3f {
        Vector3f(float X = 0, float Y = 0, float Z = 0) : x(X), y(Y), z(Z) {}

        float x, y, z;
    };

}

#endif