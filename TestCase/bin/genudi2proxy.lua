#!/usr/sbin/lua

if arg[1] == nil then
	print("usage: h2c 'headerfile'|'path'")
	print("notes: convent headerfile(s) to c source file(s)")
	os.exit()
end

local g_symboltable = {}
local g_includes = {}

function writehfile(symboltable, includes, fullfilename, hfilename, sub_dir)
	local execelfile = assert(io.open(fullfilename,"w"))

	local hmacro = "_"..string.gsub(string.upper(hfilename), "%.", "_").."_PROXY"
	execelfile:write("#ifndef ", hmacro, "\r\n")
	execelfile:write("#define ", hmacro, "\r\n\r\n")
	
	for y,v in ipairs(g_includes) do
		--print(v)
		execelfile:write(v.."\n")
	end
	
	execelfile:write("#include \"../udi2"..sub_dir.."/", hfilename, "\"\r\n\r\n")
	execelfile:write("#ifdef __cplusplus\nextern \"C\"\n{\n#endif\n\n")
	
	for y,v in ipairs(symboltable) do
		
		local funcname = symboltable[y][2]
		local args = symboltable[y][4]
		local funcmacro_noarg = "#define "..funcname.."() "..funcname.."_PROXY(__FUNCTION__, __LINE__)\n"
		local funcmacro = "#define "..funcname.."(format, ...) "..funcname.."_PROXY(format, ##__VA_ARGS__, __FUNCTION__, __LINE__)\n"
		
		if (string.gsub(args, " ", "") == "void") then
			execelfile:write(funcmacro_noarg)
		else
			if (funcname == "CSDebugDefaultOutputString") then
				execelfile:write("//")
			end
			execelfile:write(funcmacro)
		end
		
		for x,vv in ipairs(v) do

			if (x == 1) then
				execelfile:write(vv.." ")
			elseif (x == 2) then
				execelfile:write(vv.."_PROXY")
			elseif (vv == "void") then
				--do nothing
			elseif (vv == ")") then
				if (string.lower(args) ~= "void") then
					execelfile:write(", ")
				end
					execelfile:write("const char *pcFuncName, const int nLine"..vv)
			else
					execelfile:write(vv)
			end
			
		end
		
		execelfile:write(";")
		execelfile:write("\r\n\n")
	
	end
	
	execelfile:write("#ifdef __cplusplus\n}\n#endif\n\n")
	
	execelfile:write("#endif")
	execelfile:write("\r\n")
	execelfile:close()
end

function writecfile(symboltable, fullfilename, hfilename)
	local execelfile = assert(io.open(fullfilename,"w"))

	execelfile:write("#include \"udiplus_debug.h\"\r\n")
	execelfile:write("#include \"udi2/", hfilename, "\"\r\n\r\n")
	local modelname = string.sub(hfilename,1,-3)
	execelfile:write("#define MMNAME \"", modelname, "\"\n\n")
	
	for y,v in ipairs(symboltable) do

		local functionname = symboltable[y][2]
		local args_org = symboltable[y][4]
		local args_input = ""
			
		--print("[", args_org, "]")
		
		for temp in string.gmatch(args_org, "%w+%s*,") do
			--print(temp)
			if (temp ~= nil) then
				args_input = args_input..temp.." "
			end	
		end
		
		local  temp = string.match(args_org, "%w+%s*$")
		--print(temp)
		if (temp ~= nil) then
			args_input = args_input..temp
		end	
					
		--print(args_input)
		
		for x,vv in ipairs(v) do

			if (x == 1) then
				execelfile:write(vv.." ")
			elseif (x == 2) then
				execelfile:write(vv.."_PROXY")
			elseif (vv == "void") then
				--do nothing
			elseif (vv == ")") then
				if (string.lower(args_input) ~= "void") then
					execelfile:write(", ")
				end
					execelfile:write("const char *pcFuncName, const int nLine"..vv)
			else
					execelfile:write(vv)
			end
			
		end

		execelfile:write("\r\n")
		execelfile:write("{\r\n")

		if (args_input == "void") then
			args_input = ""
		end
		
		execelfile:write("\tCSDebug\(MMNAME, INFO_LEVEL, \"[%s, called by %s, in line %d]\\n\", __FUNCTION__, pcFuncName, nLine);\r\n\r\n")
		
		execelfile:write("\treturn ", functionname, "(", args_input, ");\r\n")

		execelfile:write("}\r\n\r\n")
	end

	execelfile:write("\r\n")
	execelfile:close()
end

