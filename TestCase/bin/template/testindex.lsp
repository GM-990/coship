<html xmlns:fo="http://www.w3.org/1999/XSL/Format">
<head>
<META http-equiv="Content-Type" content="text/html; charset=UTF-8">
<link type="text/css" href="style.css" rel="stylesheet">
</head>
<body style="background: url(bg.gif) ">
<TITLE>Coship MMCP Test Case Document Index</TITLE>
<table align="center" border="0" width="93%">
<tr>
<td>
<table width="100%">
<tr>
<td style="width : 40%" class="title-td">Coship MMCP Test Case Document</td>
</tr>
<tr class="title-tr">
<td class="title-left-td"></td><td class="title-right-td"></td>

</tr>
<tr>
<td class="title-path-td" colspan="2">Version 0.1</td>
</tr>
</table>
</td>
</tr>
<tr>
<td>
<?lua
function generatedirectory(tree,level)
	if not tree or type(tree) ~= "table" then return end
	
	for k,v in pairs(tree) do
		--print("ti "..k)
		
		lsp.output("<ul class=\"menu\" >\n")
		
		local page = k..".html"
		if k == "." then 
			k = "Test Case Docment Root"
			page = "root.html"
		end
		
		if type(v) == "table" then
			if v["*leaf"] then
				lsp.output("<li style=\"\"><A HREF="..page..">"..k.."</A><br></li>\n")
			else
				lsp.output("<li style=\"\">"..k.."<br></li>\n")
			end
		end
		
		generatedirectory(v,level+1)
		
		lsp.output("</ul>")
	end
end

generatedirectory(dirtree,1)

?>

</td>
</tr>
</table>
</body>
</html>
