--[[
A Lua Server Page engine module

Translates and executes a Lua server page
]]

--module之后，全局环境就不能用了，所以要在这里把需要用的先产生本地引用
local assert, error, getfenv, loadstring, setfenv = assert, error, getfenv, loadstring, setfenv
local find, format, gsub, strsub = string.find, string.format, string.gsub, string.sub
local concat, tinsert = table.concat, table.insert
local open = io.open
local print = print

module (...) --use ... to means no name? or need not care it.require could usr filename

----------------------------------------------------------------------------
-- function to do output
local outfunc = "io.write"

----------------------------------------------------------------------------
-- Builds a piece of Lua code which outputs the (part of the) given string.
-- @param s String.
-- @param i Number with the initial position in the string.
-- @param f Number with the final position in the string (default == -1).
-- @return String with the correspondent Lua code which outputs the part of the string.
----------------------------------------------------------------------------
local function out (s, i, f)
	s = strsub(s, i, f or -1)
	if s == "" then return s end

	--todo why not use %q or need this format

	-- we could use `%q' here, but this way we have better control
	s = gsub(s, "([\\\n\'])", "\\%1") --add a \ before \ \n '
	-- substitute '\r' by '\'+'r' and let `loadstring' reconstruct it
	s = gsub(s, "\r", "\\r")
	return format(" %s('%s'); ", outfunc, s)
end

----------------------------------------------------------------------------
-- Translate the template to Lua code.
-- @param s String to translate.
-- @return String with translated code.
----------------------------------------------------------------------------
local function translate (s)
	local res = {}

	tinsert(res,"local lsp = require('LuaServerPage');\n") --require this module in lsp file for using assist in it like output

	local start = 1   -- start of untranslated part in `s'
	while true do
		local ip, fp, exp, code = find(s, "<%?lua[ \t]*(=?)(.-)%?>", start)
		if not ip then break end
		tinsert(res, out(s, start, ip-1))
		if exp == "=" then   -- expression?
			tinsert(res, format(" %s(%s);", outfunc, code))
		else  -- command
			tinsert(res, format(" %s ", code))
		end
		start = fp + 1
	end
	tinsert(res, out(s, start))
	return concat(res)
end

----------------------------------------------------------------------------
-- Translates a template into a Lua function.
-- Does NOT execute the resulting function.
-- @param string String with the template to be translated.
-- @param chunkname String with the name of the chunk, for debugging purposes.
-- @return Function with the resulting translation.
----------------------------------------------------------------------------
local function compile (string, chunkname)
	local str = translate (string)
	--print("---------------------------")
	--print(str)
	--print("---------------------------")
	local f, err = loadstring (translate (string), chunkname)
	if not f then error (err, 3) end
	return f
end

----------------------------------------------------------------------------
-- function to output msg with outfunc set by setoutfunc used in lsp file
-- Does NOT USR _env["output"]=output for this way will pollution the user env
-- @param string String to output
-- @return The return of outfunc set by setoutfunc
----------------------------------------------------------------------------
function output(str)
	local cmd = (format(" %s(%q);", outfunc, str))
	local f = loadstring(cmd)
	return f()
end

----------------------------------------------------------------------------
-- Translates and executes a Lua page in a given file.
-- The translation creates a Lua function which will be executed in an
-- optionally given environment.
-- @param filename String with the name of the file containing the template.
-- @param env Table with the environment to run the resulting function.
----------------------------------------------------------------------------
function include (filename, env)
	-- read the whole contents of the file
	local fh = assert (open (filename))
	local src = fh:read("*a")
	fh:close()
	-- translates the file into a function
	local prog = compile (src, '@'..filename)
	local _env = env or {}
	setfenv (prog, _env)
	prog ()
end

----------------------------------------------------------------------------
-- Defines the name of the output function.
-- @param f String with the name of the function which produces output.
----------------------------------------------------------------------------
function setoutfunc (f)
	outfunc = f
end

