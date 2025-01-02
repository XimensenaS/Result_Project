#include <iostream>
#include <string>
#include "gtest/gtest.h"
#include <fstream>
#include "InvertedIndex.h"
#include "utilities.h"

#define MULTI_THREAD
#define MAX_THREAD 4

using namespace std;

string GenerateFileName(const string &fileName, int numberFile) {
    return fileName +
           string(3 - to_string(numberFile).size(), '0') + to_string(numberFile) + ".txt";
}

void Test_Inverted_Index_Functionality(
        const vector<string> &Docs,
        const vector<string> &requests,
        const vector<vector<Entry>> &expected) {
    vector<vector<Entry>> result;
    vector<string> file_names({});
    Inverted_Index idx;

    fstream file;
    int number_file = 1;
    for (const auto &oneDoc:Docs) {
        string file_name = GenerateFileName("testfile", number_file);
        file_names.push_back(file_name);
        file.open(file_name, ios::out);
        file << oneDoc;
        file.close();
        number_file++;
    }

#ifdef MULTI_THREAD
    idx.Update_Document_Base(file_names, MAX_THREAD);
#else
    idx.Update_Document_Base(file_names);
#endif

    for (const auto &fileName:file_names) {
        remove(fileName.c_str());
    }

    for (auto &request : requests) {
        vector<Entry> word_count = idx.Get_Word_Count(request);
		
		sort(word_count.begin(), word_count.end(),
              [](const auto &a, const auto &b) {
                  return a.doc_Id < b.doc_Id;
              });
		
        result.push_back(word_count);
    }

    ASSERT_EQ(result, expected);
};

TEST(TestCaseInvertedIndex, TestBasic) {
    const vector<string> docs = {
            "london is the capital of great britain",
            "big ben is the nickname for the Great bell of the striking clock",
    };
    const vector<string> requests = {"london", "the"};
    const vector<vector<Entry>> expected = {
            {
                    {0, 1}
            },
            {
                    {0, 1}, {1, 3}
            }
    };
    Test_Inverted_Index_Functionality(docs, requests, expected);
}

TEST(TestCaseInvertedIndex, TestBasic2) {
    const vector<string> docs = {
            "milk milk milk milk water water water",
            "milk water water",
            "milk milk milk milk milk water water water water water",
            "americano cappuccino"
    };
    const vector<string> requests = {"milk", "water", "cappuccino"};
    const vector<vector<Entry>> expected = {
            {
                    {0, 4}, {1, 1}, {2, 5}
            },
            {
                    {0, 3}, {1, 2}, {2, 5}
            },
            {
                    {3, 1}
            }
    };
    Test_Inverted_Index_Functionality(docs, requests, expected);

}

TEST(TestCaseInvertedIndex, TestMissingWordInvertedIndex) {
    const vector<string> docs = {
            "a b c d e f g h i j k l",
            "statement"
    };
    const vector<string> requests = {"m", "statement", "z"};
    vector<vector<Entry>> expected = {
            {
            },
            {
                    {1, 1}
            },
            {
            }
    };

    Test_Inverted_Index_Functionality(docs, requests, expected);
}

