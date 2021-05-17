#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "ptsh.h"

#define DEFAULT_CONFIG_PATH "/.local/share/ptSh/config"
#define CONFIG_PATH "/.config/ptSh/config"

void setPattern(char** destination, char* line, const char* pattern)
{
  if(strlen(line) < strlen(pattern) +4) return; 

  for(int i = 0; i < strlen(pattern); i++)
    if(line[i] != pattern[i]) return; 

  char* ptr = &(line[strlen(pattern)]);

  if(*ptr != '=') return;
  ptr++;

  if(!(*ptr == '\'' || *ptr == '\"')) return;
  int length = 0;

  do
  {
    if(ptr == &(line[strlen(line)-1])) return;
 
    ptr++;
    length++; 
  } while(!(*ptr == '\'' || *ptr == '\"'));

  ptr-=length-1;
  length--;

  if(length == 0) return;

  *destination = calloc(1, (length+1) * sizeof(char)); 
  strncpy(*destination, ptr, length);

  int escapeCodes = 0;
  ptr = *destination;

  for(int i = 0; i < strlen(*destination)-3; i++)
  {
    if(
        *ptr == '\\' &&
        *(ptr+1) == 'x' &&
        *(ptr+2) == '1' &&
        (*(ptr+3) == 'B' || *(ptr+3) == 'b'))
      escapeCodes++;
    ptr++;  
  }
  if(escapeCodes > 0)
  {
    int newLength = strlen(*destination) - 3*escapeCodes; 
    char* buff = calloc(1, (newLength+1) * sizeof(char));
    for(int i = 0; i < strlen(*destination); i++)
    {
      ptr = (*destination)+i;
      if(
          *ptr == '\\' &&
          *(ptr+1) == 'x' &&
          *(ptr+2) == '1' &&
          (*(ptr+3) == 'B' || *(ptr+3) == 'b'))
      {
        strcat(buff, "\x1b");
        i+=3;
        continue;  
      }
      strncat(buff, ptr, 1);
    }

    free(*destination);
    *destination = buff;
  }

}
void parseLine(PtShConfig *config, char* line)
{
  setPattern(&config->dirPrefix, line, "DIR_PREFIX");
  setPattern(&config->filePrefix, line, "FILE_PREFIX");
  setPattern(&config->linkPrefix, line, "LINK_PREFIX");
  setPattern(&config->errorPrefix, line, "ERROR_PREFIX");
  setPattern(&config->successPrefix, line, "SUCCESS_PREFIX");
  setPattern(&config->successMessage, line, "SUCCESS_MESSAGE");
  setPattern(&config->dirPrefixEscapeCodes, line, "DIR_PREFIX_ESCAPE_CODES");
  setPattern(&config->filePrefixEscapeCodes, line, "FILE_PREFIX_ESCAPE_CODES");
  setPattern(&config->linkPrefixEscapeCodes, line, "LINK_PREFIX_ESCAPE_CODES");
  setPattern(&config->dirNameEscapeCodes, line, "DIR_NAME_ESCAPE_CODES");
  setPattern(&config->fileNameEscapeCodes, line, "FILE_NAME_ESCAPE_CODES");
  setPattern(&config->linkNameEscapeCodes, line, "LINK_NAME_ESCAPE_CODES");
  setPattern(&config->errorPrefixEscapeCodes, line, "ERROR_PREFIX_ESCAPE_CODES");
  setPattern(&config->errorMessageEscapeCodes, line, "ERROR_MESSAGE_ESCAPE_CODES");
  setPattern(&config->successPrefixEscapeCodes, line, "SUCCESS_PREFIX_ESCAPE_CODES");
  setPattern(&config->successMessageEscapeCodes, line, "SUCCESS_MESSAGE_ESCAPE_CODES");
}

void readData(PtShConfig *config, char* path)
{
  FILE *f = fopen(path, "r");

  if(f == NULL) return;

  char buff[1000];
  while(fgets(buff, 1000, f))
  {
    parseLine(config, &buff[0]); 
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

  char* homePath = getenv("HOME");
  char* defaultPath = (char*)calloc(1,strlen(homePath) + strlen(DEFAULT_CONFIG_PATH) +1);
  strcpy(defaultPath, homePath);
  strcat(defaultPath, DEFAULT_CONFIG_PATH);

  char* path = (char*)calloc(1,strlen(homePath) + strlen(CONFIG_PATH) +1);
  strcpy(path, homePath);
  strcat(path, CONFIG_PATH);

  readData(config, defaultPath);
  readData(config, path);

  free(defaultPath);
  free(path);
  
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


char *getPrefix(PtShConfig *config, struct stat *stats)
{
  if(S_ISDIR(stats->st_mode))
  {
    if(config->dirPrefix != NULL) return config->dirPrefix;
    else return "";
  }
  if(S_ISLNK(stats->st_mode))
  {
    if(config->linkPrefix != NULL) return config->linkPrefix;
    else return "";
  }
  if(config->filePrefix != NULL) return config->filePrefix;
  else return "";
}
char *getPrefixEscapeCodes(PtShConfig *config, struct stat *stats)
{
  if(S_ISDIR(stats->st_mode))
  {
    if(config->dirPrefixEscapeCodes != NULL) return config->dirPrefixEscapeCodes;
    else return "";
  }
  if(S_ISLNK(stats->st_mode))
  {
    if(config->linkPrefixEscapeCodes != NULL) return config->linkPrefixEscapeCodes;
    else return "";
  }
  if(config->filePrefixEscapeCodes != NULL) return config->filePrefixEscapeCodes;
  else return "";

}
char *getNameEscapeCodes(PtShConfig *config, struct stat *stats)
{
  if(S_ISDIR(stats->st_mode))
  {
    if(config->dirNameEscapeCodes != NULL) return config->dirNameEscapeCodes;
    else return "";
  }
  if(S_ISLNK(stats->st_mode))
  {
    if(config->linkNameEscapeCodes != NULL) return config->linkNameEscapeCodes;
    else return "";
  }
  if(config->fileNameEscapeCodes != NULL) return config->fileNameEscapeCodes;
  else return "";

}
