//
// Created by demeuren on 08/06/18.
//
#pragma once

#include <stack>
#include <string>
#include <vector>
#include <unordered_map>
#include <atomic>
#include <mutex>

using Tag = unsigned short int; // represents a block

class ShamanGlobals
{
public:
    // used to model the stacktrace
    static std::vector<std::string> tagDecryptor; // array that associate tags (indexes) with block-names
    static const Tag tagIntegerCast;
    static const Tag tagUntagged;
    static std::unordered_map<std::string, Tag> nameEncryptor; // hashtable that associate block-names with tags
    thread_local static std::stack<Tag> tagStack; // contains the current stack
    static std::mutex mutexAddName; // guards against concurent addition of names in the encryptor/decryptor

    // counter for the number of unstable branches
    static std::atomic_int unstableBranchCounter;
};
