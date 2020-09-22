#include <iostream>
#include "gtest/gtest.h"
#include "pch.h"

int main(int argc, char** argv) {
	// run all tests 
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();

}