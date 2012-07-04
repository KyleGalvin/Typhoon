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

bool suite_TestSuiteHierarchalGrid_init = false;
#include "hierarchal_grid_test.ctt"

static TestSuiteHierarchalGrid suite_TestSuiteHierarchalGrid;

static CxxTest::List Tests_TestSuiteHierarchalGrid = { 0, 0 };
CxxTest::StaticSuiteDescription suiteDescription_TestSuiteHierarchalGrid( "hierarchal_grid_test.ctt", 7, "TestSuiteHierarchalGrid", suite_TestSuiteHierarchalGrid, Tests_TestSuiteHierarchalGrid );

static class TestDescription_suite_TestSuiteHierarchalGrid_testInitializeHierarchalBoundingBox : public CxxTest::RealTestDescription {
public:
 TestDescription_suite_TestSuiteHierarchalGrid_testInitializeHierarchalBoundingBox() : CxxTest::RealTestDescription( Tests_TestSuiteHierarchalGrid, suiteDescription_TestSuiteHierarchalGrid, 9, "testInitializeHierarchalBoundingBox" ) {}
 void runTest() { suite_TestSuiteHierarchalGrid.testInitializeHierarchalBoundingBox(); }
} testDescription_suite_TestSuiteHierarchalGrid_testInitializeHierarchalBoundingBox;

