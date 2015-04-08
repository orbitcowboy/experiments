#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <list>
#include <cstdlib>
#include <cstdio>
#include <algorithm>
#include <functional>
#include <vector>
#include <iterator>
#include <sys/types.h>
#include <cctype> // isalnum

#include "simple_testsuite.hpp"
#include "simple_shell_macros.hpp"

std::ostringstream errout;
std::ostringstream output;

simple_testsuite_settings::simple_testsuite_settings()
    : m_OutputMode(COLORED_OUTPUT)
{}

simple_testsuite_settings::simple_testsuite_settings(const simple_testsuite_settings &rhs)
{
    m_OutputMode = rhs.m_OutputMode;
}

simple_testsuite_settings& simple_testsuite_settings::operator=(const simple_testsuite_settings &rhs)
{
    // avoid copy when self assign
    if( this != &rhs )
    {
        m_OutputMode = rhs.m_OutputMode;
    }
    return *this;
}

void simple_testsuite_settings::vEnableASCIIOutput(void)
{
    m_OutputMode = ASCII_OUTPUT;
}

void simple_testsuite_settings::vEnableTXT2TagsOutput(void)
{
    m_OutputMode = TXT2TAGS_OUTPUT;
}

void simple_testsuite_settings::vEnableColoredOutput(void)
{
    m_OutputMode = COLORED_OUTPUT;
}

bool simple_testsuite_settings::bIsASCIIOutputSet(void)    const
{
    return m_OutputMode == ASCII_OUTPUT;
}

bool simple_testsuite_settings::bIsTXT2TagsOutputSet(void) const
{
    return m_OutputMode == TXT2TAGS_OUTPUT;
}

bool simple_testsuite_settings::bIsColoredOutputSet(void) const
{
    return m_OutputMode == COLORED_OUTPUT;
}

simple_testsuite_assert_results::simple_testsuite_assert_results(void)
{
    m_strColorTags[RED_COLOR_BEG          ] = "\033[22;31m";
    m_strColorTags[GREEN_COLOR_BEG        ] = "\033[22;32m";
    m_strColorTags[RED_COLOR_REVERSE_BEG  ] = "\033[22;31m\33[7m";
    m_strColorTags[GREEN_COLOR_REVERSE_BEG] = "\033[22;32m\33[7m";
    m_strColorTags[COLOR_END              ] = "\033[22;39m\33[0m";
}

simple_testsuite_assert_results::simple_testsuite_assert_results(const simple_testsuite_assert_results &rhs)
    : m_Asserts(rhs.m_Asserts)
{
    for(size_t  i = 0; i < NUMBER_OF_COLORS ; ++i)
    {
        m_strColorTags[i] = rhs.m_strColorTags[i];
    }
}

simple_testsuite_assert_results& simple_testsuite_assert_results::operator=(const simple_testsuite_assert_results &rhs)
{
    // avoid copy when self assign
    if( this != &rhs )
    {
        m_Asserts   = rhs.m_Asserts;
        for(size_t  i = 0; i < NUMBER_OF_COLORS ; ++i)
        {
            m_strColorTags[i] = rhs.m_strColorTags[i];
        }
    }
    return *this;
}

simple_testsuite_assert_results::~simple_testsuite_assert_results(void) {}


bool simple_testsuite_assert_results::bAddAssertResult(const std::string &strFileName, const unsigned int &uiLineNumber, const std::string &strExpected, const std::string &strActual)
{
    SAssertResults Item;
    std::ostringstream strStream;
    strStream << uiLineNumber;
    std::string strLineNumber(strStream.str());
    Item.strFileName   = strFileName;
    Item.strLineNumber = strLineNumber;
    Item.strExpected   = strExpected;
    Item.strActual     = strActual;
    const size_t Size  = m_Asserts.size();
    m_Asserts.push_back(Item);
    // did the push back work?
    return (Size + 1 == m_Asserts.size());
}

