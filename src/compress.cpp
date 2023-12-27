#include <ctype.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>

std::vector<std::vector<std::string>> read_csv(const std::string& filename)
{
    std::ifstream file(filename);
    std::vector<std::vector<std::string>> data;

    if (file)
    {
        std::string line;
        while (std::getline(file, line)) {
            std::vector<std::string> row;
            std::stringstream ss(line);
            std::string value;

            while (std::getline(ss, value, ',')) {
                row.push_back(value);
            }
            data.push_back(row);
        }
    } 
    else 
    {
        std::cout << "Failed to open the file!" << std::endl;
    }

    return data;
}

std::vector<std::vector<double>> getRandomCodebook(
  std::vector<std::vector<double>> universe, 
  int codewordCount) 
{
  std::vector<std::vector<double>> codebook;
  srand(time(0));
  for(int i = 0; i < codewordCount; i++)
  {
    codebook.push_back(universe[rand() % universe.size()]);
  }

  return codebook;
}

double calculateMAE(const std::vector<double>& values, const std::vector<double>& reference) {
    double sum = 0.0;
    for (size_t i = 0; i < values.size(); i++)
      sum += std::abs(values[i] - reference[i]);
    return sum / values.size();
}

int getBestMatchingCodebookIndex(
  const std::vector<std::vector<double>> &codebook,
  const std::vector<double> &row)
{
  if(codebook.size() == 0)
    return -1;

  double score = calculateMAE(codebook[0], row);
  int selectedCodebook = 0;
  for (size_t i = 1; i < codebook.size(); i++)
  {
    auto tempScore = calculateMAE(codebook[i], row);
    if(score > tempScore)
    {
      score = tempScore;
      selectedCodebook = i;
    }
  }

  return selectedCodebook;
  
}

int main(int argc, char *argv[])
{
  if(argc < 2)
  {
    printf("Missing required arguments.\n");
    printf("Example usage: %s universalFile [codebook]\n", argv[0]);
    return -1;
  }

  const char *universeFileName = argv[1]; // universe file name
  char *codebookFile = NULL;
  if(argc > 2)
    codebookFile = argv[2];
  
  int universeSize = 6340, p = 12, rowSize = 1024, K = 8;

  double *initialCodebook = NULL;
  int i = 0, j = 0; // loop variables

  std::vector<std::vector<double>> universe;
  auto universe_csv = read_csv(universeFileName);
  for(auto record: universe_csv)
  {
    std::vector<double> tempRowData;
    for(auto &colstr : record)
      tempRowData.push_back(std::stof(colstr));
    universe.emplace_back(tempRowData);
  }

  std::vector<std::vector<double>> codebook;
  if(codebookFile)
  {
    auto codebook_csv = read_csv(codebookFile);
    for(auto record: codebook_csv)
    {
      std::vector<double> tempRowData;
      for(auto &colstr : record)
        tempRowData.push_back(std::stof(colstr));
      codebook.emplace_back(tempRowData);
    }
    std::cout << "Read codebook with " << codebook.size() << " code words"
      << std::endl;
  }

  if(codebook.size() == 0)
  {
    codebook = getRandomCodebook(universe, 8);
  }

  std::vector<uint8_t> indexes;

  for (int i = 0; i < universe.size(); i++)
  {
    indexes.push_back(getBestMatchingCodebookIndex(codebook, universe[i]));
  }

  std::ofstream outfile("compressed.bin", std::ios::binary);

  // Check if the file was opened successfully
  if (!outfile) {
      std::cout << "Failed to open the file!" << std::endl;
      return -1;
  }

  outfile.write((char *)indexes.data(), indexes.size());
}