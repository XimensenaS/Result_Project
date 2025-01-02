#pragma once

#include <algorithm>
#include "InvertedIndex.h"
#include "utilities.h"

struct RelativeIndex {
    size_t docId;
    double rank;

    bool operator==(const RelativeIndex &other) const {
        return (docId == other.docId && rank == other.rank);
    }

    RelativeIndex() = default;

    RelativeIndex(size_t docId, double rank) {
        this->rank = rank;
        this->docId = docId;
    }

    RelativeIndex(std::pair<size_t, double> __p) {
        this->rank = __p.second;
        this->docId = __p.first;
    }
};

class SearchServer {
private:
    Inverted_Index Index;

    int method_of_search = 1;
    int max_requests = 5;

    void Print_Result(const std::vector<std::vector<RelativeIndex>> &result);

public:

    /**Метод поиска документов соответствующих запросу
     * метод подразумевает наличие всех слов в документе имеющиеся в запросе
     * релевантность вычисляется подсчётом количества слов в документе,
     * сумма количества всех слов запроса, встречающихся в документе затем
     * нормируется.
     * В случае отсутствия одного из слов запроса в документе возвращается
     * пустой результат
     *
     * @param queryInput - строка запрос
     * @return вектор содержит документID и его релевантность данному запросу
     */
    std::vector<RelativeIndex> Search_By_Query_method2(const std::string &queryInput);

    /**Метод поиска документов соответствующих запросу
     * Метод подразумевает наличие хотя бы одного  слова в документе из запроса.
     * Релевантность вычисляется подсчётом количества слов в документе,
     * сумма количества всех слов запроса, встречающихся в документе затем
     * нормируется.
     * Пустой результат возвращается если нет ни одного документа содержащего
     * хотя бы одно слово из запроса
     *
     * @param queryInput - строка запрос
     * @return вектор содержит документID и его релевантность данному запросу
     */
    std::vector<RelativeIndex> Search_By_Query_method3(const std::string &query);

/** Конструктор
* @param idx в конструктор класса передаётся ссылка на класс
* InvertedIndex, чтобы SearchServer мог узнать частоту слов
* встречаемых в запросе
*/
    SearchServer(Inverted_Index &idx, int maxRequests = 5, int methodSearch = 1);

/** Метод обработки поисковых запросов
* @param queries_input поисковые запросы взятые из файла requests.json
* @return возвращает отсортированный список релевантных ответов для
* заданных запросов
*/
    std::vector<std::vector<RelativeIndex>> Search(const std::vector<std::string> &queriesInput);

/** Метод задания метода поиска
 * 1 -
 * 2 -
 * остальное игнорируется
 * @param mMethodOfSearch
 */
    void Set_Method_Of_Search(int mMethodOfSearch);

/** Метод задания максимального количества выдаваемых резуьтатов по зхапросу
 *
 * @param maxRequests
 */
    void Set_Max_Requests(int maxRequests);
};        
