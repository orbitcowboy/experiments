/// This is a fork from cppchecks (http://sourceforge.net/apps/mediawiki/cppcheck/index.php?title=Main_Page) test suite with a few
/// modifications. This modifications where needed to adapt it to our project structure.
/// @author Dr. Martin Ettl
/// @date   2014-04-22

#pragma once
#ifndef SIMPLE_TESTSUITE_H
#define SIMPLE_TESTSUITE_H

// enforce posix compliance for tested code
#define _POSIX_SOURCE 1

#include <sstream>
#include <iostream>
#include <iomanip>
#include <list>
#include <map>
#include <vector>
#include <utility>
#include <stdlib.h> // needed for rand_r() function

#ifndef _WIN32 //LINUX
#include <inttypes.h>
#include <unistd.h>
using namespace std;
#elif _WIN32
#include <Windows.h>
#include <minmax.h>
#endif //_WIN32


///\brief A class to store current settings for tests
class simple_testsuite_settings
{
    public:

        simple_testsuite_settings(void);
        simple_testsuite_settings(const simple_testsuite_settings &rhs);
        simple_testsuite_settings& operator=(const simple_testsuite_settings &rhs);

        void vEnableASCIIOutput(void);
        void vEnableTXT2TagsOutput(void);
        void vEnableColoredOutput(void);

        bool bIsASCIIOutputSet(void) const;
        bool bIsTXT2TagsOutputSet(void) const;
        bool bIsColoredOutputSet(void) const;

        static std::string strGetOptionInformationString(void);

    protected:

        typedef enum
        {
            ASCII_OUTPUT
            , TXT2TAGS_OUTPUT
            , COLORED_OUTPUT
        } EOutputMode;

        EOutputMode m_OutputMode;
};

///\brief  A class to record asserts
class simple_testsuite_assert_results
{
    public:

        simple_testsuite_assert_results(void);
        simple_testsuite_assert_results(const simple_testsuite_assert_results &rhs);
        simple_testsuite_assert_results& operator=(const simple_testsuite_assert_results &rhs);
        ~simple_testsuite_assert_results(void);

        /// \brief Record an assert result.
        /// \param strFileName   The associated file name
        /// \param uiLineNumber  The associated line number
        /// \param strExpected   The expected result as string
        /// \param strActual     The actual result as string
        /// \return true on success, false otherwise
        bool bAddAssertResult(const std::string &strFileName, const unsigned int &uiLineNumber, const std::string &strExpected, const std::string &strActual);
        std::string strGetResults(const bool &bColorize = true);

    protected:

        void vColourize(std::string &strExpected, std::string &strActual, const std::string &strE, const std::string &strA);

        /// Compute the longest common subsequence between X and Y
        /// On return, C will contain the LCS table.
        /// C[m][n] will contain the length of the longest common subsequence.
        /// HTTP://en.wikipedia.org/wiki/Longest_common_subsequence_problem
        /// \param X  The expected value
        /// \param Y  The actual value
        /// \return C[m][n] will contain the length of the longest common subsequence
        template<typename T> size_t ** pLCSLength(const T &X, const T &Y)
        {
            const size_t m(X.size());
            const size_t n(Y.size());

            size_t **C = NULL;
            try
            {
                C = new size_t*[m + 1];
            }
            catch(std::bad_alloc)
            {
                return NULL;
            }
            try
            {
                for(size_t i = 0; i < m + 1; ++i)
                {
                    C[i] = new size_t[n + 1];
                }
            }
            catch(std::bad_alloc)
            {
                delete [] C;
                return NULL;
            }

            for (size_t i = 0; i <= m; ++i)
            {
                C[i][0] = 0;
            }

            for (size_t j = 0; j <= n; ++j)
            {
                C[0][j] = 0;
            }

            for (size_t i = 0; i < m; ++i)
            {
                for (size_t j = 0; j < n; ++j)
                {
                    if (X[i] == Y[j])
                    {
                        C[i + 1][j + 1] = C[i][j] + 1;
                    }
                    else
                    {
                        C[i + 1][j + 1] = max(C[i + 1][j], C[i][j + 1]);
                    }
                }
            }
            return C;
        }

