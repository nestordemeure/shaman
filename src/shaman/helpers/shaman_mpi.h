#pragma once

#include <cstddef>

/*
 * to use :
 * - include Shaman_mpi.h
 * - replace 'MPI_Init' with 'MPI_Shaman_Init'
 * - replace 'MPI_Finalize' with 'MPI_Shaman_Finalize'
 * - use the shaman MPI types ('MPI_FLOAT' -> 'MPI_SFLOAT')
 * - use the shaman MPI operations ('MPI_SUM' -> 'MPI_SSUM')
 *
 * Note that you currently cannot use both MPI and Shaman's tagged error (which is used to trace the sources of numerical error).
 */

//-------------------------------------------------------------------------------------------------
// TYPES

/*
 * type definitions
 */
MPI_Datatype MPI_SBOOL;
MPI_Datatype MPI_SFLOAT;
MPI_Datatype MPI_SDOUBLE;
MPI_Datatype MPI_SLONG_DOUBLE;

/*
 * builds an MPI type around a src type
 * TODO we could automatically deduce the required MPI_Datatypes
 *
 * simpler solution for a type being just two identical types :
 * MPI_Type_contiguous(2, numberType, newType);
 */
template<typename ShamanType>
int MPI_Type_shaman(MPI_Datatype numberType, MPI_Datatype errorType, MPI_Datatype* newType)
{
    int structlen = 3;
    int blocklengths[structlen];
    MPI_Datatype types[structlen];
    MPI_Aint displacements[structlen];
    int blockNum = 0;

    // number
    blocklengths[blockNum] = 1;
    types[blockNum] = numberType;
    displacements[blockNum] = offsetof(ShamanType,number);
    blockNum++;

    // error
    blocklengths[blockNum] = 1;
    types[blockNum] = errorType;
    displacements[blockNum] = offsetof(ShamanType,error);
    blockNum++;

    // error composants
    // TODO how to send pointer to vector properly
    // - send the vector
    // - receive a vector and create a pointer ?
    // - send the top n error terms by default and drop the others
    //   takes 2*nspace but can be anticipated
    // - create a pair of encrypt/decrypt functions to be used before sending data
    //   they would flatten then data into [number;error;error_term0;error_term1;...] (this format requires knowing the number of error terms to be decrypted)

    return MPI_Type_create_struct(structlen, blocklengths, displacements, types, newType);
}

//-------------------------------------------------------------------------------------------------
// OPERATORS

/*
 * operator definition
 * http://mpi-forum.org/docs/mpi-2.2/mpi22-report/node107.htm
 */
MPI_Op MPI_SMAX;
MPI_Op MPI_SMIN;
MPI_Op MPI_SSUM;
MPI_Op MPI_SPROD;

/*
 * applies an operation to two array of elements in any type
 */
#define generalShamanUserFunction(input,output,size,operation,type)\
    for(int i = 0; i < (*size); i++)\
    {\
        ((type *)output)[i] = operation( ((type *)input)[i], ((type *)output)[i]);\
    }\

/*
 * test the type to apply the operation with the correct cast
 * TODO could fallback to MPI operations on predefined types
 */
#define shamanToMpiUserFunction(invec,inoutvec,len,datatype,operation)\
    if (*datatype == MPI_SFLOAT)\
    {\
        generalShamanUserFunction(invec, inoutvec, len, operation, Sfloat);\
    }\
    else if (*datatype == MPI_SDOUBLE)\
    {\
        generalShamanUserFunction(invec, inoutvec, len, operation, Sdouble);\
    }\
    else if (*datatype == MPI_SLONG_DOUBLE)\
    {\
        generalShamanUserFunction(invec, inoutvec, len, operation, Slong_double);\
    }\
    else\
    {\
        throw std::invalid_argument("A Shaman MPI operation ( operation ) was done with a type that is not a shaman type.");\
    }\

//----------

