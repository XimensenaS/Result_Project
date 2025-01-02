#include "gtest/gtest.h"
#include "utilities.h"

using namespace std;

void testPrivateMethod(const string &strInput, const vector<string> &expected) {
    ASSERT_EQ(Split_Into_Words(strInput), expected);
};

TEST(test_Split, SplitIntoWord_oneSpace){
    const string sInput = "a b c d e f";
    const vector<string> s_expected = {"a", "b", "c", "d", "e", "f"};
    testPrivateMethod(sInput, s_expected);
}

TEST(test_Split, SplitIntoWord_empty){
    const string sInput = "";
    const vector<string> s_expected = {};
    testPrivateMethod(sInput, s_expected);
}

TEST(test_Split, SplitIntoWord_spaceStart){
    const string sInput = " a b c d e f";
    const vector<string> s_expected = {"a", "b", "c", "d", "e", "f"};
    testPrivateMethod(sInput, s_expected);
}

TEST(test_Split, SplitIntoWord_spaceEnd){
    const string sInput = "a b c d e f ";
    const vector<string> s_expected = {"a", "b", "c", "d", "e", "f"};
    testPrivateMethod(sInput, s_expected);
}

TEST(test_Split, SplitIntoWord_TwoTreeSpaces){
    const string sInput = "a  b   c      d      e     f";
    const vector<string> s_expected = {"a", "b", "c", "d", "e", "f"};
    testPrivateMethod(sInput, s_expected);
}

TEST(test_Split, SplitIntoWord_TwoStartSpaces){
    const string sInput = "  a b c d e f";
    const vector<string> s_expected = {"a", "b", "c", "d", "e", "f"};
    testPrivateMethod(sInput, s_expected);
}

TEST(test_Split, SplitIntoWord_TreeStartSpaces){
    const string sInput = "   a b c d e f";
    const vector<string> s_expected = {"a", "b", "c", "d", "e", "f"};
    testPrivateMethod(sInput, s_expected);
}

TEST(test_Split, SplitIntoWord_TwoEndtSpaces){
    const string sInput = "a b c d e f  ";
    const vector<string> s_expected = {"a", "b", "c", "d", "e", "f"};
    testPrivateMethod(sInput, s_expected);
}

TEST(test_Split, SplitIntoWord_TreeEndSpaces){
    const string sInput = "a b c d e f   ";
    const vector<string> s_expected = {"a", "b", "c", "d", "e", "f"};
    testPrivateMethod(sInput, s_expected);
}