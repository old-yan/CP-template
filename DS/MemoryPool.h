#ifndef __OY_MEMORYPOOL__
#define __OY_MEMORYPOOL__

#include <cstdint>
#include <functional>
#include <vector>

namespace OY {
    template <typename Tp, uint32_t BATCH = 1 << 15>
    struct MemoryPool {
        static std::vector<Tp *> s_pool, s_gc;
        static Tp *s_cursor, *s_end;
        static void _reserve(uint32_t count = BATCH) {
            s_pool.push_back((Tp *)malloc(count * sizeof(Tp)));
            s_cursor = s_pool.back();
            s_end = s_cursor + count;
        }
        static void *operator new(size_t) {
            if (s_gc.size()) {
                Tp *it = s_gc.back();
                s_gc.pop_back();
                return it;
            } else if (s_cursor == s_end)
                _reserve();
            return s_cursor++;
        }
        static void operator delete(void *it) { s_gc.push_back((Tp *)it); }
        static void recycle(Tp *it) { s_gc.push_back(it); }
    };
    template <typename Tp, uint32_t BATCH>
    std::vector<Tp*> MemoryPool<Tp, BATCH>::s_pool;
    template <typename Tp, uint32_t BATCH>
    std::vector<Tp*> MemoryPool<Tp, BATCH>::s_gc;
    template <typename Tp, uint32_t BATCH>
    Tp* MemoryPool<Tp, BATCH>::s_cursor = nullptr;
    template <typename Tp, uint32_t BATCH>
    Tp* MemoryPool<Tp, BATCH>::s_end = nullptr;
}

#endif