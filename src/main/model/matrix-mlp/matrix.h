#ifndef MLP_SRC_MAIN_MODEL_MATRIX_MLP_MATRIX_H_
#define MLP_SRC_MAIN_MODEL_MATRIX_MLP_MATRIX_H_

#include <initializer_list>
#include <ostream>

namespace s21 {

class Matrix {
 public:
  explicit Matrix(size_t row, size_t col);
  virtual ~Matrix();
  Matrix(const Matrix &other);
  Matrix(Matrix &&other) noexcept;
  Matrix &operator=(std::initializer_list<double> vector);
  Matrix &operator=(const Matrix &other);
  Matrix &operator=(Matrix &&other) noexcept;

  inline double *operator[](size_t i) const { return &(matrix_[i * col_]); };

  [[nodiscard]] inline size_t get_rows() const { return row_; }
  [[nodiscard]] inline size_t get_cols() const { return col_; }

  Matrix Transpose();

  static void Transpose(const Matrix &a, Matrix *dest);
  static void HadamardMull(const Matrix &a, const Matrix &b, Matrix *dest);
  static void Mull(const Matrix &a, const Matrix &b, Matrix *dest);
  static void Mull(const Matrix &a, double num, Matrix *dest);
  static void Add(const Matrix &a, const Matrix &b, Matrix *dest);
  static void Diff(const Matrix &a, const Matrix &b, Matrix *dest);

 private:
  size_t row_;
  size_t col_;
  double *matrix_;

  void Alloc(size_t row, size_t col);
  void Free();
  void Copy(const Matrix &other);
  void Move(Matrix &other);
};

std::ostream &operator<<(std::ostream &os, const Matrix &matrix);

}  // namespace s21

#endif  // MLP_SRC_MAIN_MODEL_MATRIX_MLP_MATRIX_H_
