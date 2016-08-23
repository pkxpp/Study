#! /usr/bin/env python
# -*- coding: cp936	-*-

# convert excel xls file to lua script with table data
# date/time values formatted as string, int values formatted as int
# depend on xlrd module
# author: fanlix 2008.1.25
# Modify: 
# page@2015/04/09 tolua
# page@2015/04/12 support directory recursively
# page@2015/04/12 gbk-->utf-8

import xlrd
import os.path

#2015/04/09 解决编码问题
import sys
reload(sys)
sys.setdefaultencoding('utf-8')

#==========================================================================
# 默认配置
DIR_SRC = "src"
DIR_DST = "dst"

#excel数据起始行
ROW_START = 3
#==========================================================================
FLOAT_FORMAT = "%.8f"

SCRIPT_HEAD = '''-- excel xlstable format (sparse 3d matrix)
--{	[sheet1] = { [row1] = { [col1] = value, [col2] = value, ...},
--					 [row5] = { [col3] = value, }, },
--	[sheet2] = { [row9] = { [col9] = value, }},
--}
-- nameindex table
--{ [sheet,row,col name] = index, .. }
'''

SCRIPT_END = '''-- functions for xlstable read
local __getcell = function (t, a,b,c) return t[a][b][c] end
function GetCell(sheetx, rowx, colx)
	rst, v = pcall(__getcell, xlstable, sheetx, rowx, colx)
	if rst then return v
	else return nil
	end
end

function GetCellBySheetName(sheet, rowx, colx)
	return GetCell(sheetname[sheet], rowx, colx)
end
'''

def gen_table(filename):
	if not os.path.isfile(filename):
		raise NameError, "%s is	not	a valid	filename" % filename
	book = xlrd.open_workbook(filename,formatting_info=True)
	luaT = {}
	luaN = {}

	sidx = 0
	for sheet in book.sheets():
		if sidx >= 1: 
			break;
		sdict = {}
		ridx = 0
		for ridx in xrange(sheet.nrows):
			rdict = {}
			for cidx in xrange(sheet.ncols):
				value = sheet.cell_value(ridx, cidx)
				vtype = sheet.cell_type(ridx, cidx)
				v = format_value(value, vtype, book)
				#print sidx, ridx, cidx, value, vtype, v
				if v is not None and value != "":
					rdict[cidx] = v
			if rdict: sdict[ridx] = rdict
		if sdict: luaT[sidx] = sdict

		# handle merged-cell
		for crange in sheet.merged_cells:
			rlo, rhi, clo, chi = crange
			try:
				v = sdict[rlo][clo]
			except KeyError:
				# empty cell
				continue
			if v is None or v == "": continue
			for ridx in xrange(rlo, rhi):
				if ridx not in sdict:
					sdict[ridx] = {}
				for cidx in xrange(clo, chi):
					sdict[ridx][cidx] = v
		name = sheet.name
		luaN[name] = sidx
		luaT[sidx] = sdict
		sidx += 1
	#print "--------- luaT:", luaT
	return luaT, luaN

def format_value(value, vtype, book):
	''' format excel cell value, int?date?
	'''
	if vtype == 2:
		if value == int(value):
			value = int(value)
		elif type(value) == float :
			pass
	elif vtype == 3:
		datetuple =	xlrd.xldate_as_tuple(value,	book.datemode)
		# time only	no date	component
		if datetuple[0]	== 0 and datetuple[1] == 0 and datetuple[2] == 0:
			value =	"%02d:%02d:%02d" % datetuple[3:]
		# date only, no	time
		elif datetuple[3] == 0 and datetuple[4]	== 0 and datetuple[5] == 0:
			value =	"%04d/%02d/%02d" % datetuple[:3]
		else: #	full date
			value =	"%04d/%02d/%02d	%02d:%02d:%02d"	% datetuple
	return value

def format_output(v):
	s = ("%s"%(v)).encode("utf-8")
	if s[-1] == "]":
		s = "%s "%(s)
	return s

