#include "def.h"
#include <string>
#include <iostream>
#include <fstream>
#include <chrono>
#include <cstring>

size_t writeOpenFailCount = 0;
size_t writeCount = 0;
size_t writeFailCount = 0;

bool state = false;
CriticalBlob blobToWrite = {};

void handleTextWriteTest(std::ofstream& myfile);
void handleBinaryWriteTest(std::ofstream& myfile);

void writerFunc(std::string fileName, FileTypes fileType, FileModes fileMode) {
  using namespace std;

  auto start = std::chrono::steady_clock::now();
  double elapsedSeconds = 0;
  while(elapsedSeconds < 3.0) {
    ios_base::openmode openMode;
    if (fileType == FileTypes::TEXTUAL) {
      if (fileMode == FileModes::APPEND)
        openMode = ios::app;
      else {
        openMode = ios::trunc;
      }
    }
    else {
      openMode = ios::binary;
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
  using namespace std;
  // Write alternating binary blobs to the file.
  if(state) {
    blobToWrite.someBool = true;
    blobToWrite.someCounter = 16;
    std::string path = "/tmp/hello";
    std::memcpy(blobToWrite.someName, path.data(), path.size());
    blobToWrite.someName[path.size()] = '\0';
    blobToWrite.someSignedNumber = -32;
  }
  else {
    blobToWrite.someBool = false;
    blobToWrite.someCounter = 32;
    std::string path = "/mnt/praisethesun";
    std::memcpy(blobToWrite.someName, path.data(), path.size());
    blobToWrite.someName[path.size()] = '\0';
    blobToWrite.someSignedNumber = 32;
  }

  if(myfile.is_open()) {
    // Write binary blob to file completely at once
    myfile.write(reinterpret_cast<char *>(&blobToWrite), sizeof(blobToWrite));
  }
  state = not state;
}
