#include "global_vars.h"

// used to model the stacktrace
const Tag ShamanGlobals::tagUntagged = 0;
std::vector<std::string> ShamanGlobals::tagDecryptor = std::vector<std::string>({"untagged_block"}); // array that associate with tags (indexes) with block-names
std::unordered_map<std::string, Tag> ShamanGlobals::nameEncryptor = {{"untagged_block", ShamanGlobals::tagUntagged}}; // hashtable that associate block-names with tags
thread_local std::stack<Tag> ShamanGlobals::tagStack({ShamanGlobals::tagUntagged}); // contains the current stack
std::mutex ShamanGlobals::mutexAddName;

// counter for the number of unstable branches
std::atomic_int ShamanGlobals::unstableBranchCounter(0);
std::unordered_map<Tag, unsigned int> ShamanGlobals::unstableBranchSummary;
std::mutex ShamanGlobals::mutexAddUnstableBranch;
