#ifndef _COMPLEX_TESTS
#define _COMPLEX_TESTS
#include "CO.h"
#include "gtest/gtest.h"
#include "intersection.cpp"
#include "functions.cpp"

using namespace std;

TEST (ComplexTest, FirstTest)
{


}

TEST (IntersectionTest, FieldTest)
{
    point path1; 
    path1.log = 38.280044;
    path1.lat = -76.416956;

    point bound1;
    bound1.log = 38.291657;
    bound1.lat = -76.418119;

    point path2;
    path2.log = 38.296265;
    path2.lat = -76.393505;

    point bound2;
    bound2.log = 38.279237;
    bound2.lat = -76.397041;

    EXPECT_TRUE(intersection(path1, path2, bound1, bound2));

}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}

#endif