#ifndef __OY_PB_DS_HEAP__
#define __OY_PB_DS_HEAP__

#include<ext/pb_ds/priority_queue.hpp>

namespace OY{
    template<typename _Tp,typename _Compare=std::less<_Tp>>
    using PB_DS_BinaryHeap=__gnu_pbds::priority_queue<_Tp,_Compare,__gnu_pbds::binary_heap_tag>;
    template<typename _Tp,typename _Compare=std::less<_Tp>>
    using PB_DS_PairingHeap=__gnu_pbds::priority_queue<_Tp,_Compare,__gnu_pbds::pairing_heap_tag>;
    template<typename _Tp,typename _Compare=std::less<_Tp>>
    using PB_DS_BinomialHeap=__gnu_pbds::priority_queue<_Tp,_Compare,__gnu_pbds::binomial_heap_tag>;
    template<typename _Tp,typename _Compare=std::less<_Tp>>
    using PB_DS_RCBinomialHeap=__gnu_pbds::priority_queue<_Tp,_Compare,__gnu_pbds::rc_binomial_heap_tag>;
    template<typename _Tp,typename _Compare=std::less<_Tp>>
    using PB_DS_ThinHeap=__gnu_pbds::priority_queue<_Tp,_Compare,__gnu_pbds::thin_heap_tag>;
}

#endif