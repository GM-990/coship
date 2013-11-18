//≤‚ ‘”√¿˝Œƒµµ±Í«©∫Í£¨œÍœ∏√Ë ˆ«Î≤Œøº:
//http://10.10.5.152/dokuwiki/doku.php?id=mmcp:mmcp%E5%BC%80%E5%8F%91%E6%8C%87%E5%8D%97:testkit:%E6%B5%8B%E8%AF%95%E7%94%A8%E4%BE%8B%E6%96%87%E6%A1%A3%E6%92%B0%E5%86%99%E6%8C%87%E5%8D%97
macro InsertTestCaseHeader()
{
	hbuf = GetCurrentBuf()
	ln = GetWndSelLnFirst(GetCurrentWnd())
	
	i=0
	InsBufLine(hbuf, ln + i, "//\@CASEGROUP:")
	i=i+1
	InsBufLine(hbuf, ln + i, "//\@DESCRIPTION:")
	i=i+1
	InsBufLine(hbuf, ln + i  "//\@PRECONDITION:")
	i=i+1
	InsBufLine(hbuf, ln + i, "//\@INPUT:")
	i=i+1
	InsBufLine(hbuf, ln + i, "//\@EXPECTATION:")
	i=i+1
	InsBufLine(hbuf, ln + i, "//\@REMARK:")
	i=i+1
	InsBufLine(hbuf, ln + i, "//\@EXECUTIONFLOW:")
}