std::string simple_testsuite_assert_results::strGetResults(const bool &bColorize)
{
    std::string strResult("\n");
    for(std::vector<SAssertResults>::const_iterator It = m_Asserts.begin(); It != m_Asserts.end(); ++It)
    {
        if(bColorize)
        {
            std::string strColoredExpected, strColoredActual;
            vColourize(strColoredExpected, strColoredActual, It->strExpected, It->strActual);
            strResult += "Assertion failed in " + It->strFileName + " at line " + It->strLineNumber + "\n";
            strResult += "Expected:\n" + strColoredExpected + "\n";
            strResult += "Actual:\n" + strColoredActual + "\n";
        }
        else // no color output
        {
            strResult += "Assertion failed in " + It->strFileName + " at line " + It->strLineNumber + "\n";
            strResult += "Expected:\n" + It->strExpected + "\n";
            strResult += "Actual:\n" + It->strActual + "\n";
        }
    }
    return strResult;
}

void simple_testsuite_assert_results::vColourize(std::string &strExpected, std::string &strActual, const std::string &strE, const std::string &strA)
{
    size_t ** pC = pLCSLength<std::string>(strE, strA);

    if(pC != NULL)
    {
        vGenerateDiff(pC, strE, strA, strE.size(), strA.size(), strExpected, strActual);
        for(size_t i = 0; i < strE.size() + 1; ++i)
        {
            delete [] pC[i];
        }
        delete [] pC;
    }
}

/**
 * Test Registry
 **/

TestRegistry::TestRegistry(void)
{
}

TestRegistry &TestRegistry::theInstance(void)
{
    static TestRegistry testreg;
    return testreg;
}

void TestRegistry::addTest(TestFixture *t)
{
    m_Tests.push_back(t);
}

void TestRegistry::RemoveByName(const std::string &strName)
{
    for (std::list<TestFixture *>::const_iterator it = m_Tests.begin(); it != m_Tests.end(); ++it)
    {
        if( (*it)->classname == strName)
        {
            m_Tests.remove(*it);
            break;
        }
    }
}

void TestRegistry::RevertListOrder(void)
{
    m_Tests.reverse();
}

void TestRegistry::RandomizeListOrder(void)
{
    // copy list to vector
    std::vector<TestFixture *> v;
    std::copy(m_Tests.begin(), m_Tests.end(), v.begin());
    std::random_shuffle(v.begin(), v.end());
    // copy vector to list
    m_Tests.clear();
    std::copy(v.begin(), v.end(), m_Tests.begin());
}

const std::list<TestFixture *> & TestRegistry::tests(void) const
{
    return m_Tests;
}

/**
 * Test Fixture
 **/

std::ostringstream TestFixture::errmsg;
simple_testsuite_assert_results TestFixture::m_SCAssertResultRecorder;
size_t       TestFixture::countTests;
bool               TestFixture::bRevertOrder = false;
size_t TestFixture::fails_counter = 0;
size_t TestFixture::assert_counter = 0;
size_t TestFixture::todos_counter = 0;
size_t TestFixture::m_assertCount = 0;
size_t TestFixture::fails_counter_backup = 0;
simple_testsuite_settings TestFixture::m_Settings;


std::string simple_testsuite_settings::strGetOptionInformationString(void)
{
    std::string strResult;

    strResult +=  "----------------------------------------\n";
    strResult +=  "- simple_testsuite                     -\n";
    strResult +=  "- ****************                     -\n";
    strResult +=  "- options:                             -\n";
    strResult +=  "- s Show list of available unit tests  -\n";
    strResult +=  "- r Run all tests                      -\n";
    strResult +=  "- h Help                               -\n";
    strResult +=  "- e Exit                               -\n";
    strResult +=  "----------------------------------------\n";

    return strResult;
}

TestFixture::TestFixture(const std::string &_name)
    : classname(_name)
    , bIsActivated(true)
    , m_LengthOfLinePtr(0)
    , m_bStopOnError(false)
{
    m_uiRandomSeed = static_cast<unsigned int>(time(NULL));
#ifdef _WIN32
    m_uiRandomSeed += GetCurrentProcessId();
#else // LINUX
    m_uiRandomSeed += + static_cast<unsigned int>(getpid());
#endif
    srand(m_uiRandomSeed);
    TestRegistry::theInstance().addTest(this);
}

TestFixture::~TestFixture(void)
{
}

void TestFixture::assertEquals_bool(const char * const fileName, unsigned int lineNumber, const bool &expected, const bool &actual)
{
    if(expected != actual)
    {
        const std::string expectedResult((expected ? "true" : "false"));
        const std::string actualResult((actual ? "true" : "false"));
        assertEquals(fileName, lineNumber, expectedResult, actualResult);
    }
    else
    {
        ++assert_counter;
    }
}

