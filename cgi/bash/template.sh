#!/bin/bash

:<<LICENSE
Copyright (c) 2016 WU, JHENG-JHONG <goodwater.wu@gmail.com>

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
LICENSE

# Get HTTP request string
function parseRequestString
{
	if [ -n "${CONTENT_LENGTH}" ]; then
		read QUERY_STRING_POST -n ${CONTENT_LENGTH}
		request=$(echo "${QUERY_STRING_POST}" | awk 'BEGIN { FS="[=&]" } { for(i=1;i<=NF;i++) print $i }')
	else
		request=$(echo "${QUERY_STRING}" | awk 'BEGIN { FS="[=&]" } { for(i=1;i<=NF;i++) print $i }')
	fi
}

# Separate HTTP request string into keys and values
function parseHttpRequest
{
	parseRequestString

	declare -i i=0
	declare -i key_index=0

	for str in ${post_str}
	do
		if [ $((${i}%2)) -eq 0 ]; then
			key[${key_index}]=${str}
		else
			value[${key_index}]=${str}
			key_index=$((${key_index}+1))
		fi

		i=$((${i}+1))
	done
}

echo -en "Content-type: text/html\n\n"

cat << EOF
<!DOCTYPE html>
<html>
<head>
	<meta charset="UTF-8">
	<title>MODIFY_HERE_1</title>
	<!--[if lt IE 9]>
	<script src="http://html5shiv.googlecode.com/svn/trunk/html5.js"></script>
	<![endif]-->
	<link rel="stylesheet" type="text/css" href="MODIFY_HERE_2">
	<script type="text/javascript" src="MODIFY_HERE_3"></script>
	<noscript>Not support javascript</noscript>
</head>
<body>
</body>
</html>
EOF
