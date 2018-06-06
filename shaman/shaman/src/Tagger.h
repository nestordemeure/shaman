//
// Created by demeuren on 04/06/18.
//

#ifndef SHAMAN_TAGGER_H
#define SHAMAN_TAGGER_H

#include <stdexcept>
#include "GlobalVariables.h"

/*
 * tracks the current section of the code
 */
class Block
{
public:
    // tags associated with the current block
    Shaman::Tag blockTag;

    /*
     * declares that we are now in a given block
     */
    Block(const std::string& name)
    {
        blockTag = tagOfName(name);
        Shaman::tagStack.push(blockTag);
    }

    /*
     * declares that we are now in a given block
     */
    Block(Shaman::Tag tag): blockTag(tag)
    {
        Shaman::tagStack.push(tag);
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

    /*
     * returns the name associated with a tag
     */
    static std::string nameOfTag(Shaman::Tag t)
    {
        return Shaman::tagDecryptor[t];
    }

    /*
     * returns the tag associated with a name
     * note : this operation cost an hashtable lookup
     * TODO it would be ideal to do this operation at compile time
     * TODO maybe not needed if we use numeric types (function pointers) from the beginning
     */
    static Shaman::Tag tagOfName(const std::string& name)
    {
        auto potentialTag = Shaman::nameEncryptor.find(name);
        if(potentialTag != Shaman::nameEncryptor.end())
        {
            return potentialTag->second;
        }
        else
        {
            Shaman::Tag tag = (unsigned short int) Shaman::tagDecryptor.size();
            Shaman::tagDecryptor.push_back(name);
            Shaman::nameEncryptor[name]=tag;
            //std::cout << "#TAGGER: " << name << "->" << tag << std::endl;
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
