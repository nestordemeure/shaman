//
// Created by demeuren on 08/06/18.
//

#include "GlobalVar.h"

// used to model the stacktrace
const Tag ShamanGlobals::tagIntegerCast = 0;
const Tag ShamanGlobals::tagMain = 1;
std::vector<std::string> ShamanGlobals::tagDecryptor = std::vector<std::string>({"integer_cast", "main"}); // array that associate with tags (indexes) with block-names
std::unordered_map<std::string, Tag> ShamanGlobals::nameEncryptor = {{"integer_cast", ShamanGlobals::tagIntegerCast}, {"main", ShamanGlobals::tagMain}}; // hashtable that associate block-names with tags
thread_local std::stack<Tag> ShamanGlobals::tagStack({ShamanGlobals::tagMain}); // contains the current stack
std::mutex ShamanGlobals::mutexAddName;

// counter for the number of unstable branches
std::atomic_int ShamanGlobals::unstableBranchCounter;