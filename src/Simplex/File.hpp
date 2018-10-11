#ifndef SIMPLEX_FILE_HPP
#define SIMPLEX_FILE_HPP

#include <iosfwd>
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <regex>
#include "Data.hpp"

namespace Simplex
{
    class File
    {

    public:

        File() = delete;

        static TableauxInput GetTableauxInputFromFile(std::string&& file_path);

        static void WriteSolution(const std::string& file_output_result, const TableauxOutput& output);

        static std::vector<std::string> GetFileData(std::string path);

        static std::vector<std::string> GetStdInData();

        static void WriteOnFile(std::string path, std::string data);

        static std::vector<std::string> GetSplitStringsFromStringFile(std::string&& data_string);

    private:

        static std::vector<std::string> readStream(std::istream& input_stream);

    };
}

#endif //SIMPLEX_FILE_HPP
