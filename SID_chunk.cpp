/*
 * File:   chunk.cpp
 * Author: geth2k
 *
 * Created on 18 April 2013, 20:09
 */

#include "SID_chunk.h"

sid::biomes sid::chunk::s_Biomes = sid::biomes();

sid::chunk::chunk(int chunkOffsetX, int chunkOffsetZ) :
m_Plots(nullptr),
m_rect3d(nullptr),
m_blReady(false),
m_offset(chunkOffsetX * (plots::CHUNK_DIAM - 1), chunkOffsetZ * (plots::CHUNK_DIAM - 1)),
object_ingame(VISIBLE | ENABLED) {

    this->m_vboOpaque = new sid::vbo();
    this->m_vboTransparent = new sid::vbo();

    this->set_draw_delegate(new drawVBO());
    this->set_select_delegate(new select_3Drect());
}

sid::chunk::chunk(const chunk& orig) :
object_ingame(orig),
m_offset(orig.m_offset) {

    this->m_offset = orig.m_offset;
    this->m_vboOpaque = orig.m_vboOpaque;
    this->m_vboTransparent = orig.m_vboTransparent;
    this->m_rect3d = orig.m_rect3d;
    this->m_blReady = orig.m_blReady;

    this->reset_kill_timer();
}

sid::chunk::~chunk() {
    if (this->m_Plots)
        delete m_Plots;
    if (this->m_rect3d)
        delete this->m_rect3d;
}

void sid::chunk::reset_chunk() {

}

