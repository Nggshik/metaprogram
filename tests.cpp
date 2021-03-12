#define BOOST_TEST_MODULE metaprogram

#include <boost/test/unit_test.hpp>
#include <vector>
#include "printer.hpp"

BOOST_AUTO_TEST_SUITE(metaprogram_tests)

BOOST_AUTO_TEST_CASE(simple_test)
{
    std::vector<int> vc;
    int a;
    BOOST_CHECK(is_stl_container_like<decltype(vc)>::value == true);
    BOOST_CHECK(is_stl_container_like<decltype(a)>::value == false);
}

BOOST_AUTO_TEST_SUITE_END()