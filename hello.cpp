#include <iostream>
#include <thread>
#include <fstream>
#include <cstdio>

#define TEXTUAL_MODE    0
#define BINARY_MODE     1

#define FILE_MODE       TEXTUAL_MODE

void readerFuncText();
void writerFuncText();


int main() {
  using namespace std;
  cout << "Concurrent file access test" << endl;
  int result = remove("example.txt");
  if(result == 0) {
    cout << "Removed existing example file" << endl;
  }
  thread writerTask(writerFuncText);
  std::this_thread::sleep_for(chrono::milliseconds(20));
  thread readerTask(readerFuncText);
  readerTask.join();
  writerTask.join();

  std::cout << "Finished." << std::endl;
}

void writerFuncText() {
  using namespace std;
  size_t writeOpenFailCount = 0;
  size_t writeCount = 0;
  size_t writeFailCount = 0;
  auto start = std::chrono::steady_clock::now();
  double elapsedSeconds = 0;
  while(elapsedSeconds < 3.0) {
    ofstream myfile("example.txt", ios::trunc);
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

void readerFuncText() {
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
