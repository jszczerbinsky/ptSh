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
} PtShConfig;

PtShConfig *readConfig();
void closeConfig(PtShConfig *config);

#endif