// sum
void MPI_smin( void *invec, void *inoutvec, int *len, MPI_Datatype *datatype)
{
    shamanToMpiUserFunction(invec,inoutvec,len,datatype,std::min);
};

// max
void MPI_smax( void *invec, void *inoutvec, int *len, MPI_Datatype *datatype)
{
    shamanToMpiUserFunction(invec,inoutvec,len,datatype,std::max);
};

// sum
void MPI_ssum( void *invec, void *inoutvec, int *len, MPI_Datatype *datatype)
{
    shamanToMpiUserFunction(invec,inoutvec,len,datatype,operator+);
};

// prod
void MPI_sprod( void *invec, void *inoutvec, int *len, MPI_Datatype *datatype)
{
    shamanToMpiUserFunction(invec,inoutvec,len,datatype,operator*);
};

//-------------------------------------------------------------------------------------------------
// INIT / FINALIZE

/*
 * runs MPI_Init and defines src types
 */
int MPI_Shaman_Init(int argc, char **argv )
{
    // init MPI
    int errorValue = MPI_Init(&argc, &argv);

#ifdef NO_SHAMAN
    MPI_SFLOAT = MPI_FLOAT;
    MPI_SDOUBLE = MPI_DOUBLE;
    MPI_SLONG_DOUBLE = MPI_LONG_DOUBLE;
    MPI_SMAX = MPI_MAX;
    MPI_SMIN = MPI_MIN;
    MPI_SSUM = MPI_SUM;
    MPI_SPROD = MPI_PROD;
#else
    if (errorValue == MPI_SUCCESS)
    {
        // Sfloat
        MPI_Type_shaman<Sfloat>(MPI_FLOAT, MPI_FLOAT, &MPI_SFLOAT);
        MPI_Type_commit(&MPI_SFLOAT);
        // Sdouble
        MPI_Type_shaman<Sdouble>(MPI_DOUBLE, MPI_DOUBLE, &MPI_SDOUBLE);
        MPI_Type_commit(&MPI_SDOUBLE);
        // Slong_double
        MPI_Type_shaman<Slong_double>(MPI_LONG_DOUBLE, MPI_LONG_DOUBLE, &MPI_SLONG_DOUBLE);
        MPI_Type_commit(&MPI_SLONG_DOUBLE);

        // operator for reduce
        bool isCommutative = true;
        MPI_Op_create(&MPI_smax, isCommutative, &MPI_SMAX);
        MPI_Op_create(&MPI_smin, isCommutative, &MPI_SMIN);
        MPI_Op_create(&MPI_ssum, isCommutative, &MPI_SSUM);
        MPI_Op_create(&MPI_sprod, isCommutative, &MPI_SPROD);

    }
#endif //NO_SHAMAN

    return errorValue;
}

/*
 * frees src types and finalize MPI
 */
int MPI_Shaman_Finalize()
{
#ifndef NO_SHAMAN
    // free types
    MPI_Type_free(&MPI_SBOOL);
    MPI_Type_free(&MPI_SFLOAT);
    MPI_Type_free(&MPI_SDOUBLE);
    MPI_Type_free(&MPI_SLONG_DOUBLE);

    // free operators
    MPI_Op_free(&MPI_SMAX);
    MPI_Op_free(&MPI_SMIN);
    MPI_Op_free(&MPI_SSUM);
    MPI_Op_free(&MPI_SPROD);
#endif //NO_SHAMAN

    return MPI_Finalize();
}

//-------------------------------------------------------------------------------------------------

/*
 * Note on user defined MPI type
 * use a continuous type it the base type is made of identical types
 * use a struct if it is made of different types
 * examples :
 * http://pages.tacc.utexas.edu/~eijkhout/pcse/html/mpi-data.html
 * http://mpitutorial.com/tutorials/mpi-reduce-and-allreduce/
 * TODO works as long as the Shaman types have the declared size
 *
 * see also MPI_Pack and MPI_Unpack
 */


