/*!
 * \file simple_tokenize.hpp
 * \brief Here are the routines of the simple_tokenizer class defined.
 *  It is designed to split strings according to a provided separator.
 *
 * \author Dr. Martin Ettl
 * \version $Rev: 1567 $ $Date: 2015-04-07 11:21:30 +0200 (Tue, 07 Apr 2015) $
 * @todo -> extend the tokenizer to be able to concatenate several literals with
 *        boolean operators e.g: a & b, a | b, ....
 */
#ifndef SIMPLE_TOKENIZE_HPP
#define SIMPLE_TOKENIZE_HPP

#include <functional>
#include <string>
#include <vector>
#include <algorithm>
#include <cctype>
#include <ctype.h>
#include <cstring>
#include <cstdlib>
#include <cwchar>
#include <wctype.h>

/** \addtogroup simple_tokenize simple_tokenize
 *  @{
 */

/// \brief This class can be used as template parameter for simple_tokenize.
/// In case a string should be split according to spaces, use this class
/// as template parameter.
/// This is illustrated in following example code:
/// \code{.cpp}
///  const std::string stringToTokenize = "A B C";
///  const std::vector<std::string> result;
///  result = simple_tokenize<CIsSpace>::Tokenize(stringToTokenize));
/// \endcode
class CIsSpace : public std::unary_function<char, bool>
{
    public:
        /// This operator overloading is required to check if the currently
        /// processed character is a space.
        ///
        /// @param c The character to be validated.
        /// @return  true in case it is a space and false otherwise.
        bool operator() (const char &c) const
        {
            // Zero is returned in case c is a white space and a value different
            // from zero is returned in case c is not a white space.
            return (isspace(c) != 0);
        }
};
#if 0
/// \brief This class can be used as template parameter for simple_tokenize.
/// In case a string should be split according to spaces, use this class
/// as template parameter.
/// This is illustrated in following example code:
/// \code{.cpp}
///  const std::wstring stringToTokenize = L"A B C";
///  const std::vector<std::wstring> result;
///  result = simple_tokenize<CIsWSpace>::Tokenize(stringToTokenize));
/// \endcode
class CIsWSpace : public std::unary_function<wchar_t, bool>
{
    public:
        /// This operator overloading is required to check if the currently
        /// processed character is a space.
        ///
        /// @param c The wide string character to be validated.
        /// @return  true in case it is a space and false otherwise.
        bool operator() (const wchar_t &c) const
        {
            // Zero is returned in case c is a white space and a value different
            // from zero is returned in case c is not a white space.
            return (iswspace(c) != 0);
        }
};
#endif
/// \brief This class can be used as template parameter for simple_tokenize.
/// In case a string should be split according to commas (','), use this class
/// as template parameter.
/// This is illustrated in following example code:
/// \code{.cpp}
///  const std::string stringToTokenize = "A,B,C";
///  const std::vector<std::string> result;
///  result = simple_tokenize<CIsComma>::Tokenize(stringToTokenize));
/// \endcode
class CIsComma : public std::unary_function<char, bool>
{
    public:
        /// This operator overloading is required to check if the currently
        /// processed character is a comma.
        ///
        /// @param c The string character to be validated.
        /// @return  true in case it is a comma and false otherwise.
        bool operator()(const char &c) const
        {
            return (',' == c);
        }
};
#if 0
/// \brief This class can be used as template parameter for simple_tokenize.
/// In case a string should be split according to commas (','), use this class
/// as template parameter.
/// This is illustrated in following example code:
/// \code{.cpp}
///  const std::wstring stringToTokenize = L"A,B,C";
///  const std::vector<std::wstring> result;
///  result = simple_tokenize<CIsWComma>::Tokenize(stringToTokenize));
/// \endcode
class CIsWComma : public std::unary_function<wchar_t, bool>
{
    public:
        /// This operator overloading is required to check if the currently
        /// processed character is a comma.
        ///
        /// @param c The string character to be validated.
        /// @return  true in case it is a comma and false otherwise.
        bool operator()(const wchar_t &c) const
        {
            return (L',' == c);
        }
};
#endif

//For the case the separator is an ampersand ('&')
class CIsAmpersand : public std::unary_function<char, bool>
{
    public:
        bool operator()(const char &c) const
        {
            return ('&' == c);
        }
};

