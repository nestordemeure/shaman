//
// Created by demeuren on 05/06/18.
//

#ifndef SHAMAN_GLOBALVARIABLES_H
#define SHAMAN_GLOBALVARIABLES_H

/*
 * program wide constants
 */
namespace Shaman
{
    // counter for the number of unstable branches
    static std::atomic_int unstableBranchCounter;

    // used to follow the current position in the code
    using Tag = std::string; // TODO use a more efficient representation for block identifiers such as ints
    static std::stack<Tag> tagStack({"main"}); // TODO make threadsafe
}

#endif //SHAMAN_GLOBALVARIABLES_H
