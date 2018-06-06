//
// Created by demeuren on 05/06/18.
//

#ifndef SHAMAN_GLOBALVARIABLES_H
#define SHAMAN_GLOBALVARIABLES_H

/*
 * global constants
 */
#include <stack>

namespace Shaman
{
    // used to model the stacktrace
    using Tag = unsigned short int; // represents a block
    thread_local static std::vector<std::string> tagDecryptor({"integer_cast", "main"}); // array that associate with tags (indexes) with block-names
    static Tag tagIntegerCast = 0;
    static Tag tagMain = 1;
    thread_local static std::unordered_map<std::string, Tag> nameEncryptor({{"integer_cast", tagIntegerCast}, {"main", tagMain}}); // hashtable that associate block-names with tags
    thread_local static std::stack<Tag> tagStack({tagMain}); // contains the current stack

    // counter for the number of unstable branches
    static std::atomic_int unstableBranchCounter;
}

#endif //SHAMAN_GLOBALVARIABLES_H
