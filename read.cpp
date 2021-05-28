#include "def.h"

#include <string>
#include <iostream>
#include <fstream>
#include <chrono>
#include <thread>

static size_t readOpenFailCount = 0;
static size_t readCount = 0;
static size_t goodFileCount = 0;
static size_t badFileEofCount = 0;
static size_t badFileFailCount = 0;
static size_t faultyReadCount = 0;

static size_t readLineOne = 0;
static size_t readBothLines = 0;

static size_t stateOneReadCnt = 0;
static size_t stateTwoReadCnt = 0;
static size_t invalidStateCnt = 0;

void textFileReadTest(std::ifstream& myfile, FileModes fileMode);
void binFileReadTest(std::ifstream& myfile, FileModes fileMode);
bool checkFileState(std::ifstream& myfile);
void attemptAnotherRead(std::ifstream& myfile, CriticalBlob& readBlob);


void readerFunc(std::string fileName, FileTypes fileType, FileModes fileMode) {
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
  if(fileType == FileTypes::TEXTUAL) {
    cout << "Read One Line Count: " << readLineOne << "\n";
    cout << "Read Both Lines Count: " << readBothLines << "\n";
  }
  else {
    cout << "Blob in state one read: " << stateOneReadCnt << "\n";
    cout << "Blob in state two read: " << stateTwoReadCnt << "\n";
    cout << "Blob in invalid state: " << invalidStateCnt << "\n";
  }

  cout << "Faulty Read Count: " << faultyReadCount << "\n";
  cout << "EOF Count: " << badFileEofCount << "\n";
  cout << "Fail Count: " << badFileFailCount << "\n";
  cout << endl;
}

void textFileReadTest(std::ifstream& myfile, FileModes fileMode) {
  using namespace std;
  string line;
  if(myfile.is_open()) {
    size_t lineIdx = 0;
    if(checkFileState(myfile)) {
      bool readLineOneFlag = false;
      bool readBothLinesFlag = false;
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
        if(fileMode == FileModes::APPEND and lineIdx >= 2) {
          break;
        }
      }
      if(readLineOneFlag or readBothLinesFlag) {
        readCount++;
      }
    }
  }
  else {
    readOpenFailCount++;
  }
}

void binFileReadTest(std::ifstream& myfile, FileModes fileMode) {
  using namespace std;
  CriticalBlob readBlob = {};
  if(myfile.is_open()) {
    myfile.read(reinterpret_cast<char*>(&readBlob), sizeof(readBlob));
    if(checkFileState(myfile)) {
      bool someBool = readBlob.someBool;
      uint32_t someCounter = readBlob.someCounter;
      std::string someName(readBlob.someName);
      int32_t someSignedNumber = readBlob.someSignedNumber;
      if(someBool == true and someCounter == 16 and someName == "/tmp/hello"
          and someSignedNumber == -32) {
        stateOneReadCnt++;
      }
      else if(someBool == false and someCounter == 32 and someName == "/mnt/praisethesun"
          and someSignedNumber == 32) {
        stateTwoReadCnt++;
      }
      else {
        invalidStateCnt++;
      }
    }
    else {
      // In principle, it is possible to retry the read. In reality, it would make sense
      // to do this after a small break (e.g. usleep(2) or something similar)
      // attemptAnotherRead(myfile, readBlob);
    }
  }
  else {
    readOpenFailCount++;
  }
}

bool checkFileState(std::ifstream& myfile) {
  if(myfile.good()) {
    goodFileCount++;
    return true;
  }
  else {
    if(myfile.eof()) {
      badFileEofCount++;
    }
    if(myfile.fail()) {
      badFileFailCount++;
    }
    return false;
  }
}

void attemptAnotherRead(std::ifstream& myfile, CriticalBlob& readBlob) {
  // It is important to clear the error flags!
  myfile.clear();
  size_t retryMax = 3;
  int retryIdx = 0;
  while(retryIdx < retryMax) {
    myfile.read(reinterpret_cast<char*>(&readBlob), sizeof(readBlob));
    if(myfile.good()) {
      // Read worked
    }
    else {
      // Still does not work
    }
    myfile.clear();
    retryIdx++;
  }
}
