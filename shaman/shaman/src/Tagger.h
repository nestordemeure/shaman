//
// Created by demeuren on 04/06/18.
//

#ifndef SHAMAN_TAGGER_H
#define SHAMAN_TAGGER_H

// TODO use a more efficient representation for block identifiers
using Tag = std::string;

/*
 * tracks the current section of the code
 */
class Block
{
private:
    // stores the tags
    static std::stack<Tag> tagList = {"main"};
    #ifdef _OPENMP
    #pragma omp threadprivate (tagList)
    #endif //_OPENMP

public:
    // tags associated with the current block
    Tag blockName;

    /*
     * declares that we are now in a given block
     */
    Block(const Tag& name): blockName(name)
    {
        tagList.push(name);
    }

    /*
     * leaves the block
     */
    ~Block()
    {
        if(tagList.top() == blockName)
        {
            tagList.pop();
        }
        else
        {
            Tag currentBlock = tagList.top();
            std::string errorMessage = "Tried to leave block '" + blockName + "' while still in inner block '" + currentBlock + "'.";
            throw std::exception(errorMessage);
        }
    }

    /*
     * returns the current block name
     */
    inline static Tag currentBlock()
    {
        return tagList.top();
    }
};

#endif //SHAMAN_TAGGER_H
