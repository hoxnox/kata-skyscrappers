/**@author hoxnox <hoxnox@gmail.com>
 * @date 20170905 06:37:16
 *
 * @brief skyscrappers test launcher.*/

// Google Testing Framework
#include <gtest/gtest.h>
#include "t_SkyScrappers.hpp"

int main(int argc, char *argv[])
{
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}


