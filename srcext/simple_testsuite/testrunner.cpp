// ------------------------------
/// A driver to run unit tests
//
/// @author Ettl martin
/// @date   2013-06-20
// ------------------------------

#include <cstdlib>
#include <vector>
#include <cstdio>
#include <cstring>
#include <string.h>
#ifdef _WIN32
#define snprintf sprintf_s
#endif

#include "simple_testsuite.hpp"
#include "simple_tokenize.hpp"

// Predefine functions
void vPrintHeader(void);

int main(const int argc, const char *argv[])
{
    bool bInteractiveMode = false;
    bool bRundSequential  = false;
    const std::string strASCIIOutputOption    = "--output=ascii";
    const std::string strTXT2TAGSOutputOption = "--output=txt2tags";
    const std::string strRunOption            = "--run=";
    const std::string strRevertOrderOption    = "--revert-order";
    const std::string strExcludeOption        = "--exclude=";
    const std::string strSeqOption            = "--seq";
    const std::string strShowTestsOption      = "--show-tests";
    const std::string strNumberOfTestsOption  = "--show-number-of-tests";
    std::vector<std::string> TestQueue;

    simple_testsuite_settings Settings;

    if(argc > 1)
    {
        // argc is known to be a positive number
        for(size_t index = 1 ; index < static_cast<size_t>(argc) ; ++index)
        {
            if( argv[index][0] == '-' )
            {
                switch(argv[index][1])
                {
                    case 'h':
                    {
                        std::cout << std::endl;
                        std::cout << "\nBasic usage:\n\n";
                        std::cout << "\t../bin/testrunner [options] [name of test]\n\n";
                        std::cout << "If no argument is provided all available tests will be performed.\n\n";
                        std::cout << "options:\n";
                        std::cout << "-------\n";
                        std::cout << "\t -i Enters the interactive mode\n";
                        std::cout << "\t " << strASCIIOutputOption << "|" << strTXT2TAGSOutputOption << "|colored (=default)\n";
                        std::cout << "\t " << strRunOption << "[name of test to run]:\n";
                        std::cout << "\t   In order to run several tests in sequential order, separate them \n";
                        std::cout << "\t   using a ',' operator e.g.: --run=Test1,Test2,Test3\n";
                        std::cout << "\t " << strRevertOrderOption << ": revert the order of modules during execution\n";
                        std::cout << "\t " << strExcludeOption << "[name of to exclude from run]:\n";
                        std::cout << "\t   In order to exclude several tests, separate them \n";
                        std::cout << "\t   using a ',' operator e.g.: --exclude=Test1,Test2,Test3\n";
                        std::cout << "\t " << strSeqOption << ": runs each test, but in sequential order\n";
                        std::cout << "\t " << strShowTestsOption << ": Prints a list of available tests\n";
                        std::cout << "\t " << strNumberOfTestsOption << ": Prints the current number of registered \n";
                        std::cout << "\t   testclasses\n";
                        std::cout << std::endl;
                        return 0;
                    }
                    /// activate interactive mode
                    case 'i':
                    {
                        bInteractiveMode = true;
                        break;
                    }

                    case '-':
                    {
                        const std::string option(argv[index]);
                        if(option.empty())
                        {
                            break;
                        }

                        if(strncmp(option.c_str(), strASCIIOutputOption.c_str(), strASCIIOutputOption.size()) == 0U
                                && option.size() == strASCIIOutputOption.size())
                        {
                            Settings.vEnableASCIIOutput();
                            break;
                        }
                        else if(strncmp(option.c_str(), strTXT2TAGSOutputOption.c_str(), strTXT2TAGSOutputOption.size()) == 0U
                                && option.size() == strTXT2TAGSOutputOption.size())
                        {
                            Settings.vEnableTXT2TagsOutput();
                            break;
                        }
                        else if(strncmp(option.c_str(), strSeqOption.c_str(), strSeqOption.size()) == 0U
                                && option.size() == strSeqOption.size())
                        {
                            bRundSequential = true;
                            break;
                        }
                        else if(strncmp(option.c_str(), strRevertOrderOption.c_str(), strRevertOrderOption.size()) == 0U
                                && option.size() == strRevertOrderOption.size())
                        {
                            TestFixture::bRevertOrder = true;
                            break;
                        }
                        else if(strncmp(option.c_str(), strRunOption.c_str(), strRunOption.size()) == 0U)
                        {
                            // remember option
                            std::string strListOfTests(argv[index] + strRunOption.size());
                            std::vector<std::string> strTokens;
                            simple_tokenize<CIsFromString>::Tokenize(strTokens, strListOfTests, CIsFromString(","));
                            for(size_t i = 0; i < strTokens.size(); ++i)
                            {
                                TestQueue.push_back(strTokens[i]);
                            }
                            break;
                        }
                        else if(strncmp(option.c_str(), strExcludeOption.c_str(), strExcludeOption.size()) == 0U
                                && option.size() == strExcludeOption.size())
                        {
                            // remember option
                            std::string strListOfTestsToExclude(argv[index] + strExcludeOption.size());
                            std::vector<std::string> strTokens;
                            simple_tokenize<CIsFromString>::Tokenize(strTokens, strListOfTestsToExclude, CIsFromString(","));
                            for(size_t i = 0; i < strTokens.size(); ++i)
                            {
                                TestFixture::vExcludeTest(strTokens[i]);
                            }
                            break;
                        }
                        else if(strncmp(option.c_str(), strShowTestsOption.c_str(), strShowTestsOption.size()) == 0U
                                && option.size() == strShowTestsOption.size())
                        {
                            std::cout << "\n";
                            std::cout << "Available tests are:\n\n";

                            std::list<std::string> strList(TestFixture::strGetListOfTests());

                            for (std::list<std::string>::const_iterator it = strList.begin(); it != strList.end(); ++it)
                            {
                                std::cout << "\t" << (*it).c_str() << std::endl;
                            }
                            std::cout << "\n--------------------------------------------------------------------------------\n";
                            std::cout << "\n\nIn order to run a single test, start the testrunner with the following command:\n\n";
                            std::cout << "\t\t../bin/testrunner --run=[Name]\n\n";
                            std::cout << "The variable [Name] is the name of the test, shown by this command.\n\n";
                            std::cout << "\n--------------------------------------------------------------------------------\n";
                            return 0;
                        }
                        else if(strncmp(option.c_str(), strNumberOfTestsOption.c_str(), strNumberOfTestsOption.size()) == 0U
                                && option.size() == strNumberOfTestsOption.size())
                        {
                            std::list<std::string> strList(TestFixture::strGetListOfTests());
                            std::cout << strList.size() << std::endl;
                            return 0;
                        }
                        else
                        {
                            std::cout << "Unknown command-line option. Start with option -h to get more information about how to use this test suite.\n";
                            return -1;
                        }
                        break;
                    }
                    default:
                    {
                        std::cout << "Unknown command-line option. Start with option -h to get more information about how to use this test suite.\n";
                        return -1;
                    }
                }// end switch
            }// end if
            else
            {
                std::cout << "Unknown command-line option. Start with option -h to get more information about how to use this test suite.\n";
                return -1;
            }
        }
    }
    // set the configuration
    TestFixture::vSetConfiguration(Settings);
    size_t retVal = 0;
    if(!bInteractiveMode)
    {
        // run tests
        if(TestQueue.empty()) // --> run all tests
        {
            if(bRundSequential)
            {
                // Get list of tests
                std::list<std::string> strList(TestFixture::strGetListOfTests());
                // iterate over all registered tests
                for (std::list<std::string>::const_iterator it = strList.begin(); it != strList.end(); ++it)
                {
                    retVal |= TestFixture::runTests((*it).c_str() );
                }

            }
            else // default run all at once
            {
                retVal = TestFixture::runTests(NULL);
            }
        }
        else // run a specific test
        {
            for(size_t ui = 0; ui < TestQueue.size(); ui++)
            {
                char * cBuf;
                const size_t sizeofBuf = TestQueue[ui].size() + 1;
                try
                {
                    cBuf = new char[sizeofBuf];
                }
                catch (...)
                {
                    std::cerr << "### Testrunner: error in memory allocation.";
                    exit(-1);
                }

                snprintf(cBuf, sizeofBuf, "%s", TestQueue[ui].c_str());
                std::cout << TestQueue[ui] << std::endl;
                // run tests
                retVal |= TestFixture::runTests(cBuf);
                // deallocate memory
                delete [] cBuf;
            }
        }
    }
    else // interactive mode
    {
        vPrintHeader();

        while (1)
        {
            std::string strVal;
            std::cin >> strVal;

            if(strVal == "s")
            {
                // clear screen
                std::cout << "\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n";

                std::list<std::string> strList(TestFixture::strGetListOfTests());
                unsigned int uiIndex = 0;
                for (std::list<std::string>::const_iterator it = strList.begin(); it != strList.end(); ++it)
                {
                    std::cout << uiIndex++ << "\t" << (*it).c_str() << std::endl;
                }
            }
            else if(strVal == "r")
            {
                retVal = TestFixture::runTests(NULL);
            }
            else if(strVal == "e")
            {
                break;
            }
            else
            {
                vPrintHeader();
            }
        };
    }
    fflush(stdout);
    // exit and give feedback
    return (retVal == 0 ? EXIT_SUCCESS : EXIT_FAILURE);
}

void vPrintHeader(void)
{
    // clear screen and input stream buffer
    std::cin.clear();
    // clear screen
    std::cout << "\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n";
    // print options
    std::cout << simple_testsuite_settings::strGetOptionInformationString() << std::endl;
}
