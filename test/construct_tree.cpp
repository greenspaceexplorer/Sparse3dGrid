#include "TOctreeLookup.h"
#include "TRandom3.h"

void squared(Double_t x, Double_t y, Double_t z, TVector3 *out);

void squared(Double_t x, Double_t y, Double_t z, TVector3 *out)
{
    out->SetXYZ(x * x, y * y, z * z);
}

int main()
{
    // bounding box
    Double_t xmin = 0;
    Double_t xmax = 1;
    Double_t ymin = 0;
    Double_t ymax = 1;
    Double_t zmin = 0;
    Double_t zmax = 1;

    // create root node
    TOctreeLookup* root = new TOctreeLookup(xmin, xmax, ymin, ymax, zmin, zmax);

    // set tree parameters
    Int_t minDepth  = 2;
    Int_t maxDepth  = 8;
    Double_t subdivideThreshold = 0.001;
    
    // generate tree
    lookup::generate(root, minDepth, maxDepth, subdivideThreshold, squared);

    TRandom3 *rand = new TRandom3(123);

    TVector3 out;
    for(int i = 0; i < 10; i++){
        Double_t test_point[3];
        rand->RndmArray(3, test_point);
        std::cout << "test point: " << test_point[0] << ", " << test_point[1] << ", " << test_point[2] << std::endl;
        TOctreeLookup *bounding_box = lookup::findBox(root, test_point[0], test_point[1], test_point[2]);
        out = bounding_box->interpolate(test_point[0], test_point[1], test_point[2]);
        std::cout << "interpolated value: " << out.X() << ", " << out.Y() << ", " << out.Z() << std::endl;
        std::cout << "actual value      : " << test_point[0] * test_point[0] << ", " << test_point[1] * test_point[1] << ", " << test_point[2] * test_point[2] << std::endl;
        std::cout << "depth: " << bounding_box->getDepth() << std::endl;
        std::cout << std::endl;
    }

    return 0;
}