#include "TAdaptiveGrid3.h"

int main()
{
    Double_t xmin = 0;
    Double_t xmax = 1;
    Double_t ymin = 0;
    Double_t ymax = 1;
    Double_t zmin = 0;
    Double_t zmax = 1;
    TAdaptiveGrid3 grid(xmin, xmax, ymin, ymax, zmin, zmax);
    grid.hello();
    return 0;
}