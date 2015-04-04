/* 
 * File:   environs.cpp
 * Author: geth2k
 * 
 * Created on 13 June 2013, 00:18
 */

#include "SID_environs.h"

sid::environs::environs(uint GridWidth) :
__m_blPauseStatus(false),
__m_DrawSelectionDelegate(new drawVBO()),
__m_Chunks(new chunks()),
__m_uiGridWidth(GridWidth),
__m_uiGridSize(GridWidth * GridWidth),
__m_blRenderReflections(false),
__m_ofsLastChunkOffset(INT_MAX, INT_MAX) {

    __m_Chunks->set_parent(this);
    this->_add_child_back(this->__m_Chunks);

    this->__m_blRenderReflections = true;

    SDL_EventState(SDL_KEYUP, SDL_ENABLE);
    SDL_EventState(SDL_KEYDOWN, SDL_ENABLE);
    SDL_EventState(SDL_TEXTEDITING, SDL_DISABLE);
    SDL_EventState(SDL_TEXTINPUT, SDL_DISABLE);
}

sid::environs::environs(const environs& orig) :
object(orig),
__m_blPauseStatus(orig.__m_blPauseStatus),
__m_DrawSelectionDelegate(orig.__m_DrawSelectionDelegate),
__m_Chunks(orig.__m_Chunks),
__m_uiGridWidth(orig.__m_uiGridWidth),
__m_uiGridSize(orig.__m_uiGridSize),
__m_blRenderReflections(orig.__m_blRenderReflections),
__m_ofsLastChunkOffset(orig.__m_ofsLastChunkOffset) {

}

sid::environs::~environs() {
    if (this->__m_DrawSelectionDelegate)
        delete this->__m_DrawSelectionDelegate;
}

void sid::environs::build_environs(chunk::chunkoffset ofsChunkOffset) {
    if (ofsChunkOffset != this->__m_ofsLastChunkOffset) {

        this->__populate_offset_list(ofsChunkOffset);
        if (!this->__load_chunks_from_cache())
            this->__load_chunks_from_new();

        this->__m_ofsLastChunkOffset = ofsChunkOffset;
    }
}

void sid::environs::__populate_offset_list(chunk::chunkoffset ofsChunkOffset) {

    this->__m_vRenderOffsets.clear();
    for (int iZ = 0; iZ < __m_uiGridWidth; iZ++) {
        for (int iX = 0; iX < __m_uiGridWidth; iX++) {
            const int CHUNKS_TO_EDGE = (__m_uiGridWidth - 1) / 2;
            const int DIAM = plots::CHUNK_DIAM - 1;

            int iOffsetX = (iX - CHUNKS_TO_EDGE) * DIAM + ofsChunkOffset.get_x();
            int iOffsetZ = (iZ - CHUNKS_TO_EDGE) * DIAM + ofsChunkOffset.get_z();
            chunk::chunkoffset ofsTemp(iOffsetX, iOffsetZ);
            uint uiGridPosition = iZ * __m_uiGridWidth + iX;
            this->__m_vRenderOffsets[uiGridPosition] = ofsTemp;
        }
    }
}

bool sid::environs::__load_chunks_from_cache() {
    // For each chunk, check if its offset value is in __m_vRenderOffsets.
    // Where a cached chunk is required, add a pointer to it to 
    // the render list.
    char chRetVal = this->__m_Chunks->forfirst(
            [this](chunk* ref, int loop, void* args) {
                return this->__crossref_chunks_offsets(ref);
            });

    // If we didn't complete the list return false.
    return (chRetVal == 1) ? true : false;
}

bool sid::environs::__crossref_chunks_offsets(chunk* ref) {
    chunk* chnk = ref;
    chunk::chunkoffset ofsCache = chnk->get_offset();

    bool blAllInit(true);
    for (__t_chunkoffsets_iter it2 = this->__m_vRenderOffsets.begin();
            it2 != this->__m_vRenderOffsets.end();
            ++it2) {

        chunk::chunkoffset ofsDraw = it2->second;
        if (ofsCache == ofsDraw) {
            it2->second.set_chunk(chnk);
            blAllInit = false;
            break;
        }
        if (!it2->second.is_init())
            blAllInit = false;
    }

    return blAllInit;
}