        template <typename T> void vGenerateDiff(size_t **C, const T & X, const T &Y, const size_t &i, const size_t &j, T & XOut, T &YOut)
        {
            if (i > 0 && j > 0 && X[i] == Y[j])
            {
                vGenerateDiff(C, X, Y, i - 1, j - 1, XOut, YOut);

                XOut += m_strColorTags[GREEN_COLOR_BEG]; // green color
                XOut += X[i];
                XOut += m_strColorTags[COLOR_END]; // default color

                YOut += m_strColorTags[GREEN_COLOR_BEG]; // green color
                YOut += Y[j];
                YOut += m_strColorTags[COLOR_END]; // default color
            }
            else if (j > 0 && (i == 0 || C[i][j - 1] >= C[i - 1][j]))
            {
                vGenerateDiff(C, X, Y, i, j - 1, XOut, YOut);

                if(Y[j] == ' ')
                {
                    YOut += m_strColorTags[RED_COLOR_REVERSE_BEG]; // reverse red color
                }
                else
                {
                    YOut += m_strColorTags[RED_COLOR_BEG]; // red color
                }
                YOut += Y[j];
                YOut += m_strColorTags[COLOR_END]; // default color
            }
            else if (i > 0 && (j == 0 || C[i][j - 1] <= C[i - 1][j]))
            {
                vGenerateDiff(C, X, Y, i - 1, j, XOut, YOut);
                XOut += X[i];
            }
        }

        struct SAssertResults
        {
            std::string strExpected;
            std::string strActual;
            std::string strLineNumber;
            std::string strFileName;
        };

        // this vector of pairs stores the expected and the actual results
        std::vector< SAssertResults > m_Asserts;

        typedef enum
        {
            RED_COLOR_BEG = 0,
            GREEN_COLOR_BEG,
            RED_COLOR_REVERSE_BEG,
            GREEN_COLOR_REVERSE_BEG,
            COLOR_END,
            NUMBER_OF_COLORS
        } EColorCodes;

        /// A string array to store color tags
        std::string m_strColorTags[NUMBER_OF_COLORS];
};

class TestFixture
{
    private:
        static std::ostringstream errmsg;
        static simple_testsuite_assert_results m_SCAssertResultRecorder;
        static size_t countTests;
        static size_t fails_counter;
        static size_t assert_counter;
        static size_t todos_counter;
        static size_t fails_counter_backup;

    public:
        std::string classname;
    protected:
        std::string testToRun;
        bool bIsActivated;
        size_t m_LengthOfLinePtr;
        std::string m_strCurrentClassName;
        static size_t m_assertCount;
        /// \brief stop on first error [default = false]
        bool m_bStopOnError;

        /// Generate a random number of type T
        /// \return <-- a random number
        template <typename T> T GetRandomNumber(void)
        {
#if _WIN32
            return  static_cast<int>(rand());
#else // POSIX
            // cppcheck-suppress obsoleteFunctionsrand_r
            return static_cast<T>(rand_r(&m_uiRandomSeed));
#endif
        }

        //virtual void prepare() = 0;
        virtual void run() = 0;
        //virtual void cleanup() = 0;

        bool runTest(const char testname[]);

        void vAssert(const char *filename, unsigned int linenr, bool condition);

        void assertEquals(const char * const filename, unsigned int linenr, const std::string &expected, const std::string &actual, const std::string &msg = "");

        /*! \brief Compare two values.
         * \param fileName      The filename, where the comparison belongs to.
         * \param lineNumber    The line number in the file.
         * \param expected      The expected value.
         * \param actual        The actual (computed) value.
         * \param base          The numerical base, where the comparison is taken. Allowed values are: std::dec, std::hex and std::oct.*/
        template <typename T> void assertEquals(const char * const fileName, unsigned int lineNumber, const T &expected, const T &actual, std::ios_base & (*base)(std::ios_base&) = std::dec)
        {
            if(expected != actual)
            {
                std::ostringstream ostr1;
                ostr1 << base << std::ios_base::dec << expected;
                std::ostringstream ostr2;
                ostr2 << base << std::ios_base::dec << actual;
                assertEquals(fileName, lineNumber, ostr1.str(), ostr2.str());
            }
            else
            {
                ++assert_counter;
            }
        }

