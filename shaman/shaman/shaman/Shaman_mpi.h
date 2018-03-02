#include <mpi.h>

/*
 * to use :
 * - include Shaman_mpi.h
 * - replace 'MPI_Init' with 'MPI_Shaman_Init'
 * - replace 'MPI_Finalize' with 'MPI_Shaman_Finalize'
 * - use the shaman MPI types ('MPI_FLOAT' -> 'MPI_SFLOAT')
 * - use the shaman MPI operations ('MPI_SUM' -> 'MPI_SSUM')
 */

//-------------------------------------------------------------------------------------------------
// DEFINITIONS

/*
 * type definitions
 */
MPI_Datatype MPI_SFLOAT;
MPI_Datatype MPI_SDOUBLE;
MPI_Datatype MPI_SLONG_DOUBLE;

/*
 * operator definition
 * http://mpi-forum.org/docs/mpi-2.2/mpi22-report/node107.htm
 */
MPI_Op MPI_SMAX;
MPI_Op MPI_SMIN;
MPI_Op MPI_SSUM;
MPI_Op MPI_SPROD;

//-------------------------------------------------------------------------------------------------
// OPERATOR DECLARATIONS

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
 *
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

//-----------------------------------------------

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
 * runs MPI_Init and defines shaman types
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
        MPI_Type_contiguous(2, MPI_FLOAT, &MPI_SFLOAT);
        MPI_Type_commit(&MPI_SFLOAT);
        // Sdouble
        MPI_Type_contiguous(2, MPI_DOUBLE, &MPI_SDOUBLE);
        MPI_Type_commit(&MPI_SDOUBLE);
        // Slong_double
        MPI_Type_contiguous(2, MPI_LONG_DOUBLE, &MPI_SLONG_DOUBLE);
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
 * frees shaman types and finalize MPI
 */
int MPI_Shaman_Finalize()
{
    #ifndef NO_SHAMAN
    // free types
    MPI_Type_free(&MPI_SFLOAT);
    MPI_Type_free(&MPI_SDOUBLE);
    MPI_Type_free(&MPI_SLONG_DOUBLE);

    // free operators
    MPI_Op_free(&MPI_SMAX);
    MPI_Op_free(&MPI_SMIN);
    MPI_Op_free(&MPI_SSUM);
    MPI_Op_free(&MPI_SPROD);

    #ifdef NUMERICAL_DEBUGGER_ENABLED
    // gets the process number
    int processNumber;
    MPI_Comm_rank(MPI_COMM_WORLD, &processNumber);

    // sum the unstability counts
    int unstabilityCount_red;
    int unstablePowerFunctions_red;
    int unstableDivisions_red;
    int unstableMultiplications_red;
    int unstableFunctions_red;
    int unstableBranchings_red;
    int cancelations_red;
    int numericalZeros_red;
    MPI_Reduce(&NumericalDebugger::unstabilityCount, &unstabilityCount_red, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
    MPI_Reduce(&NumericalDebugger::unstablePowerFunctions, &unstablePowerFunctions_red, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
    MPI_Reduce(&NumericalDebugger::unstableDivisions, &unstableDivisions_red, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
    MPI_Reduce(&NumericalDebugger::unstableMultiplications, &unstableMultiplications_red, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
    MPI_Reduce(&NumericalDebugger::unstableFunctions, &unstableFunctions_red, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
    MPI_Reduce(&NumericalDebugger::unstableBranchings, &unstableBranchings_red, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
    MPI_Reduce(&NumericalDebugger::cancelations, &cancelations_red, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
    MPI_Reduce(&NumericalDebugger::numericalZeros, &numericalZeros_red, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
    NumericalDebugger::unstabilityCount = unstabilityCount_red;
    NumericalDebugger::unstablePowerFunctions = unstablePowerFunctions_red;
    NumericalDebugger::unstableDivisions = unstableDivisions_red;
    NumericalDebugger::unstableMultiplications = unstableMultiplications_red;
    NumericalDebugger::unstableFunctions = unstableFunctions_red;
    NumericalDebugger::unstableBranchings = unstableBranchings_red;
    NumericalDebugger::cancelations = cancelations_red;
    NumericalDebugger::numericalZeros = numericalZeros_red;

    // deactivate display on all process but the main one
    if (processNumber != 0)
    {
        NumericalDebugger::shouldDisplay = false;
    }
    #endif //NUMERICAL_DEBUGGER_ENABLED
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
 *
 * TODO works as long as the Shaman types have the declared size
 * we might want to implement shaman using a struct instead of a class so that our implementation can easily be kept MPÏ compatible
 */