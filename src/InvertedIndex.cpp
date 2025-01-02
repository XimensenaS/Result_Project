#include "InvertedIndex.h"

template<typename T1, typename T2>
void func_merge(map<T1, vector<T2>> &map1, const map<T1, vector<T2>> &map2) {
    for (const auto&[key, vect]:map2) {
        map1[key].insert(map1[key].cend(), vect.begin(), vect.end());
    }
}

string Inverted_Index::Read_Document(const string &fileName) {
    string curr_word, curr_text_document;
    int counter_words = 0;
    int counter_char = 0;
    bool is_typing = false;
    fstream file;
    file.open(fileName, ios::in);
    if (!file.is_open()) {
        cout << "Unable to open file " << fileName << endl;
    } else {
        file.seekg(0, ios::end);
        streampos length_file = file.tellg();
        file.seekg(0, ios::beg);
        vector<char> file_buff(length_file);
        file.read(&file_buff[0], length_file);
        file.close();

        for (auto ptrc = file_buff.begin(); ptrc != file_buff.end(); ++ptrc) {
            if ((*ptrc >= 'A' && *ptrc <= 'Z') || (*ptrc >= 'a' && *ptrc <= 'z')) {
                if (++counter_char <= max_char_in_word) {
                     curr_text_document +=tolower(*ptrc);
                } else if (!is_typing) {
                    cerr << "WARNING: count of character  in word " << fileName << " greater then "
                              << max_char_in_word << endl;
                    is_typing = true;
                }
            }

            if (counter_words > max_word_in_document) {
                cerr << "WARNING: count of word in document " << fileName << " greater then "
                          << max_word_in_document << endl;
                break;
            }

            if (*ptrc == ' ') {
                if ((ptrc + 1) != file_buff.end() && *(ptrc + 1) != ' ') {
                    curr_text_document +=' ';
                    counter_words++;
                    counter_char = 0;
                    is_typing = false;
                }
            }
        }
    }
    return curr_text_document;
}
void Inverted_Index::To_Index_Doc(const size_t &doc_id, const string &docFileName) {
    map<string, vector<Entry>> mini_freq_dictionary;
    vector<string> words = Split_Into_Words(Read_Document(docFileName));
    for (const string &word: words) {
        if (!mini_freq_dictionary.count(word)) { 
            mini_freq_dictionary[word].push_back({doc_id, 1}); 
        } else {
            if (mini_freq_dictionary[word].back().doc_Id ==
                doc_id) { 
                mini_freq_dictionary[word].back().count++;  
            } else {
                mini_freq_dictionary[word].push_back(
                        {doc_id, 1}); 
            }
        }
    }
    if (lock_dictionary != nullptr) { 
        lock_dictionary->lock(); 
        all_doc_length_in_word += words.size();
        doc_length[doc_id] = words.size();
        func_merge(freq_dictionary, mini_freq_dictionary); 
        lock_dictionary->unlock();
    } else {
        all_doc_length_in_word += words.size();
        doc_length[doc_id] = words.size();
        func_merge(freq_dictionary, mini_freq_dictionary); 
    }
}


void Inverted_Index::Update_Document_Base(const vector<string> &fileNames, const int &maxThreads) {
    freq_dictionary.clear();
	lock_dictionary = new mutex;
    if (fileNames.empty()) {
        cerr << "WARNING: List of filenames is empty." << endl;
        return;
    }

    synced_stream sync_out;
    thread_pool pool;
    pool.reset(maxThreads);

    all_doc_length_in_word = 0;
    size_t docId = 0;
    for (const auto &fileName:fileNames) {
        pool.submit([this, docId, fileName] {
            To_Index_Doc(docId, fileName);
        });

        docId++;
    }

    pool.wait_for_tasks();

    delete lock_dictionary;
    lock_dictionary = nullptr;

    Calcalate_AVGDL();
    Calculate_IDF();
}

vector<Entry> Inverted_Index::Get_Word_Count(const string &word) {
    if (freq_dictionary.count(word)) {
        return freq_dictionary.at(word);
    } else {
        return vector<Entry>({});
    }
}

size_t Inverted_Index::Get_Word_Count_All_Docs(const string &word) {

    if (freq_dictionary.count(word)) { 
        int result = 0;
        for (const auto &entry_array:freq_dictionary.at(word)) {
            result += entry_array.count;
        }
        return result; 
    } else {
        return 0; 
    }

}

void Inverted_Index::Calculate_IDF() {
    size_t N = Get_Doc_Count();
    for (const auto &docFreq:freq_dictionary) {
        string current_word = docFreq.first;
        size_t nqi = docFreq.second.size(); 
        double _IDF = log(1.0 * N / nqi);
        IDF.insert(pair(current_word, _IDF));
    }
}

void Inverted_Index::Calcalate_AVGDL() {
    AVGDL = 1.f * all_doc_length_in_word / Get_Doc_Count();
}

double Inverted_Index::Get_AVGDL() {
    return AVGDL;
};

double Inverted_Index::Get_IDF(const string &word) {
    if (IDF.count(word)) {
        return IDF.at(word);
    } else {
        return 0;
    }
}

size_t Inverted_Index::Get_Doc_Length(const size_t &docID) {
    if (doc_length.count(docID)) {
        return doc_length.at(docID);
    } else {
        return 0;
    }
}

size_t Inverted_Index::Get_Doc_Count() {
    return doc_length.size(); 
}

int Inverted_Index::Get_Word_Count_In_Doc(const string &word, size_t docId) {
    for (const auto &entry:Get_Word_Count(word)) {
        if (entry.doc_Id == docId) return entry.count;
    }
    return 0;
}

void Inverted_Index::Set_Max_Word_In_Document(int _MaxWordInDocument) {
    if (_MaxWordInDocument <= 0) {
        cerr << "WARNING! Maximum word in document must greater then 0" << endl;
        cout << "Set maximum word in document is 1000" << endl;
        max_word_in_document = 1000;
    }
    max_word_in_document = _MaxWordInDocument;
};

void Inverted_Index::Set_Max_Char_In_Word(int _MaxCharInWord) {
    if (_MaxCharInWord <= 0) {
        cerr << "WARNING! Maximum character in word must greater then 0" << endl;
        cout << "Set maximum character in word is 100" << endl;
        max_word_in_document = 1000;
    }
    max_word_in_document = _MaxCharInWord;
};
