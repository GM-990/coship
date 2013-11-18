#!/usr/sbin/lua

require("base64")
require ("csrsa")

--golble param
local s_inputFile = nil
local s_outputFile = nil
local s_keyFileName = nil
local s_magicCall = false

--const
local c_CLIMagicParam = "\45\77\109\99\112\75\101\121\71\101\110\101\114\97\116\111\114\77\97\103\105\99\87\111\114\100\115"

local function showhelp(argument)
	print(argument[0].." [options]")
	print(" -in arg    input file")
	print(" -out arg   output file,default:mmcp_keyfile.txt")
	print(" -key arg   private key file in pem format")
end

local function parseCLIarg(argument)
	local index = 1

	while argument[index] do
		--print("---"..argument[index])
		if argument[index] == "-in" then
			index = index + 1
			if not argument[index] then
				print(argument[0]..":argument to '-in' is missing")
				showhelp(argument)
				os.exit()
			else
				s_inputFile = argument[index]
			end
		elseif argument[index] == "-out" then
			index = index + 1
			if not argument[index] then
				print(argument[0]..":argument to '-out' is missing")
				showhelp(argument)
				os.exit()
			else
				s_outputFile = argument[index]
			end
		elseif argument[index] == "-key" then
			index = index + 1
			if not argument[index] then
				print(argument[0]..":argument to '-key' is missing")
				showhelp(argument)
				os.exit()
			else
				s_keyFileName = argument[index]
			end
		elseif argument[index] == c_CLIMagicParam then
			s_magicCall = true
		else
			print("unkown argument:"..argument[index])
			showhelp(argument)
			os.exit()
		end

		index = index + 1
	end

	--print("s_magicCall="..tostring(s_magicCall))
	--if s_inputFile then print("s_inputFile="..s_inputFile) end
	--if s_keyFileName then print("s_inputFile="..s_keyFileName) end

	if not s_keyFileName then
		print("no key file input")
		showhelp(argument)
		os.exit()
	end

	if not s_inputFile then
		print("no input file input")
		showhelp(argument)
		os.exit()
	end

	if not s_outputFile then
		s_outputFile = "mmcp_keyfile.txt"
	end
end

local function argument2str(argument)
	local s_thisCmd = ""
	local index = 0

	while argument[index] do
		s_thisCmd = s_thisCmd..argument[index].." "
		index = index + 1
	end

	--if s_thisCmd then print("s_thisCmd="..s_thisCmd) end

	return s_thisCmd
end

local function hexstr2base64(hexstr)
	--print("hexstr="..hexstr)
	return base64.enc(hexstr)
end

local function hexstring2byte(str)
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

local function searchPrivKeys()
	--这里假设了openssl以下几个值出现的顺序
	local privKeys = {}
	local targets = {"modulus","publicExponent","privateExponent","prime1","prime2","exponent1","exponent2","coefficient"}
	local searchingIndex = 1
	local searchData = false

	local needread = true

	--find bits
	local line = io.read()
	--print(line)
	local _,_,bits = string.find(line,"([%d]+)%sbit")
	--print(bits)
	privKeys["bits"] = tonumber(bits)

	while true do
		if needread then
			line = io.read()
		else
			needread = true
		end
		if not line then break end
		--print("11"..line)

		if not searchData then

			local ss,ee = string.find(line,targets[searchingIndex])

			if ee then
				--print(string.len(line))
				--print(ee)
				if string.len(line) > ee+1 then --short key in one line

					local sss,eee,key = string.find(line,"([%d]+)",ee)
					key = string.format("%x",tonumber(key))
					privKeys[targets[searchingIndex]] = key

					searchingIndex = searchingIndex + 1
					if not targets[searchingIndex] then break end
				else
					searchData = true
					privKeys[targets[searchingIndex]] = ""
				end
			end
		else
			if string.sub(line,1,1) == " " then
				privKeys[targets[searchingIndex]] = privKeys[targets[searchingIndex]] .. line
			else
				--privKeys[targets[searchingIndex]] = string.gsub(privKeys[targets[searchingIndex]],"%W","")
				--print(targets[searchingIndex].."="..privKeys[targets[searchingIndex]])
				--print(string.len(privKeys[targets[searchingIndex]]))

				--获取最后一个元素后输入结束，不会到这里来

				needread = false
				searchData = false
				searchingIndex = searchingIndex + 1
				if not targets[searchingIndex] then break end
			end
		end
	end

	local keys = -1; -- "bits" not in table targets
	-- format key
	for k,v in pairs(privKeys) do
		keys = keys + 1
		if type(v) == "string" then
			v = string.gsub(v,"%W","")
			if string.sub(v,1,2) == "00" then
				v = string.sub(v,3,-1)
			end
			privKeys[k] = hexstring2byte(v)
		end
		--print(k.." = "..v)
	end

	if keys == #targets then
		return privKeys
	else
		return nil
	end
end

---------------------
----main function----
---------------------
parseCLIarg(arg)

if not s_magicCall then
	local magicCallCmd = "openssl rsa -in "..s_keyFileName.." -text -noout |"..argument2str(arg)..c_CLIMagicParam

	--if magicCallCmd then print("magicCallCmd="..magicCallCmd) end

	os.execute(magicCallCmd)
else
	--print("magic call")
	local key = searchPrivKeys()

	if key == nil then
		print("get private key failed!");
		os.exit()
	end

	local stbtypeid = nil
	local infile = assert(io.open(s_inputFile,"r"),"open file "..s_inputFile.." failed!")
	local outfile = assert(io.open(s_outputFile,"w"),"open file "..s_outputFile.." failed!")
	while true do
		local line = infile:read("*line")
		if not line then break end

		local ss = string.find(line,"=") --config info line
		--this script not verify macfile
		if ss then
			local ss,ee,id = string.find(line,"stbtypeid%s*=%s*(%d+)")
			if id then
				if stbtypeid then print("stbtypeid set more then once") end
				stbtypeid = id
			end

			if string.sub(line,-1,-1) == "\r" then
				line = string.sub(line,1,-2)
			end

			outfile:write(line.."\r\n")
		elseif not stbtypeid then
			print("failed to get stbtypeid")
			break
		else
			line = string.gsub(line,"%s","")
			local sighline = line.."-"..stbtypeid
			local c = csrsa.privateEncrypt(sighline,key)
			local m = csrsa.publicDecrypt(c,key)
			if sighline == m then
				c = hexstr2base64(c)
				outfile:write(line.." "..c.."\r\n")
			else
				print("warning! sigh line["..line.."] failed!")
			end
		end
	end

	infile:close()
	outfile:close()
end
