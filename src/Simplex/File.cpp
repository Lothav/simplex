//
// Created by luiz0tavio on 5/7/18.
//

#include "File.hpp"

std::vector<std::string> Simplex::File::GetFileData(std::string path)
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

std::vector<std::string> Simplex::File::GetStdInData()
{
    std::cout << "Not received a file as parameter. Trying to get it from stdin" << std::endl;
    return readStream(std::cin);
}

void Simplex::File::WriteOnFile(std::string path, std::string data)
{
    std::ofstream out_file;
    out_file.open(path, std::ios::app);
    out_file << data << "\n";
    out_file.close();
}

std::vector<long> Simplex::File::GetIntsFromStringFile(std::string data_string)
{
    std::vector<long> data_int = {};

    std::regex r("(\\+|-)?[[:digit:]]+");
    std::smatch m;

    while (std::regex_search(data_string, m, r)) {
        data_int.push_back( std::stoi (m[0]) );
        data_string = m.suffix().str();
    }

    return data_int;
}

std::vector<std::string> Simplex::File::readStream(std::istream& input_stream)
{
    std::vector<std::string> lines;

    std::string line;
    while (!input_stream.eof()) {
        if (std::getline(input_stream, line)) {
            lines.push_back(line);
        }
    }

    return lines;
}