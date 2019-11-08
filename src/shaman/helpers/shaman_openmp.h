#pragma once

// Requires openMP 4.0+ to get reductions on user defined types
#ifdef _OPENMP

// +
#pragma omp declare reduction(+:Sfloat : omp_out=omp_in+omp_out)                initializer(omp_priv=Sfloat(0.f))
#pragma omp declare reduction(+:Sdouble: omp_out=omp_in+omp_out)	            initializer(omp_priv=Sdouble(0.))
#pragma omp declare reduction(+:Slong_double: omp_out=omp_in+omp_out)	        initializer(omp_priv=Slong_double(0.L))

// -
#pragma omp declare reduction(-:Sfloat : omp_out=omp_in+omp_out)	            initializer(omp_priv=Sfloat(0.f))
#pragma omp declare reduction(-:Sdouble: omp_out=omp_in+omp_out)	            initializer(omp_priv=Sdouble(0.))
#pragma omp declare reduction(-:Slong_double: omp_out=omp_in+omp_out)	        initializer(omp_priv=Slong_double(0.L))

// *
#pragma omp declare reduction(*:Sfloat : omp_out=omp_in*omp_out)	            initializer(omp_priv=Sfloat(1.f))
#pragma omp declare reduction(*:Sdouble: omp_out=omp_in*omp_out)    	        initializer(omp_priv=Sdouble(1.))
#pragma omp declare reduction(*:Slong_double: omp_out=omp_in*omp_out)	        initializer(omp_priv=Slong_double(1.L))

// max
#pragma omp declare reduction(max:Sfloat : omp_out=Sstd::max(omp_in,omp_out))	        initializer(omp_priv=Sfloat(std::numeric_limits<float>::lowest()))
#pragma omp declare reduction(max:Sdouble : omp_out=Sstd::max(omp_in,omp_out))        initializer(omp_priv=Sdouble(std::numeric_limits<double>::lowest()))
#pragma omp declare reduction(max:Slong_double : omp_out=Sstd::max(omp_in,omp_out))   initializer(omp_priv=Slong_double(std::numeric_limits<long double>::lowest()))

// min
#pragma omp declare reduction(min:Sfloat : omp_out=Sstd::min(omp_in,omp_out))	        initializer(omp_priv=Sfloat(std::numeric_limits<float>::max()))
#pragma omp declare reduction(min:Sdouble : omp_out=Sstd::min(omp_in,omp_out))        initializer(omp_priv=Sdouble(std::numeric_limits<double>::max()))
#pragma omp declare reduction(min:Slong_double : omp_out=Sstd::min(omp_in,omp_out))   initializer(omp_priv=Slong_double(std::numeric_limits<long double>::max()))

#endif //_OPENMP