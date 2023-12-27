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
  if(argc < 3)
  {
    printf("Missing required arguments.\n");
    printf("Example usage: %s compressedFile codebook\n", argv[0]);
    return -1;
  }

  const char *compressedFilePath = argv[1]; // universe file name
  std::ifstream compressedFile(compressedFilePath, std::ios::binary);

  if (!compressedFile) {
    std::cout << "Failed to open the file!" << std::endl;
    return 1;
  }


  char *codebookFile =  argv[2];
  
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
  }

  std::vector<uint8_t> indexes(std::istreambuf_iterator<char>(compressedFile), {});
  std::ofstream outfile("decompressed.txt");

  int codewordSize = codebook[0].size();

  for (int i = 0; i < indexes.size(); i++)
  {
    for(int j = 0; j < codewordSize - 1; j++)
    {
      outfile << codebook[indexes[i]][j];
      outfile << ",";
    }

    outfile << codebook[indexes[i]][codewordSize - 1];
    outfile << std::endl;
  }
  outfile.close();
}