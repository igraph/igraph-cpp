
#define MATRIX_LIST
#define LIST_TYPE igMatList
#define LIST_TYPE_TEMPL igMatList<BASE>
#define ELEM_TYPE igMat<BASE>
#include "typed_list_pmt.hpp"
#undef ELEM_TYPE
#undef LIST_TYPE_TEMPL
#undef LIST_TYPE
#undef MATRIX_LIST