void sid::environs::__load_chunks_from_new() {

    
    for (sid::environs::__t_chunkoffsets_iter iter = this->__m_vRenderOffsets.begin();
            iter != this->__m_vRenderOffsets.end();
            ++iter) {
        if (!iter->second.is_init()) {
            chunk::chunkoffset ofsDraw = iter->second;
            const int DIAM = plots::CHUNK_DIAM - 1;
            std::string strName_Chunk = "Chunk";
            int iX = ofsDraw.get_x() / DIAM;
            int iZ = ofsDraw.get_z() / DIAM;

            chunk* chnkTemp = new chunk(iX, iZ);

            //chnkTemp->set_name(strName_Chunk);
            chnkTemp->set_parent(this->__m_Chunks);

            //this->__m_Chunks->lock_builder_thread();
            this->__m_Chunks->add_chunk(chnkTemp);
            iter->second.set_chunk(chnkTemp);
            //this->__m_Chunks->unlock_builder_thread();
            chnkTemp = nullptr;
        }
    }
}

sid::chunk* sid::environs::get_cached_chunk(__t_chunkoffset cofs, bool WaitForReady) {
    chunk* chnkRetVal(nullptr);
    bool chRetVal = this->__m_Chunks->forfirst(
        [&](chunk* ref, int loop, void* args) {
            if (ref->get_offset() == *static_cast<__t_chunkoffset*>(args)) {
                chnkRetVal = ref;
                return true;
            }
            return false;
        }, &cofs);
    if (WaitForReady) {
        while (!chnkRetVal->is_ready()) {
            SDL_Delay(100);
        }
    }
    
    return chnkRetVal;
}

void sid::environs::render_all(frustum const& frust, uint flags) {
    glPushMatrix();

    // --------------------------------------------------------------
    // Set water reflected lighting values.
    // --------------------------------------------------------------

    GLfloat lightAmbient_Reflect[] = {0.05, 0.01, 0.01, 1.0};
    GLfloat lightDiffuse_Reflect[] = {0.2, 0.2, 0.2, 1.0};
    GLfloat lightSpecular_Reflect[] = {1.0, 1.0, 1.0, 1.0};
    GLfloat lightPosition_Reflect[] = {100000.0, -100000.0, 100000.0, 0.0};
    GLfloat lightModelAmbient_Reflect[] = {0.05, 0.05, 0.1, 1.0};

    // --------------------------------------------------------------
    // Set normal terrain lighting values.
    // --------------------------------------------------------------

    GLfloat ambientLight1[] = {0.6, 0.6, 0.6, 1.0};
    GLfloat diffuseLight1[] = {0.3, 0.3, 0.3, 1.0};
    GLfloat specularLight1[] = {1.0, 1.0, 1.0, 1.0};
    GLfloat position1[] = {100000.0, 100000.0, 100000.0, 0.0};
    GLfloat lightModelAmbient1[] = {0.0, 0.0, 0.0, 1.0};

    // --------------------------------------------------------------
    // Render Terrain.
    // --------------------------------------------------------------

    glDisable(GL_LIGHT0);
    glEnable(GL_LIGHT1);
    glCullFace(GL_BACK);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_STENCIL_TEST);
    glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
    glStencilFunc(GL_ALWAYS, 0, 0xffffffff);

    glPushMatrix();
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lightModelAmbient1);
    glLightfv(GL_LIGHT1, GL_AMBIENT, ambientLight1);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, diffuseLight1);
    glLightfv(GL_LIGHT1, GL_SPECULAR, specularLight1);
    glLightfv(GL_LIGHT1, GL_POSITION, position1);
    glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 0.0);
    this->__render_opaque(frust, flags);
    glPopMatrix();


    if (this->__m_blRenderReflections) {

        // --------------------------------------------------------------
        // Render the stencils.
        // --------------------------------------------------------------    

        glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
        glStencilFunc(GL_EQUAL, 1, 0xffffffff);
        glStencilOp(GL_REPLACE, GL_INCR, GL_KEEP);

        this->__render_transparent(frust, flags);

        glStencilOp(GL_KEEP, GL_DECR, GL_KEEP);
        glStencilFunc(GL_EQUAL, 1, 0xffffffff);
        this->__render_transparent(frust, flags);

        // --------------------------------------------------------------
        // Draw the upside down world as the reflection
        // --------------------------------------------------------------

        glDisable(GL_DEPTH_TEST);
        glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
        glStencilFunc(GL_EQUAL, 1, 0xffffffff);
        glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
        glCullFace(GL_FRONT);
        glEnable(GL_CLIP_PLANE0);
        glDisable(GL_LIGHT1);
        glEnable(GL_LIGHT0);

        glPushMatrix();
        glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lightModelAmbient_Reflect);
        glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmbient_Reflect);
        glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDiffuse_Reflect);
        glLightfv(GL_LIGHT0, GL_SPECULAR, lightSpecular_Reflect);
        glLightfv(GL_LIGHT0, GL_POSITION, lightPosition_Reflect);
        glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 10.0);

        // Translate up so that sea level is at 0.
        glTranslatef(0.0f, plot::SEA_LEVEL, 0.0f);

        double eqr[] = {0.0f, -1.0f, 0.0f, 0.0f};
        glClipPlane(GL_CLIP_PLANE0, eqr);
        glTranslatef(0.0f, plot::SEA_LEVEL, 0.0f);
        glScalef(1.0f, -1.0, 1.0);
        this->__render_opaque(frust, flags | object::draw_flags::NO_TEXTURE);
        glPopMatrix();

    }

    // --------------------------------------------------------------
    // Draw the water for real. No stencils or anything.
    // --------------------------------------------------------------

    glDisable(GL_CLIP_PLANE0);
    glDisable(GL_STENCIL_TEST);
    glEnable(GL_DEPTH_TEST);
    glCullFace(GL_BACK);

    glPushMatrix();
    glEnable(GL_LIGHT1);
    glEnable(GL_LIGHT0);
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lightModelAmbient_Reflect);
    glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmbient_Reflect);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDiffuse_Reflect);
    glLightfv(GL_LIGHT0, GL_SPECULAR, lightSpecular_Reflect);
    glLightfv(GL_LIGHT0, GL_POSITION, lightPosition_Reflect);
    glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 120.0);
    this->__render_transparent(frust, flags);
    glPopMatrix();

    glPopMatrix();
}

