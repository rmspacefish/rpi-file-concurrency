#ifndef DEF_H_
#define DEF_H_

#include <string>

// Definitions

enum class FileTypes {
  BINARY,
  TEXTUAL
};

enum class FileModes {
  TRUNCATE,
  APPEND
};

struct CriticalBlob {
  uint8_t someBool;
  uint32_t someCounter;
  char someName[32];
  int32_t someSignedNumber;
};

static const std::string TXT_FILE_NAME = "example.txt";
static const std::string BIN_FILE_NAME = "example.bin";

#endif /* DEF_H_ */
