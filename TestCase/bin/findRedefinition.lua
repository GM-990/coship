#!/usr/sbin/lua

--[[
CS SearchRedefine V0.2

by lihui in lua

auto search function redefines in libs

V0.2.1 for nw -C bugs,delete -c option
V0.2  add lib and obj to redefine function
V0.1  init version
]]

--usage
local g_usage = [[
Usage: SearchRedefine libdir
]]

local tempFileName = "SearchRedefine.tmp"

local g_libsDir = "*"

local g_allFuncs = {}

--get nm cmd
function getNM()
	local platform = os.getenv("MMCP_PLATFORM")
	print("Found platform is "..platform)
	if platform == "linux" then
		return "nm -A --defined-only"
	elseif platform == "st7100" then
		return "sh4nm -A --defined-only"
	elseif platform == "bcm740x" or platform == "bcm7309" then
		return "mipsel-linux-nm -A --defined-only"
	elseif platform == nil then
		return nil,"can not get platform info"
	else
		return nil,"not supported platform \""..platform.."\""
	end
end

print("---------------------------------------------")
print("|     Welcome to SearchRedefine V0.2        |")
print("---------------------------------------------")

--check arg
if table.getn(arg) > 1 then
	g_libsDir = ""
	for i = 1,table.getn(arg),1 do
		g_libsDir = g_libsDir..arg[i].." "
	end
elseif table.getn(arg) == 1 then
	g_libsDir = arg[1]
	local file = assert(io.open(g_libsDir,"r"))
	local reads = file:read("*line")
	file:close()

	if reads == nil then	--a dir
		if string.sub(g_libsDir,-1) == "/" then
			g_libsDir = g_libsDir.."*"
		else
			g_libsDir = g_libsDir.."/*"
		end
	end
end

print("Parse libs of "..g_libsDir)
--os.exit()

local nmcmd,errorCode = getNM()

--check support
if nmcmd == nil then
	print("Error: "..errorCode)
	os.exit()
end

--gen nm result file
os.remove(tempFileName)
print("Generating symbols of all libs,pls wait...")
--print(nmcmd.." "..g_libsDir.." > "..tempFileName.." 2>&-")
os.execute(nmcmd.." "..g_libsDir.." > "..tempFileName.." 2>&-")
local nmresult = assert(io.open(tempFileName,"r"))

--parse nm result
print("Parsing result...")
local SymbolCnt = 0
while true do
	local line = nmresult:read("*line")
	if not line then break end

	local start,_,objname,funcName = string.find(line,"([%w%p]+):%x+ T ([%w%p]+)")

	if start ~= nil then
		SymbolCnt = SymbolCnt + 1

		if g_allFuncs[funcName] == nil then
			g_allFuncs[funcName] = {}
		end

		table.insert(g_allFuncs[funcName],objname)
	end

end

os.remove(tempFileName)

--Generate report and ouput result
local redefinCnt = 0

if SymbolCnt == 0 then
	print("No symbols found,maybe libs or platform error.")
else
	print("Found symbols redefine:")
	for index,value in pairs(g_allFuncs) do
		if table.getn(value) > 1 then		
			redefinCnt = redefinCnt + 1

			print(index..":")

			for _,objname in ipairs(value) do
				print("		"..objname)
			end
			print()
		end
	end

	if redefinCnt == 0 then
		print("No redefine found")
	end
end




