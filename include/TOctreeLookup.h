#ifndef TOCTREELOOKUP_H
#define TOCTREELOOKUP_H

#include <TObject.h>
#include <TVector3.h>
#include <TMath.h>
#include <TFile.h>
#include <iostream>
#include <TList.h>
#include <utility>
#include <queue>



class TOctreeLookup : public TObject
{
public:
    TOctreeLookup();
    TOctreeLookup(Double_t xmin, Double_t xmax, Double_t ymin, Double_t ymax, Double_t zmin, Double_t zmax);
    ~TOctreeLookup();


    void setCorners(TVector3 *corner0, TVector3 *corner1, TVector3 *corner2, TVector3 *corner3, TVector3 *corner4, TVector3 *corner5, TVector3 *corner6, TVector3 *corner7);
    void setCorners(TVector3 **corners);
    void setCorners(void (*func)(Double_t, Double_t, Double_t, TVector3 *));

    TVector3 *getCorner(Int_t i)
    {
        if (i < 0 || i > 7)
        {
            throw std::out_of_range("Index out of range. Expected value between 0 and 7.");
        }

        if(corners[i] == nullptr){
            throw std::runtime_error("Corner is null.");
        }
        return corners[i];
    };
    TVector3 *getCorner(bool x, bool y, bool z)
    {
        Int_t i = 0;
        if (x)
            i += 1;
        if (y)
            i += 2;
        if (z)
            i += 4;
        return getCorner(i);
    };
    void subdivide();
    TOctreeLookup *getChild(Int_t i)
    {
        if (i < 0 || i > 7)
        {
            throw std::out_of_range("Index out of range. Expected value between 0 and 7.");
        }
        return children[i];
    };
    TOctreeLookup *getChild(bool x, bool y, bool z)
    {
        Int_t i = 0;
        if (x)
            i += 1;
        if (y)
            i += 2;
        if (z)
            i += 4;
        return getChild(i);
    };
    TOctreeLookup *getParent() { return parent; };
    Int_t getDepth() { return fDepth; };    

    void setBounds(Double_t xmin, Double_t xmax, Double_t ymin, Double_t ymax, Double_t zmin, Double_t zmax);
    Double_t xMin() { return fXmin; };
    Double_t xMax() { return fXmax; };
    Double_t yMin() { return fYmin; };
    Double_t yMax() { return fYmax; };
    Double_t zMin() { return fZmin; };
    Double_t zMax() { return fZmax; };

    bool isInside(Double_t x, Double_t y, Double_t z);
    bool isInside(TVector3 *point);

    TVector3 interpolate(Double_t x, Double_t y, Double_t z);

    bool isLeaf();
    bool isRoot();
    void hello() { std::cout << "hello world" << std::endl; };

private:
    Int_t fDepth;
    Double_t fXmin, fXmax, fYmin, fYmax, fZmin, fZmax;
    TVector3 *corners[8];
    TOctreeLookup *children[8];
    TOctreeLookup *parent;

    void setParent(TOctreeLookup *parent) { this->parent = parent; };
    void setDepth(Int_t depth) { fDepth = depth; };

ClassDef(TOctreeLookup, 1)
};

void generate(TOctreeLookup *root, Int_t minDepth, Int_t maxDepth, Double_t subdivideThreshold, void (*func)(Double_t, Double_t, Double_t, TVector3 *));

TOctreeLookup *findBox(TOctreeLookup *node, Double_t x, Double_t y, Double_t z); 

void save(TOctreeLookup *root, TFile *rootFile);

#endif // TOCTREELOOKUP_H