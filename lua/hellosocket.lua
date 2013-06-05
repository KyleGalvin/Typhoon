--require 'socket'
local io = require 'io'
local http = require 'socket.http'
local ltn12 = require 'ltn12'
local ssl = require 'ssl'

-- TLS/SSL client parameters (omitted)
local params = {
	mode = "client",
	protocol = "tlsv1",
	verify = "none",
	options = "all",
}

local conn = socket.tcp()

local host = "https://msp.f-secure.com"
local file = "/web-test/common/test.html"

conn:connect(host,443)
conn = ssl.wrap(conn,params)
conn:dohandshake()

conn:send("GET ".. file .. " HTTP/1.1\n\n")

--local line,err = conn:receive()

local result
while true do

	local s,status = conn:receive()
	if(s) then
		print(s)
		result = result .. s
		count = count+string.len(s)
	end
	if status == "closed" then break end
end
print(result)
conn:close()




--[[
http.request{
	url = host .. file, 
	sink = ltn12.sink.file(io.stdout)
}


local c = assert(socket.connect(host, 80))
c:send("GET " .. file .. " HTTP/1.0\n\n")

local result = ""
local count = 0

print(result,count)
c:close()
]]--
