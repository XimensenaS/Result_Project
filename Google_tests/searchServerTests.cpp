#include "InvertedIndex.h"
#include "utilities.h"
#include "SearchServer.h"
#include "gtest/gtest.h"

using namespace std;
#define PRINT_RESULT
#define PRINT_STRING_RESULT

string Generate_File_Name(const string &fileName, int numberFile) {
    return fileName +
           std::string(3 - std::to_string(numberFile).size(), '0') + std::to_string(numberFile) + ".txt";
}

void Print_Result(std::vector<vector<RelativeIndex>> result) {
    for (auto &vec:result) {
        std::cout << "{" << std::endl;
        for (auto &rlidx:vec) {
            std::cout << "{" << rlidx.docId << ", " << rlidx.rank << "}," << std::endl;
        }
        std::cout << "}," << std::endl;
    }
}

void Print_Result(std::vector<vector<RelativeIndex>> result, const vector<string> &requests, const vector<string> &Docs) {
    int request = 0;
    for (auto &vec:result) {
        std::cout << "Request: \"" << requests[request] << "\":" << std::endl;
        for (auto &rlidx:vec) {
            std::cout << rlidx.docId << ": \t\"" << Docs[rlidx.docId] << "\"; " << rlidx.rank << std::endl;
        }
        request++;
    }
}

void Test_Search_Server_Functionality(
        const vector<string> &Docs,
        const vector<string> &requests,
        const std::vector<vector<RelativeIndex>> &expected,
        int methodSearch,
        int maxRequests) {
    std::vector<vector<RelativeIndex>> result;
    std::vector<std::string> file_names({});
    Inverted_Index idx;

    std::fstream file;
    int number_file = 1;
    for (const auto &oneDoc:Docs) {
        string file_name = Generate_File_Name("testfile", number_file);
        file_names.push_back(file_name);
        file.open(file_name, std::ios::out);
        file << oneDoc;
        file.close();
        number_file++;
    }
    idx.Update_Document_Base(file_names, 2);

    for (const auto &file_name:file_names) {
        std::remove(file_name.c_str());
    }

    SearchServer srv(idx);
    srv.Set_Method_Of_Search(methodSearch);
    srv.Set_Max_Requests(maxRequests);
    result = srv.Search(requests);


#ifdef PRINT_RESULT
    Print_Result(result);
#endif
#ifdef PRINT_STRING_RESULT
    Print_Result(result,requests,Docs);
#endif

    ASSERT_EQ(result, expected);
};

TEST(TestCaseSearchServer, Method2_Test_EmptyQuery) {
    const vector<string> docs = {
            "milk milk milk milk milk water water water",
            "milk water water",
            "milk milk milk milk milk water "
    };
    const std::vector<vector<RelativeIndex>> expected = {};
    const vector<string> request = {};
    Test_Search_Server_Functionality(docs, request, expected, 2, docs.size());
}

TEST(TestCaseSearchServer, Method2_Test_MissingQuery) {
    const vector<string> docs = {
            "milk milk milk milk milk water water water",
            "milk water water",
            "milk milk milk milk milk water "
    };
    const vector<string> request = {"back"};
    const std::vector<vector<RelativeIndex>> expected = {{}};
    Test_Search_Server_Functionality(docs, request, expected, 2, docs.size());
}

TEST(TestCaseSearchServer, Method2_TestSimple) {
    const vector<string> docs = {
            "milk milk milk milk milk milk water water water", 
            "milk water water",                           
            "milk milk milk milk milk water water water water water", 
            "Americano Cappuccino", 
            "americano milk", 
            "cappuccino water", 
            "water", 
            "milk", 
            "milk water sugar", 
            "milk water milk water" 
    };
    const vector<string> request = {"milk water", "sugar", "milk water sugar"};
    const std::vector<vector<RelativeIndex>> expected = {
            {
                    {2, 1},
                    {0, 0.9},
                    {9, 0.4},
                    {1, 0.3},
                    {8, 0.2},
            },
            {
                    {8, 1}
            },
            {
                    {8, 1},
            }
    };
    Test_Search_Server_Functionality(docs, request, expected, 2, docs.size());
}

TEST(TestCaseSearchServer, Method2_TestResultCut) {
    const vector<string> docs = {
            "milk milk milk milk milk milk water water water", 
            "milk water water",                           
            "milk milk milk milk milk water water water water water", 
            "Americano Cappuccino", 
            "americano milk", 
            "cappuccino water", 
            "water", 
            "milk", 
            "milk water sugar", 
            "milk water milk water" 
    };
    const vector<string> request = {"milk water", "sugar", "milk water sugar"};
    const std::vector<vector<RelativeIndex>> expected = {
            {
                    {2, 1},
                    {0, 0.9},
                    {9, 0.4},
            },
            {
                    {8, 1}
            },
            {
                    {8, 1},
            }
    };
    Test_Search_Server_Functionality(docs, request, expected, 2, 3);
}


