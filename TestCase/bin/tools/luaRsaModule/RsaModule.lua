#!/usr/sbin/lua
require  ("csrsa")
--local f = package.loadlib ("./csrsa.so", "luaopen_csrsa")
--f()

local function hexstring2byte(str)

	str = string.gsub(str,"%W","")
	--print(str)

	local newstr = ""

	local len = string.len(str)
	local j = 1

	if len % 2 ~= 0 then
		local tmp = string.char(tonumber("0x"..string.sub(str,1,1)))
		j = 2
		newstr = newstr..tmp
	end

	for i=j,len,2 do
		local tmp = string.char(tonumber("0x"..string.sub(str,i,i+1)))
		newstr = newstr..tmp
	end

	return newstr
end

local privkey = {}

--[=[ ]=]
privkey["bits"] = 1024

privkey["modulus"] = hexstring2byte([[
	b5:97:ff:fb:ee:8d:e6:32:2f:99:24:f0:d4:1f:
	9f:ec:46:a6:93:c4:b6:bf:b6:8a:f5:df:86:8d:2f:
	1b:47:d7:ee:72:8b:e4:d6:13:db:1a:74:f4:aa:53:
	13:4f:0c:78:fb:3f:6d:f6:93:77:38:b2:26:fb:a6:
	74:d6:0e:bd:3a:e8:a0:87:bd:1f:86:2b:98:f7:ec:
	c3:d8:91:80:52:38:51:27:fd:59:b9:22:5f:cc:6e:
	51:95:2f:cf:48:f7:27:89:51:29:98:02:22:85:f6:
	8e:4e:d8:fc:a2:6b:c4:85:d1:2c:93:b5:e1:63:da:
	d5:42:22:b6:f0:7e:45:21:1b
]])

privkey["publicExponent"] = hexstring2byte([[10001]])
privkey["privateExponent"] = hexstring2byte([[
    23:21:02:6e:34:19:d3:3c:46:24:f4:16:7a:92:9b:
    97:53:9b:89:de:76:13:e4:36:41:96:3a:2b:39:bf:
    a7:48:48:62:74:59:92:68:48:ec:77:4f:95:5b:78:
    d2:c3:f0:eb:79:bb:c6:05:84:fc:bc:c9:f9:c8:9b:
    b5:85:28:69:d7:20:3c:1e:a4:b6:52:f4:6f:74:bc:
    e5:43:d6:61:90:7a:15:5f:31:0f:6a:fa:13:21:29:
    d6:31:1d:ac:d5:5b:71:6c:62:ab:58:61:a6:03:1c:
    57:44:8f:f0:47:c4:af:db:24:b5:b8:22:f3:c8:54:
    c2:b0:37:fe:f2:7d:b6:71
]])

privkey["prime1"] = hexstring2byte([[
    ed:d5:b8:c8:10:80:aa:0b:dc:ab:f8:42:83:b3:
    89:04:b1:25:6d:61:2a:83:ef:0e:4a:5d:e9:fb:6a:
    a6:1a:b4:4f:47:40:c2:5f:ef:68:d8:27:d7:94:8c:
    e3:14:4c:48:3b:eb:f8:40:cb:88:98:5a:c9:57:03:
    14:be:15:ba:1d
]])

privkey["prime2"] = hexstring2byte([[
    c3:76:9f:00:ac:38:70:2b:9b:e5:50:fa:3b:ed:
    3a:24:15:7e:27:a5:e0:7a:77:ba:28:5f:e9:d2:8b:
    0b:12:e2:25:f8:8a:c8:91:6b:7b:af:55:a8:7a:72:
    7a:01:b6:85:8d:68:01:69:85:90:13:d2:a6:e5:40:
    9d:df:06:a2:97
]])

privkey["exponent1"] = hexstring2byte([[
    af:ff:14:c3:2f:bb:16:9a:ba:8c:a3:a5:0c:f2:
    a1:75:50:65:82:e0:81:66:aa:53:67:e2:c1:50:24:
    66:90:a2:57:7c:41:c6:d4:d9:44:3b:98:98:26:3c:
    6f:99:c8:ad:37:72:71:fd:f8:df:b6:e6:8f:d1:8c:
    dd:1f:a0:d2:5d
]])

privkey["exponent2"] = hexstring2byte([[
    c0:17:d8:c5:99:7b:51:c9:99:c9:30:de:eb:7c:
    05:72:a6:e8:b7:60:4e:8f:2d:10:f7:52:e3:81:76:
    02:69:09:75:95:a0:16:e2:cb:45:df:97:88:1e:e6:
    58:3f:4c:b8:4d:b3:14:08:37:29:d4:41:66:a1:d0:
    f2:15:d7:f8:7b
]])

