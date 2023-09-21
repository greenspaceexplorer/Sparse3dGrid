#ifndef TADAPTIVEGRID3D_H
#define TADAPTIVEGRID3D_H

#include <TObject.h>
#include <TMap.h>
#include <TClonesArray.h>
#include <TObjArray.h>
#include <TObjString.h>
#include <TParameter.h>
#include <algorithm>
#include <iostream>

template <typename T>
class TAdaptiveGrid3D : public TObject
{

private:
    TMap fDynamicData;
    TClonesArray *fStaticData; // Only populated once Freeze() is called
    TObjArray fIdx;
    TObjArray fIdy;
    TObjArray fIdz;

    void AddIndex(TObjArray &indexArray, Int_t index)
    {
        

    }

public:
    // Default constructor
    TAdaptiveGrid3D() : fStaticData(nullptr) {}

    void Freeze()
    {
        if (!fStaticData)
        { // Check if already frozen
            // Create and populate fStaticData
            fStaticData = new TClonesArray("TParameter", fDynamicData.GetSize());
            // Populate fStaticData from fDynamicData
            // ...

            // Clear fDynamicData
            fDynamicData.Clear();
        }
    }

    T GetPoint(Int_t x, Int_t y, Int_t z) const
    {
        if (fStaticData)
        {   // If frozen
            // Access using fStaticData
            // ...
        }
        else
        {
            TObject *obj = fDynamicData.GetValue(new TObjString(Form("%d_%d_%d", x, y, z)));
            if (obj)
            {
                return dynamic_cast<TParameter<T> *>(obj)->GetVal();
            }
            return T{}; // Return default value if point is not found
        }
    }

    ~TAdaptiveGrid3D()
    {
        if (fStaticData)
        {
            delete fStaticData; // Delete if it was created
        }
    }

    ClassDef(TAdaptiveGrid3D, 1)
};

#endif // TADAPTIVEGRID3D_H