        /*! \brief Compare two Boolean values.
         * \param fileName      The filename, where the comparison belongs to.
         * \param lineNumber    The line number in the file.
         * \param expected      The expected value.
         * \param actual        The actual (computed) value.
        */
        void assertEquals_bool(const char * const fileName, unsigned int lineNumber, const bool &expected, const bool &actual);
        // the vars expected and actual need to be of type double, in order to avoid overflow of unsigned int
        // e.g: ASSERT_EQUALS(-100.0, MathLib::toDoubleNumber("-1.0E+2")); would not work without this.
        void assertEquals(const char *filename, unsigned int linenr, double expected, double actual, const std::string &msg = "");
        void todoAssertEquals(const char * const filename, unsigned int linenr, const std::string &expected, const std::string &actual);
        void todoAssertEquals(const char * const filename, unsigned int linenr, unsigned int expected, unsigned int actual);
        void todoAssertEquals_bool(const char * const filename, const unsigned int &uiLineNr, const bool &expected, const bool &actual);
        void assertThrowFail(const char *const filename, unsigned int linenr);
        void vStopOnError(const bool &bStopOnError);

    public:

        void reportOut(const std::string &outmsg) const;
        //    virtual void reportErr(const ErrorLogger::ErrorMessage &msg);
        void reportStatus() const;
        static void printFinalReport(void);
        void run(const std::string &str);
        static std::string strWriteStr(const std::string &str);

        /*! \brief Execute an external tool (e.g. GCC) and capture its output.
         *
         * \param [in] toolName     The name of the tool.
         * \param [in] toolOptions  The tools options.
         * \param [in] fileNames    The file names, the tool needs as input. In case no file name is required, provide an empty string vector.
         * \param [out] capturedOutput      The output of the tool.
         * \param [in] captureFromStream 1 = stdout, 2 stderr or any other number for stdout and stderr
         *
         * \return False in case the tool cannot be executed or an internal error is produced.
         *         True in case the tool executed successfully.
         */
        static bool executeExternalTool(const std::string &toolName, const std::string &toolOptions, const std::vector<std::string> &fileNames, std::string &capturedOutput, size_t captureFromStream = 2);

        explicit TestFixture(const std::string &_name);
        virtual ~TestFixture(void);

        // ---------------------------------------
        /// Print the registered test to std::cout
        // ---------------------------------------
        static void printTests(void);

        // ---------------------------------------
        /// Get a list of registered tests
        /// \return <-- a list of names
        // ---------------------------------------
        static const std::list<std::string> strGetListOfTests(void);

        // -------------------------------------------------------------
        /// Exclude a specific test from the test chain.
        //
        /// \param strNameOfTest --> the name of the test to exclude
        // -------------------------------------------------------------
        static void vExcludeTest(const std::string &strNameOfTest);

        static size_t runTests(const char cmd[]);

        static bool bRevertOrder;

        static void vSetConfiguration(const simple_testsuite_settings &Settings);

        static simple_testsuite_settings m_Settings;
        /// \brief a variable to store the random seed
        unsigned int m_uiRandomSeed;
};

class TestRegistry
{
    private:
        std::list<TestFixture *> m_Tests;

        TestRegistry(void);

    public:

        static TestRegistry &theInstance(void);

        void addTest(TestFixture *t);

        void RemoveByName(const std::string &strName);

        void RevertListOrder(void);

        void RandomizeListOrder(void);

        const std::list<TestFixture *> &tests(void) const;
};

#define START_LISTEN_COUT \
    std::streambuf *m_pBackupCoutStream=NULL;\
    m_pBackupCoutStream = std::cout.rdbuf();\
    std::cout.rdbuf(output.rdbuf());\
     

#define END_LISTEN_COUT \
    std::cout.rdbuf(m_pBackupCoutStream);\
     

#define START_LISTEN_CERR \
    std::streambuf *m_pBackupCerrStream=NULL;\
    m_pBackupCerrStream = std::cerr.rdbuf();\
    std::cerr.rdbuf(output.rdbuf());\
     

#define END_LISTEN_CERR \
    std::cerr.rdbuf(m_pBackupCerrStream);\
     
#define START_LISTEN_ALL\
    START_LISTEN_COUT\
    START_LISTEN_CERR

#define END_LISTEN_ALL\
    END_LISTEN_COUT\
    END_LISTEN_CERR

#define CLEAR_COUT_STREAM {output.str("");}
#define CLEAR_CERR_STREAM {errout.str("");}

#define TEST_CASE( NAME )                           { if ( runTest(#NAME) ) {START_LISTEN_ALL  NAME(); END_LISTEN_ALL reportStatus();} }
#define ASSERT( CONDITION )                         vAssert(__FILE__, __LINE__, CONDITION)

