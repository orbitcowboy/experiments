#include <iostream>

#include "simple_tokenize.hpp"
#include "simple_shell_macros.hpp"

using namespace std;

bool bTestTokenizer(void);

//Testing the class
int main(void)
{
    std::cout << "Checking Tokenizer: ";
    if(!bTestTokenizer())
    {
        std::cout << COULOURIZE_RED  ("[NOK]\n");
        return -1;
    }
    else
    {
        std::cout << COULOURIZE_GREEN("[OK]\n");
    }

    return 0;
}

bool bTestTokenizer(void)
{
    std::string strToTokenize = "sum\tsum\ngoes   home\r   now!!!";
    std::vector<std::string> strResult(simple_tokenize<>::Tokenize(strToTokenize));

    if( strResult[0] != "sum"  && strResult[1] != "sum"
            && strResult[2] != "goes" && strResult[3] != "home"
            && strResult[4] != "now!!!") return false;

    strToTokenize = "sum,sum,goes,home,now!!!";
    simple_tokenize<CIsComma>::Tokenize(strResult, strToTokenize);

    if( strResult[0] != "sum"  && strResult[1] != "sum"
            && strResult[2] != "goes" && strResult[3] != "home"
            && strResult[4] != "now!!!") return false;

    strToTokenize = "sum$sum$goes$home$now!!!";
    simple_tokenize<CIsFromString>::Tokenize(strResult, strToTokenize, CIsFromString("$"));

    if( strResult[0] != "sum"  && strResult[1] != "sum"
            && strResult[2] != "goes" && strResult[3] != "home"
            && strResult[4] != "now!!!") return false;

    std::vector<std::string> strAToTokenize;
    strAToTokenize.push_back("t e s t!");
    strAToTokenize.push_back("T E S T!");
    std::vector< std::vector<std::string> > strAResult = simple_tokenize<>::Tokenize(strAToTokenize);
    if (strAResult[0][0] != "t" &&
            strAResult[0][1] != "e" &&
            strAResult[0][2] != "s" &&
            strAResult[0][3] != "t!" &&
            strAResult[1][0] != "T" &&
            strAResult[1][1] != "E" &&
            strAResult[1][2] != "S" &&
            strAResult[1][3] != "T!")
        return false;

    // test the arithmetic tokenizing
    strToTokenize = "-6*eps*u*du^2+4*eps*om^2*u^3+om^4*u+3*eps^2*u^5";
    strResult = simple_tokenize<CIsArithmetic>::Tokenize(strToTokenize);

    if( strResult[0]  != "6" &&
            strResult[1]  != "eps" &&
            strResult[2]  != "u" &&
            strResult[3]  != "du^2" &&
            strResult[4]  != "4" &&
            strResult[5]  != "eps" &&
            strResult[6]  != "om^2" &&
            strResult[7]  != "u^3" &&
            strResult[8]  != "om^4" &&
            strResult[9]  != "u" &&
            strResult[10] != "3" &&
            strResult[11] != "eps^2" &&
            strResult[12] != "u^5")
        return false;

    // test CIsFromString tokenization
    strToTokenize = "enum Weggy{a,b,c,d};";
    simple_tokenize<CIsFromString>::Tokenize(strResult, strToTokenize, CIsFromString("{"));
    if( strResult[0] != "enum Weggy" && strResult[1] !=  "a,b,c,d};")
        return false;

    // everything OK
    return true;
}