void sid::chunk::set_chunk() {
    if (m_Plots == nullptr) {

        bool blHasLand(false);

        m_Plots = new plots(this);
        for (int iZPlot = 0; iZPlot < plots::CHUNK_DIAM; iZPlot++) {
            for (int iXPlot = 0; iXPlot < plots::CHUNK_DIAM; iXPlot++) {
                int iXWorld = iXPlot + m_offset.get_x();
                int iZWorld = iZPlot + m_offset.get_z();

                plot* PlotCurrent = this->m_Plots->at(plots::coords_to_index(iXPlot, iZPlot));
                
                float fPerlinHeight = chunk::s_Biomes[0]->get_height(iXWorld, iZWorld);

                PlotCurrent->set_height(fPerlinHeight);
                fPerlinHeight = PlotCurrent->get_height();

                vertex3 vtxTemp((m_offset.get_x() + iXPlot) * plot::PLOT_WIDTH,
                        fPerlinHeight,
                        (m_offset.get_z() + iZPlot) * plot::PLOT_WIDTH);

                this->m_Plots->at(plots::coords_to_index(iXPlot, iZPlot))->set_position(vtxTemp);

                if (fPerlinHeight > plot::SEA_LEVEL) {
                    blHasLand = true;
                    this->m_Plots->m_blHasWater = true;
                }
                
                float fPerlinTrees = chunk::s_Biomes[0]->get_height(iXWorld, iZWorld);
                if (fPerlinTrees > 0.5) {
                    PlotCurrent->set_structure(new structure(1));
                }                
            }
        }

        if (blHasLand) {

            int iXChunk = m_offset.get_x();
            int iZChunk = m_offset.get_z();

            int iXNorthChunk = iXChunk + plots::CHUNK_VISIBLE_DIAM;
            int iZNorthChunk = iZChunk;
            int iXEastChunk = iXChunk;
            int iZEastChunk = iZChunk + plots::CHUNK_VISIBLE_DIAM;
            int iXSouthChunk = iXChunk - plots::CHUNK_VISIBLE_DIAM;
            int iZSouthChunk = iZChunk;
            int iXWestChunk = iXChunk;
            int iZWestChunk = iZChunk - plots::CHUNK_VISIBLE_DIAM;
            int iXNEChunk = iXChunk + plots::CHUNK_VISIBLE_DIAM;
            int iZNEChunk = iZChunk + plots::CHUNK_VISIBLE_DIAM;
            int iXSEChunk = iXChunk - plots::CHUNK_VISIBLE_DIAM;
            int iZSEChunk = iZChunk + plots::CHUNK_VISIBLE_DIAM;
            int iXSWChunk = iXChunk - plots::CHUNK_VISIBLE_DIAM;
            int iZSWChunk = iZChunk - plots::CHUNK_VISIBLE_DIAM;
            int iXNWChunk = iXChunk + plots::CHUNK_VISIBLE_DIAM;
            int iZNWChunk = iZChunk - plots::CHUNK_VISIBLE_DIAM;

            bool blWaterNorth = !chunk::s_Biomes[0]->check_height_over(
                    iXNorthChunk,
                    iXNorthChunk + plots::CHUNK_DIAM,
                    iZNorthChunk,
                    iZNorthChunk + plots::CHUNK_DIAM,
                    plot::SEA_LEVEL);
            const biome* biomeNorth;
            if (blWaterNorth) {
                biomeNorth = chunk::s_Biomes[0];
            } else {
                biomeNorth = chunk::s_Biomes.get_biome(iXNorthChunk, iZNorthChunk);
            }

            bool blWaterEast = !chunk::s_Biomes[0]->check_height_over(
                    iXEastChunk,
                    iXEastChunk + plots::CHUNK_DIAM,
                    iZEastChunk,
                    iZEastChunk + plots::CHUNK_DIAM,
                    plot::SEA_LEVEL);
            const biome* biomeEast;
            if (blWaterEast) {
                biomeEast = chunk::s_Biomes[0];
            } else {
                biomeEast = chunk::s_Biomes.get_biome(iXEastChunk, iZEastChunk);
            }

            bool blWaterSouth = !chunk::s_Biomes[0]->check_height_over(
                    iXSouthChunk,
                    iXSouthChunk + plots::CHUNK_DIAM,
                    iZSouthChunk,
                    iZSouthChunk + plots::CHUNK_DIAM,
                    plot::SEA_LEVEL);
            const biome* biomeSouth;
            if (blWaterSouth) {
                biomeSouth = chunk::s_Biomes[0];
            } else {
                biomeSouth = chunk::s_Biomes.get_biome(iXSouthChunk, iZSouthChunk);
            }

            bool blWaterWest = !chunk::s_Biomes[0]->check_height_over(
                    iXWestChunk,
                    iXWestChunk + plots::CHUNK_DIAM,
                    iZWestChunk,
                    iZWestChunk + plots::CHUNK_DIAM,
                    plot::SEA_LEVEL);
            const biome* biomeWest;
            if (blWaterWest) {
                biomeWest = chunk::s_Biomes[0];
            } else {
                biomeWest = chunk::s_Biomes.get_biome(iXWestChunk, iZWestChunk);
            }

            bool blWaterNE = !chunk::s_Biomes[0]->check_height_over(
                    iXNEChunk,
                    iXNEChunk + plots::CHUNK_DIAM,
                    iZNEChunk,
                    iZNEChunk + plots::CHUNK_DIAM,
                    plot::SEA_LEVEL);
            const biome* biomeNE;
            if (blWaterNE) {
                biomeNE = chunk::s_Biomes[0];
            } else {
                biomeNE = chunk::s_Biomes.get_biome(iXNEChunk, iZNEChunk);
            }

            bool blWaterSE = !chunk::s_Biomes[0]->check_height_over(
                    iXSEChunk,
                    iXSEChunk + plots::CHUNK_DIAM,
                    iZSEChunk,
                    iZSEChunk + plots::CHUNK_DIAM,
                    plot::SEA_LEVEL);
            const biome* biomeSE;
            if (blWaterSE) {
                biomeSE = chunk::s_Biomes[0];
            } else {
                biomeSE = chunk::s_Biomes.get_biome(iXSEChunk, iZSEChunk);
            }

            bool blWaterSW = !chunk::s_Biomes[0]->check_height_over(
                    iXSWChunk,
                    iXSWChunk + plots::CHUNK_DIAM,
                    iZSWChunk,
                    iZSWChunk + plots::CHUNK_DIAM,
                    plot::SEA_LEVEL);
            const biome* biomeSW;
            if (blWaterSW) {
                biomeSW = chunk::s_Biomes[0];
            } else {
                biomeSW = chunk::s_Biomes.get_biome(iXSWChunk, iZSWChunk);
            }

            bool blWaterNW = !chunk::s_Biomes[0]->check_height_over(
                    iXNWChunk,
                    iXNWChunk + plots::CHUNK_DIAM,
                    iZNWChunk,
                    iZNWChunk + plots::CHUNK_DIAM,
                    plot::SEA_LEVEL);
            const biome* biomeNW;
            if (blWaterNW) {
                biomeNW = chunk::s_Biomes[0];
            } else {
                biomeNW = chunk::s_Biomes.get_biome(iXNWChunk, iZNWChunk);
            }

            std::array<bool, 8 > vblWaterDirections({
                    blWaterNorth,
                    blWaterNE,
                    blWaterEast,
                    blWaterSE,
                    blWaterSouth,
                    blWaterSW,
                    blWaterWest,
                    blWaterNW});
                
            biome const* biomeCenter = this->s_Biomes.get_biome(
                    m_offset.get_x(), m_offset.get_z());
            
            std::array<bool, 8> vblBiomeDirections({
                    biomeNorth != biomeCenter || blWaterNorth,
                    biomeNE != biomeCenter || blWaterNE,
                    biomeEast != biomeCenter || blWaterEast,
                    biomeSE != biomeCenter || blWaterSE,
                    biomeSouth != biomeCenter || blWaterSouth,
                    biomeSW != biomeCenter || blWaterSW,
                    biomeWest != biomeCenter || blWaterWest,
                    biomeNW != biomeCenter || blWaterNW});

            for (int iZ = 0; iZ < plots::CHUNK_DIAM; iZ++) {
                for (int iX = 0; iX < plots::CHUNK_DIAM; iX++) {

                    float fCompoundRatio = this->__get_compound_ratio(
                            iX, iZ, vblWaterDirections);

                    int iX2, iZ2;
                    iX2 = iX + m_offset.get_x();
                    iZ2 = iZ + m_offset.get_z();

                    float fPerlinHeight = chunk::s_Biomes[1]->get_height(iX2, iZ2);
                    float fFadedHeight = fPerlinHeight * fCompoundRatio;

                    plot* PlotCurrent = this->m_Plots->at(plots::coords_to_index(iX, iZ));
                    float fCurrentHeight = PlotCurrent->get_height();

                    float fHeight = fCurrentHeight + fFadedHeight;

                    PlotCurrent->set_height(fHeight);
                    fHeight = PlotCurrent->get_height();

                    vertex3 vtxTemp((m_offset.get_x() + iX) * plot::PLOT_WIDTH,
                            fHeight,
                            (m_offset.get_z() + iZ) * plot::PLOT_WIDTH);

                    this->m_Plots->at(plots::coords_to_index(iX, iZ))->set_position(vtxTemp);

                    float fPerlinTrees = chunk::s_Biomes[1]->get_height(iX2, iZ2);
                    if (fPerlinTrees > 0.5) {
                        this->m_Plots->at(
                                plots::coords_to_index(iX, iZ))->set_structure(
                                new structure());
                    }
                }
            }

            int iBiomeCenterIdx = biomeCenter->get_index();
            
            if (iBiomeCenterIdx > 1) {
                for (int iZ = 0; iZ < plots::CHUNK_DIAM; iZ++) {
                    for (int iX = 0; iX < plots::CHUNK_DIAM; iX++) {
                        
                        int iX2, iZ2;
                        iX2 = iX + m_offset.get_x();
                        iZ2 = iZ + m_offset.get_z();
                        
                        float fCompoundRatio = this->__get_compound_ratio(
                            iX, iZ, vblBiomeDirections);
                        
                        plot* PlotCurrent = this->m_Plots->at(
                                plots::coords_to_index(iX, iZ));
                        
                        float fCurrentHeight = PlotCurrent->get_height();
                        float fPerlinHeight = biomeCenter->get_height(iX2, iZ2);
                        float fFadedHeight = fPerlinHeight * fCompoundRatio;
                        
                        float fHeight = fCurrentHeight + fFadedHeight;
                        
                        PlotCurrent->set_height(fHeight);
                        fHeight = PlotCurrent->get_height();

                        vertex3 vtxTemp((m_offset.get_x() + iX) * plot::PLOT_WIDTH,
                                fHeight,
                                (m_offset.get_z() + iZ) * plot::PLOT_WIDTH);

                        this->m_Plots->at(plots::coords_to_index(iX, iZ))->set_position(vtxTemp);
                    }
                }
            }
        }
    }

    uint uiReserveSize = pow(plots::CHUNK_DIAM - 1, 2) * 6;
    this->m_vboOpaque->reserve(uiReserveSize);
    make_land_vertices();
    make_land_indices();
    make_land_verts_array();
    make_land_normals_array();
    make_land_colors_array();
    make_land_tex_coords();

    this->m_vboTransparent->reserve(uiReserveSize);
    make_sea_vertices();
    make_sea_indices();
    make_sea_verts_array();
    make_sea_normals_array();
    make_sea_colors_array();

    vertex3 p0 = this->m_Plots->at(0, 0)->get_position();
    vertex3 p1 = this->m_Plots->at(0, plots::CHUNK_DIAM - 1)->get_position();
    vertex3 p2 = this->m_Plots->at(plots::CHUNK_DIAM - 1, plots::CHUNK_DIAM - 1)->get_position();
    vertex3 p3 = this->m_Plots->at(plots::CHUNK_DIAM - 1, 0)->get_position();
    if (this->m_rect3d != nullptr) {
        delete this->m_rect3d;
    }

    this->m_rect3d = new rectangle_3d(p0, p1, p2, p3);

    m_vboOpaque->set_texture_id(object::_s_Textures.get_tex_id_by_ref(500));
}

