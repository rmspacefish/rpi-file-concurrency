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

size_t readOpenFailCount = 0;
size_t readCount = 0;
size_t readLineOne = 0;
size_t readBothLines = 0;
size_t faultyReadCount = 0;
size_t goodFileCount = 0;
size_t badFileEofCount = 0;
size_t badFileFailCount = 0;

// Code

int main() {
  using namespace std;
  cout << "Concurrent file access test" << endl;
  int result = remove(TEXT_FILE_NAME.c_str());
  if(result == 0) {
    cout << "Removed existing example file" << endl;
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

  thread writerTask(writerFuncText, TEXT_FILE_NAME, fileType, fileMode);
  std::this_thread::sleep_for(std::chrono::milliseconds(1));
  thread readerTask(readerFuncText, TEXT_FILE_NAME, fileType, fileMode);
  readerTask.join();
  writerTask.join();

  remove(TEXT_FILE_NAME.c_str());
  std::cout << "Finished." << std::endl;
}
