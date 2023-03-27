#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>

#include "response.h"

char* encodeQueryWithValue(unsigned valSize, char* val) {
  unsigned size = 1 + 4 + valSize;
  char* response = malloc(size);
  char* cursor = response;

  char success = 0x01;
  unsigned tmp;

  memset(response, 0, size);

  memcpy(cursor, &success, sizeof(char)); cursor += sizeof(char);
  tmp = htonl(valSize);
  memcpy(cursor, &tmp, sizeof(unsigned)); cursor += sizeof(unsigned);
  memcpy(cursor, val, valSize);

  return response;
}

char* encodeQuery() {
  unsigned size = 1;
  char* response = malloc(size);
  char success = 0x01;

  memset(response, 0, size);
  memcpy(response, &success, sizeof(char));

  return response;
}

char* encodeError(unsigned errorCode, unsigned messageSize, char* message) {
  unsigned size = 1 + 4 + 4 + messageSize;
  char* response = malloc(size);
  char* cursor = response;

  char success = 0x01;
  unsigned tmp;

  memset(response, 0, size);

  memcpy(cursor, &success, sizeof(char)); cursor += sizeof(char);
  tmp = htonl(errorCode);
  memcpy(cursor, &tmp, sizeof(unsigned)); cursor += sizeof(unsigned);
  tmp = htonl(messageSize);
  memcpy(cursor, &tmp, sizeof(unsigned)); cursor += sizeof(unsigned);
  memcpy(cursor, message, messageSize);

  return response;
}