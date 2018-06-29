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
    /*
     * declares that we are now in a given block
     */
    Block(const std::string& name)
    {
        Tag blockTag = tagOfName(name);
        ShamanGlobals::tagStack.push(blockTag);
    }

    /*
     * declares that we are now in a given block
     */
    Block(Tag tag)
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
            std::lock_guard<std::mutex> guard(ShamanGlobals::mutexAddName); // TODO can it be insufficient to guard against problem when running find on an unordered_map
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
