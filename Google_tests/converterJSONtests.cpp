#include "ConverterJSON.h"
#include "utilities.h"
#include "gtest/gtest.h"
#include <iostream>
using namespace std;

#define ASSERT_EXCEPTION(TRY_BLOCK, EXCEPTION_TYPE, MESSAGE)          \
try                                                                   \
{                                                                     \
    TRY_BLOCK                                                         \
    FAIL() << "exception '" << MESSAGE << "' not thrown at all!";     \
}                                                                     \
catch( const EXCEPTION_TYPE& e )                                      \
{                                                                     \
    EXPECT_EQ( MESSAGE, e.what() )                                    \
        << " exception message is incorrect. Expected the following " \
           "message:\n"                                               \
        << MESSAGE << "\n"                                            \
        << "Presetn: \n"<< e.what() <<"\n";                           \
}                                                                     \
catch( ... )                                                          \
{                                                                     \
    FAIL() << "exception '" << MESSAGE                                \
           << "' not thrown with expected type '" << #EXCEPTION_TYPE  \
           << "'!";                                                   \
}

Converter_JSON converter_json;
string file_name_test = "test.json";

nlohmann::json config_file = {
        {"config",
                {{"max_responses", 15},
                                            {"method_search", 1},
                        {"name", "TestAppName"},
                        {"version", "0.1"}}},
        {"files",
                {"resource\\file00001.txt", "resource\\file00002.txt",
                        "resource\\file00003.txt"}}};

void Save_Test_Json_File(const string &fileName,const nlohmann::json &jFileContent) {
    fstream file(fileName, ios::out);
    if (file.is_open()) {
        file << jFileContent.dump(4);
        file.close();
    } else {
        cerr << " Test error: file not created" << endl;
    }
}

TEST(Converter, Get_Name_Of_App) {
    Save_Test_Json_File(file_name_test, config_file);
    converter_json.Set_Config_File_Name(file_name_test);
    EXPECT_EQ("TestAppName", converter_json.Get_Name_Of_App());
    remove(file_name_test.c_str());
}

TEST(Converter, Get_Version_App) {
    Save_Test_Json_File(file_name_test, config_file);
    converter_json.Set_Config_File_Name(file_name_test);
    EXPECT_EQ("0.1", converter_json.Get_Version_App());
    remove(file_name_test.c_str());
}

TEST(Converter, Get_Responses_Limit) {
    Save_Test_Json_File(file_name_test, config_file);
    converter_json.Set_Config_File_Name(file_name_test);
    EXPECT_EQ(15, converter_json.Get_Responses_Limit());
    remove(file_name_test.c_str());
}

TEST(Сonverter, Is_Valid_Version_valid) {
    config_file["config"]["version"] = VERSION_APP;
    Save_Test_Json_File(file_name_test, config_file);
    converter_json.Set_Config_File_Name(file_name_test);
    bool res = converter_json.Is_Valid_Version();
    EXPECT_EQ(true, res);
    remove(file_name_test.c_str());
}

TEST(converter, Is_Valid_Version_invalid) {
    config_file["config"]["version"] = "100.200";
    Save_Test_Json_File(file_name_test, config_file);
    config_file["config"]["version"] = VERSION_APP;
    converter_json.Set_Config_File_Name(file_name_test);
    bool res = converter_json.Is_Valid_Version();
    EXPECT_EQ(false, res);
    remove(file_name_test.c_str());
}

TEST(Сonverter, Get_Requests_OK) {
    nlohmann::json request_file;
    vector<string> expected_files;
    const int REQUEST_COUNT = 100; 

    for (int c = 0; c < REQUEST_COUNT; ++c) {
        expected_files.push_back(" a "); 
    }
    request_file["requests"] = nlohmann::json(expected_files);

    Save_Test_Json_File(file_name_test, request_file);
    converter_json.Set_Requests_File_Name(file_name_test);

    EXPECT_EQ(expected_files, converter_json.Get_Requests());
    remove(file_name_test.c_str());
}

