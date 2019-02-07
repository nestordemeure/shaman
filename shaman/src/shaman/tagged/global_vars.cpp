//
// Created by demeuren on 08/06/18.
//

#include "global_vars.h"

// used to model the stacktrace
const Tag ShamanGlobals::tagIntegerCast = 0;
const Tag ShamanGlobals::tagUntagged = 1;
std::vector<std::string> ShamanGlobals::tagDecryptor = std::vector<std::string>({"integer_cast", "untagged_block"}); // array that associate with tags (indexes) with block-names
std::unordered_map<std::string, Tag> ShamanGlobals::nameEncryptor = {{"integer_cast", ShamanGlobals::tagIntegerCast}, {"untagged_block", ShamanGlobals::tagUntagged}}; // hashtable that associate block-names with tags
thread_local std::stack<Tag> ShamanGlobals::tagStack({ShamanGlobals::tagUntagged}); // contains the current stack
std::mutex ShamanGlobals::mutexAddName;

// counter for the number of unstable branches
std::atomic_int ShamanGlobals::unstableBranchCounter(0);
std::unordered_map<Tag, unsigned int> ShamanGlobals::unstableBranchSummary;
std::mutex ShamanGlobals::mutexAddUnstableBranch;
