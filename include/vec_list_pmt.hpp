
#define VECTOR_LIST
#define LIST_TYPE VecList
#define LIST_TYPE_TEMPL VecList<BASE>
#define ELEM_TYPE Vec<BASE>
#include "typed_list_pmt.hpp"
#undef ELEM_TYPE
#undef LIST_TYPE_TEMPL
#undef LIST_TYPE
#undef VECTOR_LIST