void sid::environs::__render_opaque(frustum const& frust, uint flags) {
    const int DIAM = plots::CHUNK_DIAM - 1;
    int iChunk = -1;
    int iStepDiam = sid::environs::__m_uiGridWidth;
    int iLoop = 0;

    while (iStepDiam >= 0) {
        for (int i = 0; i < 2; i++) {
            if (iLoop == 0) {
                i = 1;
            }
            for (int j = 0; j < iStepDiam; j++) {
                switch (iLoop % 4) {
                    case 0:
                        iChunk++;
                        break;
                    case 1:
                        iChunk += sid::environs::__m_uiGridWidth;
                        break;
                    case 2:
                        iChunk--;
                        break;
                    case 3:
                        iChunk -= sid::environs::__m_uiGridWidth;
                        break;
                }

                if (iChunk < sid::environs::__m_uiGridSize && iChunk >= 0) {
                    chunk* chunkCurrent = this->get_chunk(iChunk);

                    if (chunkCurrent && chunkCurrent->is_ready()) {
                        chunkCurrent->reset_kill_timer();

                        std::array<vertex3, 4> vtxCorners;
                        plots* pltsTemp = chunkCurrent->get_plots();
                        vtxCorners[0] = pltsTemp->at(0, 0)->get_position();
                        vtxCorners[1] = pltsTemp->at(0, DIAM)->get_position();
                        vtxCorners[2] = pltsTemp->at(DIAM, DIAM)->get_position();
                        vtxCorners[3] = pltsTemp->at(DIAM, 0)->get_position();

                        bool blInFrustum = false;
                        for (int i = 0; i < vtxCorners.size(); i++) {
                            if (frust.testPoint(
                                    vtxCorners[i][0], vtxCorners[i][1], vtxCorners[i][2])) {

                                blInFrustum = true;
                                break;
                            }
                        }
                        if (blInFrustum && !chunkCurrent->is_deletable()) {
                            chunkCurrent->render_all(0, flags);
                        }
                    }
                }
            }
            iLoop++;
            if (i == 1) {
                iStepDiam--;
            }
        }
    }
}

