#include <iostream>
#include "ConverterJSON.h"
#include "InvertedIndex.h"
#include "SearchServer.h"

int main()
{
    Converter_JSON converter_json;
    Inverted_Index inverted_index;

    try {
        if (!converter_json.Is_Valid_Version()) {
            cerr << "Version application not equeal" << endl;
            cout << "Exit with error." << endl;
        }

        cout << "Start " << converter_json.Get_Name_Of_App() << endl;
        cout << "Version: " << converter_json.Get_Version_App() << endl;
        cout << "Max request limit: " << converter_json.Get_Responses_Limit() << endl;
        cout << "Method search: " << converter_json.Get_Method_Of_Search() << endl;

        inverted_index.Update_Document_Base(converter_json.Get_File_Names(), 2);
    } catch (ExceptionError &e) {
        cerr << e.what() << endl;
        exit(0);
    }

    SearchServer search_server(inverted_index);

    search_server.Set_Method_Of_Search(converter_json.Get_Method_Of_Search());
    search_server.Set_Max_Requests(converter_json.Get_Responses_Limit());
    auto queries = converter_json.Get_Requests();
    auto index = search_server.Search(queries);
    converter_json.Put_Answers(index);
}
