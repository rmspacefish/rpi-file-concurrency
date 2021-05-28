#include "def.h"

#include <string>
#include <iostream>
#include <fstream>
#include <chrono>

void writerFuncText(std::string fileName, FileTypes fileType, FileModes fileMode) {
  using namespace std;
  size_t writeOpenFailCount = 0;
  size_t writeCount = 0;
  size_t writeFailCount = 0;
  auto start = std::chrono::steady_clock::now();
  double elapsedSeconds = 0;
  while(elapsedSeconds < 3.0) {
    ios_base::openmode openMode;
    if (fileMode == FileModes::APPEND)
      openMode = ios::app;
    else {
      openMode = ios::trunc;
    }
    ofstream myfile("example.txt", openMode);
    if(myfile.is_open()) {
      myfile << "Writing this to a file.\n";
      if(myfile.bad()) {
        writeFailCount++;
      }
      else {
        writeCount++;
      }
      myfile << "Writing second line.\n";
      if(myfile.bad()) {
        writeFailCount++;
      }
      else {
        writeCount++;
      }
      myfile.close();
    }
    else {
      writeOpenFailCount++;
    }
    chrono::duration<double> elapsedTime = chrono::steady_clock::now() - start;
    elapsedSeconds = elapsedTime.count();
  }
  cout << "Write Open Fail Count: " << writeOpenFailCount << "\n";
  cout << "Write Count: " << writeCount << "\n";
  cout << "Write Fail Count: " << writeFailCount << "\n";
  cout << endl;
}

void readerFuncText(std::string fileName, FileTypes fileType, FileModes fileMode) {
  using namespace std;
  string line;
  size_t readOpenFailCount = 0;
  size_t readCount = 0;
  size_t readLineOne = 0;
  size_t readBothLines = 0;
  size_t faultyReadCount = 0;
  size_t goodFileCount = 0;
  size_t badFileEofCount = 0;
  size_t badFileFailCount = 0;
  auto start = std::chrono::steady_clock::now();
  double elapsedSeconds = 0;
  while(elapsedSeconds < 3.0) {
    ifstream myfile("example.txt");
    if(myfile.is_open()) {
      bool readLineOneFlag = false;
      bool readBothLinesFlag = false;
      size_t lineIdx = 0;
      while(not myfile.eof()) {
        if(getline(myfile, line)) {
          if(line == "Writing this to a file.") {
            readLineOne++;
            readLineOneFlag = true;
          }
          if(line == "Writing second line.") {
            readBothLines++;
            readBothLinesFlag = true;
          }
          lineIdx++;
        }
        else {
          if(myfile.good()) {
            goodFileCount++;
          }
          else {
            if(myfile.eof()) {
              badFileEofCount++;
            }
            if(myfile.fail()) {
              badFileFailCount++;
            }
          }
        }
        if(fileMode == FileModes::APPEND and lineIdx >= 2) {
           break;
        }
      }
      if(readLineOneFlag or readBothLinesFlag) {
        readCount++;
      }
    }
    else {
      readOpenFailCount++;
    }
    chrono::duration<double> elapsedTime = chrono::steady_clock::now() - start;
    elapsedSeconds = elapsedTime.count();
  }
  cout << "Read Open Fail Count: " << readOpenFailCount << "\n";
  cout << "Read Count: " << readCount << "\n";
  cout << "Read One Line Count: " << readLineOne << "\n";
  cout << "Read Both Lines Count: " << readBothLines << "\n";
  cout << "Faulty Read Count: " << faultyReadCount << "\n";
  cout << "EOF Count: " << badFileEofCount << "\n";
  cout << "Fail Count: " << badFileFailCount << "\n";
  cout << endl;
}
