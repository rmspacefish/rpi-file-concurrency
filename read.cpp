#include "def.h"

#include <string>
#include <iostream>
#include <fstream>
#include <chrono>

static size_t readOpenFailCount = 0;
static size_t readCount = 0;
static size_t readLineOne = 0;
static size_t readBothLines = 0;
static size_t faultyReadCount = 0;
static size_t goodFileCount = 0;
static size_t badFileEofCount = 0;
static size_t badFileFailCount = 0;

void textFileReadTest(std::ifstream& myfile, FileModes fileMode);
void binFileReadTest(std::ifstream& myfile, FileModes fileMode);

void readerFuncText(std::string fileName, FileTypes fileType, FileModes fileMode) {
  using namespace std;
  auto start = chrono::steady_clock::now();
  double elapsedSeconds = 0;
  while(elapsedSeconds < 3.0) {
    ios_base::openmode openMode = ios::in;
    if(fileType == FileTypes::BINARY) {
      openMode |= ios::binary;
    }
    ifstream myfile(fileName, openMode);
    if(fileType == FileTypes::TEXTUAL) {
      textFileReadTest(myfile, fileMode);
    }
    else {
      binFileReadTest(myfile, fileMode);
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

void textFileReadTest(std::ifstream& myfile, FileModes fileMode) {
  using namespace std;
  string line;
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
}

void binFileReadTest(std::ifstream& myfile, FileModes fileMode) {
  if(myfile.is_open()) {

  }
}
