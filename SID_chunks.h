/*
 * File:   chunks.h
 * Author: geth2k
 *
 * Created on 21 April 2013, 23:40
 */

#ifndef SID_CHUNKS_H
#define	SID_CHUNKS_H

#include <queue>
#include <mutex>
#include <thread>

#include "SID_chunk.h"

namespace sid {

    class chunks : public object {
    public:

        static const uchar GRID_DIAM = 15;
        static const uchar GRID_SIZE = 225;

        chunks();
        chunks(const chunks& orig);
        virtual ~chunks();

        virtual void tick();

        void make_new_vbos();

        void add_chunk(chunk* chnk) {
            this->push_to_build_queue(chnk);
            this->_add_child_front(chnk);
        }

        inline void push_to_build_queue(chunk* chnk) {
            this->__m_BuildQueue.push(chnk);
        }

        template <typename _Function>
        inline char forfirst(_Function func, void* args = nullptr) {
            return this->_forfirst<chunk*>(func, args);
        }

        inline bool is_builder_thread_stopped() const {
            return this->__m_blStopBuilderThread;
        }

        void start_chunk_builder_thread();

        inline void lock_builder_thread() {
            chunks::__m_mtxBuilderLock.lock();
        }

        inline void unlock_builder_thread() {
            chunks::__m_mtxBuilderLock.unlock();
        }

    private:

        std::queue<chunk*> __m_BuildQueue;
        std::queue<chunk*> __m_VBOQueue;

        std::mutex __m_mtxBuilderLock;

        bool __m_blStopBuilderThread;

        std::thread __m_ThreadChunkBuild;

        void __chunk_builder_thread();

        inline void __stop_builder_thread() {
            this->__m_blStopBuilderThread = true;
        }

    };
}
#endif	/* CHUNKS_H */