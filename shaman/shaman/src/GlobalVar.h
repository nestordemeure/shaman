//
// Created by demeuren on 08/06/18.
//

#ifndef SHAMAN_GLOBALVAR_H
#define SHAMAN_GLOBALVAR_H

#include <stack>
#include <string>
#include <vector>
#include <unordered_map>
#include <atomic>

using Tag = unsigned short int; // represents a block

class ShamanGlobals
{
public:
    // used to model the stacktrace
    static std::vector<std::string> tagDecryptor; // array that associate with tags (indexes) with block-names // TODO make thread safe
    static const Tag tagIntegerCast;
    static const Tag tagMain;
    static std::unordered_map<std::string, Tag> nameEncryptor; // hashtable that associate block-names with tags // TODO make thread safe
    thread_local static std::stack<Tag> tagStack; // contains the current stack

    // counter for the number of unstable branches
    static std::atomic_int unstableBranchCounter;
};


#endif //SHAMAN_GLOBALVAR_H
