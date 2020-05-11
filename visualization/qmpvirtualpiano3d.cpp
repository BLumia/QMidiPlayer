#include <cstring>
#include "qmpvirtualpiano3d.hpp"
#define configureVertex(v,sub,_x,_y,_z) v[sub].x=_x,v[sub].y=_y,v[sub].z=_z;
const double gap[] = {WK_TALWIDTH / 2 * 0.92, WK_TALWIDTH / 2 * 1.23, WK_TALWIDTH / 2 * 1.2,
        WK_TALWIDTH / 2 * 0.95, WK_TALWIDTH * 1.1, WK_TALWIDTH / 2 * 0.95,
        WK_TALWIDTH / 2 * 1.2, WK_TALWIDTH / 2 * 1.15, WK_TALWIDTH / 2 * 1,
        WK_TALWIDTH / 2 * 1.3, WK_TALWIDTH / 2 * 0.85, WK_TALWIDTH * 1.1
    };
qmpVirtualPiano3D::qmpVirtualPiano3D()
{
    ebuf = new smEntity3DBuffer();
    buildKeys();
    memset(traveld, 0, sizeof(traveld));
}
qmpVirtualPiano3D::~qmpVirtualPiano3D()
{
    delete wkcf;
    delete wkeb;
    delete wkd;
    delete wkg;
    delete wka;
    delete bk;
    wkcf = wkeb = wkd = wkg = wka = bk = nullptr;
    delete ebuf;
    ebuf = nullptr;
}
void qmpVirtualPiano3D::render(smvec3d p)
{
    p.x -= WK_TALWIDTH * 37 * 1.075;
    for (int i = 0; i < 128; ++i)
    {
        smMatrix m;
        m.loadIdentity();
        m.rotate(-0.2 * traveld[i], 1, 0, 0);
        switch (i % 12)
        {
            case 0:
            case 5:
                if (wkcf)
                    ebuf->addTransformedEntity(wkcf, m, p);
                break;
            case 2:
                if (wkd)
                    ebuf->addTransformedEntity(wkd, m, p);
                break;
            case 4:
            case 11:
                if (wkeb)
                    ebuf->addTransformedEntity(wkeb, m, p);
                break;
            case 7:
                if (wkg)
                    ebuf->addTransformedEntity(wkg, m, p);
                break;
            case 9:
                if (wka)
                    ebuf->addTransformedEntity(wka, m, p);
                break;
            case 1:
            case 3:
            case 6:
            case 8:
            case 10:
                if (bk)
                    ebuf->addTransformedEntity(bk, m, p);
                break;
        }
        p.x += gap[i % 12];
    }
    ebuf->drawBatch();
}
void qmpVirtualPiano3D::setKeyTravelDist(int k, double td)
{
    traveld[k] = td;
}

