
#include <igraph_pmt.h>

#ifdef BASE_COMPLEX
#define OBASE std::complex<double>
#define PTRCAST(X) ig::complex_cast(X)
#define REFCAST(X) ig::complex_cast(X)
#define INVPTRCAST(X) ig::complex_cast(X)
#else
#define OBASE BASE
#define PTRCAST(X) X
#define REFCAST(X) X
#define INVPTRCAST(X) X
#endif