void sid::environs::__render_transparent(frustum const& frust, uint flags) {
    const int DIAM = plots::CHUNK_DIAM - 1;
    for (int iChunk = 0; iChunk < sid::environs::__m_uiGridSize; iChunk++) {
        chunk* chunkCurrent = this->get_chunk(iChunk);
        if (chunkCurrent->is_ready()) {
            chunkCurrent->reset_kill_timer();

            std::array<vertex3, 4> vtxCorners;
            plots* pltsTemp = chunkCurrent->get_plots();
            vtxCorners[0] = pltsTemp->at(0, 0)->get_position();
            vtxCorners[1] = pltsTemp->at(0, DIAM)->get_position();
            vtxCorners[2] = pltsTemp->at(DIAM, DIAM)->get_position();
            vtxCorners[3] = pltsTemp->at(DIAM, 0)->get_position();            

            bool blInFrustum = false;
            for (int i = 0; i < vtxCorners.size(); i++) {
                if (frust.testPoint(vtxCorners[i][0], vtxCorners[i][1], vtxCorners[i][2])) {
                    blInFrustum = true;
                    break;
                }
            }
            if (blInFrustum && !chunkCurrent->is_deletable()) {
                chunkCurrent->render_all(0, object::draw_flags::TRANSPARENT);
            }
        }
    }
}

bool sid::environs::process_event(SDL_Event& evt, bool drag, bool recurse) {

    if ((this->__m_iMouseMode == 0 &&
            (evt.type == SDL_MOUSEBUTTONDOWN ||
            evt.type == SDL_MOUSEBUTTONUP)) ||
            this->__m_iMouseMode == 1 || this->__m_iMouseMode == 2 || this->__m_iMouseMode == 3) {

        int iClkX = evt.button.x;
        int iClkY = evt.button.y;
        GLint gliChunk;

        // Initialise the selection frame then determine which Chunk the cursor
        // is over by testing the Chunk's Rectangle.
        select_delegate::init_selframe_3d(iClkX, iClkY, 60, 2000);
        for (int iChunk = 0; iChunk < sid::environs::__m_uiGridSize; iChunk++) {
            chunk* chnkCurrent = this->get_chunk(iChunk);
            if (chnkCurrent->is_ready()) {
                rectangle_3d rect = chnkCurrent->get_rectangle();
                chnkCurrent->draw_select(iChunk, &rect);
            }
        }
        gliChunk = select_delegate::process_selhits();
        chunk* chnkSelected = nullptr;
        if (gliChunk != -1)
            chnkSelected = this->get_chunk(gliChunk);

        // Check that a Chunk has been found
        if (chnkSelected) {

            // Test which Plot within the selected Chunk the cursor is over.
            bool blFoundPlot = chnkSelected->process_event(evt, true);

            // When a Plot is not found, put the 8 surrounding Chunks and the 
            // central chunk into a Vector then test the Chunks again using the
            // VBO class.
            if (!blFoundPlot) {
                std::vector<chunk*> vchnks;
                this->get_surrounding_chunks(chnkSelected, vchnks);
                select_delegate::init_selframe_3d(iClkX, iClkY, 60, 2000);
                for (int i = 0; i < vchnks.size(); i++) {

                    chunk* chnkCurrent = vchnks[i];

                    // Don't bother testing the Chunk that we picked from the
                    // first test as we know that it won't work.
                    if (chnkCurrent != chnkSelected) {

                        // Change the Selection Draw delegate to use 
                        // element_arrays.
                        chnkCurrent->set_select_delegate(
                                new select_vbo());

                        // Test the Chunk using Element Arrays class.
                        chnkCurrent->draw_select(
                                i, chnkCurrent->get_opaque_vbo());

                        // Change the Selection Draw delegate back to
                        // use rectangle_3d.
                        chnkCurrent->set_select_delegate(
                                new select_3Drect());
                    }
                }
                gliChunk = select_delegate::process_selhits();
                chnkSelected = vchnks[gliChunk];

                // Check that a Chunk has been found and test the plots again.
                if (chnkSelected != nullptr && gliChunk != -1) {
                    blFoundPlot = chnkSelected->process_event(evt, true);
                }
            }

            if (blFoundPlot) {
                if (this->__m_iMouseMode == 0) {
                    this->select_plots_build(1, 1);
                    this->__m_pltDragStart = static_cast<plot*> (object_ingame::get_mouseover());
                    this->__m_pltDragEnd = __m_pltDragStart;
                } else if (this->__m_iMouseMode == 1) {
                    this->select_plots_build(1, 1);
                    this->__m_pltDragStart = static_cast<plot*> (object_ingame::get_mouseover());
                    this->__m_pltDragEnd = __m_pltDragStart;
                } else if (this->__m_iMouseMode == 2) {
                    this->select_plots_build(3, 3);
                    this->__m_pltDragStart = static_cast<plot*> (object_ingame::get_mouseover());
                    this->__m_pltDragEnd = __m_pltDragStart;
                } else if (this->__m_iMouseMode == 3) {
                    if (!drag) {
                        this->select_plots_build(1, 1);
                        this->__m_pltDragStart = static_cast<plot*> (object_ingame::get_mouseover());
                        this->__m_pltDragEnd = __m_pltDragStart;
                    } else {
                        this->__m_pltDragEnd = static_cast<plot*> (object_ingame::get_mouseover());
                    }
                    if (this->__m_pltDragStart != nullptr &&
                            this->__m_pltDragEnd != nullptr) {
                        this->select_plots_terrain(this->__m_pltDragStart,
                                this->__m_pltDragEnd);
                    }
                }
            } else {
                object_ingame::set_mouseover(nullptr);
                this->__m_pltDragStart = nullptr;
                this->__m_pltDragEnd = nullptr;
                this->__m_vpltSelections.clear();
                this->__m_vboSelection.erase();
            }
        } else {
            object_ingame::set_mouseover(nullptr);
            this->__m_pltDragStart = nullptr;
            this->__m_pltDragEnd = nullptr;
            this->__m_vpltSelections.clear();
            this->__m_vboSelection.erase();
        }
    }
}

