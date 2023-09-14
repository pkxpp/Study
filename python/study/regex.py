import getopt          # get command-line options
import os.path         # getting extension from file
import string          # string manipulation
import sys             # output and stuff
import re              # for regular expressions

_str = 'https://www.baidu.com/'
print(re.search('baidu', _str))
print(re.search('goole', _str))

re_sv_target_suffix = re.compile('(\w+): SV_Target.*?', re.I | re.VERBOSE)
re_1 = re.compile('[\w\s,\(\)]+(?=\s*\:\s*SV_Target)', re.I | re.VERBOSE)
_str = 'float4 PS(VertexOut pin, uniform int gLightCount, uniform bool gUseTexure, uniform bool gAlphaClip, uniform bool gFogEnabled) : SV_Target'

##run regex on an string array
# @returns either a found result or None
def getRegSearch(txt, regex):
	for str in txt:
		print(str)
		search = regex.search(str)
		if search is not None:
			return search.group(0)
	return None

def getRegSearchLineNotRemove(str, regex):
	search = regex.search(str)
	if search is not None:
		return search.group(0)
	return str

print(getRegSearchLineNotRemove(_str, re_1))

re_2 = re.compile('gFogEnabled\)\s*(?=\:\s*SV_Target)', re.I | re.VERBOSE)
_str1 = 'float4 PS(VertexOut pin, uniform int gLightCount, uniform bool gUseTexure, uniform bool gAlphaClip, uniform bool gFogEnabled) : SV_Target'
s = re_2.search(_str1)
if s is not None:
	print(s.group(0))
else:
	print(None)

# reg_2 = re.compile('202[0-1](?=abc)', re.I | re.VERBOSE)
# str = '2020abc, 2021ddd'
# s = reg_2.search(str)
# if s is not None:
# 	print(s.group(0))

a = "VertexShader MainVertexShader = KG3D_CompileVS(VertexShaderMain());"
# VertexShader MainInstanceVertexShader = KG3D_CompileVS(InstanceVertexShaderMain());

# VertexShader ZWriteVertexShader = KG3D_CompileVS(VS_ZWriting());
# VertexShader ZWriteInstanceVertexShader = KG3D_CompileVS(VS_ZWritingInstance());

b = "PixelShader MainPixelShader = KG3D_CompilePS(PixelShaderMain(false));"
c = "PixelShader ZWritePixelShader = KG3D_CompilePS(PS_ZWriting());"
# PixelShader ShockWavePixelShader = KG3D_CompilePS(PS_ShockWave());
# PixelShader GBufferPixelShader = KG3D_CompilePS(PS_GBufferWriting());
# PixelShader AlbedoPixelShader = KG3D_CompilePS(PS_AlbedoColor());
# PixelShader SoftMaskShader = KG3D_CompilePS(PixelShaderMain(true));
def getFunctionNameAndBody(line):
	mobj = re.match(r'[\w]+\s(\w+)\s=\sKG3D_CompileVS\((\w+\(\w*\))\)', line, re.M | re.I)
	if not mobj :
		mobj = re.match(r'[\w]+\s(\w+)\s=\sKG3D_CompilePS\((\w+\(\w*\))\)', line, re.M | re.I)
	if mobj :
		print(mobj.group(1))
		print(mobj.group(2))
	else :
		print("No match.")

getFunctionNameAndBody(a)
getFunctionNameAndBody(b)
getFunctionNameAndBody(c)