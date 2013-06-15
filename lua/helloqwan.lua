local socket = require "socket"
local http = require "socket.http"

local host = "http://192.168.1.168"
--print('socket ',tostring(socket))

local file = "/repl"
local url = host..":8085"..file

--url = "http://www.cs.princeton.edu/~diego/professional/luasocket/http.html"
--url = "http://httpbin.org/post"
local request_body="{var='kyletest'}"
local resp = {}
print('making request')
local myresp,myresp2,myresp3,myrespd4 = http.request{
	url=url,
	method="POST",
        headers =
        {
        --      ["Content-Type"] = "application/json",
                ["Content-Length"] = #request_body,
	--	["set-cookie"] = "var=thisismycookiedata"
        },
        source = ltn12.source.string(request_body),

	sink = ltn12.sink.table(resp)
}
print('done request')


print('response group: ',tostring(myresp),myresp2,myresp3,myresp4)
print('resp sink: '..table.concat(resp))