float sid::chunk::__get_compound_ratio(
        int x, int z, std::array<bool, 8 > const& boolArray) const {
    
    std::array<float, 8> vfDirectionRatios;

    if (x < plots::CHUNK_CENTER) {
        vfDirectionRatios[0] = 1;
        vfDirectionRatios[4] = this->__get_fade_ratio(x);
    } else {
        vfDirectionRatios[0] = this->__get_fade_ratio(x);
        vfDirectionRatios[4] = 1;
    }

    if (z < plots::CHUNK_CENTER) {
        vfDirectionRatios[2] = 1;
        vfDirectionRatios[6] = this->__get_fade_ratio(z);
    } else {
        vfDirectionRatios[2] = this->__get_fade_ratio(z);
        vfDirectionRatios[6] = 1;
    }

    float fCompoundRatio(1);
    for (int i = 0; i < 8; i = i + 2) {
        if (boolArray[i]) {
            fCompoundRatio *= vfDirectionRatios[i];
        }
    }

    for (int i = 1; i < 8; i = i + 2) {
        if (boolArray[i]) {
            int iLeft = i - 1;
            int iRight = i + 1;
            if (iRight == 8) iRight -= 8;
            
            float fRatioLeft = vfDirectionRatios[iLeft];
            float fRatioRight = vfDirectionRatios[iRight];
            
            if (fRatioLeft < 1.0 && fRatioRight < 1.0) {

                float fRatioLeftSquared = fRatioLeft * fRatioLeft;
                float fRatioRightSquared = fRatioRight * fRatioRight;
                
                float fCornerRatio = 
                        sqrt(fRatioLeftSquared + fRatioRightSquared);

                if (fCornerRatio > 1) fCornerRatio = 1;

                fCompoundRatio *= fCornerRatio;
            }
        }
    }

    return fCompoundRatio;
}

