#ifndef MLP_SRC_MAIN_MODEL_MATRIX_MLP_MATRIX_H_
#define MLP_SRC_MAIN_MODEL_MATRIX_MLP_MATRIX_H_

#include <initializer_list>
#include <ostream>

class Matrix {
 public:
  virtual ~Matrix();
  Matrix(const Matrix &other);
  Matrix(Matrix &&other) noexcept;
  Matrix &operator=(const Matrix &other);
  Matrix &operator=(Matrix &&other) noexcept;
  Matrix &operator=(std::initializer_list<double> vector);

  explicit Matrix(size_t row, size_t col);

  inline double *operator[](size_t i) const { return &(matrix_[i * col_]); };

  [[nodiscard]] inline size_t get_rows() const { return row_; }
  [[nodiscard]] inline size_t get_cols() const { return col_; }

  Matrix Transpose();

  static void Transpose(const Matrix &a, Matrix *dest);
  static void Mull(const Matrix &a, double num, Matrix *dest);
  static void Add(const Matrix &a, const Matrix &b, Matrix *dest);
  static void Mull(const Matrix &a, const Matrix &b, Matrix *dest);
  static void Diff(const Matrix &a, const Matrix &b, Matrix *dest);
  static void HadamardMull(const Matrix &a, const Matrix &b, Matrix *dest);

 private:
  size_t row_, col_;
  double *matrix_;

  void Free();
  void Move(Matrix &other);
  void Copy(const Matrix &other);
  void Alloc(size_t row, size_t col);
};

std::ostream &operator<<(std::ostream &os, const Matrix &matrix);

#endif  // MLP_SRC_MAIN_MODEL_MATRIX_MLP_MATRIX_H_
