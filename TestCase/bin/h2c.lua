#!/usr/sbin/lua

if arg[1] == nil then
	print("usage: h2c 'headerfile'|'path'")
	print("notes: convent headerfile(s) to c source file(s)")
	os.exit()
end

local g_symboltable = {}

function writefile(symboltable, fullfilename, hfilename)
	local execelfile = assert(io.open(fullfilename,"w"))

	execelfile:write("#include \"", hfilename, "\"\r\n\r\n")

	for y,v in ipairs(symboltable) do

		for x,vv in ipairs(v) do
			execelfile:write(vv)
			if (x == 1) then
				execelfile:write(" ")
			end

		end

		execelfile:write("\r\n")
		execelfile:write("{\r\n")

		if (v[1] ~= "void" and v[1] ~= "VOID") then
			execelfile:write("\treturn (", v[1],")(0);\r\n")
		end

		execelfile:write("}\r\n\r\n")
	end

	execelfile:write("\r\n")
	execelfile:close()
end

function readfile(symboltable, filename)
	local tabley = 1
	local mapfile = assert(io.open(filename,"r"))

	local content = mapfile:read("*all")
	local returntype, functionname, leftbracket, parameters, rightbraket, semicolon

	-- remove /* comments */
	local contentwithoutcomment1 = string.gsub(content, "%/%*.-%*%/", "")
	-- remove // comments
	local contentwithoutcomment2 = string.gsub(contentwithoutcomment1, "%/%/.-\n", "")
	-- remove #if 0 ... #endif, but can not correctly dispose string as : #if 0...#if 1...#endif...#endif
	local contentwithoutcomment3 = string.gsub(contentwithoutcomment2, "#if%s+%(?%s*0%s*%)?.-#endif", "")

	--print(contentwithoutcomment3)

	for returntype, functionname, leftbracket, parameters, rightbraket, semicolon in
		string.gfind(contentwithoutcomment3, "%s*([%a_]+%s*[%a_%d]*%s*%**)%s+([%a_]+[%a_%d]*)%s*(%()%s*([%a%d%*%,%_%s%t]*)%s*(%))%s*(;)%s*") do
		
		--print("retrun1: "..returntype)
		returntype = string.gsub(returntype, "endif%s*", "") --del "endif"
		--print("retrun: "..returntype, "  function: "..functionname, "  leftbracket: "..leftbracket, "  parameters: "..parameters, "  rigthbraket:"..rightbraket, "  semicolon:"..semicolon)
		symboltable[tabley] = {}
		symboltable[tabley][1] = returntype
		symboltable[tabley][2] = functionname
		symboltable[tabley][3] = leftbracket
		symboltable[tabley][4] = parameters
		symboltable[tabley][5] = rightbraket
		--symboltable[tabley][6] = semicolon
		tabley = tabley + 1
	end

	mapfile:close()
end

--dir browse function,to add subdir browse
--function is a file (not a dir)
function isfile(lsline)
	if string.sub(lsline, 1, 1) ~= '-' then return nil end
	local _,_,filename = string.find(lsline,"%s*([%w%p]+)$")
	return filename
end

--function is a directory line ("xxxx:")
function isdirectory(lsline)
	if string.sub(lsline,-1,-1) ~= ":" then return nil end
	return string.sub(lsline,1,-2)
end

--all files iterater
function allfiles (dir)
	local tempFileName = "files.tmp"
	local cmd = "ls -l -R "..dir.." > "..tempFileName
	os.remove(tempFileName)
	os.execute(cmd)
	local dirs = assert(io.open(tempFileName,"r"))
	local directory = nil

	return function ()
		while true do
			local line = dirs:read("*line")
			if not line then
				io.close(dirs)
				return nil
			end

			local filename = isfile(line)

			if not filename then --not a filename line,then test directory line
				directory = isdirectory(line) or directory
			end

			if filename then return filename,directory end
		end
	end
end

function parse_and_produce(dir, file)

	--local filepath, filename
	--_,_,filepath, filename = string.find(directory,"([%w%p]+%.)h")

	local cfile = string.sub(file,1,-2).."c"
	--print(cfile)
	g_symboltable = {}

	readfile(g_symboltable, dir.."/"..file)
	writefile(g_symboltable, dir.."/"..cfile, file)

end

--search all dirs and found all tags

for file,dir in allfiles(arg[1]) do
	local _,_,extname = string.find(file,"%.(.+)$")
	if extname then
		extname = string.lower(extname)
		if extname == "h"then
			parse_and_produce(dir,file)
			--os.remove(dir.."/"..file)
			--print(file)
			--print(dir)
			--print(dir.."/"..file)
		end
	end
end


--readfile(g_symboltable, "F:\\mmcp\\trunk\\include\\porting\\hdi\\cs_rfmod.h")

