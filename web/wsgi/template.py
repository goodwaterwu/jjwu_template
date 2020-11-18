#!/usr/bin/python3
#-*- coding: utf-8 -*-

from urllib import parse

REMOTE_ADDR = None
REQUEST_METHOD = None
dict_request = None
html = None

def parseHttpRequest(environ):
	'''Separate HTTP request string into dictionary of {key: value}'''
	global REMOTE_ADDR
	global REQUEST_METHOD
	global dict_request

	REMOTE_ADDR = environ.get('REMOTE_ADDR')	
	REQUEST_METHOD = environ.get('REQUEST_METHOD')	

	if REQUEST_METHOD == 'GET':
		dict_request = parse.parse_qs(environ.get('QUERY_STRING'))
	elif REQUEST_METHOD == 'POST':
		CONTENT_LENGTH = environ.get('CONTENT_LENGTH')
		
		if CONTENT_LENGTH != None:
			query_string_length = int(CONTENT_LENGTH)
			dict_request = {key.decode('utf-8'): [element.decode('utf-8') for element in value] for key, value in parse.parse_qs(environ.get('wsgi.input').read(query_string_length)).items()}
	else:
		pass

def setResponseBody():
	'''Set HTML'''
	global html
	html = '''
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
	'''

def application(environ, start_response):
	'''WSGI application interface'''
	parseHttpRequest(environ)
	setResponseBody()
	response_body = html.encode('utf-8')
	status = '200 OK'
	headers = [('Content-type', 'text/html'), ('Content-Length', str(len(response_body)))]
	start_response(status, headers)

	return [response_body]