TEST(Сonverter, Get_Requests_Max_Count) {
    nlohmann::json request_file;
    vector<string> expected_files;
    const int REQUEST_COUNT = 1010; 

    for (int c = 0; c < REQUEST_COUNT; ++c) {
        expected_files.push_back(" a "); 
    }
    request_file["requests"] = nlohmann::json(expected_files);

    Save_Test_Json_File(file_name_test, request_file);

    converter_json.Set_Requests_File_Name(file_name_test);
    expected_files.resize(1000);
    EXPECT_EQ(expected_files, converter_json.Get_Requests());
    EXPECT_EQ(converter_json.Get_Requests().size(), 1000);
    remove(file_name_test.c_str());
}

TEST(Сonverter, Get_Requests_empty_file_or_error) {
    const nlohmann::json request_file = {};
    const vector<string> expected_file_fathes = {};

    fstream file(file_name_test, ios::out);
    if (file.is_open()) {
        file.close();
    } else {
        cerr << " Test error: file not created" << endl;
    }
    converter_json.Set_Requests_File_Name(file_name_test);
    ASSERT_EXCEPTION({ converter_json.Get_Requests(); }, ExceptionError,
                     "Error: File " + file_name_test + " corrupted")
    remove(file_name_test.c_str());
}

TEST(Сonverter, Get_Requests_missingFile) {
    const string file_name_t = "missingFileName.json";
    converter_json.Set_Requests_File_Name(file_name_t);
    ASSERT_EXCEPTION({ converter_json.Get_Requests(); }, ExceptionError,
                     "Error: Unable to read file " + file_name_t);
    remove(file_name_test.c_str());
}

TEST(Сonverter, Put_Answers) {
    vector<vector<RelativeIndex>> answers = {
            {RelativeIndex(0, 1), RelativeIndex(1, 0.9), RelativeIndex(2, 0.8),
                    RelativeIndex(3, 0.7)},
            {RelativeIndex(0, 1), RelativeIndex(1, 0.8), RelativeIndex(2, 0.9),
                    RelativeIndex(3, 0.9)},
            {RelativeIndex(0, 1), RelativeIndex(1, 0.8), RelativeIndex(2, 0.9),
                    RelativeIndex(3, 0.9)},
            {}

    };
    Save_Test_Json_File("testConfig.json", config_file);
    converter_json.Set_Config_File_Name("testConfig.json");
    converter_json.Set_Answer_File_Name("testAnswerFile.json");
    converter_json.Put_Answers(answers);

    nlohmann::json read_json_answer;
    fstream file("testAnswerFile.json", ios::in);
    if (file.is_open()) {
        file >> read_json_answer;
    } else {
        cerr << "Unable to read file" << endl;
    }

    ASSERT_EQ(read_json_answer.size(), 1);
    ASSERT_EQ("answers", read_json_answer.items().begin().key());
    ASSERT_EQ("request001", read_json_answer["answers"].begin().key());
    ASSERT_EQ("relevance", read_json_answer["answers"]["request001"].begin().key());
    ASSERT_EQ(
            "docID",
            read_json_answer["answers"]["request001"]["relevance"][0].begin().key());
    ASSERT_EQ(read_json_answer["answers"].size(), answers.size());
    vector<bool> expected_result, fact_read_result;
    expected_result = {1, 1, 1, 0};
    for (const auto &item : read_json_answer["answers"]) {
        fact_read_result.push_back(item["result"]);
    }
    ASSERT_EQ(expected_result, fact_read_result);
    int numberRequest = 1;
    auto it = answers.begin();
    for (auto item = read_json_answer["answers"].begin();
         item != read_json_answer["answers"].end(); ++item, ++numberRequest) {
        ASSERT_EQ("request" +
                  string(3 - to_string(numberRequest).size(), '0') +
                  to_string(numberRequest),
                  item.key());
    }
    file.close();
    remove("testConfig.json");
    remove("testAnswerFile.json");
}

TEST(Сonverter, Get_File_Names) {
    Save_Test_Json_File(file_name_test, config_file);
    converter_json.Set_Config_File_Name(file_name_test);
    vector<string> expected = {"resource\\file00001.txt",
                               "resource\\file00002.txt",
                                "resource\\file00003.txt"};
    EXPECT_EQ(expected, converter_json.Get_File_Names());
    remove(file_name_test.c_str());
}