def write_table(luaT, luaN, outfile = '-', withfunc = True):
	''' lua table key index starts from 1
	'''
	if outfile and outfile != '-':
		outfp = open(outfile, 'w')
		outfp.write(SCRIPT_HEAD)
	else:
		import StringIO
		outfp = StringIO.StringIO()

	outfp.write("sheetname = {\n")
	for k,v in luaN.iteritems():
		outfp.write("[\"%s\"] = %d,\n"%(format_output(k), v + 1))
	outfp.write("};\n\n")
	outfp.write("sheetindex = {\n")
	for k, v in luaN.iteritems():
		outfp.write("[%d] = \"%s\",\n" %(v+1, format_output(k)))
	outfp.write("};\n\n")

	#outfp.write("xlstable = {\n")
	szName = outfile.split('.')[0]
	szName = szName.replace('/', '\\')
	szName = szName.split('\\')[-1]

	outfp.write("local %s = {\n" % szName)
	for sidx, sheet in luaT.iteritems():
		#第一行为参考
		head = sheet.get(0)
		if head is None:
			break;
		
		outfp.write("[%d] = {\n"%(sidx + 1))
		
		max_row = len(head)
		for rowidx, row in sheet.iteritems():
			#Notify: from row 3 start; index start from 0
			if rowidx >= (ROW_START-1) and row.get(0) != None:
				outfp.write("\t[%d] = {\n"%(row.get(0)))
				for colidx, col in row.iteritems():
					try:
						if type(col) is int: s = "%d"%(col)
						elif type(col) is float: s = FLOAT_FORMAT%(col)
						else : 
							szCol = col.strip();
							if szCol[0] == '{' and szCol[(len(szCol)-1)] == '}':
								s = "%s" % (col)
							else:
								s = "\"%s\""%(format_output(col))
								# s = "[[%s]]"%(format_output(col))		--2015/06/03支持换行
						# outfp.write("\t\t[%d] = %s,\n"%(colidx + 1, s))
						name = "\"%s\""%(head.get(colidx))
						outfp.write("\t\t[%s] = %s,\n"%(name, s))
					except Exception, e:
						raise Exception("Write Table error (%s,%s,%s) : %s"%(sidx+1,rowidx+1,colidx+1,str(e)))
				outfp.write("\t},\n")
		outfp.write("},\n")
	outfp.write("};\n\n")

	if withfunc: outfp.write(SCRIPT_END)
	outfp.write("\n__XLS_END = true\n")
	outfp.write("\nreturn %s[1]\n" % szName)
	if not outfile or outfile == '-':
		outfp.seek(0)
		print outfp.read()
	outfp.close()

def transfer(dir_src, dir_dst):
	if dir_dst[len(dir_dst) - 1] != '\\':
		dir_dst = dir_dst + "\\"
	# 创建dst_src的目录树
	if not os.path.exists(dir_dst):
		os.makedirs(dir_dst)
		
	names = os.listdir(dir_src)
	tag = True;
	for name in names:
		name_temp = dir_src + "\\" + name

		if os.path.isdir(name_temp): 
			if tag is True:
				dir_temp = dir_dst + name
				transfer(name_temp, dir_temp)
			else:
				continue
		if os.path.isfile(name_temp):
			t, n = gen_table(name_temp)
			#outfile
			outfile = name_temp.split('.')[0]
			outfile = outfile.replace('/', '\\')
			outfile = dir_dst + outfile.split('\\')[-1] + ".lua"
			write_table(t, n, outfile, withfunc = True)
		
def main():
	# 使用默认配置
	work_dir = os.getcwd()
	dir_src = work_dir + "\\" + DIR_SRC
	dir_dst = work_dir + "\\" + DIR_DST

	if len(sys.argv) >= 2:
		dir_src = sys.argv[1]
	if len(sys.argv) >= 3:
		dir_dst = sys.argv[2]

	transfer(dir_src, dir_dst)

if __name__=="__main__":
	main()