void sid::environs::select_plots_terrain(plot* start, plot* end) {
    const int DIAM = plots::CHUNK_DIAM - 1;

    plot::plot_offset pofsStart = start->get_plot_offset();
    plot::plot_offset pofsEnd = end->get_plot_offset();

    int iMinX, iMaxX, iMinZ, iMaxZ;
    if (pofsStart.get_x() > pofsEnd.get_x()) {
        iMaxX = pofsStart.get_x();
        iMinX = pofsEnd.get_x();
    } else {
        iMaxX = pofsEnd.get_x();
        iMinX = pofsStart.get_x();
    }
    if (pofsStart.get_z() > pofsEnd.get_z()) {
        iMaxZ = pofsStart.get_z();
        iMinZ = pofsEnd.get_z();
    } else {
        iMaxZ = pofsEnd.get_z();
        iMinZ = pofsStart.get_z();
    }

    int iWidth = iMaxZ - iMinZ + 1;
    int iLength = iMaxX - iMinX + 1;

    chunk* chnkStart = static_cast<chunk*> (start->get_parent()->get_parent());
    chunk* chnkEnd = static_cast<chunk*> (end->get_parent()->get_parent());

    chunk::chunkoffset cofsStart(chnkStart->get_offset());
    chunk::chunkoffset cofsEnd(chnkEnd->get_offset());
    chunk::chunkoffset cofsRange = cofsEnd - cofsStart;
    int iChunksX = cofsRange.get_x() / DIAM;
    int iChunksZ = cofsRange.get_z() / DIAM;

    int iDirectionX = 1;
    int iDirectionZ = 1;
    if (iChunksX != 0) {
        iDirectionX = iChunksX / abs(iChunksX);
    }
    if (iChunksZ != 0) {
        iDirectionZ = iChunksZ / abs(iChunksZ);
    }

    chunk::chunkoffset cofsDelta(DIAM * iDirectionX, DIAM * iDirectionZ);

    cofsStart -= cofsDelta;
    cofsEnd += cofsDelta;

    std::vector<chunk::chunkoffset> vcofs;
    int iX = cofsStart.get_x();
    do {
        int iZ = cofsStart.get_z();
        do {
            chunk::chunkoffset cofsTemp(iX, iZ);
            vcofs.push_back(cofsTemp);
            iZ += (DIAM * iDirectionZ);
        } while (iZ != cofsEnd.get_z() + (DIAM * iDirectionZ));
        iX += (DIAM * iDirectionX);
    } while (iX != cofsEnd.get_x() + (DIAM * iDirectionX));

    std::vector<chunk*> vchnks;
    for (int i = 0; i < this->__m_uiGridSize; i++) {
        for (int j = 0; j < vcofs.size(); j++) {
            if (this->__m_vRenderOffsets[i].get_chunk()->get_offset() == vcofs[j]) {
                vchnks.push_back(this->__m_vRenderOffsets[i].get_chunk());
                break;
            }
        }
        if (vchnks.size() == vcofs.size()) {
            break;
        }
    }

    std::vector< std::vector<plot*> > vpltCols(iWidth + 1);

    __m_vpltSelections.clear();
    for (int i = 0; i < vchnks.size(); i++) {
        for (int iZ = 0; iZ <= DIAM; iZ++) {
            for (int iX = 0; iX <= DIAM; iX++) {
                plot* pltCurrent = vchnks[i]->get_plots()->at(iX, iZ);
                vertex3 vtxCurrent = pltCurrent->get_position();
                plot::plot_offset pofsCurrent(vtxCurrent[0] / plot::PLOT_WIDTH,
                        vtxCurrent[2] / plot::PLOT_WIDTH);

                int iColID = pofsCurrent.get_z() - iMinZ;

                if (pofsCurrent.get_x() >= iMinX &&
                        pofsCurrent.get_x() <= iMaxX + 1 &&
                        pofsCurrent.get_z() >= iMinZ &&
                        pofsCurrent.get_z() <= iMaxZ + 1) {
                    vpltCols[iColID].push_back(pltCurrent);
                }
            }
        }
    }

    for (int i = 0; i < vpltCols.size(); i++) {
        std::sort(vpltCols[i].begin(), vpltCols[i].end(), sort_plot_rows);
        for (int j = 0; j < vpltCols[i].size(); j++) {
            plot* pltCurrent = vpltCols[i][j];
            this->__m_vpltSelections.push_back(pltCurrent);
        }
    }

    this->__make_selection_vertices(__m_vpltSelections, iWidth, iLength);
    this->__make_selection_indices(iWidth, iLength);
    this->__make_selection_verts_array();
}

