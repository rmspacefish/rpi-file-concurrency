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

static const std::string TEXT_FILE_NAME = "example.txt";

#endif /* DEF_H_ */
