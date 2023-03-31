#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <arpa/inet.h>

#include "request.h"
#include "logger.h"

struct Query* parseRequest(uint8_t* buf) {
	struct Query* query = malloc(sizeof(struct Query));
	memset(query, 0, sizeof(struct Query));

	uint8_t* cursor = buf;
	uint32_t tmp = 0;
	memcpy(&tmp, cursor, 1); cursor += 1;
  query->type = ntohl(tmp); tmp = 0;

	if (query->type == SetQueryType) {
		memcpy(&tmp, cursor, sizeof(unsigned)); cursor += sizeof(unsigned);
		query->keySize = ntohl(tmp); tmp = 0;
		memcpy(&tmp, cursor, sizeof(unsigned)); cursor += sizeof(unsigned);
		query->keySize = ntohl(tmp); tmp = 0;
		memcpy(&query->key, cursor, query->keySize); cursor += query->keySize;
		memcpy(&query->val, cursor, query->valSize);
	} else if (query->type & (GetQueryType | SetQueryType )) {
		memcpy(&tmp, cursor, sizeof(unsigned)); cursor += sizeof(unsigned);
		query->keySize = ntohl(tmp); tmp = 0;
		memcpy(&query->key, cursor, query->keySize); cursor += query->keySize;
	} else {
		query->isError = 0x01;
	}

  logger(INFO, "query type: %d, keySize: %d, valSize: %d, isError: %d", query->type, query->keySize, query->valSize, query->isError);
	return query;
}