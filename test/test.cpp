/**@author $username$ <$usermail$>
 * @date $date$
 *
 * @brief skyscrappers test launcher.*/

// Google Testing Framework
#include <gtest/gtest.h>

// test cases

int main(int argc, char *argv[])
{
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}


