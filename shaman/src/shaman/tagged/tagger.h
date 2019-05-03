//
// Created by demeuren on 04/06/18.
//

#ifndef SHAMAN_TAGGER_H
#define SHAMAN_TAGGER_H

#include <stdexcept>
#include <shaman/tagged/global_vars.h>

/*
 * tracks the current section of the code
 */
class CodeBlock
{
public:
    /*
     * declares that we are now in a given block
     */
    CodeBlock(const std::string& name)
    {
        Tag blockTag = tagOfName(name);
        ShamanGlobals::tagStack.push(blockTag);
    }

    /*
     * declares that we are now in a given block
     */
    CodeBlock(Tag tag)
    {
        ShamanGlobals::tagStack.push(tag);
    }

    /*
     * leaves the block
     * NOTE : we do not check that we do pop what we pushed
     */
    ~CodeBlock()
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
     * TODO can we do this operation at compile time ? (it would make it MPI proof)
     * TODO maybe not needed if we use numeric types (function pointers) from the beginning (but we lose the fact that tags are indexes which is bad)
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
    
    /*
     * returns the number of tags currently declared
     */
    static size_t tagNumber()
    {
        return ShamanGlobals::tagDecryptor.size();
    }
};

// macro to quickly define blocks
#if defined(SHAMAN_TAGGED_ERROR) && ! defined(NO_SHAMAN)
#define STR_CONCAT_HELPER(a,b) a ## b
#define STR_CONCAT(a,b) STR_CONCAT_HELPER(a,b)
#define FUNCTION_BLOCK CodeBlock STR_CONCAT(shamanFunctionBlock,__FUNCTION__)(__FUNCTION__)
#define LOCAL_BLOCK(text) CodeBlock STR_CONCAT(shamanLocalBlock,__LINE__)(text)
#else
#define FUNCTION_BLOCK
#define LOCAL_BLOCK(text)
#endif

#endif //SHAMAN_TAGGER_H
