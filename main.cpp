#include "def.h"

#include <iostream>
#include <thread>
#include <fstream>
#include <cstdio>

// Configuration

#define TEXTUAL_MODE    0
#define BINARY_MODE     1

#define APPEND_MODE     0
#define TRUNCATE_MODE   1

#define FILE_TYPE       TEXTUAL_MODE
#define FILE_MODE       APPEND_MODE

void readerFuncText(std::string fileName, FileTypes fileType, FileModes fileMode);
void writerFuncText(std::string fileName, FileTypes fileType, FileModes fileMode);

// Code

int main() {
  using namespace std;
  cout << "Concurrent file access test" << endl;
  int result = remove(TXT_FILE_NAME.c_str());
  if(result == 0) {
    cout << "Removed existing example file" << endl;
  }
  result = remove(BIN_FILE_NAME.c_str());
  if(result == 0) {
    cout << "Removed existing binary file" << endl;
  }

#if FILE_TYPE == TEXTUAL_MODE
  FileTypes fileType = FileTypes::TEXTUAL;
#elif FILE_TYPE == BINARY_MODE
  FIleTypes fileType = FileTypes::BINARY
#endif

#if FILE_MODE == APPEND_MODE
  FileModes fileMode = FileModes::APPEND;
#elif FILE_MODE == TRUNCATE_MODE
  FileModes fileMode = FileModes::TRUNCATE;
#endif

  thread writerTask(writerFuncText, TXT_FILE_NAME, fileType, fileMode);
  std::this_thread::sleep_for(std::chrono::milliseconds(1));
  thread readerTask(readerFuncText, TXT_FILE_NAME, fileType, fileMode);
  readerTask.join();
  writerTask.join();

  remove(TXT_FILE_NAME.c_str());
  remove(BIN_FILE_NAME.c_str());
  std::cout << "Finished." << std::endl;
}
