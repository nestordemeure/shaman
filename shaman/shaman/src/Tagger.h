//
// Created by demeuren on 04/06/18.
//

#ifndef SHAMAN_TAGGER_H
#define SHAMAN_TAGGER_H

#include <stdexcept>
#include "GlobalVar.h"

/*
 * tracks the current section of the code
 */
class Block
{
public:
    // tags associated with the current block
    Tag blockTag; // TODO currently useless

    /*
     * declares that we are now in a given block
     */
    Block(const std::string& name)
    {
        blockTag = tagOfName(name);
        ShamanGlobals::tagStack.push(blockTag);
    }

    /*
     * declares that we are now in a given block
     */
    Block(Tag tag): blockTag(tag)
    {
        ShamanGlobals::tagStack.push(tag);
    }

    /*
     * leaves the block
     */
    ~Block()
    {
        ShamanGlobals::tagStack.pop();
    }

    /*
     * returns the current block name
     */
    inline static Tag currentBlock()
    {
        return ShamanGlobals::tagStack.top();
    }

    /*
     * returns the name associated with a tag
     */
    static std::string nameOfTag(Tag tag)
    {
        return ShamanGlobals::tagDecryptor[tag];
    }

    /*
     * returns the tag associated with a name
     * note : this operation cost an hashtable lookup
     * TODO make threadsafe
     * TODO can we do this operation at compile time
     * TODO maybe not needed if we use numeric types (function pointers) from the beginning
     */
    static Tag tagOfName(const std::string& name)
    {
        auto potentialTag = ShamanGlobals::nameEncryptor.find(name);
        if(potentialTag != ShamanGlobals::nameEncryptor.end())
        {
            return potentialTag->second;
        }
        else
        {
            Tag tag = (unsigned short int) ShamanGlobals::tagDecryptor.size();
            ShamanGlobals::tagDecryptor.push_back(name);
            ShamanGlobals::nameEncryptor[name]=tag;
            return tag;
        }
    }
};

// macro to quickly define blocks
#define STR_CONCAT_HELPER(a,b) a ## b
#define STR_CONCAT(a,b) STR_CONCAT_HELPER(a,b)
#define FUNCTION_BLOCK Block STR_CONCAT(shamanFunctionBlock,__FUNCTION__)(__FUNCTION__)
#define LOCAL_BLOCK(text) Block STR_CONCAT(shamanLocalBlock,__LINE__)(text)

#endif //SHAMAN_TAGGER_H
