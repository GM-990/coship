<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.0 Transitional//EN">
<HTML>
<HEAD>
<link type="text/css" href="style.css" rel="stylesheet">
<?lua
	local modulename = ""
	for index,case in ipairs(moduledir) do
		modulename = case
	end
	
	if modulename == "." then modulename = "Root" end
?>
<TITLE><?lua =modulename ?> test case doc</TITLE>

</HEAD>
<BODY style="background:url(bg.gif)">

<!-- ��ҳͷ�� -->
<table align="center" border="0" width="93%">
<tr>
<td>
<table width="100%">
<tr>
<td style="width : 40%" class="title-td">
<?lua =modulename ?> test case doc
</td><td></td>
</tr>
<tr class="title-tr">
<td class="title-left-td"></td><td class="title-right-td"></td>
</tr>
<tr>
<td class="title-path-td" colspan="2">

<!-- �������� -->
<?lua
local curdir = dirtree

for index,dir in ipairs(moduledir) do
	curdir = curdir[dir]
	assert(curdir)
	
	if dir == "." then 
		lsp.output("<a href=\"index.html\"> TestCase Index </a>")
	else
		if curdir["*leaf"] then
			lsp.output("<a href=\""..dir..".html\"> "..dir.." </a>")
		else
			lsp.output(" "..dir.." ")
		end
	end
	
	if moduledir[index+1] then
		lsp.output(" &gt\n")
	else
		lsp.output("\n")
	end
end
?>

</td>
</tr>
</table>

<!-- ����������ܱ��ͷ�� -->
<table align="center" border="0" width="93%">
<tr>
<td style="padding-top : 10px;" align="center">

<?lua for index,case in ipairs(testcases) do 
	--������е����
	for k,v in pairs(case) do
		if v and string.find(v,"\n") then
			--case[k] = [[<ul class="exec-font" type="disc"><li style="">]]..string.gsub(case[k],"\n",[[</li><li style="">]]).."</li></ul>"
			case[k] = string.gsub(case[k],"\n","<br>")
		end
	end	
?>
<!-- �������ʼ -->
<table class="header-table">
<tr>
<td>

<!-- testid��� -->
<table class="testid-table" width="100%">
<tr>
<td class="testid-td">
<?lua =case["CASE"] or "None" ?>
</td>
<td align="right"></td>
</tr>
</table><!-- testid������ -->

</td>
</tr>
<tr>
<td>

<!-- test func�ȵ��б�� -->
<table width="100%" cellspacing="4">

<!-- ��Ҫ���⺯�� -->
<tr>
<td class="left-td">
Test Function
</td><td onMouseOut="this.className='right-td'" onMouseOver="this.className='right-td-over'" class="right-td">
<font class="func">
<?lua =case["CASEGROUP"] or "None" ?>
</font></td>
</tr>

<!-- ���� -->
<tr>
<td class="left-td">Description
</td><td onMouseOut="this.className='right-td'" onMouseOver="this.className='right-td-over'" class="right-td">
<font class="func">
<?lua =case["DESCRIPTION"] or "None" ?>
</font></td>
</tr>

<!-- ǰ������ -->
<tr>
<td class="left-td">
Precondition
</td><td onMouseOut="this.className='right-td'" onMouseOver="this.className='right-td-over'" class="right-td">
<font class="func">
<?lua =case["PRECONDITION"] or "None" ?>
</font></td>
</tr>

<!-- ������� -->
<tr>
<td class="left-td">
Inputs
</td><td onMouseOut="this.className='right-td'" onMouseOver="this.className='right-td-over'" class="right-td">
<font class="func">
<?lua =case["INPUT"] or "None" ?>
</font></td>
</tr>

<!-- ������� -->
<tr>
<td class="left-td">
Expectation
</td><td onMouseOut="this.className='right-td'" onMouseOver="this.className='right-td-over'" class="right-td">
<font class="func">
<?lua =case["EXPECTATION"] or "None" ?>
</font></td>
</tr>

<!-- ������� -->
<tr>
<td class="left-td">
Remarks
</td><td onMouseOut="this.className='right-td'" onMouseOver="this.className='right-td-over'" class="right-td">
<font class="func">
<?lua =case["REMARK"] or "None" ?>
</font></td>
</tr>

</table>
</td>
</tr>
<tr>
<td>

<!-- Execution Flow��������ʼ -->
<table cellpadding="0" cellspacing="2" width="100%">
<tr>
<td><b>
Execution Flow
</b></td>
</tr>
<tr>
<td>

<!-- Execution Flow����ͷ���ʼ -->
<table cellspacing="1" width="100%">
<tr class="title-tr">
<td style="background-color : #96B2C5" class="subtitle-left-td"></td><td class="subtitle-right-td"></td>
</tr>
</table><!-- Execution Flow����ͷ������ -->

</td>
</tr>
<tr>
<td onMouseOut="this.className='right-td'" onMouseOver="this.className='right-td-over'" class="right-td" colspan="2">
<?lua =case["EXECUTIONFLOW"] or "None" ?>
</td>
</tr>

<tr class="title-tr">
<td>

<!-- Execution Flowβ��� -->
<table cellspacing="1" width="100%">
<tr class="title-tr">
<td class="subtitle-right-td"></td>
</tr>

</table><!-- Execution Flowβ������ -->
</td>
</tr>
</table><!-- Execution Flow����������� -->
</td>
</tr>
</table><!-- ���������� -->

<!-- �������� -->
</td>
</tr>
<tr>
<td style="padding-top : 10px;" align="center">

<?lua end ?>

</td>
</tr>
</table>



</BODY>
</HTML>