privkey["coefficient"] = hexstring2byte([[
    72:9f:3e:70:de:c6:44:1e:aa:26:c3:ba:6c:f9:bf:
    45:5d:b7:bd:7c:ca:b8:84:52:65:f2:75:2d:cc:60:
    be:c3:29:39:c6:d1:78:ed:bd:aa:a5:3c:bf:a0:7e:
    55:f4:4e:e6:27:96:ff:d9:f3:78:5f:4a:76:27:8a:
    1d:ef:f8:21
]])
--[=[ ]=]

--[=[
privkey["bits"] = 2048

privkey["modulus"] = hexstring2byte([[
	b7:b7:a4:a3:00:89:f4:05:91:ff:37:5b:b3:7d:
    ff:de:28:c7:c0:8e:3c:08:31:5d:d4:ab:5f:df:ba:
    f5:22:4e:93:05:44:2f:16:a3:51:43:44:0f:30:fa:
    43:5b:bb:23:02:8d:94:62:69:12:19:3a:e7:17:70:
    6a:23:8a:d9:25:42:ed:bd:2e:8d:38:ef:1d:ca:2e:
    6f:41:1c:3a:60:44:09:4e:d1:65:c7:e1:1d:69:3a:
    71:20:4f:2a:c8:4e:15:ea:59:ec:db:b4:02:13:47:
    9c:1d:fd:de:71:5a:c5:ba:ff:53:c2:9b:30:e8:ee:
    ab:69:17:30:8b:f0:50:91:49:c1:f1:60:09:64:c0:
    b9:09:a5:7f:30:f0:9a:53:b0:06:86:cc:81:98:08:
    78:b8:66:5e:78:c2:e2:1b:f6:f1:cb:47:4a:97:f0:
    af:77:6b:a6:fd:68:64:27:4e:e0:b4:22:4b:ae:a7:
    89:fa:3d:36:01:e1:8e:2c:bf:ef:a4:b5:00:e1:ba:
    5f:b8:a0:3d:f0:d0:c3:fa:fe:ff:29:f1:35:36:4b:
    2a:1e:2a:77:f2:cb:75:c2:e3:20:7b:66:d6:1e:b7:
    ab:fd:ed:2c:1d:f3:b5:16:32:7b:ed:d8:cc:e1:0d:
    41:af:03:f3:3a:18:dc:12:11:25:35:14:dc:73:ce:
    22:71
]])

privkey["publicExponent"] = hexstring2byte([[10001]])
privkey["privateExponent"] = hexstring2byte([[
    0b:6e:83:f2:18:a0:7c:ec:b3:ce:fb:b4:db:10:66:
    11:7e:dc:1f:21:3a:9f:0f:e3:54:ce:6d:22:d0:d1:
    5c:17:23:92:da:51:4b:c9:51:cf:2b:b6:fe:47:34:
    55:ca:e9:3c:c9:6e:88:49:1d:75:30:73:1c:c4:43:
    f6:7e:37:50:d8:3f:4e:dc:4c:b1:54:3f:b8:4a:f7:
    65:47:ca:62:1f:19:ad:2b:de:77:77:8c:8a:f5:b6:
    00:5a:ac:c0:d6:0b:0b:10:19:6a:fa:62:7e:ac:a0:
    0c:45:b5:c7:bb:ec:3d:dc:91:6b:b4:98:70:86:19:
    68:6a:a6:fc:ae:31:75:38:d4:a2:7b:e3:a4:5c:bb:
    da:d0:44:92:51:db:2a:98:d2:04:6c:75:95:ec:5e:
    7d:32:c7:ea:73:42:15:b8:98:f7:be:e3:c2:73:ce:
    13:ec:2a:a6:81:5f:c6:aa:d8:ed:73:3a:56:ac:a4:
    f2:7f:e4:c7:bd:38:7a:39:4d:6e:45:2d:f6:3f:14:
    53:7f:5b:26:8f:6b:fd:24:4a:a6:3a:31:51:c3:32:
    ac:c9:79:2b:40:a3:4a:69:f7:d9:51:4a:37:52:63:
    5d:13:0d:b6:3f:10:93:35:93:8b:8f:69:23:5a:a6:
    c5:2e:7d:c9:bb:05:e1:19:50:0f:ef:a6:26:6c:99:
    d9
]])

privkey["prime1"] = hexstring2byte([[
    eb:3e:00:3d:44:60:2d:8b:b2:b3:ec:f2:00:4e:
    ff:ce:0c:b2:76:70:43:db:a5:fe:a6:38:a7:dd:4a:
    4d:c6:00:16:47:0f:09:3a:08:22:ae:d4:18:c0:8a:
    f2:47:ee:50:c5:75:f8:ee:33:68:6a:b4:f1:a4:11:
    53:61:32:46:0f:1c:d2:37:8e:d0:ec:e8:da:1e:44:
    5b:c1:56:93:45:7b:58:99:3f:f1:8e:1d:9d:69:44:
    e8:f7:dd:86:9f:77:00:60:72:a4:4e:a0:43:b8:ec:
    03:73:44:f4:59:b4:72:90:ec:ec:0b:4d:43:46:54:
    44:64:f9:bb:89:0a:92:6a:b3
]])

privkey["prime2"] = hexstring2byte([[
    c7:ed:b9:0e:87:0e:0b:34:de:9e:68:20:68:45:
    79:50:e1:a6:e9:7b:c3:d5:98:ea:b9:ce:28:0a:e8:
    d8:55:d4:fe:52:6c:2e:1c:6f:85:52:e4:a1:fb:be:
    d9:6b:ae:54:9d:13:f5:91:a9:25:f2:1e:7b:a3:d5:
    41:23:20:89:0b:6a:f2:4a:67:19:59:f3:56:48:84:
    c6:a9:03:a6:c8:f0:05:ff:bb:2f:1f:85:f8:07:07:
    00:2b:2c:2e:15:b8:e0:ef:6c:3f:46:1e:98:94:b1:
    64:b8:fd:13:dd:57:ce:a9:3c:4d:34:5f:49:5e:3d:
    69:3d:b4:86:44:97:63:a0:4b
]])

privkey["exponent1"] = hexstring2byte([[
    b8:55:3b:26:c7:1f:7e:51:0b:55:06:04:93:cd:
    53:91:88:4d:87:95:6c:f6:fe:d9:ce:32:73:07:6a:
    24:0b:55:22:1f:c8:5e:78:84:cb:78:a7:66:45:ea:
    1a:f3:d6:80:a0:5a:53:17:84:5c:fb:d0:91:d0:f8:
    6a:97:ad:27:4c:cc:4e:c5:8e:39:94:8a:d9:f0:01:
    ac:cc:91:24:91:bb:97:af:6e:75:bf:e0:e0:ea:0e:
    3f:67:8a:c4:e4:cc:01:15:92:7f:c1:c4:bf:8c:76:
    14:36:0d:00:54:12:9b:fb:88:4b:66:87:c2:81:a7:
    a2:78:65:60:58:80:5a:3f:15
]])

privkey["exponent2"] = hexstring2byte([[
    29:cb:ec:05:3a:7c:d8:20:ec:6d:87:6d:46:ff:47:
    36:7e:86:4b:87:a1:50:55:50:b5:19:81:4b:8e:94:
    33:95:7c:b9:b4:7e:0b:59:d1:30:9c:35:b5:10:98:
    19:a7:ef:6e:8b:4e:aa:6e:07:4d:cf:cb:9f:5b:a4:
    eb:78:10:ce:7f:44:3a:03:c5:cc:05:50:43:86:e6:
    95:4c:fb:20:44:f5:ac:19:f0:e7:11:35:e2:1a:37:
    91:cf:01:af:6e:e0:2f:8d:9c:c8:c6:fa:b5:13:44:
    f0:5b:90:3c:94:ef:d2:77:95:d8:5e:17:53:89:59:
    28:1a:46:7b:ae:7e:98:73
]])

privkey["coefficient"] = hexstring2byte([[
    b4:0c:eb:41:5b:4d:3d:77:7e:32:c6:d8:51:38:
    1c:b6:e6:c2:9f:5e:46:c1:df:1c:69:f5:11:ee:ba:
    db:18:07:65:b0:7d:6b:04:96:aa:9b:df:7a:08:2c:
    a4:b9:f3:06:59:97:55:ea:0c:46:00:5b:1b:97:65:
    26:fe:88:ec:05:9d:7f:ba:7f:c7:6f:1e:98:c6:cf:
    ec:db:c2:78:78:b1:e0:f0:0c:1a:8c:31:e7:38:f0:
    63:84:dc:ba:93:84:ad:51:ef:27:d1:ed:71:17:44:
    e3:b0:80:08:11:52:76:e1:97:88:a0:7a:d7:0d:8b:
    24:a3:55:83:a6:6f:1f:f3:47
]])
]=]

local seed = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"
local seedlen = string.len(seed)
math.randomseed(os.time())

local testtimes = 1


for j=1,testtimes do

	local len = math.random(14,20)
	local testcase = ""
	for i=1,len,1 do
		local index = math.random(1,seedlen)
		testcase = testcase .. string.sub(seed,index,index)
	end

	--print(testcase)

	local c = csrsa.privateEncrypt(testcase,privkey)

	--print(string.len(c))
	local m = csrsa.publicDecrypt(c,privkey)

	print(m)

	assert(testcase == m)

end
