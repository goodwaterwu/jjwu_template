/*
 * Copyright (c) 2016 WU, JHENG-JHONG <goodwater.wu@gmail.com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated 
 * documentation files (the "Software"), to deal in the Software without restriction, including without limitation 
 * the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and 
 * to permit persons to whom the Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all copies or substantial portions of 
 * the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO 
 * THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE 
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF 
 * CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS 
 * IN THE SOFTWARE.
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/** Max HTTP request length */
#define MAX_REQUEST_SIZE 2048 // 2KB

/** @brief Structure for HTTP request
 * 
 * Structure for HTTP request
 */
typedef struct HTTP_REQUEST_DATA
{
	char request[MAX_REQUEST_SIZE]; /**< GET/POST request string */
	int key_index[MAX_REQUEST_SIZE]; /**< Array for component name indexes in HTTP request string */
	int value_index[MAX_REQUEST_SIZE]; /**< Array for component value indexes in HTTP request string */
	int count; /**< Amount of {key: value} */
} HttpRequestData;

/** @brief Parse HTTP GET/POST request into {key: value}
 *
 * Parse HTTP GET/POST request into {ikey: value}
 * 
 * @param pData Pointer to struct HTTP_REQUEST_DATA
 * @return void
 */
void parseHttpRequest(HttpRequestData *pData)
{
	char tmp[MAX_REQUEST_SIZE] = {0};
	char *env = getenv("CONTENT_LENGTH");
	int length = (env == NULL)? 0 : atoi(env);

	if (length > 0)
	{
		fgets(tmp, length, stdin);
	}
	else
	{
		env = getenv("QUERY_STRING");

		if (env != NULL)
			memcpy(tmp, env, strlen(env));
	}

	if (tmp != NULL)
	{
		pData->request[0] = '&';
		memcpy(pData->request+1, tmp, MAX_REQUEST_SIZE);
		pData->request[strlen(pData->request)] = '&';

		for (int index = 0 ; index != strlen(pData->request) ; index++)
		{
			if (pData->request[index] == '&')
				pData->key_index[pData->count] = index + 1;

			if (pData->request[index] == '=')
			{
				pData->value_index[pData->count] = index + 1;
				pData->count++;
			}
		}
	}
}

/** @brief Get key by index
 * 
 * Get key by index
 * 
 * @param data Structure HTTP_REQUEST_DATA
 * @param index Key sequence number in key_index
 * @param key Character array for storing a key
 * 
 * @return void
 */
void getRequestKey(HttpRequestData data, int index, char key[])
{
	int offset = data.key_index[index];
	char *pos1 = data.request + offset;
	char *pos2 = strchr(data.request+offset, '=');

	memcpy(key, data.request+offset, pos2-pos1);
}

/** @brief Get value by index
 * 
 * Get value by index
 * 
 * @param data Structure HTTP_REQUEST_DATA
 * @param index Value sequence number in value_index
 * @param value Character array for storing a value
 * 
 * @return void
 */
void getRequestValue(HttpRequestData data, int index, char value[])
{
	int offset = data.value_index[index];
	char *pos1 = data.request + offset;
	char *pos2 = strchr(data.request+offset, '&');

	memcpy(value, data.request+offset, pos2-pos1);
}

/** @brief Get {key: value} by index
 * 
 * Get {key: value} by index
 * 
 * @param data Structure HTTP_REQUEST_DATA
 * @param index Sequence number in key_index and value_index
 * @param key Character array for storing a key
 * @param value Character array for storing a value
 * 
 * @return void
 */
void getRequestPair(HttpRequestData data, int index, char key[], char value[])
{
	getRequestKey(data, index, key);
	getRequestValue(data, index, value);
}

/** @brief C CGI template
 * 
 * C CGI template
 */
int main(int argc, const char *argv[])
{
	HttpRequestData data;

	memset(&data, 0, sizeof(HttpRequestData));

	printf("Content-type: text/html\n\n");
	printf("<!DOCTYPE html>");
	printf("<html>");
	printf("<head>");
	printf("<meta charset=\"UTF-8\">");
	printf("<title>MODIFY_HERE_1</title>");
	printf("<!--[if lt IE 9]>\
			<script src=\"http://html5shiv.googlecode.com/svn/trunk/html5.js\"></script>\
			<![endif]-->");
	printf("<link rel=\"stylesheet\" type=\"text/css\" href=\"MODIFY_HERE_2\">");
	printf("<script type=\"text/javascript\" src=\"MODIFY_HERE_3\"></script>");
	printf("<noscript>Not support javascript</noscript>");
	printf("</head>");
	printf("<body>");
	printf("</body>");
	printf("</html>");

	return 0;
}
