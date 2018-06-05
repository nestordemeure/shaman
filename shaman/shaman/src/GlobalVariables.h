//
// Created by demeuren on 05/06/18.
//

#ifndef SHAMAN_GLOBALVARIABLES_H
#define SHAMAN_GLOBALVARIABLES_H

/*
 * global constants
 */
namespace Shaman
{
    // used to follow the current position in the code
    using Tag = std::string; // TODO use a more efficient representation for block identifiers such as ints
    thread_local static std::stack<Tag> tagStack({"main"});

    // counter for the number of unstable branches
    static std::atomic_int unstableBranchCounter;
}

#endif //SHAMAN_GLOBALVARIABLES_H
