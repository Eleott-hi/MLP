#ifndef MLP_SRC_MAIN_MODEL_VIRTUAL_MLP_H_
#define MLP_SRC_MAIN_MODEL_VIRTUAL_MLP_H_

#include "config.h"

namespace s21 {

class VirtualMLP {
 public:
  virtual ~VirtualMLP() = default;
  virtual bool SetInputLayer(const std::vector<double>& data) = 0;
  virtual const std::vector<double>& GetOutput() = 0;
  virtual const Config& GetConfig() = 0;
  virtual int FeedForward() = 0;
  virtual bool BackPropagation(size_t right_index, double learning_rate) = 0;
  virtual double GetError(size_t right_answer) = 0;

 protected:
  Config config_;
};

}  // namespace s21

#endif  // MLP_SRC_MAIN_MODEL_VIRTUAL_MLP_H_
