//
// Created by luiz0tavio on 5/7/18.
//

#ifndef SIMPLEX_FILE_HPP
#define SIMPLEX_FILE_HPP


#include <iosfwd>
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <regex>

class File {

public:

    File() = delete;

    static std::vector<std::string> GetFileData(std::string path);

    static std::vector<std::string> GetStdInData();

    static void WriteOnFile(std::string path, std::string data);

    static std::vector<long long> GetIntsFromStringFile(std::string data_string);

private:

    static std::vector<std::string> readStream(std::istream& input_stream);

};


#endif //SIMPLEX_FILE_HPP