void TestFixture::vStopOnError(const bool &bStopOnError)
{
    m_bStopOnError = bStopOnError;
}

void TestFixture::vSetConfiguration(const simple_testsuite_settings &Settings)
{
    m_Settings = Settings;
}

bool TestFixture::runTest(const char testname[])
{
    if (testToRun.empty() || testToRun == testname)
    {
        if (m_Settings.bIsTXT2TagsOutputSet())
        {
            std::cout << "| " << classname << "::" << testname << " | ";
        }
        else
        {
            //if(m_strCurrentClassName != classname)
            //{
            std::cout << classname << "::" << testname;
            m_strCurrentClassName = classname;
            //}
            m_LengthOfLinePtr = classname.length() + 2 + std::string(testname).length();
        }
        std::cout.flush();

        ++countTests;
        return true;
    }
    else
    {
        reportStatus();
        printFinalReport();
        //exit(-1);
    }
    return false;
}

void TestFixture::reportStatus(void) const
{
    // report results
    // fill up with dots
    if(!m_Settings.bIsTXT2TagsOutputSet())
    {
        int width = static_cast<int>(m_LengthOfLinePtr);
        if(m_LengthOfLinePtr < 80)
        {
            width = 80 - static_cast<int>(m_LengthOfLinePtr);
        }
        std::cout << std::setfill('.') << std::setw(width);
    }
    if(m_Settings.bIsColoredOutputSet())
    {
        if(fails_counter_backup == fails_counter)
        {
            COUT_LIGHT_GREEN(" [OK]");
        }
        else
        {
            fails_counter_backup = fails_counter;
            COUT_RED(" [NOK]");
        }
    }
    else // ASCII
    {
        if(fails_counter_backup == fails_counter)
        {
            std::cout << " [OK]";
        }
        else
        {
            fails_counter_backup = fails_counter;
            std::cout << " [NOK]";
        }
    }
    if(m_Settings.bIsTXT2TagsOutputSet())
    {
        std::cout << " | ";
    }
    std::cout << "\n";
}

std::string TestFixture::strWriteStr(const std::string &str)
{
    if(str.empty())
        return "";

    std::ostringstream ostr;
    ostr << "\"";
    for (std::string::const_iterator i = str.begin(); i != str.end(); ++i)
    {
        if (*i == '\n')
        {
            ostr << "\\n";
        }
        else if (*i == '\t')
        {
            ostr << "\\t";
        }
        else if (*i == '\r')
        {
            ostr << "\\r";
        }
        else
        {
            ostr << *i;
        }
    }
    ostr << "\"";
    return ostr.str();
}

void TestFixture::vAssert(const char *filename, unsigned int linenr, bool condition)
{
    ++assert_counter;
    if (!condition)
    {
        ++fails_counter;
        errmsg << "Assertion failed in " << filename << " at line " << linenr << std::endl;
    }
}

/*
void wait_for_key ()
{
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__TOS_WIN__)  // every key press registered, also arrow keys
    std::cout << std::endl << "Press any key to continue..." << std::endl;

    FlushConsoleInputBuffer(GetStdHandle(STD_INPUT_HANDLE));
    _getch();
#elif defined(unix) || defined(__unix) || defined(__unix__) || defined(__APPLE__)
    std::cout << std::endl << "Press ENTER to continue..." << std::endl;

    std::cin.clear();
    std::cin.ignore(std::cin.rdbuf()->in_avail());
    std::cin.get();
#endif
    return;
}
*/
void TestFixture::assertEquals(const char * const cFilename, unsigned int uiLineNr, const std::string &strExpected, const std::string &strActual, const std::string &strMsg)
{
    ++assert_counter;
    if (strExpected != strActual)
    {
        ++fails_counter;

        if(!m_SCAssertResultRecorder.bAddAssertResult(cFilename, uiLineNr, TestFixture::strWriteStr(strExpected),  TestFixture::strWriteStr(strActual)))
        {
            std::cerr << "### Internal error in TestFixture::assertEquals, please report\n";
        }

        // write the results to the output stream
        errmsg << m_SCAssertResultRecorder.strGetResults(m_Settings.bIsColoredOutputSet());

        if (!strMsg.empty())
        {
            errmsg << strMsg << std::endl;
        }

        //printf("\n%s\n\n", m_SCAssertResultRecorder.strGetResults(m_Settings.bIsColoredOutputSet()).c_str() );
        //printf("\n\n<Push ENTER to proceed>\n");
        //fflush(stdout);
        //wait_for_key ();
    }
}

