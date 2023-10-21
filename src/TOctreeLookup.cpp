#include "TOctreeLookup.h"
ClassImp(TOctreeLookup);

TOctreeLookup::TOctreeLookup(){
    parent = nullptr;
    fDepth = 0;

    for (Int_t i = 0; i < 8; i++)
    {
        children[i] = nullptr;
    }

    for (Int_t i = 0; i < 8; i++)
    {
        corners[i] = nullptr;
    }
}

TOctreeLookup::TOctreeLookup(Double_t xmin, Double_t xmax, Double_t ymin, Double_t ymax, Double_t zmin, Double_t zmax)
{
    fXmin = xmin;
    fXmax = xmax;
    fYmin = ymin;
    fYmax = ymax;
    fZmin = zmin;
    fZmax = zmax;

    parent = nullptr;
    fDepth = 0;

    for (Int_t i = 0; i < 8; i++)
    {
        children[i] = nullptr;
    }

    for (Int_t i = 0; i < 8; i++)
    {
        corners[i] = nullptr;
    }
}

TOctreeLookup::~TOctreeLookup()
{
}

void TOctreeLookup::setCorners(TVector3 *corner0, TVector3 *corner1, TVector3 *corner2, TVector3 *corner3, TVector3 *corner4, TVector3 *corner5, TVector3 *corner6, TVector3 *corner7)
{
    corners[0] = corner0;
    corners[1] = corner1;
    corners[2] = corner2;
    corners[3] = corner3;
    corners[4] = corner4;
    corners[5] = corner5;
    corners[6] = corner6;
    corners[7] = corner7;
};

void TOctreeLookup::setCorners(TVector3 **corners)
{
    setCorners(corners[0],
               corners[1],
               corners[2],
               corners[3],
               corners[4],
               corners[5],
               corners[6],
               corners[7]);
};

void TOctreeLookup::setCorners(void (*func)(Double_t, Double_t, Double_t, TVector3 *))
{
    for (Int_t i = 0; i < 8; i++)
    {
        if (corners[i] == nullptr)
        {
            corners[i] = new TVector3(0.0, 0.0, 0.0);
        }
    }

    func(fXmin, fYmin, fZmin, corners[0]);
    func(fXmax, fYmin, fZmin, corners[1]);
    func(fXmax, fYmin, fZmin, corners[1]);
    func(fXmin, fYmax, fZmin, corners[2]);
    func(fXmax, fYmax, fZmin, corners[3]);
    func(fXmin, fYmin, fZmax, corners[4]);
    func(fXmax, fYmin, fZmax, corners[5]);
    func(fXmin, fYmax, fZmax, corners[6]);
    func(fXmax, fYmax, fZmax, corners[7]);
};

void TOctreeLookup::subdivide()
{
    if (!isLeaf())
    {
        throw std::runtime_error("Cannot subdivide a non-leaf node.");
    }
    Double_t xmid = (fXmin + fXmax) / 2;
    Double_t ymid = (fYmin + fYmax) / 2;
    Double_t zmid = (fZmin + fZmax) / 2;
    children[0] = new TOctreeLookup(fXmin, xmid, fYmin, ymid, fZmin, zmid);
    children[1] = new TOctreeLookup(xmid, fXmax, fYmin, ymid, fZmin, zmid);
    children[2] = new TOctreeLookup(fXmin, xmid, ymid, fYmax, fZmin, zmid);
    children[3] = new TOctreeLookup(xmid, fXmax, ymid, fYmax, fZmin, zmid);
    children[4] = new TOctreeLookup(fXmin, xmid, fYmin, ymid, zmid, fZmax);
    children[5] = new TOctreeLookup(xmid, fXmax, fYmin, ymid, zmid, fZmax);
    children[6] = new TOctreeLookup(fXmin, xmid, ymid, fYmax, zmid, fZmax);
    children[7] = new TOctreeLookup(xmid, fXmax, ymid, fYmax, zmid, fZmax);
    for (Int_t i = 0; i < 8; i++)
    {
        children[i]->parent = this;
        children[i]->setDepth(this->getDepth() + 1);
    }
}

void TOctreeLookup::setBounds(Double_t xmin, Double_t xmax, Double_t ymin, Double_t ymax, Double_t zmin, Double_t zmax)
{
    fXmin = xmin;
    fXmax = xmax;
    fYmin = ymin;
    fYmax = ymax;
    fZmin = zmin;
    fZmax = zmax;
}

bool TOctreeLookup::isInside(Double_t x, Double_t y, Double_t z)
{
    return x >= fXmin && x <= fXmax && y >= fYmin && y <= fYmax && z >= fZmin && z <= fZmax;
}
bool TOctreeLookup::isInside(TVector3 *point)
{
    return isInside(point->X(), point->Y(), point->Z());
}

