#ifndef TSPARSEGRID3D_H
#define TSPARSEGRID3D_H
#include <TObject.h>
#include <TMatrixDSparse.h>
#include <iostream>
#include <map>

class TSparseGrid3D : public TObject
{
    private:
        std::map<int, TMatrixDSparse> layers;

    public:
        TSparseGrid3D();
        ~TSparseGrid3D();
        void hello(){std::cout << "Hello from TSparseGrid3D" << std::endl; };
};

#endif // TSPARSEGRID3D_H