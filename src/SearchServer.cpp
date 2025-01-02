
#include "SearchServer.h"

SearchServer::SearchServer(Inverted_Index &idx, int maxRequests, int methodSearch) : Index(idx) {
    Set_Method_Of_Search(methodSearch);
    Set_Max_Requests(maxRequests);
};

vector<RelativeIndex> SearchServer::Search_By_Query_method2(const string &queryInput) {
    vector<pair<string, int>> count_words_of_query;
    for (const auto &queryWord:Split_Into_Words(queryInput)) {
        count_words_of_query.push_back(pair(queryWord, Index.Get_Word_Count_All_Docs(queryWord)));}
    sort(count_words_of_query.begin(), count_words_of_query.end(),[](const auto &a, const auto &b) {
                  return a.second < b.second;}
        );
    vector<Entry> find_docs = Index.Get_Word_Count(count_words_of_query[0].first); 
    if (find_docs.empty()) {
        return vector<RelativeIndex>();
        }
    for (auto it = count_words_of_query.begin() + 1; it != count_words_of_query.end(); ++it) {
        vector<Entry> nextDocs = Index.Get_Word_Count(it->first); 
        auto docIdWordIt = find_docs.begin();
        size_t i = 0;
        while (docIdWordIt != find_docs.end()) {
            auto it = find_if(nextDocs.begin(), nextDocs.end(), [docIdWordIt](const Entry &a) {
                return a.doc_Id == docIdWordIt->doc_Id;
            });
            if (it == nextDocs.end()) {
                docIdWordIt = find_docs.erase(docIdWordIt);
            } else {
                docIdWordIt->count += it->count;
                docIdWordIt++;
            }
        }
        if (find_docs.empty()) return vector<RelativeIndex>();
    }

    vector<RelativeIndex> result;
    multimap<size_t, size_t, greater<size_t>> sort_result;
    for (const auto &[docID, rank]:find_docs) {
        sort_result.insert({rank, docID});
        if (sort_result.size() > max_requests) {
            sort_result.erase(prev(sort_result.end()));
        }
    }
    size_t maxRank = sort_result.begin()->first;
    for (const auto &[rank, docId]:sort_result) {
        result.push_back(RelativeIndex(docId, round(static_cast<double>(rank) / maxRank * 1000) / 1000));
    }
    return result;
};

vector<RelativeIndex> SearchServer::Search_By_Query_method3(const string &query) {
    map<size_t, size_t> docIdFreqWord; 
    vector<RelativeIndex> result;
    for (const auto &queryWord: Split_Into_Words(query)) {
        for (auto &entry:Index.Get_Word_Count(queryWord)) {
            docIdFreqWord[entry.doc_Id] += entry.count;
        }
    }
    if (docIdFreqWord.empty()) return vector<RelativeIndex>(); 

    multimap<size_t, size_t, greater<size_t>> sort_result;
    for (const auto &[docID, rank]:docIdFreqWord) {
        sort_result.insert({rank, docID});
        if (sort_result.size() > max_requests) {
            sort_result.erase(prev(sort_result.end()));
        }
    }
    size_t maxRank = sort_result.begin()->first;
    for (const auto &[rank, docId]:sort_result) {
        result.push_back(RelativeIndex(docId, round(static_cast<double>(rank) / maxRank * 1000) / 1000));
    }
    return result;
}

vector<vector<RelativeIndex>> SearchServer::Search(const vector<string> &queriesInput) {
    vector<vector<RelativeIndex>> result;

    typedef vector<RelativeIndex>(SearchServer::*func_ptr)(const string &);
    func_ptr current_ptr;

    switch (method_of_search) {
        case 1: {
            current_ptr = &SearchServer::Search_By_Query_method2;
            break;
        }
        case 2: {
            current_ptr = &SearchServer::Search_By_Query_method3;
            break;
        }
    }
    for (const auto &query:queriesInput) {
        result.push_back((this->*current_ptr)(query));
    }
    return result;
};

void SearchServer::Print_Result(const vector<vector<RelativeIndex>> &result) {
    for (auto &el_vector:result) {
        cout << "{" << endl;
        for (auto &rel_index:el_vector) {
            cout << "{ " << rel_index.docId << ", " << rel_index.rank << " }," << endl;
        }
        cout << "}" << endl;
    }
}

void SearchServer::Set_Method_Of_Search(int mMethodOfSearch) {
    if (mMethodOfSearch < 1 || mMethodOfSearch > 2) {
        cerr << "WARNING! Search method must from 1 to 2 " << endl;
        cout << "Search method set 1" << endl;
        SearchServer::method_of_search = 1;
    } else {
        SearchServer::method_of_search = mMethodOfSearch;
    }
}

void SearchServer::Set_Max_Requests(int maxRequests) {
    if (maxRequests <= 0) {
        cerr << "WARNING! Max request bust be greater 0" << endl;
        cout << "Max request set 5" << endl;
        SearchServer::max_requests = 5;
    } else {
        SearchServer::max_requests = maxRequests;
    }
}