#if 0
//For the case the separator is an ampersand ('&')
class CIsWAmpersand : public std::unary_function<wchar_t, bool>
{
    public:
        bool operator()(const wchar_t &c) const
        {
            return (L'&' == c);
        }
};
#endif

// For the case the separator is + (plus) or - (minus) or / (division) or * (multiplicand)
class CIsArithmetic : public std::unary_function<char, bool>
{
    public:
        bool operator()(const char &c) const
        {
            return (('/' == c) || ('+' == c) || ('-' == c) || ('*' == c));
        }
};

#if 0
// For the case the separator is + (plus) or - (minus) or / (division) or * (multiplicand)
class CIsWArithmetic : public std::unary_function<wchar_t, bool>
{
    public:
        bool operator()(const wchar_t &c) const
        {
            return ((L'/' == c) || (L'+' == c) || (L'-' == c) || (L'*' == c));
        }
};
#endif

//For the case the separator is a character from a set of characters given in a string
class CIsFromString : public std::unary_function<char, bool>
{
    public:
        CIsFromString(const std::string & rostr) : m_ostr(rostr) {}
        bool operator()(const char &c) const
        {
            return (m_ostr.find(c) != std::string::npos);
        }

    private:
        std::string m_ostr;
};

#if 0
//For the case the separator is a character from a set of characters given in a string
class CIsFromWString : public std::unary_function<wchar_t, bool>
{
    public:
        //Constructor specifying the separators
        explicit CIsFromWString(const std::wstring & rostr) : m_ostr(rostr) {}
        bool operator()(const wchar_t &c) const
        {
            return (m_ostr.find(c) != std::wstring::npos);
        }

    private:
        std::wstring m_ostr;
};
#endif

/// \brief This class is capable of splitting strings according to a
///  provided separator.
///  The template parameter Pred consists of a set of predefined classes
///  to simplify splitting.
template < class Pred = CIsSpace > class simple_tokenize
{
    public:

        static void Tokenize(std::vector<std::string>& roResult
                             , const std::string & rostr
                             , const Pred & roPred = Pred());

        static void TokenizeAndGetNthToken(std::string& roResult
                                           , const std::string & rostr
                                           , const Pred & roPred
                                           , const size_t &tokenIndex);

        static void Tokenize(std::vector<std::string>& strVecResult
                             , const std::string &strToTokenize
                             , const std::string &strPattern);

        // a more convenient function. It returns an vector of strings
        static std::vector<std::string> Tokenize(const std::string & rostr
                , const Pred & roPred = Pred());

        // tokenize a string according to gives front and back token
        static std::string TokenizeByFrontAndBack(const std::string &strToTokenize
                , const std::string &strTokenFront
                , const std::string &strTokenBack);

        static std::vector< std::vector<std::string> > Tokenize(const std::vector<std::string> & vector_of_strings
                , const Pred & roPred = Pred());

        // tokenize a string according to multiple tokens
        static std::vector<std::string> MultiTokenize(const std::string &strToTokenize
                , const std::string &strMultiTokens);

        // tokenize a string according to multiple tokens and keep the separators
        static std::vector<std::string> MultiTokenizeAndKeepSeparators(const std::string& stringToSplit, const std::string &separators, const std::string &filter = "");

        // wstring version
#if 0
        static void Tokenize(std::vector<std::wstring>& roResult
                             , const std::wstring & rostr
                             , const Pred & roPred = Pred());
        static void TokenizeAndGetNthToken(std::wstring& roResult
                                           , const std::wstring & rostr
                                           , const Pred & roPred
                                           , const size_t &tokenIndex);
        static void Tokenize(std::vector<std::wstring>& strVecResult
                             , const std::wstring &strToTokenize
                             , const std::wstring &strPattern);
        // a more convenient function. It returns an vector of strings
        static std::vector<std::wstring> Tokenize(const std::wstring & rostr
                , const Pred & roPred = Pred());
        // tokenize a string according to gives front and back token
        static std::wstring TokenizeByFrontAndBack(const std::wstring &strToTokenize
                , const std::wstring &strTokenFront
                , const std::wstring &strTokenBack);
        static std::vector< std::vector<std::wstring> > Tokenize(const std::vector<std::wstring> & vector_of_strings
                , const Pred & roPred = Pred());
#endif
};