float sid::chunk::__get_fade_ratio(int value) const {
    float fCenter = plots::CHUNK_CENTER;
    float fCenterOuter = fCenter / 2;

    float fDistance1;
    float fDistance2;
    fDistance1 = value - plots::CHUNK_CENTER - plots::CHUNK_CENTER / 2 + 1;
    if (fDistance1 < -(fCenterOuter - 1))
        fDistance2 = fDistance1 + plots::CHUNK_VISIBLE_DIAM - (value * 2);
    else
        fDistance2 = fDistance1;

    float fRatio = fDistance2 / fCenterOuter;
    float fCosineRatio;
    float fNormalisedRatio;
    int iSign;

    if (fRatio == 0) {
        iSign = 0;
    } else {
        iSign = fRatio / std::abs(fRatio);
    }

    fCosineRatio = (std::acos(1 - std::abs(fRatio)) * iSign) / M_PI_2;
    fNormalisedRatio = (1 + fCosineRatio) / 2;

    return 1 - fNormalisedRatio;
}

sid::plots* sid::chunk::get_plots() {
    return m_Plots;
}

sid::chunk::chunkoffset sid::chunk::get_offset() {
    return (m_offset);
}

void sid::chunk::make_land_vertices() {
    sid::chunk::chunkoffset offset = this->m_offset;

    for (int i = 0; i < plots::CHUNK_SIZE; i++) {

        int iPlotX = i % plots::CHUNK_DIAM;
        int iPlotZ = i / plots::CHUNK_DIAM;

        uint uiPlotPointer = plots::coords_to_index(iPlotX, iPlotZ);
        float fArrayX = (offset.get_x() * plot::PLOT_WIDTH) + (iPlotX * plot::PLOT_WIDTH);
        float fArrayY = this->m_Plots->at(uiPlotPointer)->get_height();
        float fArrayZ = (offset.get_z() * plot::PLOT_WIDTH) + (iPlotZ * plot::PLOT_WIDTH);
        sid::t_vertset vsVerts(fArrayX, fArrayY, fArrayZ);

        m_vboOpaque->push_vertset(vsVerts);
    }
}

