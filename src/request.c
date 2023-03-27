#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>

#include "request.h"

struct Query* parseRequest(char* buf) {
	struct Query* query = malloc(sizeof(struct Query));
	memset(query, 0, sizeof(struct Query));

	char* cursor = buf;
	unsigned tmp;
	memcpy(&query->type, cursor, 1); cursor += 1;

	if (query->type == SetQueryType) {
		memcpy(&tmp, cursor, sizeof(unsigned)); cursor += sizeof(unsigned);
		query->keySize = ntohl(tmp);
		memcpy(&tmp, cursor, sizeof(unsigned)); cursor += sizeof(unsigned);
		query->keySize = ntohl(tmp);
		memcpy(&query->key, cursor, query->keySize); cursor += query->keySize;
		memcpy(&query->val, cursor, query->valSize);
	} else if (query->type & (GetQueryType | SetQueryType )) {
		memcpy(&tmp, cursor, sizeof(unsigned)); cursor += sizeof(unsigned);
		query->keySize = ntohl(tmp);
		memcpy(&query->key, cursor, query->keySize); cursor += query->keySize;
	} else {
		query->isError = 0x01;
	}

	return query;
}