void qmpVirtualPiano3D::buildKeys()
{
    wkcf = new smEntity3D();
    wkeb = new smEntity3D();
    wkd = new smEntity3D();
    wkg = new smEntity3D();
    wka = new smEntity3D();
    bk = new smEntity3D();
    wkcf->addVertices(19,
        makeVertex(-WK_TALWIDTH / 2, 0, WK_HEIGHT, 0xFFFFFFFF, 0, 0),
        makeVertex(-WK_TALWIDTH / 2 + WK_PREWIDTH, 0, WK_HEIGHT, 0xFFFFFFFF, 0, 0),
        makeVertex(-WK_TALWIDTH / 2 + WK_PREWIDTH, WK_PRELEN, WK_HEIGHT, 0xFFFFFFFF, 0, 0),
        makeVertex(-WK_TALWIDTH / 2, WK_PRELEN, WK_HEIGHT, 0xFFFFFFFF, 0, 0),
        makeVertex(WK_TALWIDTH / 2, WK_PRELEN, WK_HEIGHT, 0xFFFFFFFF, 0, 0),
        makeVertex(WK_TALWIDTH / 2, WK_PRELEN + WK_TALLEN + WK_WING, WK_HEIGHT, 0xFFFFFFFF, 0, 0),
        makeVertex(-WK_TALWIDTH / 2, WK_PRELEN + WK_TALLEN + WK_WING, WK_HEIGHT, 0xFFFFFFFF, 0, 0), ////
        makeVertex(-WK_TALWIDTH / 2, 0, WK_HEIGHT, 0xFFCCCCCC, 0, 0),
        makeVertex(-WK_TALWIDTH / 2 + WK_PREWIDTH, 0, WK_HEIGHT, 0xFFCCCCCC, 0, 0),
        makeVertex(-WK_TALWIDTH / 2 + WK_PREWIDTH, WK_PRELEN, WK_HEIGHT, 0xFFCCCCCC, 0, 0),
        makeVertex(WK_TALWIDTH / 2, WK_PRELEN, WK_HEIGHT, 0xFFCCCCCC, 0, 0),
        makeVertex(WK_TALWIDTH / 2, WK_PRELEN + WK_TALLEN, WK_HEIGHT, 0xFFCCCCCC, 0, 0),
        makeVertex(-WK_TALWIDTH / 2, WK_PRELEN + WK_TALLEN, WK_HEIGHT, 0xFFCCCCCC, 0, 0),
        makeVertex(-WK_TALWIDTH / 2, 0, 0, 0xFFCCCCCC, 0, 0),
        makeVertex(-WK_TALWIDTH / 2 + WK_PREWIDTH, 0, 0, 0xFFCCCCCC, 0, 0),
        makeVertex(-WK_TALWIDTH / 2 + WK_PREWIDTH, WK_PRELEN, 0, 0xFFCCCCCC, 0, 0),
        makeVertex(WK_TALWIDTH / 2, WK_PRELEN, 0, 0xFFCCCCCC, 0, 0),
        makeVertex(WK_TALWIDTH / 2, WK_PRELEN + WK_TALLEN, 0, 0xFFCCCCCC, 0, 0),
        makeVertex(-WK_TALWIDTH / 2, WK_PRELEN + WK_TALLEN, 0, 0xFFCCCCCC, 0, 0));
    wkcf->addIndices(48,
        0, 1, 3, 1, 2, 3, 3, 4, 6, 4, 5, 6,
        7, 8, 13, 8, 13, 14, 8, 14, 15, 8, 9, 15,
        9, 10, 15, 10, 15, 16, 10, 11, 17, 10, 16, 17,
        11, 12, 18, 11, 17, 18, 18, 12, 13, 12, 13, 7);

    wkeb->addVertices(19,
        makeVertex(WK_TALWIDTH / 2, 0, WK_HEIGHT, 0xFFFFFFFF, 0, 0),
        makeVertex(WK_TALWIDTH / 2 - WK_PREWIDTH, 0, WK_HEIGHT, 0xFFFFFFFF, 0, 0),
        makeVertex(WK_TALWIDTH / 2 - WK_PREWIDTH, WK_PRELEN, WK_HEIGHT, 0xFFFFFFFF, 0, 0),
        makeVertex(WK_TALWIDTH / 2, WK_PRELEN, WK_HEIGHT, 0xFFFFFFFF, 0, 0),
        makeVertex(-WK_TALWIDTH / 2, WK_PRELEN, WK_HEIGHT, 0xFFFFFFFF, 0, 0),
        makeVertex(-WK_TALWIDTH / 2, WK_PRELEN + WK_TALLEN + WK_WING, WK_HEIGHT, 0xFFFFFFFF, 0, 0),
        makeVertex(WK_TALWIDTH / 2, WK_PRELEN + WK_TALLEN + WK_WING, WK_HEIGHT, 0xFFFFFFFF, 0, 0), ////
        makeVertex(WK_TALWIDTH / 2, 0, WK_HEIGHT, 0xFFCCCCCC, 0, 0),
        makeVertex(WK_TALWIDTH / 2 - WK_PREWIDTH, 0, WK_HEIGHT, 0xFFCCCCCC, 0, 0),
        makeVertex(WK_TALWIDTH / 2 - WK_PREWIDTH, WK_PRELEN, WK_HEIGHT, 0xFFCCCCCC, 0, 0),
        makeVertex(-WK_TALWIDTH / 2, WK_PRELEN, WK_HEIGHT, 0xFFCCCCCC, 0, 0),
        makeVertex(-WK_TALWIDTH / 2, WK_PRELEN + WK_TALLEN, WK_HEIGHT, 0xFFCCCCCC, 0, 0),
        makeVertex(WK_TALWIDTH / 2, WK_PRELEN + WK_TALLEN, WK_HEIGHT, 0xFFCCCCCC, 0, 0),
        makeVertex(WK_TALWIDTH / 2, 0, 0, 0xFFCCCCCC, 0, 0),
        makeVertex(WK_TALWIDTH / 2 - WK_PREWIDTH, 0, 0, 0xFFCCCCCC, 0, 0),
        makeVertex(WK_TALWIDTH / 2 - WK_PREWIDTH, WK_PRELEN, 0, 0xFFCCCCCC, 0, 0),
        makeVertex(-WK_TALWIDTH / 2, WK_PRELEN, 0, 0xFFCCCCCC, 0, 0),
        makeVertex(-WK_TALWIDTH / 2, WK_PRELEN + WK_TALLEN, 0, 0xFFCCCCCC, 0, 0),
        makeVertex(WK_TALWIDTH / 2, WK_PRELEN + WK_TALLEN, 0, 0xFFCCCCCC, 0, 0));
    wkeb->addIndices(48,
        0, 1, 3, 1, 2, 3, 3, 4, 6, 4, 5, 6,
        7, 8, 13, 8, 13, 14, 8, 14, 15, 8, 9, 15,
        9, 10, 15, 10, 15, 16, 10, 11, 17, 10, 16, 17,
        11, 12, 18, 11, 17, 18, 18, 12, 13, 12, 13, 7);

    wkd->addVertices(24,
        makeVertex(-WK_TALWIDTH / 2 + (WK_TALWIDTH - WK_PREWIDTH) / 2, 0, WK_HEIGHT, 0xFFFFFFFF, 0, 0),
        makeVertex(WK_TALWIDTH / 2 - (WK_TALWIDTH - WK_PREWIDTH) / 2, 0, WK_HEIGHT, 0xFFFFFFFF, 0, 0),
        makeVertex(WK_TALWIDTH / 2 - (WK_TALWIDTH - WK_PREWIDTH) / 2, WK_PRELEN, WK_HEIGHT, 0xFFFFFFFF, 0, 0),
        makeVertex(-WK_TALWIDTH / 2 + (WK_TALWIDTH - WK_PREWIDTH) / 2, WK_PRELEN, WK_HEIGHT, 0xFFFFFFFF, 0, 0),
        makeVertex(WK_TALWIDTH / 2, WK_PRELEN, WK_HEIGHT, 0xFFFFFFFF, 0, 0),
        makeVertex(WK_TALWIDTH / 2, WK_PRELEN + WK_TALLEN + WK_WING, WK_HEIGHT, 0xFFFFFFFF, 0, 0),
        makeVertex(-WK_TALWIDTH / 2, WK_PRELEN + WK_TALLEN + WK_WING, WK_HEIGHT, 0xFFFFFFFF, 0, 0),
        makeVertex(-WK_TALWIDTH / 2, WK_PRELEN, WK_HEIGHT, 0xFFFFFFFF, 0, 0), ////
        makeVertex(-WK_TALWIDTH / 2 + (WK_TALWIDTH - WK_PREWIDTH) / 2, 0, WK_HEIGHT, 0xFFCCCCCC, 0, 0),
        makeVertex(WK_TALWIDTH / 2 - (WK_TALWIDTH - WK_PREWIDTH) / 2, 0, WK_HEIGHT, 0xFFCCCCCC, 0, 0),
        makeVertex(WK_TALWIDTH / 2 - (WK_TALWIDTH - WK_PREWIDTH) / 2, WK_PRELEN, WK_HEIGHT, 0xFFCCCCCC, 0, 0),
        makeVertex(-WK_TALWIDTH / 2 + (WK_TALWIDTH - WK_PREWIDTH) / 2, WK_PRELEN, WK_HEIGHT, 0xFFCCCCCC, 0, 0),
        makeVertex(WK_TALWIDTH / 2, WK_PRELEN, WK_HEIGHT, 0xFFCCCCCC, 0, 0),
        makeVertex(WK_TALWIDTH / 2, WK_PRELEN + WK_TALLEN, WK_HEIGHT, 0xFFCCCCCC, 0, 0),
        makeVertex(-WK_TALWIDTH / 2, WK_PRELEN + WK_TALLEN, WK_HEIGHT, 0xFFCCCCCC, 0, 0),
        makeVertex(-WK_TALWIDTH / 2, WK_PRELEN, WK_HEIGHT, 0xFFCCCCCC, 0, 0), ////
        makeVertex(-WK_TALWIDTH / 2 + (WK_TALWIDTH - WK_PREWIDTH) / 2, 0, 0, 0xFFCCCCCC, 0, 0),
        makeVertex(WK_TALWIDTH / 2 - (WK_TALWIDTH - WK_PREWIDTH) / 2, 0, 0, 0xFFCCCCCC, 0, 0),
        makeVertex(WK_TALWIDTH / 2 - (WK_TALWIDTH - WK_PREWIDTH) / 2, WK_PRELEN, 0, 0xFFCCCCCC, 0, 0),
        makeVertex(-WK_TALWIDTH / 2 + (WK_TALWIDTH - WK_PREWIDTH) / 2, WK_PRELEN, 0, 0xFFCCCCCC, 0, 0),
        makeVertex(WK_TALWIDTH / 2, WK_PRELEN, 0, 0xFFCCCCCC, 0, 0),
        makeVertex(WK_TALWIDTH / 2, WK_PRELEN + WK_TALLEN, 0, 0xFFCCCCCC, 0, 0),
        makeVertex(-WK_TALWIDTH / 2, WK_PRELEN + WK_TALLEN, 0, 0xFFCCCCCC, 0, 0),
        makeVertex(-WK_TALWIDTH / 2, WK_PRELEN, 0, 0xFFCCCCCC, 0, 0));
    wkd->addIndices(60,
        0, 1, 2, 0, 2, 3, 4, 5, 6, 4, 6, 7,
        8, 9, 16, 9, 16, 17, 9, 10, 18, 9, 17, 18,
        10, 12, 18, 12, 18, 19, 12, 13, 20, 12, 19, 20,
        13, 14, 21, 13, 20, 21, 14, 15, 21, 15, 21, 22,
        15, 11, 22, 11, 22, 23, 11, 23, 8, 23, 16, 8);

    wkg->addVertices(24,
        makeVertex(-WK_TALWIDTH / 2 + WK_WING + WK_TALWIDTH / 12, 0, WK_HEIGHT, 0xFFFFFFFF, 0, 0),
        makeVertex(-WK_TALWIDTH / 2 + WK_WING + WK_TALWIDTH / 12 + WK_PREWIDTH, 0, WK_HEIGHT, 0xFFFFFFFF, 0, 0),
        makeVertex(-WK_TALWIDTH / 2 + WK_WING + WK_TALWIDTH / 12 + WK_PREWIDTH, WK_PRELEN, WK_HEIGHT, 0xFFFFFFFF, 0, 0),
        makeVertex(-WK_TALWIDTH / 2 + WK_WING + WK_TALWIDTH / 12, WK_PRELEN, WK_HEIGHT, 0xFFFFFFFF, 0, 0),
        makeVertex(WK_TALWIDTH / 2, WK_PRELEN, WK_HEIGHT, 0xFFFFFFFF, 0, 0),
        makeVertex(WK_TALWIDTH / 2, WK_PRELEN + WK_TALLEN + WK_WING, WK_HEIGHT, 0xFFFFFFFF, 0, 0),
        makeVertex(-WK_TALWIDTH / 2, WK_PRELEN + WK_TALLEN + WK_WING, WK_HEIGHT, 0xFFFFFFFF, 0, 0),
        makeVertex(-WK_TALWIDTH / 2, WK_PRELEN, WK_HEIGHT, 0xFFFFFFFF, 0, 0), ////
        makeVertex(-WK_TALWIDTH / 2 + WK_WING + WK_TALWIDTH / 12, 0, WK_HEIGHT, 0xFFCCCCCC, 0, 0),
        makeVertex(-WK_TALWIDTH / 2 + WK_WING + WK_TALWIDTH / 12 + WK_PREWIDTH, 0, WK_HEIGHT, 0xFFCCCCCC, 0, 0),
        makeVertex(-WK_TALWIDTH / 2 + WK_WING + WK_TALWIDTH / 12 + WK_PREWIDTH, WK_PRELEN, WK_HEIGHT, 0xFFCCCCCC, 0, 0),
        makeVertex(-WK_TALWIDTH / 2 + WK_WING + WK_TALWIDTH / 12, WK_PRELEN, WK_HEIGHT, 0xFFCCCCCC, 0, 0),
        makeVertex(WK_TALWIDTH / 2, WK_PRELEN, WK_HEIGHT, 0xFFCCCCCC, 0, 0),
        makeVertex(WK_TALWIDTH / 2, WK_PRELEN + WK_TALLEN, WK_HEIGHT, 0xFFCCCCCC, 0, 0),
        makeVertex(-WK_TALWIDTH / 2, WK_PRELEN + WK_TALLEN, WK_HEIGHT, 0xFFCCCCCC, 0, 0),
        makeVertex(-WK_TALWIDTH / 2, WK_PRELEN, WK_HEIGHT, 0xFFCCCCCC, 0, 0), ////
        makeVertex(-WK_TALWIDTH / 2 + WK_WING + WK_TALWIDTH / 12, 0, 0, 0xFFCCCCCC, 0, 0),
        makeVertex(-WK_TALWIDTH / 2 + WK_WING + WK_TALWIDTH / 12 + WK_PREWIDTH, 0, 0, 0xFFCCCCCC, 0, 0),
        makeVertex(-WK_TALWIDTH / 2 + WK_WING + WK_TALWIDTH / 12 + WK_PREWIDTH, WK_PRELEN, 0, 0xFFCCCCCC, 0, 0),
        makeVertex(-WK_TALWIDTH / 2 + WK_WING + WK_TALWIDTH / 12, WK_PRELEN, 0, 0xFFCCCCCC, 0, 0),
        makeVertex(WK_TALWIDTH / 2, WK_PRELEN, 0, 0xFFCCCCCC, 0, 0),
        makeVertex(WK_TALWIDTH / 2, WK_PRELEN + WK_TALLEN, 0, 0xFFCCCCCC, 0, 0),
        makeVertex(-WK_TALWIDTH / 2, WK_PRELEN + WK_TALLEN, 0, 0xFFCCCCCC, 0, 0),
        makeVertex(-WK_TALWIDTH / 2, WK_PRELEN, 0, 0xFFCCCCCC, 0, 0));
    wkg->addIndices(60,
        0, 1, 2, 0, 2, 3, 4, 5, 6, 4, 6, 7,
        8, 9, 16, 9, 16, 17, 9, 10, 18, 9, 17, 18,
        10, 12, 18, 12, 18, 19, 12, 13, 20, 12, 19, 20,
        13, 14, 21, 13, 20, 21, 14, 15, 21, 15, 21, 22,
        15, 11, 22, 11, 22, 23, 11, 23, 8, 23, 16, 8);

    wka->addVertices(24,
        makeVertex(WK_TALWIDTH / 2 - WK_WING - WK_TALWIDTH / 24 - WK_PREWIDTH, 0, WK_HEIGHT, 0xFFFFFFFF, 0, 0),
        makeVertex(WK_TALWIDTH / 2 - WK_WING - WK_TALWIDTH / 24, 0, WK_HEIGHT, 0xFFFFFFFF, 0, 0),
        makeVertex(WK_TALWIDTH / 2 - WK_WING - WK_TALWIDTH / 24, WK_PRELEN, WK_HEIGHT, 0xFFFFFFFF, 0, 0),
        makeVertex(WK_TALWIDTH / 2 - WK_WING - WK_TALWIDTH / 24 - WK_PREWIDTH, WK_PRELEN, WK_HEIGHT, 0xFFFFFFFF, 0, 0),
        makeVertex(WK_TALWIDTH / 2, WK_PRELEN, WK_HEIGHT, 0xFFFFFFFF, 0, 0),
        makeVertex(WK_TALWIDTH / 2, WK_PRELEN + WK_TALLEN + WK_WING, WK_HEIGHT, 0xFFFFFFFF, 0, 0),
        makeVertex(-WK_TALWIDTH / 2, WK_PRELEN + WK_TALLEN + WK_WING, WK_HEIGHT, 0xFFFFFFFF, 0, 0),
        makeVertex(-WK_TALWIDTH / 2, WK_PRELEN, WK_HEIGHT, 0xFFFFFFFF, 0, 0), ////
        makeVertex(WK_TALWIDTH / 2 - WK_WING - WK_TALWIDTH / 24 - WK_PREWIDTH, 0, WK_HEIGHT, 0xFFCCCCCC, 0, 0),
        makeVertex(WK_TALWIDTH / 2 - WK_WING - WK_TALWIDTH / 24, 0, WK_HEIGHT, 0xFFCCCCCC, 0, 0),
        makeVertex(WK_TALWIDTH / 2 - WK_WING - WK_TALWIDTH / 24, WK_PRELEN, WK_HEIGHT, 0xFFCCCCCC, 0, 0),
        makeVertex(WK_TALWIDTH / 2 - WK_WING - WK_TALWIDTH / 24 - WK_PREWIDTH, WK_PRELEN, WK_HEIGHT, 0xFFCCCCCC, 0, 0),
        makeVertex(WK_TALWIDTH / 2, WK_PRELEN, WK_HEIGHT, 0xFFCCCCCC, 0, 0),
        makeVertex(WK_TALWIDTH / 2, WK_PRELEN + WK_TALLEN, WK_HEIGHT, 0xFFCCCCCC, 0, 0),
        makeVertex(-WK_TALWIDTH / 2, WK_PRELEN + WK_TALLEN, WK_HEIGHT, 0xFFCCCCCC, 0, 0),
        makeVertex(-WK_TALWIDTH / 2, WK_PRELEN, WK_HEIGHT, 0xFFCCCCCC, 0, 0), ////
        makeVertex(WK_TALWIDTH / 2 - WK_WING - WK_TALWIDTH / 24 - WK_PREWIDTH, 0, 0, 0xFFCCCCCC, 0, 0),
        makeVertex(WK_TALWIDTH / 2 - WK_WING - WK_TALWIDTH / 24, 0, 0, 0xFFCCCCCC, 0, 0),
        makeVertex(WK_TALWIDTH / 2 - WK_WING - WK_TALWIDTH / 24, WK_PRELEN, 0, 0xFFCCCCCC, 0, 0),
        makeVertex(WK_TALWIDTH / 2 - WK_WING - WK_TALWIDTH / 24 - WK_PREWIDTH, WK_PRELEN, 0, 0xFFCCCCCC, 0, 0),
        makeVertex(WK_TALWIDTH / 2, WK_PRELEN, 0, 0xFFCCCCCC, 0, 0),
        makeVertex(WK_TALWIDTH / 2, WK_PRELEN + WK_TALLEN, 0, 0xFFCCCCCC, 0, 0),
        makeVertex(-WK_TALWIDTH / 2, WK_PRELEN + WK_TALLEN, 0, 0xFFCCCCCC, 0, 0),
        makeVertex(-WK_TALWIDTH / 2, WK_PRELEN, 0, 0xFFCCCCCC, 0, 0));
    wka->addIndices(60,
        0, 1, 2, 0, 2, 3, 4, 5, 6, 4, 6, 7,
        8, 9, 16, 9, 16, 17, 9, 10, 18, 9, 17, 18,
        10, 12, 18, 12, 18, 19, 12, 13, 20, 12, 19, 20,
        13, 14, 21, 13, 20, 21, 14, 15, 21, 15, 21, 22,
        15, 11, 22, 11, 22, 23, 11, 23, 8, 23, 16, 8);

    bk->addVertices(10,
        makeVertex(-BK_WIDTH / 2, 0, BK_HEIGHT + BK_BOTTOM, 0xFF000000, 0, 0),
        makeVertex(BK_WIDTH / 2, 0, BK_HEIGHT + BK_BOTTOM, 0xFF000000, 0, 0),
        makeVertex(BK_WIDTH / 2, BK_PRELEN, BK_HEIGHT + BK_BOTTOM, 0xFF000000, 0, 0),
        makeVertex(-BK_WIDTH / 2, BK_PRELEN, BK_HEIGHT + BK_BOTTOM, 0xFF000000, 0, 0),
        makeVertex(-BK_WIDTH / 2, WK_PRELEN * 0.995, BK_DBOTTOM + BK_BOTTOM, 0xFF000000, 0, 0),
        makeVertex(BK_WIDTH / 2, WK_PRELEN * 0.995, BK_DBOTTOM + BK_BOTTOM, 0xFF000000, 0, 0),
        makeVertex(BK_WIDTH / 2, WK_PRELEN * 0.995, BK_BOTTOM, 0xFF000000, 0, 0),
        makeVertex(-BK_WIDTH / 2, WK_PRELEN * 0.995, BK_BOTTOM, 0xFF000000, 0, 0),
        makeVertex(-BK_WIDTH / 2, 0, BK_BOTTOM, 0xFF000000, 0, 0),
        makeVertex(BK_WIDTH / 2, 0, BK_BOTTOM, 0xFF000000, 0, 0));
    bk->addIndices(48,
        0, 1, 3, 1, 2, 3, 2, 3, 4, 2, 4, 5,
        4, 5, 7, 5, 6, 7, 6, 7, 8, 6, 8, 9,
        0, 1, 8, 1, 8, 9, 2, 5, 6, 2, 6, 9,
        2, 9, 1, 3, 4, 7, 3, 7, 8, 3, 8, 0);
}
