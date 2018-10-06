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

std::vector<std::string> Simplex::File::GetSplitStringsFromStringFile(std::string&& data_string)
{
    std::vector<std::string> result;
    std::istringstream iss(data_string);

    for (std::string token; std::getline(iss, token, ' ');) {
        result.push_back(std::move(token));
    }
    return result;
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

Simplex::TableauxInput Simplex::File::GetTableauxInputFromFile(std::string&& file_path)
{
    std::vector<std::string> file_data = Simplex::File::GetFileData(file_path);

    auto variables_count    = std::stoi(file_data[0]);
    auto restrictions_count = std::stoi(file_data[1]);

    auto non_negative = Simplex::File::GetSplitStringsFromStringFile(std::move(file_data[2]));
    std::vector<bool> is_non_negative = {};
    for (auto &non_negative_i : non_negative) {
        is_non_negative.push_back(std::atoi(non_negative_i.c_str()) == 1);
    }

    std::vector<Simplex::Operator> operators = {};
    auto obj_values = Simplex::File::GetSplitStringsFromStringFile(std::move(file_data[3]));

    std::vector<double> cells;

    // Set cells with objective function line
    for (auto &obj_value: obj_values) {

        errno = 0;
        char *endptr;
        auto *nptr = const_cast<char*>(obj_value.c_str());

        auto number = std::strtod(nptr, &endptr);

        if (errno == 0 && nptr != endptr) {
            cells.push_back(number);
        }
    }
    cells.push_back(0); // Initial Objective value

    for (int i = 0; i < restrictions_count; ++i) {
        auto restriction_line = Simplex::File::GetSplitStringsFromStringFile(std::move(file_data[4+i]));
        for (auto &restriction_item : restriction_line){
            if(restriction_item == "<="){
                operators.push_back(Simplex::Operator::LESS_EQUAL);
            } else if (restriction_item == ">=") {
                operators.push_back(Simplex::Operator::GREATER_EQUAL);
            } else if (restriction_item == "=" || restriction_item == "==") {
                operators.push_back(Simplex::Operator::EQUAL);
            } else {

                auto value = std::strtod(restriction_item.c_str(), nullptr);
                cells.push_back(value);
            }
        }
    }

    return Simplex::TableauxInput{
            .m                  = restrictions_count + 1,
            .n                  = variables_count + 1,
            .type               = Type::NON_INT,
            .cells              = cells,
            .operators          = operators,
            .is_non_negative    = is_non_negative,
    };
}

void Simplex::File::WriteSolution(const std::string& file_output_result, const TableauxOutput& output)
{
    // Clear output file.
    std::ofstream ofs;
    ofs.open(file_output_result, std::ofstream::out | std::ofstream::trunc);
    ofs.close();

    auto removeTrailingZeros = [](std::string&& obj) -> std::string
    {
        obj.erase(obj.find_last_not_of('0') + 1, std::string::npos);
        obj.erase(obj.find_last_not_of('.') + 1, std::string::npos);
        return obj;
    };

    std::string output_str;

    switch (output.solution_type)
    {
        case SolutionType::VIABLE:
        {
            std::string obj = removeTrailingZeros(std::to_string(output.objective_value));

            output_str  = "Status: otimo\n";
            output_str += "Objetivo: " + obj + "\n";
            output_str += "Solucao:\n";
            for (int i = 0; i < output.solution.size(); i++) {

                std::string sol = removeTrailingZeros(std::to_string(output.solution[i]));

                output_str += (i == 0 ? "" : " ") + sol;
            }
            output_str += "\nCertificado:";
            output_str += "\n";
        }
            break;

        case SolutionType::NON_VIABLE:
            output_str  = "Status: inviavel\n";
            output_str += "Certificado:";
            output_str += "\n";
            break;

        case SolutionType::UNBOUNDED:
            output_str  = "Status: inviavel\n";
            output_str += "Certificado:";
            output_str += "\n";
            break;

        default:
            return;
    }

    File::WriteOnFile(file_output_result, output_str);
}