#define ASSERT_EQUALS( EXPECTED , ACTUAL )          { assertEquals(__FILE__, __LINE__, EXPECTED, ACTUAL); ++m_assertCount; }
#define ASSERT_EQUALS_BOOL( EXPECTED , ACTUAL )     { assertEquals_bool(__FILE__, __LINE__, EXPECTED, ACTUAL); ++m_assertCount; }

// decimal
#define ASSERT_EQUALS_CHAR( EXPECTED , ACTUAL )            { assertEquals<char>(__FILE__, __LINE__, EXPECTED, ACTUAL, std::dec); ++m_assertCount; }
#define ASSERT_EQUALS_SIZE_T( EXPECTED , ACTUAL )            { assertEquals<size_t>(__FILE__, __LINE__, EXPECTED, ACTUAL, std::dec); ++m_assertCount; }
#define ASSERT_EQUALS_INT( EXPECTED , ACTUAL )              { assertEquals<int>(__FILE__, __LINE__, EXPECTED, ACTUAL, std::dec); ++m_assertCount; }
#define ASSERT_EQUALS_LONG_INT( EXPECTED , ACTUAL )          { assertEquals<long int>(__FILE__, __LINE__, EXPECTED, ACTUAL, std::dec); ++m_assertCount; }
#define ASSERT_EQUALS_LONG_LONG_INT( EXPECTED , ACTUAL )     { assertEquals<long long int>(__FILE__, __LINE__, EXPECTED, ACTUAL, std::dec); ++m_assertCount; }
#define ASSERT_EQUALS_INT8( EXPECTED , ACTUAL )              { assertEquals<int8_t>(__FILE__, __LINE__, EXPECTED, ACTUAL, std::dec); ++m_assertCount; }
#define ASSERT_EQUALS_INT16( EXPECTED , ACTUAL )             { assertEquals<int16_t>(__FILE__, __LINE__, EXPECTED, ACTUAL, std::dec); ++m_assertCount; }
#define ASSERT_EQUALS_INT32( EXPECTED , ACTUAL )             { assertEquals<int32_t>(__FILE__, __LINE__, EXPECTED, ACTUAL, std::dec); ++m_assertCount; }
#define ASSERT_EQUALS_INT64( EXPECTED , ACTUAL )             { assertEquals<int64_t>(__FILE__, __LINE__, EXPECTED, ACTUAL, std::dec); ++m_assertCount; }
#define ASSERT_EQUALS_UNSIGNED_LONG_INT( EXPECTED , ACTUAL )     { assertEquals<unsigned long int>(__FILE__, __LINE__, EXPECTED, ACTUAL, std::dec); ++m_assertCount; }
#define ASSERT_EQUALS_UNSIGNED_LONG_LONG_INT( EXPECTED , ACTUAL ){ assertEquals<unsigned long long int>(__FILE__, __LINE__, EXPECTED, ACTUAL, std::dec); ++m_assertCount; }
#define ASSERT_EQUALS_UINT8( EXPECTED , ACTUAL )             { assertEquals<uint8_t>(__FILE__, __LINE__, EXPECTED, ACTUAL, std::dec); ++m_assertCount; }
#define ASSERT_EQUALS_UINT16( EXPECTED , ACTUAL )            { assertEquals<uint16_t>(__FILE__, __LINE__, EXPECTED, ACTUAL, std::dec); ++m_assertCount; }
#define ASSERT_EQUALS_UINT32( EXPECTED , ACTUAL )            { assertEquals<uint32_t>(__FILE__, __LINE__, EXPECTED, ACTUAL, std::dec); ++m_assertCount; }
#define ASSERT_EQUALS_UINT64( EXPECTED , ACTUAL )            { assertEquals<uint64_t>(__FILE__, __LINE__, EXPECTED, ACTUAL, std::dec); ++m_assertCount; }
#define ASSERT_EQUALS_L( EXPECTED , ACTUAL )             { ASSERT_EQUALS_LONG_INT(EXPECTED , ACTUAL) }
#define ASSERT_EQUALS_LL( EXPECTED , ACTUAL )            { ASSERT_EQUALS_LONG_LONG_INT(EXPECTED , ACTUAL) }
#define ASSERT_EQUALS_UL( EXPECTED , ACTUAL )            { ASSERT_EQUALS_UNSIGNED_LONG_INT(EXPECTED , ACTUAL) }
#define ASSERT_EQUALS_ULL( EXPECTED , ACTUAL )           { ASSERT_EQUALS_UNSIGNED_LONG_LONG_INT(EXPECTED , ACTUAL) }

