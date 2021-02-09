#pragma once

#include <stack>
#include <string>
#include <vector>
#include <unordered_map>
#include <atomic>
#include <mutex>

// represents a block
using Tag = unsigned short int;

class ShamanGlobals
{
public:
    // used to model the stacktrace
    static std::vector<std::string> tagDecryptor; // array that associate tags (indexes) with block-names
    static const Tag tagUntagged;
    static std::unordered_map<std::string, Tag> nameEncryptor; // hashtable that associate block-names with tags
    thread_local static std::stack<Tag> tagStack; // contains the current stack
    static std::mutex mutexAddName; // guards against concurent addition of names in the encryptor/decryptor

    // counter for the number of unstable branches
    static std::atomic_int unstableBranchCounter;
    static std::unordered_map<Tag, unsigned int> unstableBranchSummary; // hashtable that associate block-names with the number of untable branch detected within
    static std::mutex mutexAddUnstableBranch; // guards against concurent addition of unstable branches in unstableBranchSummary
};
