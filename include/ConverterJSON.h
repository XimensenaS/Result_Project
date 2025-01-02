#pragma once
#include <string>
#include "nlohmann/json.hpp"
#include <fstream>
#include <iostream>
#include "config.h"
#include "SearchServer.h"
#include "utilities.h"
using namespace std;

class Converter_JSON {
private:
    bool is_configured = false;
    string answer_file_name = defaultAnswerFileName;
    string config_file_name = defaultConfigFileName;
    string requests_file_Name = defaultRequestsFileName;

    nlohmann::json json_config;

    void Read_Json_File(const string &fileName, nlohmann::json &jsonResult);
    void Write_Json_File(const string &fileName, nlohmann::json &jsonToWrite);

    void Read_Config();

    nlohmann::json Creat_Answer_Json(const vector<vector<RelativeIndex>> &answers);

public:
Converter_JSON();
    ~Converter_JSON() = default;

    vector<string> Get_Requests();

/** Метода возвращает вектор имён файлов для индексаци
 *
 * @return - вектор строк имен файлов
 */
    vector<string> Get_File_Names();

/**
* Метод считывает поле mMaxResponses для определения предельного
* количества ответов на один запрос
* @return максимальное количество результатов поиска max_responses
*/
    int Get_Responses_Limit();

/** Метод считывает поле name
* @return строка с наименованием программного продукта
*/
    string Get_Name_Of_App();

/** Метод считывает поле version
* @return строка-версия файла config.json
*
*/
    string Get_Version_App();

/** Метод проверяет соответсвыие версии ПО и версии JSON файла config
 * @return true если версия файла config.json свпадает с версией программы
 */
    bool Is_Valid_Version();

/** Положить в файл answers.json результаты поисковых запросов
*/
    void Put_Answers(const vector<vector<pair<size_t, double>>> &answers);

/** Положить в файл answers.json результаты поисковых запросов
*/
    void Put_Answers(const vector<vector<RelativeIndex>> &answers);

    void Set_Config_File_Name(const string &fileName);

    void Set_Requests_File_Name(const string &fileName);

    void Set_Answer_File_Name(const string &fileName);

    int Get_Method_Of_Search();

    void Set_Method_Of_Search(int methodOfSearch);

    void Set_Max_Responses(int newMaxResponses);
};