void TestFixture::assertEquals(const char * const cFileName, unsigned int uiLineNr, double dExpected, double dActual, const std::string &strMsg)
{
    std::ostringstream ostr1;
    ostr1 << dExpected;
    std::ostringstream ostr2;
    ostr2 << dActual;
    assertEquals(cFileName, uiLineNr, ostr1.str(), ostr2.str(), strMsg);
}

void TestFixture::todoAssertEquals(const char * const filename, unsigned int uiLineNr, const std::string &strExpected, const std::string &strActual)
{
    if (strExpected == strActual)
    {
        assertEquals(filename, uiLineNr, "TODO assertion", "The assertion succeeded");
    }
    else
    {
        ++todos_counter;
    }
}

void TestFixture::todoAssertEquals(const char * const cFileName, unsigned int uiLineNr, unsigned int uiExpected, unsigned int uiActual)
{
    std::ostringstream ostr1;
    ostr1 << uiExpected;
    std::ostringstream ostr2;
    ostr2 << uiActual;
    todoAssertEquals(cFileName, uiLineNr, ostr1.str(), ostr2.str());
}

void TestFixture::todoAssertEquals_bool(const char * const filename, const unsigned int &lineNumber, const bool &expected, const bool &actual)
{
    todoAssertEquals(filename, lineNumber, (expected == true ? "true" : "false"), (actual == true ? "true" : "false"));
}

void TestFixture::assertThrowFail(const char * const cFileName, unsigned int uiLineNr)
{
    ++fails_counter;

    errmsg << "Assertion failed in " << cFileName << " at line " << uiLineNr << std::endl
           << "The expected exception was not thrown" << std::endl;
}

void TestFixture::printTests(void)
{
    const std::list<TestFixture *> &tests = TestRegistry::theInstance().tests();

    for (std::list<TestFixture *>::const_iterator it = tests.begin(); it != tests.end(); ++it)
    {
        std::cout << (*it)->classname << std::endl;
    }
}

const std::list<std::string> TestFixture::strGetListOfTests(void)
{
    std::list<std::string> List;
    const std::list<TestFixture *> &tests = TestRegistry::theInstance().tests();

    for (std::list<TestFixture *>::const_iterator it = tests.begin(); it != tests.end(); ++it)
    {
        List.push_back((*it)->classname);
    }

    return List;
}

void TestFixture::vExcludeTest(const std::string &strNameOfTest)
{
    TestRegistry::theInstance().RemoveByName(strNameOfTest);
}

void TestFixture::run(const std::string &str)
{
    testToRun = str;
    run();
}

size_t TestFixture::runTests(const char cmd[])
{
    std::string strClassName(cmd ? cmd : "");
    std::string testname("");
    if (strClassName.find("::") != std::string::npos)
    {
        testname = strClassName.substr(strClassName.find("::") + 2);
        strClassName.erase(strClassName.find("::"));
    }
    std::cout.flush();

    countTests = 0;
    errmsg.str("");
    // revert the order during execution
    if(bRevertOrder)
    {
        TestRegistry::theInstance().RevertListOrder();
    }
    const std::list<TestFixture *> &tests = TestRegistry::theInstance().tests();
    for (std::list<TestFixture *>::const_iterator it = tests.begin(); it != tests.end(); ++it)
    {
        // perform default processing: by default a test is set as activated
        if ((strClassName.empty() || (*it)->classname == strClassName) && (*it)->bIsActivated)
        {
            (*it)->run(testname);
        }
        // perform specific tests that are explicitly activated
        else if ( ((*it)->classname == strClassName) && !(*it)->bIsActivated)
        {
            (*it)->run(testname);
        }
    }

    printFinalReport();

    return fails_counter;
}

