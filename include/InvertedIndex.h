#pragma once

#include <iostream>
#include <vector>
#include <set>
#include <map>
#include <thread>
#include <mutex>
#include <cmath>
#include <fstream>
#include "thread_pool.hpp"
#include "utilities.h"
using namespace std;

struct Entry {
    size_t doc_Id, count;

    bool operator==(const Entry &other) const {
        return (doc_Id == other.doc_Id &&
                count == other.count);
    }

    Entry() = default;

    Entry(size_t docID, size_t Count) : doc_Id(docID), count(Count) {};
};

class Inverted_Index {
private:
    mutex *lock_dictionary = nullptr;
    map<string, vector<Entry>> freq_dictionary; 
    map<string, double> IDF;
    map<size_t, int> doc_length;

    size_t all_doc_length_in_word = 0;
    double AVGDL;

    int max_word_in_document = 1000;
    int max_char_in_word = 100;

    void Calculate_IDF();

    void Calcalate_AVGDL();

/**
 * Метод частоты встречания слова в документе
 * @param doc_id - идентификатор документа
 * @param docFileName - строка-текст документа
 */
    void To_Index_Doc(const size_t &doc_id, const string &docFileName);

/** Чтение документа из файла
 * 
 * @param fileName - имя файла (полный путь к файлу)
 */

    string Read_Document(const string &fileName);

public:
    Inverted_Index() = default;

/**
 * Метод для обновить или заполнить базу документов, по которой будем совершать поиск
 * @param fileNames - вектор строк-имён файлов для индексации
 */
    void Update_Document_Base(const vector<string> &fileNames);

/**
 * Метод выполняет обновление или заполнение базы документов, по которой будем совершать поиск.
 * Выполняется в несколькоих потоках
 * @param fileNames - вектор строк-имён файлов для индексации
 * @param maxThreads - число, количество создаваемых потоков
 */
    void Update_Document_Base(const vector<string> &inputDocs, const int &maxThreads);

/**
 * Метод определяет количество вхождений слова word в загруженной базе документов
 * @param word слово, частоту вхождений которого необходимо определить
 * @return возвращает подготовленный список с частотой слов
 */
    vector<Entry> Get_Word_Count(const string &word);

/**
 * Метод определяет количество вхождений слова word в документ doc_Id
 * @param word - слово количество вхождение которого определяем
 * @param doc_Id  - номер документа в котоом определяем количество вхождений слова word
 * @return количество вхождение слова word  в документ doc_Id
 */
    int Get_Word_Count_In_Doc(const string &word, size_t doc_Id);

/**
 * Метод определяет количество вхождений слова word в загруженной базе документов
 * @param word слово, количество вхождений которого необходимо определить
 * @return число количество вхождений солова во всех документах
 */
    size_t Get_Word_Count_All_Docs(const string &word);

/**
 * Метод возвращает среднюю длинну документа в списке поиска в словах
 * @return средняя длинна документа
 */
    double Get_AVGDL();

/**
 * Метод возвращает inverse document frequency IDF, заданного слова
 * @param word - слово для которого вычислялась IDF
 * @param doc_Id - документ для которого вычисляется IDF
 * @return IDF
 */
    double Get_IDF(const string &word);

/**
 * Метод возвращает длину лдокумена в словах
 * @param docID - номер документа длинну которого узнаём
 * @return длинна документа в словах
 */
    size_t Get_Doc_Length(const size_t &docID);

/**
 * Метод возвращает количество документов в базе
 * @return количество документов в базе
 */
    size_t Get_Doc_Count();

/**
 * Метод возвращает содержанеи документа
 * @param doc_Id - номер документа
 * @return содержание документа
 */
    string Get_Doc(size_t doc_Id);

/** Мкетод задания уставки максимального количества обрабатываемых слов в документе
 *
 * @param _MaxWordInDocument - максимальное количество слов в документе
 */
    void Set_Max_Word_In_Document(int _MaxWordInDocument);

/** Мкетод возвращает текущее количество обрабатываемых слов в документе
 *
 * @return - установленное максимальное количество слов в документе
 */

    int Get_Max_Word_In_Document() { return max_word_in_document; };

/** Метод задания уставки максимального количества символов в слове
 * @param _MaxCharInWord - - максимальное количество символов в слове
 */
    void Set_Max_Char_In_Word(int _MaxCharInWord);

/** Метод возвращает установленное количество символов в слове
* @return - установленное максимальное количество символов в слове
*/

    int Get_Max_Char_In_Word() { return max_char_in_word; };

};