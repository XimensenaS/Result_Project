#include "ConverterJSON.h"
#include <fstream>
Converter_JSON::Converter_JSON(){};
 

void Converter_JSON::Read_Json_File(const string &fileName, nlohmann::json &jsonResult) {
    std::ifstream mFile(fileName);
    if (!mFile.is_open()) {
        throw ExceptionError("Unable to read file " + fileName);
    } else {
        try {
            mFile >> jsonResult;
        } catch (...) {
            throw ExceptionError("File " + fileName + " corrupted");
        }
    }
}

void Converter_JSON::Write_Json_File(const std::string &fileName, nlohmann::json &jsonResult) {
    std::ofstream mFile(fileName);
    if (!mFile.is_open()) {
        throw ExceptionError("Unable to create file " + fileName);
    }

    try {
        mFile << jsonResult.dump(4);
    } catch (...) {
        throw ExceptionError("Unable to write file " + fileName);
    }
}

nlohmann::json Converter_JSON::Creat_Answer_Json(const vector<vector<RelativeIndex>> &answers){
    nlohmann::json answer_json;

    int max_responses = Get_Responses_Limit();
    int number_request = 1;
    for (auto &request_answer:answers) {
        int response = 0;
        nlohmann::json relevance = nlohmann::json::array();
        for (auto item2 = request_answer.begin();
             item2 != request_answer.end() && response < max_responses; ++item2, ++response) {
            relevance.push_back({{"docID", item2->docId},
                                 {"rank",  item2->rank}});
        }
        string str_number_request =
                "request" + string(3 - to_string(number_request).size(), '0') + to_string(number_request);

        if (!request_answer.empty()) {
            answer_json["answers"][str_number_request]["result"] = true;
            answer_json["answers"][str_number_request]["relevance"] = relevance; } else {
            answer_json["answers"][str_number_request]["result"] = false; }
        number_request++;
    }
    return answer_json;
}

void Converter_JSON::Read_Config() {
    Read_Json_File(config_file_name, json_config);
    is_configured = true;
    if (!Is_Valid_Version()){
        is_configured = false;
        json_config.clear();
    }
}


void Converter_JSON::Put_Answers(const vector<vector<RelativeIndex>> &answers) {
    if (!is_configured) {
        Read_Config();
    }
    auto answers_json = Creat_Answer_Json(answers);
    Write_Json_File(answer_file_name, answers_json);
}


vector<string> Converter_JSON::Get_Requests() {
    nlohmann::json request_JSON;
    Read_Json_File(requests_file_Name, request_JSON);

    try {
    if (request_JSON["requests"].empty() || request_JSON.empty()) {
        cout << "WARNING!!: Request file empty" << endl;
        return vector<string>();
    }
    auto end_requests_list = request_JSON["requests"].end();
    if (request_JSON["requests"].size() > MAX_REQUESTS) {
        cout << "WARNING!!: To many requests" << endl;
        end_requests_list = request_JSON["requests"].begin() + MAX_REQUESTS;
    }
    return vector<string>(request_JSON["requests"].begin(), end_requests_list);
    } catch (...) {
    throw ExceptionError("Error config in " + config_file_name + " file.");
    }   
};

vector<string> Converter_JSON::Get_File_Names() {
    if (!is_configured) {
        Read_Config();
    }
    if (json_config.empty()) {
        cout << "WARNING!!: List of file names is empty" << endl;
        return vector<string>();
    }
       try {
         return vector<string>(json_config["files"].begin(), json_config["files"].end());
    } catch (...) {
        throw ExceptionError("Error config in " + config_file_name + " file.");
    }
}

int Converter_JSON::Get_Responses_Limit() {
    if (!is_configured) {
        Read_Config();
    }
    if (!json_config.empty()) {
        try {
            return json_config["config"]["max_responses"];
        } catch (...) {
            throw ExceptionError("Error config in " + config_file_name + " file.");
        }
    }else {
        return 0;
    }
}

string Converter_JSON::Get_Name_Of_App() {
    if (!is_configured) {
        Read_Config();
    }
    if (!json_config.empty()) {
        try {
        return json_config["config"]["name"];
    } catch (...) {
        throw ExceptionError("Error config in " + config_file_name + " file.");
		}
	}else{
	return string();
	}
}

string Converter_JSON::Get_Version_App() {
    if (!is_configured) {
        Read_Config();
    }
    if (!json_config.empty()) {
        try {
        return json_config["config"]["version"];
    } catch (...) {
        throw ExceptionError("Error config in " + config_file_name + " file.");
		}
	}else{
        return string();
	}
}

bool Converter_JSON::Is_Valid_Version() {
    if (!is_configured) {
        Read_Config();
    }
    return Get_Version_App() == VERSION_APP;
}

void Converter_JSON::Set_Config_File_Name(const string &fileName) {
    config_file_name = fileName;
    is_configured = false;
}

void Converter_JSON::Set_Requests_File_Name(const string &fileName) {
    requests_file_Name = fileName;
    is_configured = false;
}

void Converter_JSON::Set_Answer_File_Name(const string &fileName) {
    answer_file_name = fileName;
    is_configured = false;
}

int Converter_JSON::Get_Method_Of_Search() {
    if (!is_configured) {
        Read_Config();
    }
    int method_of_search;
    if (!json_config.empty()) {
        try {
        method_of_search = json_config["config"]["method_search"];
        if (method_of_search >= 1 && method_of_search <=3) {
            return method_of_search;
        }
        else {
             cout << "WARNING!!! Method of search must be from 1 to 3. Set 1." << endl;
             return 1;
        }
    } catch (...) {
        throw ExceptionError("Error config in " + config_file_name + " file.");
		}
	}else{
		return 0;	
	}
}
