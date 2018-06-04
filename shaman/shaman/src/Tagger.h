//
// Created by demeuren on 04/06/18.
//

#ifndef SHAMAN_TAGGER_H
#define SHAMAN_TAGGER_H

#include <stack>
#include <stdexcept>

using Tag = std::string; // TODO use a more efficient representation for block identifiers
static std::stack<std::string> tagStack({"main"}); // TODO make threadsafe

/*
 * tracks the current section of the code
 */
class Block
{
public:
    // tags associated with the current block
    Tag blockName;

    /*
     * declares that we are now in a given block
     */
    Block(const Tag& name): blockName(name)
    {
        tagStack.push(name);
    }

    /*
     * leaves the block
     */
    ~Block()
    {
        tagStack.pop();
        /*
        if(tagStack.top() == blockName)
        {
            tagStack.pop();
        }
        else
        {
            Tag currentBlock = tagStack.top();
            std::string errorMessage = "Tried to leave block '" + blockName + "' while still in inner block '" + currentBlock + "'.";
            throw std::runtime_error(errorMessage);
        }
        */
    }

    /*
     * returns the current block name
     */
    inline static Tag currentBlock()
    {
        return tagStack.top();
    }
};

// macro to quickly define blocks
#define STR_CONCAT_HELPER(a,b) a ## b
#define STR_CONCAT(a,b) STR_CONCAT_HELPER(a,b)
#define FUNCTION_BLOCK Block STR_CONCAT(shamanFunctionBlock,__FUNCTION__)(__FUNCTION__)
#define LOCAL_BLOCK(text) Block STR_CONCAT(shamanLocalBlock,__LINE__)(text)

#endif //SHAMAN_TAGGER_H