void TestFixture::printFinalReport(void)
{
    // define output text here
    const std::string strHeadline          ("Test statistics\n"      );
    const std::string strAssertTestsLabel  ("#Asserts (#Tests)...: " );
    const std::string strToDoLabel         ("#To do\'s............: ");
    const std::string strSuccessAssertLabel("#Successful asserts.: " );
    const std::string strFailedAssertLabel ("#Failed asserts.....: " );

    std::cout << "\n\n";
    if(m_Settings.bIsColoredOutputSet())
    {
        COUT_UNDERLINED(strHeadline);
        std::cout << strAssertTestsLabel;
        std::cout << assert_counter << " (" << countTests << ")";
        std::cout << "\n";

        std::cout << strToDoLabel;
        COUT_YELLOW(todos_counter);
        std::cout << "\n";

        std::cout << strSuccessAssertLabel;
        COUT_GREEN(assert_counter - fails_counter);
        std::cout << "\n";

        std::cout << strFailedAssertLabel;
        if(fails_counter > 0)
        {
            COUT_RED(fails_counter);
        }
        else
        {
            COUT_GREEN(fails_counter);
        }
        // calling flush here, to do all output before the error messages (in case the output is buffered)
        std::cout.flush();
    }
    else if(m_Settings.bIsTXT2TagsOutputSet()) // txt2tags
    {
        std::cout << strHeadline;
        std::cout << "| " << strAssertTestsLabel   << " | " << assert_counter << " (" << countTests << ")" << " | " << std::endl;
        std::cout << "| " << strToDoLabel          << " | " << todos_counter                               << " | " << std::endl;
        std::cout << "| " << strSuccessAssertLabel << " | " << assert_counter - fails_counter              << " | " << std::endl;
        std::cout << "| " << strFailedAssertLabel  << " | " << fails_counter                               << " | " << std::endl;

        // calling flush here, to do all output before the error messages (in case the output is buffered)
        std::cout.flush();
    }
    else // ASCII
    {
        std::cout << strHeadline;
        std::cout << strAssertTestsLabel   << assert_counter << " (" << countTests << ")" << std::endl;
        std::cout << strToDoLabel          << todos_counter                               << std::endl;
        std::cout << strSuccessAssertLabel << assert_counter - fails_counter              << std::endl;
        std::cout << strFailedAssertLabel  << fails_counter                               << std::endl;

        // calling flush here, to do all output before the error messages (in case the output is buffered)
        std::cout.flush();
    }

    std::cout << "\n";
    std::cerr << errmsg.str();
    std::cerr.flush();
    std::cout.flush();
}

void TestFixture::reportOut(const std::string & strOutput) const
{
    output << strOutput << std::endl;
}

static char GetRandomAlphaNum(void)
{
    char c;
    while (!std::isalnum(c = static_cast<char>(std::rand())))
        ;
    return c;
}

static std::string GetRandomFileName (std::string::size_type sz)
{
    std::string s;
    s.reserve (sz);
    generate_n (std::back_inserter(s), sz, GetRandomAlphaNum);
    return s;
}

bool TestFixture::executeExternalTool(const std::string &toolName, const std::string &toolOptions, const std::vector<std::string> &fileNames, std::string &capturedOutput, size_t captureFromStream)
{
    // generate a random file name
    const std::string::size_type randomFileNameLength = 8U;
    const std::string tempFileName(GetRandomFileName(randomFileNameLength));

    // create system call string
    std::string streamCapture;
    if(captureFromStream == 2)
        streamCapture = " 2>";
    else if (captureFromStream == 1)
        streamCapture = " 1>";
    else
        streamCapture = " 2>&1";

    std::string filePaths;
    for (std::vector<std::string>::const_iterator it = fileNames.begin(); it != fileNames.end(); ++it)
    {
        filePaths += *it + " ";
    }
    // create system call command
    const std::string systemCallCommand = toolName + " " + toolOptions + " " + filePaths + streamCapture + " " + tempFileName;
    // execute system call
    if(std::system(systemCallCommand.c_str()))   // System call is failing
    {
        // delete temp file
        if( remove( tempFileName.c_str() ) != 0 )
        {
            return false;
        }
        return false;
    }
    // read captured file content and open an input stream object
    std::ifstream ifs(tempFileName.c_str());
    // get the whole file
    const std::string fileContent((std::istreambuf_iterator<char>(ifs)), std::istreambuf_iterator<char>());

    capturedOutput = fileContent;
    // delete temp file
    if( remove( tempFileName.c_str() ) != 0 )
    {
        return false;
    }

    return true;
}
