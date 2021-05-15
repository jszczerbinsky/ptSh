#include <stdlib.h>
#include <stdio.h>
#include "ptsh.h"

#define DEFAULT_CONFIG_PATH "~/.local/share/ptSh/config"
#define CONFIG_PATH "~/.config/ptSh/config"

void readData(PtShConfig *config, const char* path)
{
  FILE *f = fopen(path, "r");

  if(f == NULL) return;

  char buff[1000];

  while(fscanf(f, "%[^\n]", buff) != EOF)
  {
    printf("%s\n", buff); 
  }
}

PtShConfig *readConfig()
{
  PtShConfig *config = (PtShConfig*)malloc(sizeof(PtShConfig));
  config->dirPrefix = NULL;
  config->filePrefix = NULL;
  config->linkPrefix = NULL;
  config->errorPrefix = NULL;
  config->successPrefix = NULL;
  config->successMessage = NULL;
  config->dirPrefixEscapeCodes = NULL;
  config->filePrefixEscapeCodes = NULL;
  config->linkPrefixEscapeCodes = NULL;
  config->dirNameEscapeCodes = NULL;
  config->fileNameEscapeCodes = NULL;
  config->linkNameEscapeCodes = NULL;
  config->errorPrefixEscapeCodes = NULL;
  config->errorMessageEscapeCodes = NULL;
  config->successPrefixEscapeCodes = NULL;
  config->successMessageEscapeCodes = NULL;

  readData(config, DEFAULT_CONFIG_PATH);
  readData(config, CONFIG_PATH);
  
  return config;
}

void closeConfig(PtShConfig *config)
{
  free(config->dirPrefix);
  free(config->filePrefix);
  free(config->linkPrefix);
  free(config->errorPrefix);
  free(config->successPrefix);
  free(config->successMessage);
  free(config->dirPrefixEscapeCodes);
  free(config->filePrefixEscapeCodes);
  free(config->linkPrefixEscapeCodes);
  free(config->dirNameEscapeCodes);
  free(config->fileNameEscapeCodes);
  free(config->linkNameEscapeCodes);
  free(config->errorPrefixEscapeCodes);
  free(config->errorMessageEscapeCodes);
  free(config->successPrefixEscapeCodes);
  free(config->successMessageEscapeCodes);
  free(config);
}
