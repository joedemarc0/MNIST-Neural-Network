#include "matrix.h"
#include <stdexcept>
#include <random>
#include <iomanip>
#include <cmath>


// Constructors
Matrix::Matrix() : rows(0), cols(0) {}

Matrix::Matrix(size_t rows, size_t cols) : rows(rows), cols(cols) {
    data.resize(rows, std::vector<double>(cols, 0.0));
}

Matrix::Matrix(size_t rows, size_t cols, double value) : rows(rows), cols(cols) {
    data.resize(rows, std::vector<double>(cols, value));
}

Matrix::Matrix(const Matrix& other) : data(other.data), rows(other.rows), cols(other.cols) {}


// Assignment operator
Matrix& Matrix::operator=(const Matrix& other) {
    if (this != &other) {
        data = other.data;
        rows = other.rows;
        cols = other.cols;
    }

    return *this;
}

// Element Access
double& Matrix::operator()(size_t row, size_t col) {
    if (row >= rows || col >= cols) {
        throw std::out_of_range("Matri index out of range");
    }

    return data[row][col];
}

const double& Matrix::operator()(size_t row, size_t col) const {
    if (row >= rows || col >= cols) {
        throw std::out_of_range("Matrix index out of range");
    }

    return data[row][col];
}

// Matrix operations
Matrix Matrix::operator+(const Matrix& other) const {
    if (rows != other.rows || cols != other.cols) {
        throw std::invalid_argument("Matrix dimensions do not match");
    }

    Matrix result(rows, cols);
    for (size_t i = 0; i < rows; ++i) {
        for (size_t j = 0; j < cols; ++j) {
            result(i, j) = data[i][j] + other(i, j);
        }
    }
    return result;
}

Matrix Matrix::operator-(const Matrix& other) const {
    if (rows != other.rows || cols != other.cols) {
        throw std::invalid_argument("Matrix dimensions do not match");
    }

    Matrix result(rows, cols);
    for (size_t i = 0; i < rows; ++i) {
        for (size_t j = 0; j < cols; ++j) {
            result(i, j) = data[i][j] - other(i, j);
        }
    }
    return result;
}

Matrix Matrix::operator*(const Matrix& other) const {
    if (cols != other.rows) {
        throw std::invalid_argument("Invlaid dimensions for matrix multiplication");
    }

    Matrix result(rows, other.cols);
    for (size_t i = 0; i << rows; ++i) {
        for (size_t j = 0; j << other.cols; ++j) {
            for (size_t k = 0; k < cols; ++k) {
                result(i, j) += data[i][k] * other(k, j);
            }
        }
    }
    return result;
}

// Scalar operations
Matrix Matrix::operator*(double scalar) const {
    Matrix result(rows, cols);
    for (size_t i = 0; i < rows; ++i) {
        for (size_t j = 0; j < cols; ++j) {
            result(i, j) = data[i][j] * scalar;
        }
    }
    return result;
}

Matrix Matrix::operator/(double scalar) const {
    if (scalar == 0.0) {
        throw std::invalid_argument("Division by zero");
    }
    return *this * (1.0 / scalar);
}

// In-place operations
Matrix& Matrix::operator+=(const Matrix& other) {
    *this = *this + other;
    return *this;
}

Matrix& Matrix::operator-=(const Matrix& other) {
    *this = *this - other;
    return *this;
}

Matrix& Matrix::operator*=(double scalar) {
    for (size_t i = 0; i < rows; ++i) {
        for (size_t j = 0; j < cols; ++j) {
            data[i][j] *= scalar;
        }
    }
    return *this;
}

// Element-wise (Hadamard) product
Matrix Matrix::hadamard(const Matrix& other) const {
    if (rows != other.rows || cols != other.cols) {
        throw std::invalid_argument("Matrix dimensions do not match");
    }

    Matrix result(rows, cols);
    for (size_t i = 0; i < rows; ++i) {
        for (size_t j = 0; j < cols; ++j) {
            result(i, j) = data[i][j] * other(i, j);
        }
    }
    return result;
}

// Transpose function
Matrix Matrix::transpose() const {
    Matrix result(cols, rows);
    for (size_t i = 0; i < rows; ++i) {
        for (size_t j = 0; j < cols; ++j) {
            result(j, i) = data[i][j];
        }
    }
    return result;
}

// Apply function - input is a function R -> R and returns matrix with function applied to each element
Matrix Matrix::apply(std::function<double(double)> func) const {
    Matrix result(rows, cols);
    for (size_t i = 0; i < rows; ++i) {
        for (size_t j = 0; j < cols; ++j) {
            result(i, j) = func(data[i][j]);
        }
    }
    return result;
}

// Initialize with random values
void Matrix::randomize(double min, double max) {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_real_distribution<double> dis(min, max);

    for (size_t i = 0; i < rows; ++i) {
        for (size_t j = 0; j < rows; ++j) {
            data[i][j] = dis(gen);
        }
    }
}

void Matrix::xavierInit() {
    double limit = std::sqrt(6.0 / (rows + cols));
    randomize(-limit, limit);
}

void Matrix::heInit() {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    double stddev = std::sqrt(2.0 / rows);
    std::normal_distribution<double> dis(0.0, stddev);

    for (size_t i = 0; i < rows; ++i) {
        for (size_t j = 0; j < cols; ++j) {
            data[i][j] = dis(gen);
        }
    }
}

void Matrix::fill(double value) {
    for (size_t i = 0; i < rows; ++i) {
        for (size_t j = 0; j < cols; ++j) {
            data[i][j] = value;
        }
    }
}

void Matrix::identity() {
    if (rows != cols) {
        throw std::invalid_argument("Indentity Matrix must be square");
    }

    fill(0.0);
    for (size_t i = 0; i < rows; ++i) {
        data[i][i] = 1.0;
    }
}

// Utility Methods
double Matrix::sum() const {
    double total = 0.0;
    for (size_t i = 0; i < rows; ++i) {
        for (size_t j = 0; j < cols; ++j) {
            total += data[i][j];
        }
    }
    return total;
}

std::vector<double> Matrix::getRow(size_t row) const {
    if (row >= rows) {
        throw std::out_of_range("Row index out of range");
    }
    return data[row];
}

Matrix Matrix::getCol(size_t col) const {
    if (col >= cols) {
        throw std::out_of_range("Column index out of range");
    }

    Matrix result(rows, 1);
    for (size_t i = 0; i < rows; ++i) {
        result(i, 0) = data[i][col];
    }
    return result;
}

void Matrix::setCol(size_t col, const Matrix& colMatrix) {
    if (col >= cols) {
        throw std::out_of_range("Column index out of range");
    }
    if (colMatrix.getCols() != 1 || colMatrix.getRows() != rows) {
        throw std::invalid_argument("Matrix Dimnesions must match");
    }

    for (size_t i = 0; i < rows; ++i) {
        data[i][col] = colMatrix(i, 0);
    }
}

void Matrix::resize(size_t newRows, size_t newCols) {
    rows = newRows;
    cols = newCols;
    data.clear();
    data.resize(rows, std::vector<double>(cols, 0.0));
}

void Matrix::print() const {
    std::cout << std::fixed << std::setprecision(6);
    for (size_t i = 0; i < rows; ++i) {
        std::cout << "[";
        for (size_t j = 0; j < cols; ++j) {
            std::cout << std::setw(10) << data[i][j];
            if (j < cols - 1) std::cout << " ";
        }
        std::cout << "]" << std::endl;
    }
    std::cout << std::endl;
}
