//
// Created by luiz0tavio on 5/5/18.
//

#include <iostream>
#include <fstream>

void readStream(std::istream& input_stream)
{
    std::string line;
    while (!input_stream.eof()) {
        if (std::getline(input_stream, line)) {
            std::cout << "You typed:" << line << std::endl;
        }
    }
}

void buildFromFile(std::string path)
{
    std::ifstream input_file (path);
    if (!input_file.is_open()) {
        std::cerr << "Can not open file with path [" << path << "]" << std::endl;
        std::cerr << "\tTry to pass full path instead." << std::endl;
        return;
    }
    readStream(input_file);
    input_file.close();
}

void buildFromStdIn()
{
    readStream(std::cin);
}

int main(int argc, char** argv)
{
    if (argc > 1) {
        buildFromFile(argv[1]);
    } else {
        buildFromStdIn();
    }

    return EXIT_SUCCESS;
}