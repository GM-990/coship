#!/usr/sbin/lua

if arg[1] == nil then
	print("usage: parsegccmap 'mapfile'")
	os.exit()
end

local mapfile = assert(io.open(arg[1],"r"))

local SumTitle = "合计"
local mainobj = "主函数"

local startFlagFount = false

local filterSection = {}


filterSection[".sbss"] = true
filterSection[".bss"] = true


-- Table  [.a][.o][.section] = size
-- spectial .o		:SumTitle("合计") or mainobj("主函数")
-- spectial .section:SumTitle("合计")
local objdump_flash_table = {}
local objdump_memory_table = {}

function produce_objdumpTable(objtable, archive, obj, section, size)
	objtable[archive] = objtable[archive] or {}

	objtable[archive][SumTitle] = objtable[archive][SumTitle] or {}
	local oldsize = objtable[archive][SumTitle][section] or 0
	objtable[archive][SumTitle][section] = oldsize + size

	oldsize = objtable[archive][SumTitle][SumTitle] or 0
	objtable[archive][SumTitle][SumTitle] = oldsize + size

	objtable[archive][obj] = objtable[archive][obj] or {}
	oldsize = objtable[archive][obj][section] or 0
	objtable[archive][obj][section] = oldsize + size

	oldsize = objtable[archive][obj][SumTitle] or 0
	objtable[archive][obj][SumTitle] = oldsize + size
end

function produce_csvfile(objtable, filename)
	local execelfile = assert(io.open(filename,"w"))

	execelfile:write("Archive,Obj,Section,Size\n")

	for archive,v in pairs(objtable) do
		for obj,vv in pairs(v) do
			for section,size in pairs(vv) do
				--print(archive..","..obj..","..section..","..size)
				execelfile:write(archive..","..obj..","..section..","..size.."\n")
			end
		end
	end

	execelfile:close()
end


while true do
	local line = mapfile:read("*line")
	if not line then break end

	if not startFlagFount then

		if line == "Discarded input sections" then
			startFlagFount = true
			--print(line)
		end
	else
		--print(line)
		if string.find(line," %.") == 1 then -- obj mode

			local section,addr,size,archive,obj

			if string.sub(line,-1) == ")" then -- a obj in 1 line, within archive

				_,_,section,addr,size,archive,obj = string.find(line,"^%s([%w%p]+)%s+([%w%p]+)%s+([%w%p]+)%s+([%w%p]+)%(([%w%p]+)%)")

			elseif string.sub(line,-2) == ".o"
				or  string.sub(line,-2) == ".0"
				or string.sub(line,-3) == ".so" then -- a obj in 1 line, without archive

				archive = mainobj

				_,_,section,addr,size,obj = string.find(line,"^%s([%w%p]+)%s+([%w%p]+)%s+([%w%p]+)%s+([%w%p]+)")

			else -- a obj in 2 lines

				_,_,section = string.find(line,"^%s([%w%p]+)")

				line = mapfile:read("*line")

				if string.sub(line,-1) == ")" then -- within archive

					--print(line)
					_,_,addr,size,archive,obj = string.find(line,"^%s+([%w%p]+)%s+([%w%p]+)%s+([%w%p]+)%(([%w%p]+)%)")

				elseif string.sub(line,-2) == ".o"
					or string.sub(line,-2) == ".0"
					or string.sub(line,-3) == ".so" then --without archive

					archive = mainobj

					_,_,addr,size,obj = string.find(line,"^%s+([%w%p]+)%s+([%w%p]+)%s+([%w%p]+)")

				else
					--print("errorline:"..line)
					--assert(false,line)
				end
			end

			--assert(section,line)
			--assert(addr,line)
			--assert(size,line)
			--assert(archive,line)
			--assert(obj,line)


			if section and addr and size and archive and obj then

				if archive ~= mainobj then
					_,_,archive = string.find(archive,"[%w%p]+/([%w%p]+)$")
				end

				local _,_,tmpsection = string.find(section,"(.[%w_]+)")
				section = tmpsection or section

				size = tonumber(size)

				if not filterSection[section] then
					produce_objdumpTable(objdump_flash_table, archive, obj, section, size)
				end

				produce_objdumpTable(objdump_memory_table, archive, obj, section, size)

			else
				--print("errline:"..line)
			end
		end
	end
end
mapfile:close()
produce_csvfile(objdump_flash_table,"file_flash.csv")
produce_csvfile(objdump_memory_table,"file_memory.csv")

print ("ouput flash map to 'file_flash.csv'")
print ("ouput memory map to 'file_memory.csv'")

--[[

function archiveSortfunc(a,b)
	if a[SumTitle][SumTitle] < b[SumTitle][SumTitle] then
		return true
	else
		return false
	end
end

table.sort(objdumpTable,archiveSortfunc)

]]