// octal
#define ASSERT_EQUALS_SIZE_T_OCT( EXPECTED , ACTUAL )   { assertEquals<size_t>(__FILE__, __LINE__, EXPECTED, ACTUAL, std::oct); ++m_assertCount; }
#define ASSERT_EQUALS_INT_OCT( EXPECTED , ACTUAL ) { assertEquals<int>(__FILE__, __LINE__, EXPECTED, ACTUAL, std::oct); ++m_assertCount; }
#define ASSERT_EQUALS_LONG_INT_OCT( EXPECTED , ACTUAL ) { assertEquals<long int>(__FILE__, __LINE__, EXPECTED, ACTUAL, std::oct); ++m_assertCount; }
#define ASSERT_EQUALS_LONG_LONG_INT_OCT( EXPECTED , ACTUAL ) { assertEquals<long long int>(__FILE__, __LINE__, EXPECTED, ACTUAL, std::oct); ++m_assertCount; }
#define ASSERT_EQUALS_INT8_OCT( EXPECTED , ACTUAL )     { assertEquals<int8_t>(__FILE__, __LINE__, EXPECTED, ACTUAL, std::oct); ++m_assertCount; }
#define ASSERT_EQUALS_INT16_OCT( EXPECTED , ACTUAL )    { assertEquals<int16_t>(__FILE__, __LINE__, EXPECTED, ACTUAL, std::oct); ++m_assertCount; }
#define ASSERT_EQUALS_INT32_OCT( EXPECTED , ACTUAL )    { assertEquals<int32_t>(__FILE__, __LINE__, EXPECTED, ACTUAL, std::oct); ++m_assertCount; }
#define ASSERT_EQUALS_INT64_OCT( EXPECTED , ACTUAL )    { assertEquals<int64_t>(__FILE__, __LINE__, EXPECTED, ACTUAL, std::oct); ++m_assertCount; }
#define ASSERT_EQUALS_UNSIGNED_LONG_INT_OCT( EXPECTED , ACTUAL ) { assertEquals<unsigned long int>(__FILE__, __LINE__, EXPECTED, ACTUAL, std::oct); ++m_assertCount; }
#define ASSERT_EQUALS_UNSIGNED_LONG_LONG_INT_OCT( EXPECTED , ACTUAL ) { assertEquals<unsigned long long int>(__FILE__, __LINE__, EXPECTED, ACTUAL, std::oct); ++m_assertCount; }
#define ASSERT_EQUALS_UINT8_OCT( EXPECTED , ACTUAL )    { assertEquals<uint8_t>(__FILE__, __LINE__, EXPECTED, ACTUAL, std::oct); ++m_assertCount; }
#define ASSERT_EQUALS_UINT16_OCT( EXPECTED , ACTUAL )   { assertEquals<uint16_t>(__FILE__, __LINE__, EXPECTED, ACTUAL, std::oct); ++m_assertCount; }
#define ASSERT_EQUALS_UINT32_OCT( EXPECTED , ACTUAL )   { assertEquals<uint32_t>(__FILE__, __LINE__, EXPECTED, ACTUAL, std::oct); ++m_assertCount; }
#define ASSERT_EQUALS_UINT64_OCT( EXPECTED , ACTUAL )   { assertEquals<uint64_t>(__FILE__, __LINE__, EXPECTED, ACTUAL, std::oct); ++m_assertCount; }
#define ASSERT_EQUALS_L_OCT(EXPECTED , ACTUAL)      { ASSERT_EQUALS_LONG_INT_OCT( EXPECTED , ACTUAL ) }
#define ASSERT_EQUALS_LL_OCT(EXPECTED , ACTUAL)     { ASSERT_EQUALS_LONG_LONG_INT_OCT( EXPECTED , ACTUAL ) }
#define ASSERT_EQUALS_UL_OCT(EXPECTED , ACTUAL)     { ASSERT_EQUALS_UNSIGNED_LONG_INT_OCT( EXPECTED , ACTUAL ) }
#define ASSERT_EQUALS_ULL_OCT(EXPECTED , ACTUAL)    { ASSERT_EQUALS_UNSIGNED_LONG_LONG_INT_OCT( EXPECTED , ACTUAL ) }

