local serpent = require 'serpent'
local https = require 'ssl.https'
local ltn12 = require 'ltn12'

local host = "www.google.ca"
local port = 443
local file = "/"

local params = {
	mode = "client",
	protocol = "tlsv1",
	verify = "none",
	options = "all",
	redirect = "false"
}

local resp = {}
local r,response_code,header,status = https.request{
	url = "https://"..host..file,
	sink = ltn12.sink.table(resp),
	protocol = "tlsv1"
}
print('content: '..serpent.dump(resp))
print('status: '..status)
print('response: '..r)
print('response code: '..response_code)
print('header: '..serpent.dump(header))