/// This function splits up a string into pieces according to a provided separator.
///
/// It can be used as follows:
///  \code{.cpp}
///         // declare a string that will be tokenized
///         std::string strToTokenize = "A\tB\\nC   D   E!!!";
///         // create an empty buffer, where the splitted string will be stored
///         vector<std::string> strResult;
///         // split the string
///         simple_tokenize<>::Tokenize(strResult, strToTokenize);
///
///         // verify the result
///         if( strResult[0]!="A"  && strResult[1]!="B"
///         && strResult[2]!="C" && strResult[3]!="D"
///         && strResult[4]!="E!!!") { return false; }
///  \endcode
///  In the example code, a string is split up according to spaces, newlines and tab.
///
/// \param roResult An empty vector of type string.
/// \param rostr    The string to be tokenized.
/// \param roPred   The separator.
///
template <class Pred> inline void simple_tokenize<Pred>::Tokenize(std::vector<std::string>& roResult, const std::string & rostr, const Pred & roPred)
{
    //First clear the results vector
    roResult.clear();
    std::string::const_iterator it          = rostr.begin();
    std::string::const_iterator itTokenEnd  = it;
    while(it != rostr.end())
    {
        //Eat separators
        while (it != rostr.end() && roPred(*it)) ++it;

        //Find next token
        itTokenEnd = find_if(it, rostr.end(), roPred);
        //Append token to result
        if(it < itTokenEnd)
            roResult.push_back(std::string(it, itTokenEnd));
        it = itTokenEnd;
    }
}
#if 0
template <class Pred> inline void simple_tokenize<Pred>::Tokenize(std::vector<std::wstring>& roResult, const std::wstring & rostr, const Pred & roPred)
{
    //First clear the results vector
    roResult.clear();
    std::wstring::const_iterator it          = rostr.begin();
    std::wstring::const_iterator itTokenEnd  = rostr.begin();
    while(it != rostr.end())
    {
        //Eat separators
        while (it != rostr.end() && roPred(*it)) ++it;

        //Find next token
        itTokenEnd = find_if(it, rostr.end(), roPred);
        //Append token to result
        if(it < itTokenEnd)
            roResult.push_back(std::wstring(it, itTokenEnd));
        it = itTokenEnd;
    }
}
#endif
// --------------------------------------------------------------------------------------------
/// Tokenize a string and return the Nth token.
/// This function splits a string into several pieces, according to the provided
/// delimiter and selects returns the Nth token.
///
/// \param roResult <--> the Nth token
/// \param rostr    --> the string to be tokenized
/// \param roPred   --> the token
/// \param tokenIndex --> the desired token
///
// --------------------------------------------------------------------------------------------
template <class Pred> inline void simple_tokenize<Pred>::TokenizeAndGetNthToken(std::string& roResult, const std::string & rostr, const Pred & roPred, const size_t &tokenIndex)
{
    std::vector<std::string> resultVector;
    simple_tokenize<Pred>::Tokenize(resultVector, rostr, roPred);
    if(tokenIndex < resultVector.size())
    {
        roResult = resultVector[tokenIndex];
    }
}
#if 0
template <class Pred> inline void simple_tokenize<Pred>::TokenizeAndGetNthToken(std::wstring& roResult, const std::wstring & rostr, const Pred & roPred, const size_t &tokenIndex)
{
    std::vector<std::wstring> resultVector;
    simple_tokenize<Pred>::Tokenize(resultVector, rostr, roPred);
    if(tokenIndex < resultVector.size())
    {
        roResult = resultVector[tokenIndex];
    }
}
#endif

// --------------------------------------------------------------------------------------------
/// tokenize function
/// here, the string will be split up by the provided tokens
///
/// usage:
///         std::string strToTokenize = "sum\tsum\ngoes   home   now!!!";
///         std::vector<std::string> strResult(simple_tokenize<>::Tokenize(strToTokenize));
///
///         if( strResult[0]!="sum"  && strResult[1]!="sum"
///         && strResult[2]!="goes" && strResult[3]!="home"
///         && strResult[4]!="now!!!") return false;
///
/// \param rostr    --> the string to be tokenized
/// \param roPred   --> the token
///
///
/// \return <-- an stl vector of type string with the tokenized items
// --------------------------------------------------------------------------------------------
template <class Pred> inline std::vector<std::string> simple_tokenize<Pred>::Tokenize(std::string const& rostr, Pred const& roPred)
{
    // allocate memory
    std::vector<std::string> roResult;
    // tokenize
    Tokenize(roResult, rostr, roPred);
    // return result
    return roResult;
}
#if 0
template <class Pred> inline std::vector<std::wstring> simple_tokenize<Pred>::Tokenize(std::wstring const& rostr, Pred const& roPred)
{
    // allocate memory
    std::vector<std::wstring> roResult;
    // tokenize
    Tokenize(roResult, rostr, roPred);
    // return result
    return roResult;
}
#endif

