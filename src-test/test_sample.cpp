// -------------------------------------------------
/// A class to unit test [project name]
/// @author Dr. Martin Ettl
/// @date   2013-06-20
// -------------------------------------------------

#include <string>

#include "simple_testsuite.hpp"

class TestSample : public TestFixture
{
    public:

        TestSample(void) : TestFixture("TestSample")
        { }

    private:

        void run(void)
        {
            TEST_CASE(SampleFunction)
        }

        void SampleFunction(void)
        {
            // ASSERT_EQUALS(0, CallOfFunctionToTest());
        }
};

REGISTER_TEST(TestSample)
