#!/usr/sbin/lua
--author : lihui 2008-09-04


if arg[1] == nil then
	print("usage: parseStMap.lua mapfile")
	os.exit()
end

local mapfile = assert(io.open(arg[1],"r"))

local SumTitle = "合计"
local mainobj = "主函数"

local startFlagFount = false

local FlashSection = {}
FlashSection["def_code"] = true
FlashSection["def_data"] = true
FlashSection["def_const"] = true

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
		start = string.find(line,"Module Contribution Map")

		if start ~= nil then
			startFlagFount = true
		end
	else
		--Object File(.lib)        Module Name(.c)        Section Name       Start      Length
		local _,_,archive,obj,section,addr,size = string.find(line,"([%w%p]+)%s+([%w%p]+)%s+([%w%p]+)%s+(0x[%x]+)%s+([%d]+)")

		if archive and obj and section and addr and size then

			local oldarchive = archive

			if string.sub(archive,-4) == ".tco" then
				archive = mainobj
			else
				local _,_,shortarchive = string.find(archive,"[%w%p]+/([%w%p]+)$")--remove path
				archive = shortarchive or archive
			end

			assert(archive,"Error archive:"..oldarchive)

			size = tonumber(size)

			assert(size)

			if FlashSection[section] then
				produce_objdumpTable(objdump_flash_table, archive, obj, section, size)
			end

			produce_objdumpTable(objdump_memory_table, archive, obj, section, size)

		else
			--print("errline:"..line)
		end

	end
end

mapfile:close()
produce_csvfile(objdump_flash_table,"file_flash.csv")
produce_csvfile(objdump_memory_table,"file_memory.csv")

print ("ouput flash map to 'file_flash.csv'")
print ("ouput memory map to 'file_memory.csv'")