// hex
// FIXME: add macros for L, LL, UL, ULL
#define ASSERT_EQUALS_SIZE_T_HEX( EXPECTED , ACTUAL )   { assertEquals<size_t>(__FILE__, __LINE__, EXPECTED, ACTUAL, std::hex); ++m_assertCount; }
#define ASSERT_EQUALS_INT_HEX( EXPECTED , ACTUAL )      { assertEquals<int>(__FILE__, __LINE__, EXPECTED, ACTUAL, std::hex); ++m_assertCount; }
#define ASSERT_EQUALS_LONG_INT_HEX( EXPECTED , ACTUAL ) { assertEquals<long int>(__FILE__, __LINE__, EXPECTED, ACTUAL, std::hex); ++m_assertCount; }
#define ASSERT_EQUALS_INT8_HEX( EXPECTED , ACTUAL )     { assertEquals<int8_t>(__FILE__, __LINE__, EXPECTED, ACTUAL, std::hex); ++m_assertCount; }
#define ASSERT_EQUALS_INT16_HEX( EXPECTED , ACTUAL )    { assertEquals<int16_t>(__FILE__, __LINE__, EXPECTED, ACTUAL, std::hex); ++m_assertCount; }
#define ASSERT_EQUALS_INT32_HEX( EXPECTED , ACTUAL )    { assertEquals<int32_t>(__FILE__, __LINE__, EXPECTED, ACTUAL, std::hex); ++m_assertCount; }
#define ASSERT_EQUALS_INT64_HEX( EXPECTED , ACTUAL )    { assertEquals<int64_t>(__FILE__, __LINE__, EXPECTED, ACTUAL, std::hex); ++m_assertCount; }
#define ASSERT_EQUALS_UINT8_HEX( EXPECTED , ACTUAL )    { assertEquals<uint8_t>(__FILE__, __LINE__, EXPECTED, ACTUAL, std::hex); ++m_assertCount; }
#define ASSERT_EQUALS_UINT16_HEX( EXPECTED , ACTUAL )   { assertEquals<uint16_t>(__FILE__, __LINE__, EXPECTED, ACTUAL, std::hex); ++m_assertCount; }
#define ASSERT_EQUALS_UINT32_HEX( EXPECTED , ACTUAL )   { assertEquals<uint32_t>(__FILE__, __LINE__, EXPECTED, ACTUAL, std::hex); ++m_assertCount; }
#define ASSERT_EQUALS_UINT64_HEX( EXPECTED , ACTUAL )   { assertEquals<uint64_t>(__FILE__, __LINE__, EXPECTED, ACTUAL, std::hex); ++m_assertCount; }

#define ASSERT_EQUALS_MSG( EXPECTED , ACTUAL, MSG )  assertEquals(__FILE__, __LINE__, EXPECTED, ACTUAL, MSG)
#define ASSERT_THROW( CMD, EXCEPTION ) try { CMD ; assertThrowFail(__FILE__, __LINE__); } catch (EXCEPTION &) { } catch (...) { assertThrowFail(__FILE__, __LINE__); }
// FIXME: add todo-macros for different types
#define TODO_ASSERT_EQUALS( EXPECTED , ACTUAL ) todoAssertEquals(__FILE__, __LINE__, EXPECTED, ACTUAL)
#define TODO_ASSERT_EQUALS_BOOL( EXPECTED , ACTUAL )  todoAssertEquals_bool(__FILE__, __LINE__, EXPECTED, ACTUAL)
#define REGISTER_TEST( CLASSNAME ) namespace { CLASSNAME instance; }

/* FIXME extend by
TEST_ASSERT_EQUAL_INT(expected, actual)
TEST_ASSERT_EQUAL(expected, actual)
TEST_ASSERT_NOT_EQUAL(expected, actual)
TEST_ASSERT_EQUAL_UINT(expected, actual)
TEST_ASSERT_EQUAL_HEX(expected, actual)
TEST_ASSERT_EQUAL_HEX8(expected, actual)
TEST_ASSERT_EQUAL_HEX16(expected, actual)
TEST_ASSERT_EQUAL_HEX32(expected, actual)
TEST_ASSERT_EQUAL_HEX64(expected, actual)
TEST_ASSERT_BITS(mask, expected, actual)
TEST_ASSERT_BITS_HIGH(mask, actual)
TEST_ASSERT_BITS_LOW(mask, actual)
TEST_ASSERT_BIT_HIGH(bit, actual)
TEST_ASSERT_BIT_LOW(bit, actual)
*/

#ifndef __EXTERN_STREAMS__
#define __EXTERN_STREAMS__
extern std::ostringstream errout;
extern std::ostringstream output;
#endif //__EXTERN_STREAMS__


#endif // SIMPLE_TESTSUITE_H

