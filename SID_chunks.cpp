/*
 * File:   chunks.cpp
 * Author: geth2k
 *
 * Created on 21 April 2013, 23:40
 */

#include "SID_chunks.h"

sid::chunks::chunks() :
__m_blStopBuilderThread(false) {
    this->start_chunk_builder_thread();
}

sid::chunks::chunks(const chunks& orig) :
__m_blStopBuilderThread(orig.__m_blStopBuilderThread) {
    
}

sid::chunks::~chunks() {
    this->__stop_builder_thread();
    this->__m_ThreadChunkBuild.join();
}

void sid::chunks::tick() {
    this->lock_builder_thread();
    this->_foreach_if_delete_else<chunk*, chunk*>(
            [](chunk* ref, int loop, void* args) {
                return ref->is_deletable();
            },
            [](chunk* ref, int loop, void* args) {
                ref->tick(); return true;
            });

    this->unlock_builder_thread();
}

void sid::chunks::make_new_vbos() {
    if (!this->__m_VBOQueue.empty()) {
        int iLoop(0);
        this->lock_builder_thread();
        while (!this->__m_VBOQueue.empty()) {
            chunk* ChunkFront = this->__m_VBOQueue.front();
            this->__m_VBOQueue.pop();
            ChunkFront->make_new_vbo();
            iLoop++;
        }
        this->unlock_builder_thread();
    }
}

// -------------------------------------------------------------------------- //

void sid::chunks::start_chunk_builder_thread() {
    this->__m_ThreadChunkBuild = std::thread(
            [this](void* ref) {
                this->__chunk_builder_thread();
            }, nullptr);
}

void sid::chunks::__chunk_builder_thread() {
    while (!this->is_builder_thread_stopped()) {

        if (!this->__m_BuildQueue.empty()) {
            int iLoop(0);
            while (!this->__m_BuildQueue.empty()) {
                this->lock_builder_thread();
                chunk* ChunkFront = this->__m_BuildQueue.front();
                if (!ChunkFront->is_deletable()) {
                    this->__m_BuildQueue.pop();
                    ChunkFront->build_chunk();
                    this->__m_VBOQueue.push(ChunkFront);
                }
                this->unlock_builder_thread();
                iLoop++;
                
                if (iLoop >= 1) {
                    SDL_Delay(1);
                    break;
                }
            }
        } else {
            SDL_Delay(1);
        }
    }
#ifdef DEBUG
    std::cout << "Exiting chunk_builder thread" << std::endl;
#endif
}