bool sid::sort_plot_rows(plot* i, plot* j) {
    vertex3 vtxI = i->get_position();
    vertex3 vtxJ = j->get_position();

    return (vtxI[0] < vtxJ[0]);
}

void sid::environs::select_plots_build(int width, int length) {
    __m_vpltSelections.clear();
    if (object_ingame::get_mouseover() != nullptr) {
        plot* pltSelection =
                static_cast<plot*> (object_ingame::get_mouseover());

        bool blCompleteGrid = this->get_sel_area_grid(pltSelection,
                __m_vpltSelections, width, length);

        if (blCompleteGrid) {
            this->__make_selection_vertices(__m_vpltSelections, width, length);
            this->__make_selection_indices(width, length);
            this->__make_selection_verts_array();
        }
    }
}

bool sid::environs::get_sel_area_grid(
        plot* plt,
        std::vector<plot*>& vplt,
        int width,
        int length) {

    const int DIAM = plots::CHUNK_DIAM - 1;

    std::vector<chunk*> vchnkSelection;

    chunk* chnkCentre = static_cast<chunk*> (plt->get_parent()->get_parent());
    chunk::chunkoffset ofsCentre = chnkCentre->get_offset();

    if (width == 1 && length == 1) {
        vchnkSelection.push_back(chnkCentre);
    } else {
        this->get_surrounding_chunks(chnkCentre, vchnkSelection);
    }

    int iCentre = chnkCentre->get_plots()->find(plt);
    plot::plot_offset pofs = plots::index_to_coords(iCentre);
    plot::plot_offset pofsZero(pofs.get_x() - (width / 2),
            pofs.get_z() - (length / 2));

    plots* plts = chnkCentre->get_plots();

    for (int i = 0; i <= width; i++) {
        for (int j = 0; j <= length; j++) {
            plot::plot_offset pofsTemp(pofsZero.get_x() + j,
                    pofsZero.get_z() + i);

            if (pofsTemp.get_x() <= DIAM && pofsTemp.get_x() >= 0 &&
                    pofsTemp.get_z() <= DIAM && pofsTemp.get_z() >= 0) {
                vplt.push_back(plts->at(pofsTemp));
            } else {
                float fOffsetX = ofsCentre.get_x();
                float fOffsetX2 = fOffsetX * 2;
                float fOffsetZ = ofsCentre.get_z();
                float fOffsetZ2 = fOffsetZ * 2;
                
                float fPlotPosX = fOffsetX2 * plot::PLOT_WIDTH;
                float fPlotPosY = 0;
                float fPlotPosZ = fOffsetZ2 * plot::PLOT_WIDTH;

                vertex3 vtxPlotPosition(fPlotPosX, fPlotPosY, fPlotPosZ);
                chunk::chunkoffset ofsPerimeter(get_chunk_offset(vtxPlotPosition));

                for (std::vector<chunk*>::iterator it1 = vchnkSelection.begin();
                        it1 != vchnkSelection.end();
                        it1++) {

                    chunk* chnkSelection = *it1;
                    if (chnkSelection && chnkSelection->is_ready()) {
                        chunk::chunkoffset ofsSelection(chnkSelection->get_offset());

                        int iModX(0);
                        int iModZ(0);
                        if (ofsSelection == ofsPerimeter) {
                            if (pofsTemp.get_x() >= DIAM) {
                                iModX = -DIAM;
                            } else if (pofsTemp.get_x() < 0) {
                                iModX = DIAM;
                            }

                            if (pofsTemp.get_z() >= DIAM) {
                                iModZ = -DIAM;
                            } else if (pofsTemp.get_z() < 0) {
                                iModZ = DIAM;
                            }
                            plot::plot_offset pofsMod(iModX, iModZ);
                            pofsTemp += pofsMod;

                            plots* pltsPeremeter = chnkSelection->get_plots();
                            vplt.push_back(pltsPeremeter->at(pofsTemp));
                        }
                    }
                }
            }
        }
    }

    return (vplt.size() == (width + 1) * (length + 1));
}

