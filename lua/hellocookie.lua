local serpent = require 'serpent'
local https = require 'ssl.https'
local ltn12 = require 'ltn12'

--Get credentials from local file
io.input("/home/kgee/fauxcred")
url = io.read("*line")
username = io.read("*line")
password = io.read("*line")
local request_body = "cn="..username.."&cv="..password
local cookie = "MYCOOKIE1"

--SSL Parameters
local params = {
	mode = "client",
	protocol = "tlsv1",
	verify = "none",
	options = "all",
	redirect = "true"
}

print(request_body)

--Make the HTTP POST call
local resp = {}
local r,response_code,header,status = https.request{
	url = "https://"..url,
	sink = ltn12.sink.table(resp),
	method = "POST",
	headers = 
	{
		["Content-Type"] = "application/x-www-form-urlencoded",
		["Content-Length"] = #request_body,
		["Cookie"] = cookie
	},
	source = ltn12.source.string(request_body),
	protocol = "tlsv1"
}

--Dump result to console
print('content: '..serpent.dump(resp))
print('status: '..status)
print('response: '..r)
print('response code: '..response_code)
print('header: '..serpent.dump(header))
