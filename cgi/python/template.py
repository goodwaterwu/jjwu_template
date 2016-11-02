#!/usr/bin/python3
# -*- coding: utf-8 -*-

'''
Copyright (c) 2016 WU, JHENG-JHONG <goodwater.wu@gmail.com>

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
'''

import cgi, os

REMOTE_ADDR = None
REQUEST_METHOD = None
dict_request = None

def parseHttpRequest():
	'''Separate HTTP request string into dictionary of {key: value}'''
	global REMOTE_ADDR
	global REMOTE_HOST
	global REQUEST_METHOD
	global dict_request
	REMOTE_ADDR = os.environ.get('REMOTE_ADDR')
	REQUEST_METHOD = os.environ.get('REQUEST_METHOD')
	form = cgi.FieldStorage()
	keys = form.keys()
	dict_request = {key: cgi.escape(form.getvalue(key)) for key in keys}

parseHttpRequest()

print('Content-type: text/html')
print()

print('''
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
''')
