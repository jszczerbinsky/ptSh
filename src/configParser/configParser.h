#include <sys/stat.h>

#ifndef PTSH_H
#define PTSH_H

typedef struct PtShConfig_{
  
  //Prefixes
  char* dirPrefix;
  char* filePrefix;
  char* linkPrefix;
  char* errorPrefix;
  char* successPrefix;

  //Messages
  char* successMessage;

  //Escape codes
  char* dirPrefixEscapeCodes;
  char* filePrefixEscapeCodes;
  char* linkPrefixEscapeCodes;
  char* dirNameEscapeCodes;
  char* fileNameEscapeCodes;
  char* linkNameEscapeCodes;
  char* errorPrefixEscapeCodes;
  char* errorMessageEscapeCodes;
  char* successPrefixEscapeCodes;
  char* successMessageEscapeCodes;

  //ptpwd
  char* pwdDirSeparator;
  char* pwdDirSeparatorEscapeCodes;
  char* pwdNextline;
  char* pwdNextlineMargin;
  char* pwdShowDirPrefix;
} PtShConfig;

typedef enum FileType_{
  FT_Directory,
  FT_Link,
  FT_File
} FileType;

typedef struct FileConfigValues_{
  char* prefixEscapeCodes; 
  char* prefix;
  char* nameEscapeCodes;
} FileConfigValues;

PtShConfig *readConfig();
void closeConfig(PtShConfig *config);

char *getValueStr(char* field);
int getValueInt(char* field);

FileConfigValues *getFileConfigValues(PtShConfig *config, FileType type);

#endif
