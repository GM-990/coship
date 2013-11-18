#!/usr/sbin/lua

local svnurlhead = "svn://192.168.99.107/mmcp/trunk/"

local s_libdirmap = {
	dtv 		= "dtv",
	MP3PLAYER 	= "codec",
	DTVM		= "dtvmx",
	graph		= "graph",
	guitool		= "guitool",
	jvm			= "jvm",
	Kernel		= "kernel",
	MP			= "mediaplayer",
	midp		= "midp",
	PROTOCOL	= "protocol",
	Shell		= "shell",
	Udiadapter	= "udiadapter"
}

print("---------------------------------------------")
print("|  Welcome to use Lib Version Checker V0.1  |")
print("---------------------------------------------")

if arg[1] == nil then
	print("usage: LibChecker.lua libfiles")
	os.exit()
end

local tempFileName = "files.tmp"
for i = 1,table.getn(arg),1 do
	--print(arg[i])
	_,_,lib = string.find(arg[i],"[%w%p]+/([%w%p]+)$")
	--print(lib)

	local cmd = "strings "..arg[i].." | grep MMCP_ > "..tempFileName
	os.remove(tempFileName)
	os.execute(cmd)

	local stringsresult = assert(io.open(tempFileName,"r"))
	while true do
		local line = stringsresult:read("*line")
		if not line then
			print(string.format("%-16s:Version info not Found",lib))
			break
		end

		local _,_,mode,ver,udi,dd = string.find(line,"MMCP_([%w%p]+)_V([%d%.]+).-%s+([%w%p]+)%s+(.+)")

		--print("----------------")
		--print(line)
		--print(mode)
		--print(ver)
		--print(udi)
		--print(dd)


		if mode and ver and udi and dd then
			if s_libdirmap[mode] then
				local _,_,vv = string.find(ver,"[%w%p]+%.([%w%p]+)$")
				assert(vv,vv)

				local svnFileName = "svn.tmp"
				local svncmd = "svn log -q -l 100 "..svnurlhead..s_libdirmap[mode].." > "..svnFileName
				os.remove(svnFileName)
				os.execute(svncmd)

				local svnresult = assert(io.open(svnFileName,"r"))

				local index = 1

				while true do
					local svnline = svnresult:read("*line")
					if not svnline then
						print(lib.." version not found on svn")
						break
					end

					_,_,svnver,svnuser,svndate = string.find(svnline,"r(%d+)%s+|%s+(%w+)%s+|%s+([%d%-:%s]+)")

					if svnver and svnuser and svndate then

						index = index+1

						--print("["..svnver.."]")
						--print("["..ver.."]")
						if svnver == vv then
							print(string.format("%-32s Ver:%-60s LastModify:%s  %d updates after it",lib,line,svndate,index))
							--print(svnline)
							--print(svnver)
							--print(svnuser)
							--print(svndate)

							break
						end
					end

				end
				svnresult:close()

			else
				print(string.format("%-16s:svn url for mode %s not Found",lib,mode))
			end

			break
		end
	end
	stringsresult:close()

end