TEST(TestCaseSearchServer, Method2_TestRsult0) {
    const vector<string> docs = {
            "milk milk milk milk milk milk water water water", 
            "milk water water",                           
            "milk milk milk milk milk water water water water water", 
            "Americano Cappuccino",
            "americano milk", 
            "cappuccino water", 
            "water", 
            "milk", 
            "milk water sugar", 
            "milk water milk water" 
    };
    const vector<string> request = {"milk water", "sugar", "milk water sugar"};
    const std::vector<vector<RelativeIndex>> expected = {
            {
                    {2, 1},
                    {0, 0.9},
                    {9, 0.4},
                    {1, 0.3},
                    {8, 0.2},
            },
            {
                    {8, 1}
            },
            {
                    {8, 1},
            }
    };
    Test_Search_Server_Functionality(docs, request, expected, 2, 0);
}


TEST(TestCaseSearchServer, Method2_TestTop5) {
    const vector<string> docs = {
            "london is the capital of great britain",
            "paris is the capital of france",
            "berlin is the capital of germany",
            "rome is the capital of italy",
            "madrid is the capital of spain",
            "lisboa is the capital of portugal",
            "bern is the capital of switzerland",
            "moscow is the capital of russia",
            "kiev is the capital of ukraine",
            "minsk is the capital of belarus",
            "astana is the capital of kazakhstan",
            "beijing is the capital of china",
            "tokyo is the capital of japan",
            "bangkok is the capital of thailand",
            "welcome to moscow the capital of russia the third rome",
            "amsterdam is the capital of netherlands",
            "helsinki is the capital of finland",
            "oslo is the capital of norway",
            "stockholm is the capital of sweden",
            "riga is the capital of latvia",
            "tallinn is the capital of estonia",
            "warsaw is the capital of poland",
            "london is the capital of great britain",
            "paris is the capital of france",
            "berlin is the capital of germany",
            "rome is the capital of italy",
            "madrid is the capital of spain",
            "lisboa is the capital of portugal",
            "bern is the capital of switzerland",
            "moscow is the capital of russia",
            "kiev is the capital of ukraine",
            "minsk is the capital of belarus",
            "astana is the capital of kazakhstan",
            "beijing is the capital of china",
            "tokyo is the capital of japan",
            "bangkok is the capital of thailand",
            "welcome to moscow the capital of russia the third rome",
            "amsterdam is the capital of netherlands",
            "helsinki is the capital of finland",
            "oslo is the capital of norway",
            "stockholm is the capital of sweden",
            "riga is the capital of latvia",
            "tallinn is the capital of estonia",
            "warsaw is the capital of poland",
            "london is the capital of great britain",
            "paris is the capital of france",
            "berlin is the capital of germany",
            "rome is the capital of italy",
            "madrid is the capital of spain",
            "lisboa is the capital of portugal",
            "bern is the capital of switzerland",
            "moscow is the capital of russia",
            "kiev is the capital of ukraine",
            "minsk is the capital of belarus",
            "astana is the capital of kazakhstan",
            "beijing is the capital of china",
            "tokyo is the capital of japan",
            "bangkok is the capital of thailand",
            "welcome to moscow the capital of russia the third rome",
            "amsterdam is the capital of netherlands",
            "helsinki is the capital of finland",
            "oslo is the capital of norway",
            "stockholm is the capital of sweden",
            "riga is the capital of latvia",
            "tallinn is the capital of estonia",
            "warsaw is the capital of poland",
            "london is the capital of great britain",
            "paris is the capital of france",
            "berlin is the capital of germany",
            "rome is the capital of italy",
            "madrid is the capital of spain",
            "lisboa is the capital of portugal",
            "bern is the capital of switzerland",
            "moscow is the capital of russia",
            "kiev is the capital of ukraine",
            "minsk is the capital of belarus",
            "astana is the capital of kazakhstan",
            "beijing is the capital of china",
            "tokyo is the capital of japan",
            "bangkok is the capital of thailand",
            "welcome to moscow the capital of russia the third rome",
            "amsterdam is the capital of netherlands",
            "helsinki is the capital of finland",
            "oslo is the capital of norway",
            "stockholm is the capital of sweden",
            "riga is the capital of latvia",
            "tallinn is the capital of estonia",
            "warsaw is the capital of poland"

    };
    const vector<string> request = {"moscow is the capital of russia"};
    const std::vector<vector<RelativeIndex>> expected = {
            {
                    {7, 1},
                    {29, 1},
                    {51, 1},
                    {73, 1}
            }
    };
    Test_Search_Server_Functionality(docs, request, expected, 2, docs.size());
}

TEST(TestCaseSearchServer, Method3_Test_EmptyQuery) {
    const vector<string> docs = {
            "milk milk milk milk milk water water water",
            "milk water water",
            "milk milk milk milk milk water "
    };
    const std::vector<vector<RelativeIndex>> expected = {};
    const vector<string> request = {};
    Test_Search_Server_Functionality(docs, request, expected, 3, docs.size());
}