function writecfileperfunction(symboltable, fullfilename, hfilename, sub_dir)
		
	for y,v in ipairs(symboltable) do
		
		local returntype = symboltable[y][1]
		local functionname = symboltable[y][2]
		local args_org = symboltable[y][4]
		local args_input = ""
			
		--print("[", args_org, "]")

		local udi2proxyfilename = string.sub(fullfilename,1,-3).."_"..functionname..".c"
		local execelfile = assert(io.open(udi2proxyfilename,"w"))

		execelfile:write("#include \"udiplus_debug.h\"\r\n")
		execelfile:write("#include \"udi2"..sub_dir.."/", hfilename, "\"\r\n\r\n")
		local modelname = string.sub(hfilename,1,-3)
		execelfile:write("#define MMNAME \"", modelname, "\"\n\n")
		
		for temp in string.gmatch(args_org, "%w+%s*,") do
			--print(temp)
			if (temp ~= nil) then
				args_input = args_input..temp.." "
			end	
		end
		
		local  temp = string.match(args_org, "%w+%s*$")
		--print(temp)
		if (temp ~= nil) then
			args_input = args_input..temp
		end	
					
		--print(args_input)
		
		for x,vv in ipairs(v) do

			if (x == 1) then
				execelfile:write(vv.." ")
			elseif (x == 2) then
				execelfile:write(vv.."_PROXY")
			elseif (vv == "void") then
				--do nothing
			elseif (vv == ")") then
				if (string.lower(args_input) ~= "void") then
					execelfile:write(", ")
				end
					execelfile:write("const char *pcFuncName, const int nLine"..vv)
			else
					execelfile:write(vv)
			end
			
		end

		execelfile:write("\r\n")
		execelfile:write("{\r\n")

		if (args_input == "void") then
			args_input = ""
		end
		
		execelfile:write("\tCSDebug\(MMNAME, INFO_LEVEL, \"[%s, called by %s, in line %d]\\n\", __FUNCTION__, pcFuncName, nLine);\r\n\r\n")
		
		execelfile:write("\t")
		if (string.find(returntype, "void") ~= nil and string.find(returntype, "*") == nil) then
			--当返回值类型为void时，不加return，否则st5105有警告			
		else
			execelfile:write("return ")
		end
		
		execelfile:write("", functionname, "(", args_input, ");\r\n")

		execelfile:write("}\r\n\r\n")
		
		execelfile:write("\r\n")
		execelfile:close()
		
	end
	
end

function readfile(symboltable, includes, filename)
	local tabley = 1
	local mapfile = assert(io.open(filename,"r"))

	local content = mapfile:read("*all")
	local returntype, functionname, leftbracket, parameters, rightbraket, semicolon
	local includetmp
	
	-- remove /* comments */
	local contentwithoutcomment1 = string.gsub(content, "%/%*.-%*%/", "")
	-- remove // comments
	local contentwithoutcomment2 = string.gsub(contentwithoutcomment1, "%/%/.-\n", "")
	-- remove #if 0 ... #endif
	local contentwithoutcomment3 = string.gsub(contentwithoutcomment2, "#if%s+%(?%s*0%s*%)?.-#endif", "")

	--print(contentwithoutcomment3)

	for returntype, functionname, leftbracket, parameters, rightbraket, semicolon in
		string.gfind(contentwithoutcomment3, "%s*([%a_]+%s*[%a_%d]*%s*%**)%s+([%a_]+[%a_%d]*)%s*(%()%s*([%a%d%*%,%_%s%t]*)%s*(%))%s*(;)%s*") do
		
		returntype = string.gsub(returntype, "endif", "") --del "endif"
		--print(returntype, functionname, leftbracket, parameters, rightbraket, semicolon)
		
		symboltable[tabley] = {}
		symboltable[tabley][1] = returntype
		symboltable[tabley][2] = functionname
		symboltable[tabley][3] = leftbracket
		symboltable[tabley][4] = parameters
		symboltable[tabley][5] = rightbraket
		--symboltable[tabley][6] = semicolon
		tabley = tabley + 1
	end
	
	tabley = 1
	for includetmp in
		string.gfind(contentwithoutcomment3, "(#include%s+.+.h\")") do	
			
		--print(includetmp)
	 	includes[tabley] = includetmp
	 	
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
	g_includes = {}
	
	readfile(g_symboltable, g_includes, dir.."/"..file)
	
	--解析出include/udi2目录包含的子目录
	local s, e = string.find(dir, "udi2")
	local len = string.len(dir)
	local sub_dir = string.sub(dir, e+1, len)

	--检查目录，并生成代理头文件
	local dir_udi2_proxy_h = string.gsub(dir, "udi2", "udi2_proxy")
	os.execute("mkdir "..dir_udi2_proxy_h.." -p");
	writehfile(g_symboltable, g_includes, dir_udi2_proxy_h.."/"..file, file, sub_dir)
	
	--检查目录，并生成代理接口的C代码实现文件
	local dir_udi2_proxy_c = string.gsub(dir_udi2_proxy_h, "udi2_proxy", "udi2_proxy_c")
	os.execute("mkdir "..dir_udi2_proxy_c.." -p");
	writecfileperfunction(g_symboltable, dir_udi2_proxy_c.."/"..file, file, sub_dir)
	
	--print("-------------------------------------------------------------------------")
	
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

