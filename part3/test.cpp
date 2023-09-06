//============================================================================
// Name        : Test.cpp
// Author      : W. Craig Scratchley and Mohammad Akbari
// Description : Course Project Part 3 (Tree) - Testing Scheme
// Copyright   : Copyright (c) 2023 School of Engineering Science
//============================================================================

#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MAIN
#define BOOST_TEST_MODULE Course Project Part 3
#include <boost/test/unit_test.hpp>
#include <boost/algorithm/string.hpp>
using namespace boost;

#include <sstream>
#include <fstream>
#include <iostream>
#include <string>

#include "makeDirectoryTree.hpp"
#include "FileSystem.hpp"

#define DONT_RUN_MAIN

using namespace std;

void check(std::string outputFile)
{
	ifstream fin(outputFile);
	stringstream sstr(OUT.str());
	cout << OUT.str();
	if (fin.is_open()) {
		std::string strDesired;
		std::string line_str;
		while ( getline (fin,strDesired) && getline(sstr, line_str) )
		{
			trim(line_str); trim(strDesired);
			if(strDesired != line_str)
			  BOOST_ERROR(line_str + "   !=   " + strDesired);
		}
		while (getline(fin,strDesired))
			BOOST_ERROR("Extra line expected: " + strDesired);
		fin.close();
		while (getline(sstr, line_str))
			BOOST_ERROR("Extra line output: " + line_str);
	}
	else {
		// report some sort of error, perhaps on cout
	}
	OUT.str(""); // Clear the stream for the next test case!
}

BOOST_AUTO_TEST_CASE(test1) {
	std::cout << "***** test1 *****" << endl;
	{
		auto root_dir1P{make_directory_tree("./testDirectory")};  // function to make directory tree
		if (root_dir1P) {
			root_dir1P->print_traverse();
		}
	}
	check("test1.txt");
}

BOOST_AUTO_TEST_CASE(test2) {
	std::cout << "***** test2 *****" << endl;
	{
		auto root_dir1P{make_directory_tree("./testDirectory/Notes")};  // function to make directory tree
		if (root_dir1P) {
			root_dir1P->print_traverse();
		}
	}
	check("test2.txt");
}

// copy construction
BOOST_AUTO_TEST_CASE(test3) {
	std::cout << "***** test3 *****" << endl;
	{
		auto root_dir1P{make_directory_tree("./testDirectory")};  // function to make directory tree

		if (root_dir1P) {
			auto root_dir2{Directory(*root_dir1P)}; // copy construction
			root_dir2.print_traverse();
		}
	}
	check("test3.txt");
}

// overloaded copy assignment operation
BOOST_AUTO_TEST_CASE(test4) {
	std::cout << "***** test4 *****" << endl;
	{
		auto root_dir1P{make_directory_tree("./testDirectory")};  // function to make directory tree
		if (root_dir1P) {
			auto root_dir2{Directory(*root_dir1P)}; // copy construction
			auto root_dir3P{make_directory_tree("./testDirectory/Lectures")};
			if (root_dir3P) {
				root_dir2 = *root_dir3P; // overloaded copy assignment operation
			}
			root_dir2.print_traverse();
		}
		root_dir1P->count_traverse();
		OUT << "Count:  " << root_dir1P->get_count() << endl;
	}
	check("test4.txt");
}

BOOST_AUTO_TEST_CASE(test5) {
	std::cout << "***** test5 *****" << endl;

	Directory dirTest("test");

	dirTest.set_name("test2");
	if(dirTest.get_name()!="test2")
		BOOST_ERROR("Failed in (set_dir_name)!");
}