void sid::environs::get_surrounding_chunks(
        chunk* chnk,
        std::vector<chunk*>& vcnhnk) {

    const int DIAM = plots::CHUNK_DIAM - 1;

    chunk::chunkoffset ofsCentre = chnk->get_offset();

    chunk::chunkoffset ofsNW(ofsCentre.get_x() - DIAM, ofsCentre.get_z() - DIAM);

    chunk* chnk0 = this->__m_vRenderOffsets[0].get_chunk();
    chunk::chunkoffset ofs0 = chnk0->get_offset();
    chunk::chunkoffset ofsDelta = ofsNW - ofs0;

    int iSelPos0_X = ofsDelta.get_x() / DIAM;
    int iSelPos0_Z = ofsDelta.get_z() / DIAM;

    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            int iSelCurr_X = iSelPos0_X + j;
            int iSelCurr_Z = iSelPos0_Z + i;

            if (iSelCurr_X >= 0 && iSelCurr_X < sid::environs::__m_uiGridWidth &&
                    iSelCurr_Z >= 0 && iSelCurr_X < sid::environs::__m_uiGridWidth) {

                int iSelCurr = iSelCurr_X + iSelCurr_Z * sid::environs::__m_uiGridWidth;
                vcnhnk.push_back(this->__m_vRenderOffsets[iSelCurr].get_chunk());
            }
        }
    }
}

void sid::environs::__make_selection_vertices(
        std::vector<plot*>& vplt,
        int selWidth,
        int selLength) {

    __m_vboSelection.erase();
    float fPrevX = FLT_MAX;
    for (int i = 0; i < vplt.size(); i++) {
        plot* plt = vplt[i];
        vertex3 vtxPos = plt->get_position();
        if (vtxPos[euclid::DIMENSION_X] != fPrevX) {
            float fArrayX = vtxPos[euclid::DIMENSION_X];
            float fArrayY = vtxPos[euclid::DIMENSION_Y] + 0.01;
            float fArrayZ = vtxPos[euclid::DIMENSION_Z];
            sid::t_vertset vsVerts(fArrayX, fArrayY, fArrayZ);

            __m_vboSelection.push_vertset(vsVerts);
        }
        fPrevX = vtxPos[euclid::DIMENSION_X];
    }
}

void sid::environs::__make_selection_indices(int selWidth, int selLength) {
    uint uiStripLength = selLength * 6;
    bool blFlipPoly = false;
    int i = 0;
    for (int iStrip = 0; iStrip < selWidth; iStrip++) {
        for (int iPoint = 0; iPoint < uiStripLength; iPoint++) {

            GLuint gluiIndexValue;
            gluiIndexValue = __triangle_point_to_index(selLength + 1, iStrip,
                    iPoint, &blFlipPoly);

            __m_vboSelection.push_index(gluiIndexValue);
            i++;
        }
    }
}

void sid::environs::__make_selection_verts_array() {
    for (int iIndex = 0; iIndex < __m_vboSelection.get_indeces_size(); iIndex++) {
        sid::t_vertset fvPoint;
        __coords_from_index(__m_vboSelection, iIndex, fvPoint);
        __m_vboSelection.push_arrayvert(fvPoint);
    }

    this->__m_vboSelection.set_depth_buffer_usage(false);
    this->__m_vboSelection.set_default_color(sid::t_color(0, 255, 0, 100));
    this->__m_vboSelection.make_vbo(GL_DYNAMIC_DRAW);
}

