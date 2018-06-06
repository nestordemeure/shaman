//
// Created by demeuren on 05/06/18.
//

#include "GlobalVariables.h"

// used to model the stacktrace
std::vector<std::string> Shaman::tagDecryptor = std::vector<std::string>({"integer_cast", "main"}); // array that associate with tags (indexes) with block-names
thread_local std::unordered_map<std::string, Shaman::Tag> Shaman::nameEncryptor = {{"integer_cast", Shaman::tagIntegerCast}, {"main", Shaman::tagMain}}; // hashtable that associate block-names with tags
thread_local std::stack<Shaman::Tag> Shaman::tagStack = {Shaman::tagMain}; // contains the current stack

// counter for the number of unstable branches
std::atomic_int Shaman::unstableBranchCounter = 0;