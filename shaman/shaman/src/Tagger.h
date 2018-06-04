//
// Created by demeuren on 04/06/18.
//

#ifndef SHAMAN_TAGGER_H
#define SHAMAN_TAGGER_H

#include <stack>
#include <stdexcept>

// TODO use a more efficient representation for block identifiers
using Tag = std::string;
static std::stack<std::string> blockNamesStack({"main"}); // TODO make threadsafe

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
        blockNamesStack.push(name);
    }

    /*
     * leaves the block
     */
    ~Block()
    {
        blockNamesStack.pop();
        /*
        if(blockNamesStack.top() == blockName)
        {
            blockNamesStack.pop();
        }
        else
        {
            Tag currentBlock = blockNamesStack.top();
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
        return blockNamesStack.top();
    }
};

// macro to quickly define blocks
#define STR_CONCAT_HELPER(a,b) a ## b
#define STR_CONCAT(a,b) STR_CONCAT_HELPER(a,b)
#define FUNCTION_BLOCK Block STR_CONCAT(shamanFunctionBlock,__FUNCTION__)(__FUNCTION__)
#define LOCAL_BLOCK(text) Block STR_CONCAT(shamanLocalBlock,__LINE__)(text)

#endif //SHAMAN_TAGGER_H
