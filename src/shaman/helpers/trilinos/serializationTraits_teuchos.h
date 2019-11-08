#pragma  once

#include <Teuchos_SerializationTraits.hpp>

/*
 * defines traits needed by Tpetra to serialize a user-defined type
 * see Teuchos_SerializationTraits.hpp
 * TODO probably invalid
 */
namespace Teuchos
{
    template<typename Ordinal>
    class SerializationTraits<Ordinal,Sfloat> : public DirectSerializationTraits<Ordinal,Sfloat>
    {};

    template<typename Ordinal>
    class SerializationTraits<Ordinal,Sdouble> : public DirectSerializationTraits<Ordinal,Sdouble>
    {};

    template<typename Ordinal>
    class SerializationTraits<Ordinal,Slong_double> : public DirectSerializationTraits<Ordinal,Slong_double>
    {};
}

