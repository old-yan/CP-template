/*
最后修改:
20240506
测试环境:
gcc11.2,c++11
clang12.0,C++11
msvc14.2,C++14
*/
#ifndef __OY_OFFLINEPOINTADDRECTSUMMAINTAINER2D__
#define __OY_OFFLINEPOINTADDRECTSUMMAINTAINER2D__

#include "PointAddRectSumCounter2D.h"

namespace OY {
    namespace OFFLINEPARSM2D {
        using size_type = uint32_t;
        template <typename SizeType, typename WeightType = bool, typename SumType = typename std::conditional<std::is_same<WeightType, bool>::value, size_type, WeightType>::type>
        struct Solver {
            static constexpr bool is_bool = std::is_same<WeightType, bool>::value;
            using weight_type = typename std::conditional<is_bool, size_type, WeightType>::type;
            using inner_table = PARSC2D::Table<SizeType, weight_type, SumType, true>;
            struct event {
                bool m_flag;
                union {
                    struct {
                        size_type m_id;
                        weight_type m_w;
                    };
                    struct {
                        SizeType m_x_min, m_x_max, m_y_min, m_y_max;
                    };
                } m_data;
            };
            inner_table m_table;
            size_type m_point_id;
            std::vector<event> m_events;
            Solver(size_type point_cnt = 0, size_type op_cnt_since_query = 0) : m_table(point_cnt), m_point_id() { m_events.reserve(op_cnt_since_query); }
            void add_point(SizeType x, SizeType y, weight_type w = 1) {
                if (m_events.empty())
                    m_table.add_point(x, y, w);
                else {
                    m_table.add_point(x, y, {});
                    event e;
                    e.m_flag = false, e.m_data.m_id = m_point_id, e.m_data.m_w = w;
                    m_events.push_back(e);
                }
                m_point_id++;
            }
            void add_query(SizeType x_min, SizeType x_max, SizeType y_min, SizeType y_max) {
                event e;
                e.m_flag = true, e.m_data.m_x_min = x_min, e.m_data.m_x_max = x_max, e.m_data.m_y_min = y_min, e.m_data.m_y_max = y_max;
                m_events.push_back(e);
            }
            std::vector<SumType> solve() {
                m_table.prepare();
                std::vector<SumType> res;
                res.reserve(m_events.size());
                for (auto &e : m_events)
                    if (e.m_flag)
                        res.push_back(m_table.query(e.m_data.m_x_min, e.m_data.m_x_max, e.m_data.m_y_min, e.m_data.m_y_max));
                    else
                        m_table.add_point_value(e.m_data.m_id, e.m_data.m_w);
                return res;
            }
        };
    };
}

#endif