void sid::chunk::make_land_indices() {
    uint uiStripLength = plots::CHUNK_VISIBLE_DIAM * 6;

    bool blFlipPoly = false;

    for (int iStrip = 0; iStrip < plots::CHUNK_VISIBLE_DIAM; iStrip++) {
        for (int iPoint = 0; iPoint < uiStripLength; iPoint++) {
            m_vboOpaque->push_index(
                    triangle_point_to_index(iStrip, iPoint, &blFlipPoly));
        }
    }
}

void sid::chunk::make_land_verts_array() {
    m_vboOpaque->build_verts_array();
}

void sid::chunk::make_land_normals_array() {
    vector3 eucvPoints[TRI_POINTS];

    for (int iIndex = 0; iIndex < m_vboOpaque->get_indeces_size(); iIndex += TRI_POINTS) {
        for (int iPoint = 0; iPoint < TRI_POINTS; iPoint++) {
            sid::t_vertset Verts(m_vboOpaque->get_arrayvert(iIndex + iPoint));
            eucvPoints[iPoint] = euclid(
                    Verts[0], Verts[1], Verts[2]);

        }

        euclid eucNormal = vector3::get_vector_normal(eucvPoints[0],
                eucvPoints[1], eucvPoints[2]);

        sid::t_normal fvNormal(eucNormal[0], eucNormal[1], eucNormal[2]);

        for (int iNormPoint = 0; iNormPoint < TRI_POINTS; iNormPoint++) {
            m_vboOpaque->push_normal(fvNormal);
        }
    }
}

void sid::chunk::make_land_colors_array() {
    sid::t_color csSeabedColor(250, 250, 200, 255);
    sid::t_color csRockFace(230, 230, 230, 255);
    sid::t_color csLandColor(115, 240, 128, 255);
    sid::t_color csIceCapColor(255, 255, 255, 255);
    sid::triplet<GLfloat> fvPoint;

    biome const* biomeTemp =
            this->s_Biomes.get_biome(m_offset.get_x(), m_offset.get_z());

    csLandColor = biomeTemp->get_color();

    for (int iIndex = 0; iIndex < m_vboOpaque->get_indeces_size(); iIndex += (TRI_POINTS * 2)) {
        float fMinHeight = FLT_MAX;
        float fMaxHeight = FLT_MIN;
        for (int iPoint = 0; iPoint < TRI_POINTS * 2; iPoint++) {
            fvPoint = m_vboOpaque->get_arrayvert(iIndex + iPoint);
            if (fvPoint[1] < fMinHeight) {
                fMinHeight = fvPoint[1];
            }
            if (fvPoint[1] > fMaxHeight) {
                fMaxHeight = fvPoint[1];
            }
        }

        float fNormalYValue(0.0);
        float fNormalYValue1(m_vboOpaque->get_normal(iIndex)[1]);
        float fNormalYValue2(m_vboOpaque->get_normal(iIndex + TRI_POINTS)[1]);

        if (fNormalYValue1 < fNormalYValue2) {
            fNormalYValue = fNormalYValue1;
        } else {
            fNormalYValue = fNormalYValue2;
        }

        if (fMinHeight >= plot::SNOW_LEVEL) {
            for (int iColorPoint = 0; iColorPoint < TRI_POINTS * 2; iColorPoint++) {
                m_vboOpaque->push_color(csIceCapColor);
            }
        } else if (fNormalYValue < 0.7) {
            for (int iColorPoint = 0; iColorPoint < TRI_POINTS * 2; iColorPoint++) {
                m_vboOpaque->push_color(csRockFace);
            }
        } else if (fMinHeight <= plot::BEACH_MIN_HEIGHT) {
            for (int iColorPoint = 0; iColorPoint < TRI_POINTS * 2; iColorPoint++) {
                m_vboOpaque->push_color(csSeabedColor);
            }
        } else {
            for (int iColorPoint = 0; iColorPoint < TRI_POINTS * 2; iColorPoint++) {
                m_vboOpaque->push_color(csLandColor);
            }
        }
    }
}

