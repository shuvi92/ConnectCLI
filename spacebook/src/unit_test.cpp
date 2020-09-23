#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE "user Unit_test"
#include <boost/test/unit_test.hpp>
#include "user.h"
#include <iostream>
#include "post.h"

//Unit tests for User class implementation
User unit_test;

//test case for getter function
BOOST_AUTO_TEST_CASE(getter_test)
{
        unit_test.set_fname("Ashish");
        unit_test.set_lname("Karma");
        unit_test.set_age(25);
        
	std::string res;
        std::string res1;
	int res2;

        res = unit_test.get_fname();
        res1 = unit_test.get_lname();
	res2 = unit_test.get_age();


        BOOST_CHECK_EQUAL(res,"Ashish");
	BOOST_CHECK_EQUAL(res1,"Karma");
	BOOST_CHECK_EQUAL(res2,25);
}

//test case for set sn
BOOST_AUTO_TEST_CASE(setter_test_sn)
{

        Post setter;
        int sn = 1;
        setter.set_sn(sn);

        BOOST_CHECK_EQUAL(sn, setter.get_sn());

}

//test cases for setter functions
BOOST_AUTO_TEST_CASE(setter_test1)
{

        User setter;
        std::string _first = "Ashish";
        setter.set_fname(_first);

	BOOST_CHECK_EQUAL(_first, setter.get_fname());

}
	
	
BOOST_AUTO_TEST_CASE(setter_test2)
{

        User setter;
        std::string _last = "Karma";
        setter.set_lname(_last);

        BOOST_CHECK_EQUAL(_last, setter.get_lname());

}

BOOST_AUTO_TEST_CASE(setter_test4)
{

        User setter;
        int age = 19;
        setter.set_age(age);

        BOOST_CHECK_EQUAL(age, setter.get_age());

}

//test case for intersest counter
BOOST_AUTO_TEST_CASE(counter_test2)
{
        User unit_test;
	int num;
        num = unit_test.get_interest_count();
        BOOST_CHECK(num != -1);
}

//test case for adding interest
BOOST_AUTO_TEST_CASE(interest_test1)
{
        User unit_test;
	std::string test_int = "Swimming";
	std::string test_int1 = "Singing";
        
	unit_test.add_interest(test_int);
	unit_test.add_interest(test_int1);
        
	int index = 1;
        BOOST_CHECK(!unit_test.get_interest(index).empty());
        BOOST_ERROR("Add interest failed");
}