// --------------------------------------------------------------------------------------------
/// tokenize function
/// here, the vector of strings will be split up by the provided tokens
///
/// usage:
///         std::vector<std::string> strAToTokenize;
///                                  strAToTokenize.push_back("t e s t!");
///                                  strAToTokenize.push_back("T E S T!");
///         std::vector< std::vector<std::string> > strAResult = simple_tokenize<>::Tokenize(strAToTokenize);
///         if (strAResult[0][0] != "t" &&
///             strAResult[0][1] != "e" &&
///             strAResult[0][2] != "s" &&
///             strAResult[0][3] != "t!"&&
///             strAResult[1][0] != "T" &&
///             strAResult[1][1] != "E" &&
///             strAResult[1][2] != "S" &&
///             strAResult[1][3] != "T!")
///             return false;
///
///
///
///
/// \param vector_of_strings    --> a vector of string that should be tokenized
/// \param roPred               --> the token
///
///
/// \return <-- an stl vector that contains vectors of tokenized strings
// --------------------------------------------------------------------------------------------
template <class Pred> inline std::vector< std::vector<std::string> > simple_tokenize<Pred>::Tokenize(const std::vector<std::string> & vector_of_strings
        , const Pred & roPred)
{
    std::vector< std::vector < std::string > > result;
    for(size_t ui = 0; ui < vector_of_strings.size(); ui++)
    {
        result.push_back(Tokenize(vector_of_strings[ui], roPred));
    }
    return result;
}
#if 0
template <class Pred> inline std::vector< std::vector<std::wstring> > simple_tokenize<Pred>::Tokenize(const std::vector<std::wstring> & vector_of_strings
        , const Pred & roPred)
{
    std::vector< std::vector < std::wstring > > result;
    for(unsigned int ui = 0; ui < vector_of_strings.size(); ui++)
    {
        result.push_back(Tokenize(vector_of_strings[ui], roPred));
    }
    return result;
}
#endif
// --------------------------------------------------------------------------------------------
/// tokenize function
/// here, the vector of strings will be split up by the provided tokens
///
/// \param strResult            <--> an stl vector of type std::string, it will be filled with tokens
/// \param strToTokenize        --> a std::string that will be tokenized
/// \param strPattern           --> a pattern
///
// --------------------------------------------------------------------------------------------
template <class Pred> void simple_tokenize<Pred>::Tokenize(std::vector<std::string>& strResult
        , const std::string &strToTokenize
        , const std::string &strPattern)
{
    // ---------
    // precheck:
    // ---------
    if(strToTokenize.empty())
        return;
    if(strPattern.empty())
        return;

    // create a working copy
    std::string str(strToTokenize);
    // remove preceding pattern
    if(str.find(strPattern) == 0)
    {
        str = str.substr(strPattern.length());
    }
    // loop over patterns
    while(true)
    {
        size_t foundPatternBeg = str.find(strPattern);
        if (foundPatternBeg != std::string::npos)
        {
            strResult.push_back(str.substr(0, foundPatternBeg));
            str = str.substr(foundPatternBeg + strPattern.length());
        }
        else
        {
            strResult.push_back(str.substr(0, foundPatternBeg));
            break;
        }
    }
}

#if 0
template <class Pred> void simple_tokenize<Pred>::Tokenize(std::vector<std::wstring>& strResult
        , const std::wstring &strToTokenize
        , const std::wstring &strPattern)
{
    // ---------
    // precheck:
    // ---------
    if(strToTokenize.empty())
        return;
    if(strPattern.empty())
        return;

    // create a working copy
    std::wstring str(strToTokenize);
    // remove preceding pattern
    if(str.find(strPattern) == 0)
    {
        str = str.substr(strPattern.length());
    }
    // loop over patterns
    while(true)
    {
        size_t foundPatternBeg = str.find(strPattern);
        if (foundPatternBeg != std::wstring::npos)
        {
            strResult.push_back(str.substr(0, foundPatternBeg));
            str = str.substr(foundPatternBeg + strPattern.length());
        }
        else
        {
            strResult.push_back(str.substr(0, foundPatternBeg));
            break;
        }
    }
}
#endif