void sid::chunk::make_land_tex_coords() {

    sid::duplet<GLfloat> fvPoint;

    sid::duplet<GLfloat> texBtm_Lt(1.0, 0.0);
    sid::duplet<GLfloat> texBtm_Rt(1.0, 1.0);
    sid::duplet<GLfloat> texTop_Rt(0.0, 1.0);
    sid::duplet<GLfloat> texTop_Lt(0.0, 0.0);

    for (int iIndex = 0;
            iIndex < m_vboOpaque->get_indeces_size();
            iIndex += TRI_POINTS * 2) {

        m_vboOpaque->push_texset(texBtm_Lt);
        m_vboOpaque->push_texset(texBtm_Rt);
        m_vboOpaque->push_texset(texTop_Rt);

        m_vboOpaque->push_texset(texTop_Rt);
        m_vboOpaque->push_texset(texTop_Lt);
        m_vboOpaque->push_texset(texBtm_Lt);
    }
}

void sid::chunk::make_sea_vertices() {
    plots* plotsCurrent = m_Plots;
    plot* plotArray = plotsCurrent->first();
    sid::chunk::chunkoffset offset = m_offset;

    for (int i = 0; i < plots::CHUNK_SIZE; i++) {

        int iPlotZ = i / plots::CHUNK_DIAM;
        int iPlotX = i % plots::CHUNK_DIAM;

        float fArrayX = (offset.get_x() * plot::PLOT_WIDTH) + (iPlotX * plot::PLOT_WIDTH);
        float fArrayY = plot::SEA_LEVEL;
        float fArrayZ = (offset.get_z() * plot::PLOT_WIDTH) + (iPlotZ * plot::PLOT_WIDTH);
        sid::triplet<GLfloat> vsVerts(fArrayX, fArrayY, fArrayZ);

        m_vboTransparent->push_vertset(vsVerts);
    }

}

void sid::chunk::make_sea_indices() {
    uint uiVisiblePlotDiam = plots::CHUNK_DIAM - 1;
    uint uiStripLength = uiVisiblePlotDiam * 6;

    float fMinHeight = FLT_MAX;
    bool blFlipPoly = false;

    for (int iStrip = 0; iStrip < uiVisiblePlotDiam; iStrip++) {
        for (int iPoint = 0; iPoint < uiStripLength; iPoint++) {

            int iTriPoint = iPoint % 3;
            int iArrayPos = iStrip * uiStripLength + iPoint;

            GLuint gluiIndexValue;
            gluiIndexValue = triangle_point_to_index(iStrip,
                    iPoint, &blFlipPoly);

            GLuint gluiPreTri[3];
            gluiPreTri[iTriPoint] = gluiIndexValue;

            sid::triplet<GLfloat> fvVert;
            sid::chunk::coords_from_index(*m_vboOpaque, iArrayPos, fvVert);
            if (fvVert[1] < fMinHeight) {
                fMinHeight = fvVert[1];
            }
            if (iTriPoint == 2) {
                if (fMinHeight <= plot::BEACH_MIN_HEIGHT) {
                    for (int i = 0; i < TRI_POINTS; i++) {
                        m_vboTransparent->push_index(gluiPreTri[i]);
                    }
                }
                fMinHeight = FLT_MAX;
            }
        }
    }
}

