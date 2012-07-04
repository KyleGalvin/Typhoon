/* Generated file, do not edit */

#ifndef CXXTEST_RUNNING
#define CXXTEST_RUNNING
#endif

#define _CXXTEST_HAVE_STD
#include <cxxtest/TestListener.h>
#include <cxxtest/TestTracker.h>
#include <cxxtest/TestRunner.h>
#include <cxxtest/RealDescriptions.h>
#include <cxxtest/TestMain.h>
#include <cxxtest/ErrorPrinter.h>

bool suite_TestSuiteSingleGrid_init = false;
#include "single_grid_test.ctt"

static TestSuiteSingleGrid suite_TestSuiteSingleGrid;

static CxxTest::List Tests_TestSuiteSingleGrid = { 0, 0 };
CxxTest::StaticSuiteDescription suiteDescription_TestSuiteSingleGrid( "single_grid_test.ctt", 7, "TestSuiteSingleGrid", suite_TestSuiteSingleGrid, Tests_TestSuiteSingleGrid );

static class TestDescription_suite_TestSuiteSingleGrid_testInitializeBoundingBox : public CxxTest::RealTestDescription {
public:
 TestDescription_suite_TestSuiteSingleGrid_testInitializeBoundingBox() : CxxTest::RealTestDescription( Tests_TestSuiteSingleGrid, suiteDescription_TestSuiteSingleGrid, 9, "testInitializeBoundingBox" ) {}
 void runTest() { suite_TestSuiteSingleGrid.testInitializeBoundingBox(); }
} testDescription_suite_TestSuiteSingleGrid_testInitializeBoundingBox;

static class TestDescription_suite_TestSuiteSingleGrid_testSinglegridIterator : public CxxTest::RealTestDescription {
public:
 TestDescription_suite_TestSuiteSingleGrid_testSinglegridIterator() : CxxTest::RealTestDescription( Tests_TestSuiteSingleGrid, suiteDescription_TestSuiteSingleGrid, 97, "testSinglegridIterator" ) {}
 void runTest() { suite_TestSuiteSingleGrid.testSinglegridIterator(); }
} testDescription_suite_TestSuiteSingleGrid_testSinglegridIterator;

