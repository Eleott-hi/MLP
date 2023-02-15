#include "matrix.h"

#include <exception>
#include <initializer_list>
#include <ostream>

Matrix::Matrix(size_t row, size_t col)
    : row_(row), col_(col), matrix_(nullptr) {
  Alloc(row, col);
}

Matrix::~Matrix() { Free(); }

Matrix::Matrix(const Matrix& other) : Matrix(other.row_, other.col_) {
  Copy(other);
}

Matrix::Matrix(Matrix&& other) noexcept { Move(other); }

Matrix Matrix::Transpose() {
  Matrix new_matrix(col_, row_);

  for (size_t i = 0; i < new_matrix.row_; ++i) {
    for (size_t j = 0; j < new_matrix.col_; ++j) {
      new_matrix[i][j] = (*this)[j][i];
    }
  }

  return new_matrix;
}

void Matrix::Transpose(const Matrix& a, Matrix* dest) {
#ifdef S21_COMPILE_MATRIX_WITH_EXCEPTIONS
  if (a.row_ != dest->col_ || a.col_ != dest->row_) {
    throw std::range_error(
        "Transpose: matrix A cannot be placed in matrix dest.");
  }
#endif

  for (size_t i = 0; i < dest->row_; ++i) {
    for (size_t j = 0; j < dest->col_; ++j) {
      (*dest)[i][j] = a[j][i];
    }
  }
}

void Matrix::HadamardMull(const Matrix& a, const Matrix& b, Matrix* dest) {
#ifdef S21_COMPILE_MATRIX_WITH_EXCEPTIONS
  if (a.row_ != b.row_ || a.col_ != b.col_) {
    throw std::range_error(
        "HadamardMull: Matrix A cannot be multiplied by matrix B.");
  }

  if (a.row_ != dest->row_ || a.col_ != dest->col_) {
    throw std::range_error(
        "HadamardMull: result of the multiplication cannot be placed in the "
        "dest matrix.");
  }
#endif

  for (size_t i = 0; i < a.row_; ++i) {
    for (size_t j = 0; j < a.col_; ++j) {
      (*dest)[i][j] = a[i][j] * b[i][j];
    }
  }
}

void Matrix::Mull(const Matrix& m_a, const Matrix& m_b, Matrix* dest) {
#ifdef S21_COMPILE_MATRIX_WITH_EXCEPTIONS
  if (m_a.col_ != m_b.row_) {
    throw std::range_error("Mull: Matrix A cannot be multiplied by matrix B.");
  }

  if (m_a.row_ != dest->row_ || m_b.col_ != dest->col_) {
    throw std::range_error(
        "Mull: result of the multiplication cannot be placed in the dest "
        "matrix");
  }
#endif

  for (size_t i = 0; i < dest->row_; ++i) {
    double* c = dest->matrix_ + i * dest->col_;
    for (size_t j = 0; j < dest->col_; ++j) {
      c[j] = 0;
    }
    for (size_t k = 0; k < m_a.col_; ++k) {
      const double* b = m_b.matrix_ + k * dest->col_;
      double a = m_a.matrix_[i * m_a.col_ + k];
      for (size_t j = 0; j < dest->col_; ++j) c[j] += a * b[j];
    }
  }
}

void Matrix::Mull(const Matrix& a, double num, Matrix* dest) {
#ifdef S21_COMPILE_MATRIX_WITH_EXCEPTIONS
  if (a.row_ != dest->row_ || a.col_ != dest->col_) {
    throw std::range_error(
        "Mull: result of the multiplication cannot be placed in the dest "
        "matrix");
  }
#endif

  for (size_t i = 0; i < dest->row_; ++i) {
    for (size_t j = 0; j < dest->col_; ++j) {
      (*dest)[i][j] = a[i][j] * num;
    }
  }
}

void Matrix::Add(const Matrix& a, const Matrix& b, Matrix* dest) {
#ifdef S21_COMPILE_MATRIX_WITH_EXCEPTIONS
  if (a.row_ != b.row_ || a.col_ != b.col_) {
    throw std::range_error("Add: Matrix A cannot be added to the matrix B.");
  }

  if (a.row_ != dest->row_ || b.col_ != dest->col_) {
    throw std::range_error(
        "Add: result of the addition cannot be placed in the dest matrix");
  }
#endif

  for (size_t i = 0; i < a.row_; ++i) {
    for (size_t j = 0; j < b.col_; ++j) {
      (*dest)[i][j] = a[i][j] + b[i][j];
    }
  }
}

void Matrix::Diff(const Matrix& a, const Matrix& b, Matrix* dest) {
#ifdef S21_COMPILE_MATRIX_WITH_EXCEPTIONS
  if (a.row_ != b.row_ || a.col_ != b.col_) {
    throw std::range_error(
        "Diff: from matrix A it is impossible to subtract matrix B. Difference "
        "size.");
  }

  if (a.row_ != dest->row_ || b.col_ != dest->col_) {
    throw std::range_error(
        "Diff: result of the addition cannot be placed in the dest matrix");
  }
#endif

  for (size_t i = 0; i < a.get_rows(); ++i) {
    for (size_t j = 0; j < b.get_cols(); ++j) {
      (*dest)[i][j] = a[i][j] - b[i][j];
    }
  }
}

Matrix& Matrix::operator=(const Matrix& other) {
  if (this == &other) return *this;
  Free();
  Alloc(other.row_, other.col_);
  Copy(other);
  return *this;
}

Matrix& Matrix::operator=(Matrix&& other) noexcept {
  if (this == &other) return *this;
  Free();
  Move(other);
  return *this;
}

Matrix& Matrix::operator=(std::initializer_list<double> vector) {
#ifdef S21_COMPILE_MATRIX_WITH_EXCEPTIONS
  if (vector.size() != row_ * col_) {
    throw std::length_error("operator=: invalid size");
  }
#endif

  std::copy(vector.begin(), vector.end(), matrix_);
  return *this;
}

void Matrix::Alloc(size_t row, size_t col) {
#ifdef S21_COMPILE_MATRIX_WITH_EXCEPTIONS
  if (row * col == 0) {
    throw std::length_error("Alloc: array of zero length");
  }
#endif
  row_ = row;
  col_ = col;
  matrix_ = new double[row_ * col_]();
}

void Matrix::Free() {
  delete[] matrix_;
  matrix_ = nullptr;
  row_ = 0;
  col_ = 0;
}

void Matrix::Copy(const Matrix& other) {
  std::copy(other.matrix_, other.matrix_ + other.row_ * other.col_, matrix_);
}

void Matrix::Move(Matrix& other) {
  matrix_ = other.matrix_;
  row_ = other.row_;
  col_ = other.col_;

  other.matrix_ = nullptr;
  other.row_ = 0;
  other.col_ = 0;
}

std::ostream& operator<<(std::ostream& os, const Matrix& matrix) {
  for (size_t i = 0; i < matrix.get_rows(); ++i) {
    for (size_t j = 0; j < matrix.get_cols(); ++j) {
      os << matrix[i][j] << " ";
    }
    os << std::endl;
  }
  return os;
}
