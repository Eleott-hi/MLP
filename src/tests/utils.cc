#include "utils.h"

#include <fstream>
#include <sstream>
#include <iostream>

Config LoadWeights(const std::string &filename) {
  std::ifstream file(filename);
  if (!file.is_open()) {
    std::cout << "Can not open file " << filename << std::endl;
    return Config{2};
  }

  Config config;
  std::string line;

  for (int i = 0; std::getline(file, line); i++) {
    std::stringstream ss(line);
    double num = 0.0;
    while (ss >> num) {
      if (i == 0)
        config.AddLayer((int)num);
      else if (i == 1)
        config.AddWeight(num);
      else if (i == 2)
        config.AddBias(num);
    }
  }

  if (config.WeightsSize() != Config(config.GetLayers()).WeightsSize()) {
    std::cout << "Configuration file is corrupted: " << filename << std::endl;
    return Config{2};
  }

  return config;
}
