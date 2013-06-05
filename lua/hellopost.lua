local serpent = require 'serpent'
local https = require 'ssl.https'
local ltn12 = require 'ltn12'

local host = "httpbin.org"
local port = 443
local file = "/post"

local params = {
	mode = "client",
	protocol = "tlsv1",
	verify = "none",
	options = "all",
	redirect = "false"
}

local resp = {}
local request_body = [[login=user&password=123]]

local r,response_code,header,status = https.request{
	url = "https://"..host..file,
	sink = ltn12.sink.table(resp),
	method = "POST",
	headers = 
	{
		["Content-Type"] = "application/x-www-form-urlencoded",
		["Content-Length"] = #request_body
	},
	source = ltn12.source.string(request_body),
	protocol = "tlsv1"
}
print('content: '..serpent.dump(resp))
print('status: '..status)
print('response: '..r)
print('response code: '..response_code)
print('header: '..serpent.dump(header))