TVector3 TOctreeLookup::interpolate(Double_t x, Double_t y, Double_t z)
{
    if (!isInside(x, y, z))
    {
        throw std::runtime_error("Cannot interpolate outside of grid.");
    }
    // trilinear interpolation using corners
    Double_t xDiff = fXmax - fXmin;
    Double_t yDiff = fYmax - fYmin;
    Double_t zDiff = fZmax - fZmin;

    Double_t xNorm = (x - fXmin) / xDiff;
    Double_t yNorm = (y - fYmin) / yDiff;
    Double_t zNorm = (z - fZmin) / zDiff;

    // interpolate along x
    TVector3 fx00 = *getCorner(0, 0, 0) + (*getCorner(1, 0, 0) - *getCorner(0, 0, 0)) * xNorm;
    TVector3 fx01 = *getCorner(0, 0, 1) + (*getCorner(1, 0, 1) - *getCorner(0, 0, 1)) * xNorm;
    TVector3 fx10 = *getCorner(0, 1, 0) + (*getCorner(1, 1, 0) - *getCorner(0, 1, 0)) * xNorm;
    TVector3 fx11 = *getCorner(0, 1, 1) + (*getCorner(1, 1, 1) - *getCorner(0, 1, 1)) * xNorm;

    // interpolate along y
    TVector3 fxy0 = fx00 + (fx10 - fx00) * yNorm;
    TVector3 fxy1 = fx01 + (fx11 - fx01) * yNorm;

    // interpolate along z
    TVector3 fxyz = fxy0 + (fxy1 - fxy0) * zNorm;

    return fxyz;
}

bool TOctreeLookup::isLeaf()
{
    for (Int_t i = 0; i < 8; i++)
    {
        if (children[i] != nullptr)
        {
            return false;
        }
    }
    return true;
}

bool TOctreeLookup::isRoot()
{
    return parent == nullptr;
}

void lookup::generate(TOctreeLookup *root, Int_t minDepth, Int_t maxDepth, Double_t subdivideThreshold, void (*func)(Double_t, Double_t, Double_t, TVector3 *))
{
    std::queue<TOctreeLookup *> queue;
    queue.push(root);

    Int_t max_depth = 0;
    Int_t test_counter = 0;
    Int_t update_every = 1000;
    bool update = false;
    while (!queue.empty())
    {
        test_counter++;
        if(test_counter % update_every == 0){
            update = true;
        }
        else{
            update = false;
        }
        if(update){
            std::cout << "test_counter: " << test_counter << std::endl;
            std::cout << "queue size: " << queue.size() << std::endl;
            std::cout << "max_depth: " << max_depth << std::endl;
        }

        // get node at front of queue and remove it
        TOctreeLookup *node = queue.front();
        queue.pop();

        if(node->getDepth() > max_depth){
            max_depth = node->getDepth();
        }

        // set corners
        // TODO: share common corners between nodes
        node->setCorners(func);
        // keep subdividing if we have not reached the minimum depth
        if (node->getDepth() < minDepth)
        {
            node->subdivide();
            for (Int_t i = 0; i < 8; i++)
            {
                queue.push(node->getChild(i));
            }
            continue;
        }
        // move on if we've reached maximum depth
        if (node->getDepth() >= maxDepth)
        {
            continue;
        }

        // calculate midpoint of node
        Double_t xMid = (node->xMin() + node->xMax()) / 2;
        Double_t yMid = (node->yMin() + node->yMax()) / 2;
        Double_t zMid = (node->zMin() + node->zMax()) / 2;

        // calculate difference between interpolated and actual values at midpoint
        TVector3 interpolated = node->interpolate(xMid, yMid, zMid);
        TVector3 actual;
        func(xMid, yMid, zMid, &actual);
        TVector3 diff = interpolated - actual;

        // subdivide and add children to queue if the magnitude of the difference is greater than threshold
        if (diff.Mag() > subdivideThreshold)
        {
            node->subdivide();
            for (Int_t i = 0; i < 8; i++)
            {
                queue.push(node->getChild(i));
            }
        }
    }
}

TOctreeLookup *lookup::findBox(TOctreeLookup *node, Double_t x, Double_t y, Double_t z)
{
    if (!node->isInside(x, y, z))
    {
        throw std::runtime_error("Point is not inside the grid.");
    }
    if (node->isLeaf())
    {
        return node;
    }
    for (Int_t i = 0; i < 8; i++)
    {
        if (node->getChild(i)->isInside(x, y, z))
        {
            return lookup::findBox(node->getChild(i), x, y, z);
        }
    }
    throw std::runtime_error("Point is not inside the grid.");
}

void lookup::save(TOctreeLookup *root, TFile *rootFile){
    if(root == nullptr){ return;}

    rootFile->cd();
    root->Write();

    for(Int_t i = 0; i < 8; i++){
        save(root->getChild(i), rootFile);
    }
}