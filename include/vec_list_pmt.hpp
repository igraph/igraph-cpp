
#define VECTOR_LIST
#define LIST_TYPE igVecList
#define LIST_TYPE_TEMPL igVecList<BASE>
#define ELEM_TYPE igVec<BASE>
#include "typed_list_pmt.hpp"
#undef ELEM_TYPE
#undef LIST_TYPE_TEMPL
#undef LIST_TYPE
#undef VECTOR_LIST
