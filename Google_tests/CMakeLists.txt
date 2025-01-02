project(Google_tests)

include_directories(../include
                   ../src
)

add_executable(Google_Tests_All_Test
        limits_test.cpp
        invertedIndexTests.cpp
        ../src/InvertedIndex.cpp

        searchServerTests.cpp
        ../src/SearchServer.cpp

        converterJSONtests.cpp
        ../src/ConverterJSON.cpp
        ../src/utilities.cpp
         )

target_link_libraries(Google_Tests_All_Test gtest gtest_main)
target_link_libraries(Google_Tests_All_Test nlohmann_json::nlohmann_json)

add_executable(GTests_Limits_Test limits_test.cpp)
target_link_libraries(GTests_Limits_Test gtest gtest_main)

add_executable(GTests_InvertedIndex_Test        
        invertedIndexTests.cpp
        ../src/InvertedIndex.cpp
        ../src/utilities.cpp
)
target_link_libraries(GTests_InvertedIndex_Test gtest gtest_main)


add_executable(GTests_SearchServer_Test         
        searchServerTests.cpp
        ../src/SearchServer.cpp
        ../src/utilities.cpp
        ../src/InvertedIndex.cpp
        
)
target_link_libraries(GTests_SearchServer_Test gtest gtest_main)


add_executable(GTests_ConverterJSON_Test        
        converterJSONtests.cpp
        ../src/ConverterJSON.cpp
        ../src/utilities.cpp
)

target_link_libraries(GTests_ConverterJSON_Test gtest gtest_main)