
#define MATRIX_LIST
#define LIST_TYPE MatList
#define LIST_TYPE_TEMPL MatList<BASE>
#define ELEM_TYPE Mat<BASE>
#include "typed_list_pmt.hpp"
#undef ELEM_TYPE
#undef LIST_TYPE_TEMPL
#undef LIST_TYPE
#undef MATRIX_LIST