TEST(TestCaseInvertedIndex, TestInvertedIndexFile) {
    const vector<string> docs = {
            "robert a heinlein the number of the beast for walter and marion minton i it is better to marry than to burn saul of tarsus hes a mad scientist and im his beautiful daughter thats what she said the oldest clich in pulp fiction she wasnt old enough to remember the pulps the thing to do with a silly remark is to fail to hear it i went on waltzing while taking another look down her evening formal nice view not foam rubber she waltzed well today most girls who even attempt ballroom dancing drape themselves around your neck and expect you to shove them around the floor she kept her weight on her own feet danced close without snuggling and knew what i was going to do a split second before i led it a perfect partneras long as she didnt talk well she persisted my paternal grandfatheran unsavory old reactionary the femlibbers would have lynched himused to say zebadiah the mistake we made was not in putting shoes on them or in teaching them to readwe should never have taught them to talk i signaled a twirl by pressure she floated into it and back into my arms right on the beat i inspected her hands and the outer corners of her eyes yes she really was youngminimum eighteen hilda corners never permitted legal infants at her parties maximum twentyfive first approximation twentytwo yet she danced like her grandmothers generation well she repeated more firmly this time i openly stared is that cantilevering natural or is there an invisible bra you being in fact the sole support of two dependents she glanced down looked up and grinned they do stick out dont they your comment is rude crude unrefined and designed to change the subject what subject i made a polite inquiry you parried it with amphigory amphigory my tired feet i answered precisely amphigory i repeated the operative symbols were mad scientist beautiful and daughter the first has several meaningsthe others denote opinions semantic content zero she looked thoughtful rather than angry pop isnt rabid although i did use mad in ambivalent mode scientist and beautiful each contain descriptive opinions i stipulate but are you in doubt as to my sex if so are you qualified to check my twentythird chromosome pair with transsexual surgery so common i assume that anything less would not satisfy you i prefer a field test on the dance floor no the bushes back of the pool yes im qualifiedlaboratory or field but it was not your sex that lay in the area of opinion that is a fact that can be establishedlthough the gross evidence is convincing i ninetyfive centimeters isnt gross not for my height one hundred seventy barefooted one eighty in these heels its just that im waspwaisted for my massfortyeight centimeters versus fiftynine kilos and your teeth are your own and you dont have dandruff take it easy deedee i didnt mean to shake your aplombor those twin glands that are not gross but delicious i have an infantile bias and have known it since i was sixsix months that is but the symbol daughter encompasses two statements one factualsexand the other a matter of opinion even when stated by a forensic genetohematologist gosh what big words you know mister i mean doctor mister is correct on this campus it is swank to assume that everyone holds a doctorate even i have one phd do you know what that stands for doesnt everybody i have a phd too piled higher and deeper i raised that maximum to twentysix and assigned it as second approximation phys ed mister doctor you are trying to get my goat wont work i had an undergraduate double major one being phys ed with teachers credentials in case i needed a job but my real major was mathwhich i continued in graduate school and here i had been assuming that deedee meant doctor of divinity go wash out your mouth with soap my nickname is my initialsdee tee or deety doctor d t burroughs if being formal as i cant be mister and refuse to be miz or miss see here mister im supposed to be luring you with my radiant beauty then hooking you with my feminine charm and not getting anywhere lets try another tack tell me what you piled higher and deeper let me think flycasting or was it basketweaving it was one of those transdisciplinary things in which the committee simply weighs the dissertation tell you what ive got a copy around my digs ill find it and see what title the researcher who wrote it put on it dont bother the title is some implications of a sixdimensional nonnewtonian continuum pop wants to discuss it i stopped waltzing huh hed better discuss that paper with the bloke who wrote it nonsense i saw you blinkive hooked you pop wants to discuss it then offer you a job job i just slipped off the hook oh dear pop will be really mad please please sir you said that you had used mad in ambivalent mode how oh madangry because his colleagues wont listen to him madpsychotic in the opinions of some colleagues they say his papers dont make sense do they make sense im not that good a mathematician sir my work is usually simplifying software childs play compared with ndimensional spaces i wasnt required to express an opinion the trio started blue tango deety melted into my arms you dont talk if you know tango deety knew after an eternity of sensual bliss i swung her out into position precisely on coda she answered my bow and scrape with a deep curtsy thank you sir whew after a tango like that the couple ought to get married all right ill find our hostess and tell pop five minutes front door or side she looked serenely happy i said deety do you mean what you appear to mean that you "
    };
    const vector<string> requests = {"number", "marion", "is",
                                     "what", "scientist", "to",
                                     "saul", "it", "and", "ti"};
    const vector<vector<Entry>> expected = {
            {
                    {{0, 1}}, {{0, 1}}, {{0, 13}},
                    {{0, 9}}, {{0, 3}}, {{0, 26}},
                    {{0, 1}}, {{0, 18}}, {{0, 22}}, {}
            }
    };

    Test_Inverted_Index_Functionality(docs, requests, expected);
}