//
// Created by luiz0tavio on 5/5/18.
//

#include <iostream>
#include <fstream>
#include <cstring>
#include <vector>
#include <regex>
#include "Matrix.hpp"

std::vector<std::string> readStream(std::istream& input_stream)
{
    std::vector<std::string> lines;

    std::string line;
    while (!input_stream.eof()) {
        if (std::getline(input_stream, line)) {
            lines.push_back(line);
            std::cout << "You typed:" << line << std::endl;
        }
    }

    return lines;
}

std::vector<std::string> buildFromFile(std::string path)
{
    std::vector<std::string> file_data = {};

    std::ifstream input_file (path);
    if (!input_file.is_open()) {
        std::cerr << "Can not open file with path [" << path << "]" << std::endl;
        std::cerr << "\tTry to pass full path instead." << std::endl;
    } else {
        file_data = readStream(input_file);
        input_file.close();
    }

    return file_data;
}

std::vector<std::string> buildFromStdIn()
{
    return readStream(std::cin);
}

std::vector<long> getMatrixDataFromString(std::string matrix_data_string)
{
    std::vector<long> matrix_data_long = {};

    std::regex r("(\\+|-)?[[:digit:]]+");
    std::smatch m;

    while (std::regex_search(matrix_data_string, m, r)) {
        matrix_data_long.push_back( std::stoi (m[0]) );
        matrix_data_string = m.suffix().str();
    }

    return matrix_data_long;
}

int main(int argc, char** argv)
{
    std::vector<std::string> file_data;
    if (argc > 1) {
        file_data = buildFromFile(argv[1]);
    } else {
        file_data = buildFromStdIn();
    }

    if (file_data.size() != 3) {
        std::cerr << "Invalid file lines!" << std::endl;
        std::cerr << "File must have 3 lines:" << std::endl;
        std::cerr << "\tm+1" << std::endl;
        std::cerr << "\tn+1" << std::endl;
        std::cerr << "\t[[line 0 ], [line 1], ..., [line n]]" << std::endl;
        return EXIT_FAILURE;
    }

    auto matrix_m = std::stoi (file_data[0]);
    auto matrix_n = std::stoi (file_data[1]);
    auto matrix_cells = getMatrixDataFromString(file_data[2]);

    auto matrix = new Matrix(matrix_m, matrix_n, matrix_cells);


    return EXIT_SUCCESS;
}