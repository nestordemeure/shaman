//
// Created by demeuren on 04/06/18.
//

#ifndef SHAMAN_TAGGER_H
#define SHAMAN_TAGGER_H

#include <stdexcept>
#include <stack>
#include "GlobalVariables.h"

/*
 * tracks the current section of the code
 */
class Block
{
public:
    // tags associated with the current block
    Shaman::Tag blockName;

    /*
     * declares that we are now in a given block
     */
    Block(const Shaman::Tag& name): blockName(name)
    {
        Shaman::tagStack.push(name);
    }

    /*
     * leaves the block
     */
    ~Block()
    {
        Shaman::tagStack.pop();
    }

    /*
     * returns the current block name
     */
    inline static Shaman::Tag currentBlock()
    {
        return Shaman::tagStack.top();
    }
};

// macro to quickly define blocks
#define STR_CONCAT_HELPER(a,b) a ## b
#define STR_CONCAT(a,b) STR_CONCAT_HELPER(a,b)
#define FUNCTION_BLOCK Block STR_CONCAT(shamanFunctionBlock,__FUNCTION__)(__FUNCTION__)
#define LOCAL_BLOCK(text) Block STR_CONCAT(shamanLocalBlock,__LINE__)(text)

#endif //SHAMAN_TAGGER_H
