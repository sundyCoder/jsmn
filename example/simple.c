#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../jsmn.h"

/*
 * A small example of jsmn parsing when JSON structure is known and number of
 * tokens is predictable.
 */
static const char *JSON_STRING = "{\"result\":0, \"data\":{ \"tmcStatus\":\"11100000\", \"srvTime\":\"20161230230849\" } }";

static int jsoneq(const char *json, jsmntok_t *tok, const char *s) {
	if (tok->type == JSMN_STRING && (int) strlen(s) == tok->end - tok->start &&
			strncmp(json + tok->start, s, tok->end - tok->start) == 0) {
		return 0;
	}
	return -1;
}

int main() {
	int i;
	int r;
        int result;
	char tmcStatus[8];
	char srvTime[14];

	jsmn_parser p;
	jsmntok_t t[128]; /* We expect no more than 128 tokens */

	jsmn_init(&p);
	r = jsmn_parse(&p, JSON_STRING, strlen(JSON_STRING), t, sizeof(t)/sizeof(t[0]));
	if (r < 0) {
		printf("Failed to parse JSON: %d\n", r);
		return 1;
	}

	/* Assume the top-level element is an object */
	if (r < 1 || t[0].type != JSMN_OBJECT) {
		printf("Object expected\n");
		return 1;
	}

	/* Loop over all keys of the root object */
	for (i = 1; i < r; i++) {
		if (jsoneq(JSON_STRING, &t[i], "result") == 0) {
			char tmp[1];
			sprintf(tmp,"%.*s",t[i+1].end-t[i+1].start,JSON_STRING + t[i+1].start);
			result = tmp[0] - '0';
			printf("result:%d\n",result);
		
			//printf("- result: %s,%s\n", t[i+1].end-t[i+1].start, JSON_STRING + t[i+1].start);
			//sprintf(result,"%.*s\n",t[i+1].end-t[i+1].start, JSON_STRING + t[i+1].start);
			//printf("result: %.*s\n", t[i+1].end-t[i+1].start, JSON_STRING + t[i+1].start);
			//sprintf("result","%.*s\n", t[i+1].end-t[i+1].start, JSON_STRING + t[i+1].start);
			i++;
		} else if (jsoneq(JSON_STRING, &t[i], "tmcStatus") == 0) {
			sprintf(tmcStatus, "%.*s", t[i+1].end-t[i+1].start, JSON_STRING + t[i+1].start);
			printf("tmcStatus:%s\n",tmcStatus);
			i++;
		} else if(jsoneq(JSON_STRING, &t[i], "srvTime") == 0) {
			sprintf(srvTime,"%.*s", t[i+1].end-t[i+1].start, JSON_STRING + t[i+1].start);
			printf("srvTime:%s\n",srvTime);
			i++;
		}
	}
	return EXIT_SUCCESS;
}