TEST(TestCaseSearchServer, Method3_Test_MissingQuery) {
    const vector<string> docs = {
            "milk milk milk milk milk water water water",
            "milk water water",
            "milk milk milk milk milk water "
    };
    const vector<string> request = {"back"};
    const std::vector<vector<RelativeIndex>> expected = {{}};
    Test_Search_Server_Functionality(docs, request, expected, 3, docs.size());
}

TEST(TestCaseSearchServer, Method3_TestSimple) {
    const vector<string> docs = {
            "milk milk milk milk milk milk water water water", 
            "milk water water",                           
            "milk milk milk milk milk water water water water water", 
            "Americano Cappuccino", 
            "americano milk", 
            "cappuccino water", 
            "water", 
            "milk", 
            "milk water sugar", 
            "milk water milk water" 
    };
    const vector<string> request = {"milk water", "sugar", "milk water sugar"};
    const std::vector<vector<RelativeIndex>> expected = {
            {
                    {2, 1},
                    {0, 0.9},
                    {9, 0.4},
                    {1, 0.3},
                    {8, 0.2},
                    {4, 0.1},
                    {5, 0.1},
                    {6, 0.1},
                    {7, 0.1},
            },
            {
                    {8, 1}
            },
            {
                    {2, 1},
                    {0, 0.9},
                    {9, 0.4},
                    {1, 0.3},
                    {8, 0.3},
                    {4, 0.1},
                    {5, 0.1},
                    {6, 0.1},
                    {7, 0.1},
            }
    };
    Test_Search_Server_Functionality(docs, request, expected, 3, docs.size());
}

TEST(TestCaseSearchServer, Method3_TestResultCut) {
    const vector<string> docs = {
            "milk milk milk milk milk milk water water water", 
            "milk water water",                           
            "milk milk milk milk milk water water water water water", 
            "Americano Cappuccino", 
            "americano milk", 
            "cappuccino water", 
            "water", 
            "milk", 
            "milk water sugar", 
            "milk water milk water" 
    };
    const vector<string> request = {"milk water", "sugar", "milk water sugar"};
    const std::vector<vector<RelativeIndex>> expected = {
            {
                    {2, 1},
                    {0, 0.9},
                    {9, 0.4}
            },
            {
                    {8, 1}
            },
            {
                    {2, 1},
                    {0, 0.9},
                    {9, 0.4}
            }
    };
    Test_Search_Server_Functionality(docs, request, expected, 3, 3);
}


TEST(TestCaseSearchServer, Method3_TestRsult0) {
    const vector<string> docs = {
            "milk milk milk milk milk milk water water water", 
            "milk water water",                           
            "milk milk milk milk milk water water water water water", 
            "Americano Cappuccino", 
            "americano milk", 
            "cappuccino water", 
            "water", 
            "milk", 
            "milk water sugar", 
            "milk water milk water" 
    };
    const vector<string> request = {"milk water", "sugar", "milk water sugar"};
    const std::vector<vector<RelativeIndex>> expected = {
            {
                    {2, 1},
                    {0, 0.9},
                    {9, 0.4},
                    {1, 0.3},
                    {8, 0.2}
            },
            {
                    {8, 1}
            },
            {
                    {2, 1},
                    {0, 0.9},
                    {9, 0.4},
                    {1, 0.3},
                    {8, 0.3}
            }
    };
    Test_Search_Server_Functionality(docs, request, expected, 3, 0);
}


TEST(TestCaseSearchServer, Method3_TestTop5) {
    const vector<string> docs = {
            "london is the capital of great britain",
            "paris is the capital of france",
            "berlin is the capital of germany",
            "rome is the capital of italy",
            "madrid is the capital of spain",
            "lisboa is the capital of portugal",
            "bern is the capital of switzerland",
            "moscow is the capital of russia",
            "kiev is the capital of ukraine",
            "minsk is the capital of belarus",
            "astana is the capital of kazakhstan",
            "beijing is the capital of china",
            "tokyo is the capital of japan",
            "bangkok is the capital of thailand",
            "welcome to moscow the capital of russia the third rome",
            "amsterdam is the capital of netherlands",
            "helsinki is the capital of finland",
            "oslo is the capital of norway",
            "stockholm is the capital of sweden",
            "riga is the capital of latvia",
            "tallinn is the capital of estonia",
            "warsaw is the capital of poland"
    };
    const vector<string> request = {"moscow is the capital of russia"};
    const std::vector<vector<RelativeIndex>> expected = {
            {
                    {7, 1},
                    {14, 1},
                    {0, 0.667},
                    {1, 0.667},
                    {2, 0.667}
            }
    };
    Test_Search_Server_Functionality(docs, request, expected, 3, 5);
}