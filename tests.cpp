#define BOOST_TEST_MODULE metaprogram

#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_SUITE(metaprogram_tests)

BOOST_AUTO_TEST_CASE(simple_test)
{
    BOOST_CHECK(1 == 1);
}

BOOST_AUTO_TEST_SUITE_END()