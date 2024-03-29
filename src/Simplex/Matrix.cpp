#include <cmath>
#include "Matrix.hpp"
#include "File.hpp"

Simplex::Matrix::Matrix(long m, long n, const std::vector<double> &cells): m_(m), n_(n)
{
    int i, j;
    for (i = 0 ; i < m_; i++) {
        cells_.push_back({});
        for (j = 0 ; j < n_; j++) {

            auto value = cells[(i * n_) + j];

            double integral = std::floor(value);
            double frac = value - integral;

            const long precision = 1000000000;

            long gcd_ = Simplex::Fraction::gcd(round(frac * precision), precision);

            long denominator = precision / gcd_;
            auto numerator = static_cast<long>(round(frac * precision) / gcd_);

            cells_[i].push_back(new Fraction(static_cast<long long>((integral*denominator) + numerator), denominator));
        }
    }
}

void Simplex::Matrix::updateCell(long i, long j, Fraction* cell)
{
    if (i >= this->m_) {
        std::cerr << "Error trying update matrix. Invalid line index " << i << std::endl;
        return;
    }

    if (j >= this->n_) {
        std::cerr << "Error trying update matrix. Invalid column index " << j << std::endl;
        return;
    }

    this->cells_[i][j] = cell;
}

void Simplex::Matrix::addColumn(long pos, std::vector<Fraction*> column)
{
    if (pos > this->getN()-1 || column.size() != this->getM()) {
        std::cerr << "Error adding column." << std::endl;
        return;
    }

    for (int i = 0; i < this->getM(); ++i) {
        this->cells_[i].insert(this->cells_[i].begin() + pos, column[i]);
    }

    this->n_ += 1;
}

void Simplex::Matrix::addLine(long pos, std::vector<Fraction*> line)
{
    if (pos > this->getM() || line.size() != this->getN()) {
        std::cerr << "Error adding line." << std::endl;
        return;
    }

    this->cells_.insert(this->cells_.begin() + pos, line);

    this->m_ += 1;
}

void Simplex::Matrix::removeColumn(long column)
{
    if (column > this->getN()-1) {
        std::cerr << "Cannot remove " << column << "th column from a matrix with " << this->getN() << " columns!" << std::endl;
        return;
    }

    for (int k = 0; k < this->getM(); ++k) {
        this->cells_[k].erase(this->cells_[k].begin() + column);
    }

    this->n_ -= 1;
}

std::string Simplex::Matrix::toString() const
{
    std::string matrix_string = "[";

    // Get max length number string.
    // This will be used to generate constant spaces between numbers when generate string it.
    ulong max_length = 0;
    for (int i = 0; i < this->getM(); ++i) {
        for (int j = 0; j < this->getN(); ++j) {
            long double value = this->cells_[i][j]->getFloatValue();
            auto cell_string = std::to_string(value);
            if (cell_string.length() > max_length) {
                max_length = cell_string.length();
            }
        }
    }

    // Generate matrix string.
    for (int i = 0; i < this->getM(); ++i) {

        matrix_string += "[";

        for (int j = 0; j < this->getN(); ++j) {
            long double value = this->cells_[i][j]->getFloatValue();

            auto cell_string = std::to_string(value);

            auto total_space = (i == 0 && j == 0 ? max_length : max_length+1) - cell_string.length();
            while (total_space > 0) {
                matrix_string += ' ';
                total_space--;
            }

            matrix_string += std::to_string(value);
            if (j != this->getN()-1) {
                matrix_string += ", ";
            }
        }

        matrix_string += "]";

        if (i != this->getM()-1) {
            matrix_string += ", \n";
        }
    }

    matrix_string += "]\n";

    return matrix_string;
}

void Simplex::Matrix::printMatrixOnFile(std::string file_output) const
{
    // Pivot finished. Write modifications on 'file_output_steps' file.
    std::string matrix_str = toString();
    Simplex::File::WriteOnFile(file_output, matrix_str);
}

long Simplex::Matrix::getM() const
{
    return m_;
}

long Simplex::Matrix::getN() const
{
    return n_;
}

std::vector<std::vector<Simplex::Fraction*>> Simplex::Matrix::getCells() const
{
    return cells_;
}