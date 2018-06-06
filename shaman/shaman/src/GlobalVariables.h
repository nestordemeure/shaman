//
// Created by demeuren on 05/06/18.
//

#ifndef SHAMAN_GLOBALVARIABLES_H
#define SHAMAN_GLOBALVARIABLES_H

/*
 * global constants
 */
#include <stack>
#include <string>
#include <vector>
#include <unordered_map>
#include <atomic>

namespace Shaman
{
    // used to model the stacktrace
    using Tag = unsigned short int; // represents a block
    thread_local static std::vector<std::string> tagDecryptor; // array that associate with tags (indexes) with block-names
    static const Tag tagIntegerCast = 0;
    static const Tag tagMain = 1;
    thread_local static std::unordered_map<std::string, Tag> nameEncryptor; // hashtable that associate block-names with tags
    thread_local static std::stack<Tag> tagStack; // contains the current stack

    // counter for the number of unstable branches
    static std::atomic_int unstableBranchCounter;
}

#endif //SHAMAN_GLOBALVARIABLES_H
