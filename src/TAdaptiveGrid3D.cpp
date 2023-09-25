#include "TAdaptiveGrid3D.h"

////////////////////////////////////////////////////////////////////////////////
template <class T>
void TAdaptiveGrid3D<T>::AddPoint(Int_t x, Int_t y, Int_t z, T val)
{
    std::cout << "TAdaptiveGrid3D::AddPoint" << std::endl;

    if (fStaticData)
    { // If frozen
      // Access using fStaticData
      // ...
    }
    else
    {
        // Add to fDynamicData
        fDynamicData.Add(new TParameter<T>(Form("%d_%d_%d", x, y, z), val));
    }
}


////////////////////////////////////////////////////////////////////////////////
template <class T>
void TAdaptiveGrid3D<T>::Freeze()
{
    if (!fStaticData)
    { // Check if already frozen
        // Create and populate fStaticData
        fStaticData = new TClonesArray("TParameter", fDynamicData.GetSize());
        // Populate fStaticData from fDynamicData
        // ...

        // Clear fDynamicData
        // fDynamicData.Clear();
    }
}