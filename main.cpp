//
// Created by luiz0tavio on 5/5/18.
//

#include <iostream>
#include <fstream>
#include <regex.h>
#include <cstring>
#include <vector>

char** regexRow(char* src, unsigned int row_size)
{
    int res, len;
    auto *result = (char *)malloc(BUFSIZ);
    auto **return_value = (char **)malloc(row_size * sizeof(char *));
    char err_buf[BUFSIZ];
    char *aux = src;

    const char* pattern = "\\[((\\-*[0-9]+[\\,\\ ]*)+)\\]";

    regex_t preg = {};
    regmatch_t pmatch[10];

    if( (res = regcomp(&preg, pattern, REG_EXTENDED)) != 0)
    {
        regerror(res, &preg, err_buf, BUFSIZ);
        printf("regcomp() error: %s\n", err_buf);
        exit(res);
    }

    long offset = 0;
    int i;
    for(i = 0; i < row_size; i++)
    {
        aux += offset;
        res = regexec(&preg, aux, 10, pmatch, REG_NOTBOL);
        if(res == REG_NOMATCH) break;

        offset = pmatch[0].rm_eo - pmatch[0].rm_so;

        len = pmatch[1].rm_eo - pmatch[1].rm_so;
        memcpy(result, aux + pmatch[1].rm_so, (size_t)len);
        result[len] = '\0';

        return_value[i] = (char *)malloc((size_t)len + 1);
        memcpy(return_value, result, (size_t)len);
        printf("%s\n", result);
    }

    regfree(&preg);
    free(result);

    return return_value;
}


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

int main(int argc, char** argv)
{
    std::vector<std::string> file_data;
    if (argc > 1) {
        file_data = buildFromFile(argv[1]);
    } else {
        file_data = buildFromStdIn();
    }

    if (file_data.size() == 3) {
        std::cerr << "Invalid file lines!" << std::endl;
        std::cerr << "File must have 3 lines:" << std::endl;
        std::cerr << "\tm+1" << std::endl;
        std::cerr << "\tn+1" << std::endl;
        std::cerr << "\t[[line 0 ], [line 1], ..., [line n]]" << std::endl;
        return EXIT_FAILURE;
    }

    std::cout << file_data.size() << std::endl;

    return EXIT_SUCCESS;
}