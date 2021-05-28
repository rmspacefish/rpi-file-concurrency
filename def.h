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

static const std::string TXT_FILE_NAME = "example.txt";
static const std::string BIN_FILE_NAME = "example.bin";

#endif /* DEF_H_ */
