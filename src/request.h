#define SetQueryType 0x01
#define GetQueryType 0x02
#define DelQueryType 0x04

struct Query {
	char type;
	unsigned keySize;
	unsigned valSize; // 0 when query type is Get or Del
	char* key;
	char* val; // NULL when query type is Get or Del
	char isError; // 1 when error
};

// TODO: think about parsing multiple buf (bigger than BUF_SIZE)
struct Query* parseRequest(char* buf);