template <class Pred> std::string simple_tokenize<Pred>::TokenizeByFrontAndBack(const std::string &strToTokenize
        , const std::string &strTokenFront
        , const std::string &strTokenBack)
{
    std::vector<std::string>  strResult;
    // does the string contain the tokens?
    if(((strToTokenize.find(strTokenFront) != std::string::npos)
            && (strToTokenize.find(strTokenBack)) != std::string::npos))
    {
        simple_tokenize<>::Tokenize(strResult, strToTokenize, strTokenFront);
        // determine the index for the second tokenization
        const unsigned short index = ( (strResult.size() > 1U) ? 1U : 0U );
        std::vector<std::string>  strResult1;
        simple_tokenize<>::Tokenize(strResult1, strResult[index], strTokenBack);
        if(strResult1.empty() == false)
        {
            return strResult1[0];
        }
    }
    return std::string("");
}

#if 0
template <class Pred> std::wstring simple_tokenize<Pred>::TokenizeByFrontAndBack(const std::wstring &strToTokenize
        , const std::wstring &strTokenFront
        , const std::wstring &strTokenBack)
{
    std::vector<std::wstring>  strResult;
    // does the string contain the tokens?
    if(((strToTokenize.find(strTokenFront) != std::wstring::npos)
            && (strToTokenize.find(strTokenBack)) != std::wstring::npos))
    {
        simple_tokenize<Pred>::Tokenize(strResult, strToTokenize, strTokenFront);
        // determine the index for the second tokenization
        const unsigned short index = ( (strResult.size() > 1U) ? 1U : 0U );
        std::vector<std::wstring>  strResult1;
        simple_tokenize<Pred>::Tokenize(strResult1, strResult[index], strTokenBack);
        if(strResult1.empty() == false)
        {
            return strResult1[0];
        }
    }
    return std::wstring(L"");
}
#endif


template <class Pred> std::vector<std::string> simple_tokenize<Pred>::MultiTokenize(const std::string &strToTokenize
        , const std::string &strMultiTokens)
{
    // allocate a buffer (needed by strtok_r and strtok_s)
    char *cBuf  = NULL;

    // the vector where the result will be stored
    std::vector<std::string>  strVResult;
    // a local copy of a the string to be tokenized
    std::string str(strToTokenize);

    // start tokenizing the string
#ifdef WIN32
    char *token = strtok_s( const_cast<char*>( str.c_str() ), strMultiTokens.c_str(), &cBuf );
#else // LINUX
    char *token = strtok_r( const_cast<char*>( str.c_str() ), strMultiTokens.c_str(), &cBuf );
#endif

    // iterate over all tokens
    while ( token != NULL )
    {
        // save the current result
        strVResult.push_back( token );

        // get next token (if any)
#ifdef WIN32
        token = strtok_s( NULL, strMultiTokens.c_str(), &cBuf);
#else // LINUX
        token = strtok_r( NULL, strMultiTokens.c_str(), &cBuf);
#endif
    }
    return strVResult;
}

template <class Pred> std::vector<std::string> simple_tokenize<Pred>::MultiTokenizeAndKeepSeparators(const std::string& stringToSplit, const std::string &separators, const std::string &filter)
{
    std::vector<std::string> result;
    std::string currentToken;
    // iterate over the string to be splitted
    for(std::string::size_type i = 0; i < stringToSplit.size(); ++i)
    {
        // check if the current character is a splitting character
        if(separators.find(stringToSplit[i]) != std::string::npos)
        {
            // in case we have collected already characters, added the to the result vector
            if(!currentToken.empty())
            {
                // add the current characters to the result vector
                result.push_back(currentToken);
                // prepare for next run and clear the currentToken buffer
                currentToken.clear();
            }
            // in case a filter character is not found, do append it to the result
            if(filter.find(stringToSplit[i]) == std::string::npos)
            {
                const std::string currentSeparator(stringToSplit.substr(i, 1));
                result.push_back(currentSeparator);
            }
        }
        else
        {
            // there was nothing to split, collect characters in currentToken buffer
            currentToken += stringToSplit[i];
        }
    }
    // in case the current token has captured trailing characters, add them to the result.
    if(!currentToken.empty())
    {
        result.push_back(currentToken);
    }
    return result;
}

/** @}*/

#endif // SIMPLE_TOKENIZE_HPP

