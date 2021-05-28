#include "def.h"
#include <string>
#include <iostream>
#include <fstream>
#include <chrono>

size_t writeOpenFailCount = 0;
size_t writeCount = 0;
size_t writeFailCount = 0;

void handleTextWriteTest(std::ofstream& myfile);
void handleBinaryWriteTest(std::ofstream& myfile);

void writerFuncText(std::string fileName, FileTypes fileType, FileModes fileMode) {
  using namespace std;

  auto start = std::chrono::steady_clock::now();
  double elapsedSeconds = 0;
  while(elapsedSeconds < 3.0) {
    ios_base::openmode openMode;
    if (fileMode == FileModes::APPEND)
      openMode = ios::app;
    else {
      openMode = ios::trunc;
    }

    if(fileType == FileTypes::BINARY) {
      openMode |= ios::binary;
    }
    ofstream myfile(fileName, openMode);
    if(fileType == FileTypes::TEXTUAL) {
      handleTextWriteTest(myfile);
    }
    else {
      handleBinaryWriteTest(myfile);
    }
    chrono::duration<double> elapsedTime = chrono::steady_clock::now() - start;
    elapsedSeconds = elapsedTime.count();
  }
  cout << "Write Open Fail Count: " << writeOpenFailCount << "\n";
  cout << "Write Count: " << writeCount << "\n";
  cout << "Write Fail Count: " << writeFailCount << "\n";
  cout << endl;
}

void handleTextWriteTest(std::ofstream& myfile) {
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
}

void handleBinaryWriteTest(std::ofstream& myfile) {

}
