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

  if(strlen(*destination) < 4) return;

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
  setPattern(&config->pwdDirSeparator, line, "PWD_DIR_SEPARATOR");
  setPattern(&config->pwdDirSeparatorEscapeCodes, line, "PWD_DIR_SEPARATOR_ESCAPE_CODES");
  setPattern(&config->pwdNextline, line, "PWD_NEXTLINE");
  setPattern(&config->pwdNextlineMargin, line, "PWD_NEXTLINE_MARGIN");
  setPattern(&config->pwdShowDirPrefix, line, "PWD_SHOW_DIR_PREFIX");
  
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
  PtShConfig *config = (PtShConfig*)calloc(1, sizeof(PtShConfig));

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
  free(config->pwdDirSeparator);
  free(config->pwdDirSeparatorEscapeCodes);
  free(config->pwdNextline);
  free(config->pwdNextlineMargin);
  free(config);
}

char *getValueStr(char* field)
{
  if(field == NULL) return "";
  return field;
}

int getValueInt(char* field)
{
  for(int i = 0; i < strlen(field); i++)
    if(field[i] < 48 || field[i] > 57)
      return 0;
  return atoi(field);
}

FileConfigValues *getFileConfigValues(PtShConfig *config, FileType type)
{
  FileConfigValues *val = calloc(1, sizeof(FileConfigValues));
  switch(type)
  {
    case FT_Directory:
      val->prefixEscapeCodes = getValueStr(config->dirPrefixEscapeCodes);
      val->prefix = getValueStr(config->dirPrefix);
      val->nameEscapeCodes = getValueStr(config->dirNameEscapeCodes);
      break;
    case FT_Link:
      val->prefixEscapeCodes = getValueStr(config->linkPrefixEscapeCodes);
      val->prefix = getValueStr(config->linkPrefix);
      val->nameEscapeCodes = getValueStr(config->linkNameEscapeCodes);
      break;
    case FT_File:
      val->prefixEscapeCodes = getValueStr(config->filePrefixEscapeCodes);
      val->prefix = getValueStr(config->filePrefix);
      val->nameEscapeCodes = getValueStr(config->fileNameEscapeCodes);
  }

  return val;
}