void sid::environs::__coords_from_index(sid::vbo& elArr,
        GLuint index,
        sid::t_vertset& returnArray) {

    returnArray = elArr.get_vertset(elArr.get_index(index));
}

GLuint sid::environs::__triangle_point_to_index(
        int selWidth,
        int Strip,
        int Point,
        bool* Flip) {

    int iPlotNumber = Point / 6;
    int iPlotPoint = Point % 6;
    int iVertexOffsetX = Strip * selWidth;
    int iSubPointOffset = iVertexOffsetX + iPlotNumber;

    GLuint gluiIndexValue;
    if (!*Flip) {
        switch (iPlotPoint) {
            case 0:
                gluiIndexValue = iSubPointOffset;
                break;
            case 1:
                gluiIndexValue = iSubPointOffset + selWidth;
                break;
            case 2:
                gluiIndexValue = iSubPointOffset + selWidth + 1;
                break;
            case 3:
                gluiIndexValue = iSubPointOffset + selWidth + 1;
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
                gluiIndexValue = iSubPointOffset + selWidth;
                break;
            case 1:
                gluiIndexValue = iSubPointOffset + selWidth + 1;
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
                gluiIndexValue = iSubPointOffset + selWidth;
                *Flip = false;
                break;
        }
    }
    return (gluiIndexValue);
}

sid::plot* sid::environs::get_plot_at(vertex3 vert) {
    chunk::chunkoffset cofsCamFocus = get_chunk_offset(vert);
    chunk* chnkCamFocus = this->get_cached_chunk(cofsCamFocus, true);
    return chnkCamFocus->get_plots()->get_plot_at(vert);
}

void sid::environs::zoom_to_plot(double td, camera* cam) {
    if (object_ingame::get_zoom_object() != nullptr) {
        object_ingame* objgSelection =
                static_cast<object_ingame*> (object_ingame::get_zoom_object());

        vertex3 vtxSelPos = objgSelection->get_position();
        if (objgSelection->is_zoom_to()) {
            cam->set_zoom_to(vtxSelPos);
            if (cam->zoom_to(td)) {
                objgSelection->set_zoom_to(false);
            }
        }
    }
}

void sid::environs::set_mouse_mode(int iClickMode) {
    this->__m_iMouseMode = iClickMode;
    static int siLastClickMod;
    if (iClickMode != siLastClickMod) {
        this->__m_vboSelection.erase();
        this->__m_vpltSelections.clear();
        siLastClickMod = iClickMode;
    }
}

void sid::environs::process_keyboard(SDL_Event& evt) {
    if (evt.type == SDL_KEYDOWN) {
        switch (evt.key.keysym.sym) {
            case SDLK_f:
                float fHeight;
                if (this->__m_vpltSelections.size() > 0) {
                    fHeight = this->__m_pltDragStart->get_height();
                }
                std::vector<chunk*> vchnks;
                for (int i = 0; i < this->__m_vpltSelections.size(); i++) {
                    this->__m_vpltSelections[i]->set_height(fHeight);
                    chunk* chnkParent = static_cast<chunk*> (this->__m_vpltSelections[i]->get_parent()->get_parent());
                    bool blChunkFound = false;
                    for (int j = 0; j < vchnks.size(); j++) {
                        if (vchnks[j] == chnkParent) {
                            blChunkFound = true;
                        }
                    }
                    if (!blChunkFound) {
                        vchnks.push_back(chnkParent);
                    }
                }
                for (int i = 0; i < vchnks.size(); i++) {
                    this->__m_Chunks->push_to_build_queue(vchnks[i]);
                }
                break;
        }
    }
}

sid::chunk::chunkoffset sid::environs::get_chunk_offset(float PosX, float PosZ) {
    float fPlotX = floor(PosX / plot::PLOT_WIDTH);
    float fPlotZ = floor(PosZ / plot::PLOT_WIDTH);
    int iVisChunkDiam = plots::CHUNK_DIAM - 1;
    int iX = floor(fPlotX / iVisChunkDiam) * iVisChunkDiam;
    int iZ = floor(fPlotZ / iVisChunkDiam) * iVisChunkDiam;

    chunk::chunkoffset ofsChunkCoords(iX, iZ);

    return ofsChunkCoords;
}

sid::chunk::chunkoffset sid::environs::get_chunk_offset(vertex3 const& vtx) {
    return get_chunk_offset(vtx[0], vtx[2]);
}