void sid::chunk::make_sea_verts_array() {
    m_vboTransparent->build_verts_array();
}

void sid::chunk::make_sea_normals_array() {
    sid::triplet<GLfloat> fvNormal(0.0, 1.0, 0.0);

    for (int iIndex = 0; iIndex < m_vboTransparent->get_indeces_size(); iIndex++) {
        m_vboTransparent->push_normal(fvNormal);
    }
}

void sid::chunk::make_sea_colors_array() {
    sid::t_color SeaColor(20, 20, 40, 150);
    m_vboTransparent->set_default_color(SeaColor);
}

sid::t_index sid::chunk::triangle_point_to_index(int Strip, int Point, bool* Flip) {

    int iPlotNumber = Point / 6;
    int iPlotPoint = Point % 6;
    int iVertexOffsetX = Strip * plots::CHUNK_DIAM;
    int iSubPointOffset = iVertexOffsetX + iPlotNumber;

    GLuint gluiIndexValue;
    if (!*Flip) {
        switch (iPlotPoint) {
            case 0:
                gluiIndexValue = iSubPointOffset;
                break;
            case 1:
                gluiIndexValue = iSubPointOffset + plots::CHUNK_DIAM;
                break;
            case 2:
                gluiIndexValue = iSubPointOffset + plots::CHUNK_DIAM + 1;
                break;
            case 3:
                gluiIndexValue = iSubPointOffset + plots::CHUNK_DIAM + 1;
                break;
            case 4:
                gluiIndexValue = iSubPointOffset + 1;
                break;
            case 5:
                gluiIndexValue = iSubPointOffset;
                *Flip = true;
                break;
        }
    } else {
        switch (iPlotPoint) {
            case 0:
                gluiIndexValue = iSubPointOffset + plots::CHUNK_DIAM;
                break;
            case 1:
                gluiIndexValue = iSubPointOffset + plots::CHUNK_DIAM + 1;
                break;
            case 2:
                gluiIndexValue = iSubPointOffset + 1;
                break;
            case 3:
                gluiIndexValue = iSubPointOffset + 1;
                break;
            case 4:
                gluiIndexValue = iSubPointOffset;
                break;
            case 5:
                gluiIndexValue = iSubPointOffset + plots::CHUNK_DIAM;
                *Flip = false;
                break;
        }
    }
    return (gluiIndexValue);
}

void sid::chunk::coords_from_index(sid::vbo& VBO,
        GLuint index,
        sid::triplet<GLfloat>& returnArray) {

    returnArray = VBO.get_vertset(VBO.get_index(index));
}

void sid::chunk::tick() {
    int iChunkCount = this->get_parent()->get_child_count();
    if (this->is_ready()) {
        this->inc_kill_timer(iChunkCount / 200);
    }
}

void sid::chunk::set_rectangle(rectangle_3d* rect3d) {
    this->m_rect3d = rect3d;
}

sid::rectangle_3d sid::chunk::get_rectangle() const {
    return *m_rect3d;
}

bool sid::chunk::process_event(SDL_Event& evt, bool recurse) {
    int iClkX = evt.button.x;
    int iClkY = evt.button.y;
    GLint gliPlot;

    select_delegate::init_selframe_3d(iClkX, iClkY, 60, 2000);
    for (int i = 0; i < plots::CHUNK_SIZE; i++) {
        this->m_Plots->draw_select(i, nullptr);
    }
    gliPlot = select_delegate::process_selhits();

    if (gliPlot >= 0) {
        int iRow = gliPlot / (plots::CHUNK_DIAM);
        int iCol = gliPlot % (plots::CHUNK_DIAM);
        switch (evt.type) {
            case SDL_MOUSEBUTTONDOWN:
                this->m_Plots->at(iCol, iRow)->set_as_mouseover();
                break;
            case SDL_MOUSEBUTTONUP:
                this->m_Plots->at(iCol, iRow)->set_as_focus();
                break;
            case SDL_MOUSEMOTION:
                this->m_Plots->at(iCol, iRow)->set_as_mouseover();
                break;
        }
        return true;
    } else {
        return false;
    }

}

bool sid::chunk::is_ready() const {
    return m_blReady;
}

void sid::chunk::set_unready() {
    this->m_blReady = false;
}
