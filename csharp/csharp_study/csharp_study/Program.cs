using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Collections;
using System.Collections.Concurrent;
using System.IO;
using System.Text.RegularExpressions;
using System.Diagnostics;

namespace csharp_study
{
    // 1. object convert to struct
    public struct A
    {
        public int m;
        public int n;
    }
    public partial class Rules
    {
        //static ConcurrentDictionary<string, bool> ms_npcs = null;
        ArrayList m_Rules = new ArrayList();
        public void ReadRules(string strRule)
        {
            if (!File.Exists(strRule))
                return;

            using (StreamReader sr = new StreamReader(strRule, Encoding.GetEncoding("gb2312")))
            {
                string line = sr.ReadLine();
                while (!string.IsNullOrEmpty(line))
                {
                    m_Rules.Add(line.ToLower());
                    line = sr.ReadLine();
                }
            }
        }

        public bool IsInRules(string strFile)
        {
            foreach (string strRule in m_Rules)
            {
                if (strFile.Contains(strRule))
                    return true;
            }
            return false;
        }

        public bool IsInRegexRules(string strFile)
        {
            //string str01 = "hello i am study";
            //string str02 = Regex.Replace(str01, "^", "Change ");
            //bool bRet = Regex.IsMatch("data\\source\\maps_source\\远景树\\s_远景枫树001_001_lod2.jsondesc", @"data\\source.*");
            //Console.WriteLine("bRet = {0}", bRet ? "true" : "false");

            //bool bRet = Regex.IsMatch("data\\resourcebindings.xml", @"data\\resourcebindings.xml"); // true
            bool bRet = strFile == "data\\resourcebindings.xml";
            //bRet = Regex.IsMatch(strFile, @"data\\resourcebindings.xml"); // true
            //bRet = Regex.IsMatch(strFile, strFile); // false
            //bRet = Regex.IsMatch(strFile, @strFile); // false
            foreach (string strRule in m_Rules)
            {
                if (Regex.IsMatch(strFile, strRule))
                    return true;
            }
            
            return false;
        }
    }

    class Program
    {
        static void Main(string[] args)
        {
            A objA = new A();
            objA.m = 0;
            objA.n = 1;
            object o = (object)objA;
            A objB = (A)o;

            //System.Console.WriteLine("m = {0},n = {1}", objB.m, objB.n);
            //System.Console.Read();

            Rules r = new Rules();
            //ms_npcs = new ConcurrentDictionary<string, bool>();
            r.ReadRules("npcs.rules");
            bool bRet = r.IsInRules("data\\source\\item\\prop\\d002002b_hd_combi.mesh");
            Console.WriteLine("Ret = {0}", bRet ? "true" : "false");

            Rules reg = new Rules();
            reg.ReadRules("base.whitelist");
            //Debug.Assert(reg.IsInRegexRules("cachedshaders\\{00022677-93e5-63dc-0000-000000000000}.bin")); // true
            //Debug.Assert(reg.IsInRegexRules("data\\resourcebindings.xml"));   // true
            //bRet = Regex.IsMatch("data\\rtspv\\rt_caustics.rchit.spv", @"data\\rtspv\\.*"); // true
            //Debug.Assert(reg.IsInRegexRules("data\\rtspv\\rt_caustics.rchit.spv"));
            //bRet = Regex.IsMatch("data\\movie\\rt_caustics.rchit.spv", @"data\\movie\\(稻香村|camera|facedata|ikbonebound|script|subtitlecontentpool.txt).*"); // false
            //bRet = Regex.IsMatch("data\\movie\\稻香村\\rt_caustics.rchit.spv", @"data\\movie\\(稻香村|camera|facedata|ikbonebound|script|subtitlecontentpool.txt).*"); // true
            //bRet = Regex.IsMatch("data\\movie\\subtitlecontentpool.txt", @"data\\movie\\(稻香村|camera|facedata|ikbonebound|script|subtitlecontentpool.txt).*"); // true
            //Debug.Assert(reg.IsInRegexRules("data\\movie\\稻香村\\rt_caustics.rchit.spv"));   // true
            //bRet = Regex.IsMatch("data\\rcdata\\textures\\transmittancetexsunset.dds", @"data\\rcdata\\(?!pythonlib\\).*"); // true
            //bRet = Regex.IsMatch("data\\rcdata\\pythonlib\\textures\\transmittancetexsunset.dds", @"data\\rcdata\\(?!pythonlib\\).*"); // false
            //Debug.Assert(reg.IsInRegexRules("data\\rcdata\\textures\\transmittancetexsunset.dds")); // true
            //Debug.Assert(reg.IsInRegexRules("data\\rcdata\\pythonlib\\textures\\transmittancetexsunset.dds")); // true
            //bRet = Regex.IsMatch("data\\source\\npc_source\\face\\模型\\e01003_face_combi.jsoninspack", @"data\\source\\.*?\\face\\.*?(.tga|.dds)"); // false
            //bRet = Regex.IsMatch("data\\source\\npc_source\\face\\模型\\e01003_face_combi.dds", @"data\\source\\.*?\\face\\.*?(.tga|.dds)"); // true
            //bRet = Regex.IsMatch("data\\source\\npc_source\\face\\模型\\e01003_face_combi_dds", @"data\\source\\.*?\\face\\.*?(\.tga|\.dds)"); // false
            //bRet = Regex.IsMatch("data\\source\\npc_source\\face\\模型\\e01003_face_combi.dds", @"data\\source\\.*?\\face\\.*?(\.tga|\.dds)"); // true
            //var match = Regex.Match("data\\source\\npc_source\\face\\模型\\e01003_face_combi.dds", @"data\\source\\(.*?)\\face\\(.*)(.tga|.dds)");
            //Debug.Assert(reg.IsInRegexRules("data\\source\\npc_source\\face\\模型\\e01003_face_combi.dds")); // true
            //Debug.Assert(reg.IsInRegexRules("data\\source\\npc_source\\face\\模型\\e01003_face_combi_dds")); // true
            //bRet = Regex.IsMatch("data\\source\\player\\f1\\动作\\f1b01ty奔跑.ani", @"data\\source\\.*?动作.*"); // true
            //Debug.Assert(reg.IsInRegexRules("data\\source\\player\\f1\\动作\\f1b01ty奔跑.ani")); // true
            //bRet = Regex.IsMatch("data\\source\\player\\autolodconfig.ini", @"data\\source\\player\\autolodconfig.ini--11"); // false
            //Debug.Assert(reg.IsInRegexRules("data\\resourcebindings.xml"));   // true
            //bRet = Regex.IsMatch("data\\source\\player\\minimap\\autolodconfig.ini", @"data\\source\\.*?(minimap\\).*?(?!(\.dds|\.tga))"); // true
            //bRet = Regex.IsMatch("data\\source\\player\\minimap\\autolodconfig.dds", @"data\\source\\.*?\.(?!dds)"); // false
            //bRet = Regex.IsMatch("data\\source\\player\\minimap\\autolodconfig.ini", @"data\\source\\.*?minimap\\.*?\.(?!dds)|(?!tga)"); // true
            //bRet = Regex.IsMatch("data\\source\\player\\minimap\\autolodconfig.dds", @"data\\source\\.*?minimap\\.*?\.(?!(dds|tga))"); // false
            //bRet = Regex.IsMatch("data\\source\\player\\minimap\\autolodconfig.ini", @"data\\source\\.*?minimap\\.*?\.(?!(dds|tga))"); // true
            //var match = Regex.Match("data\\source\\player\\minimap\\autolodconfig.dds", @"(data\\source\\.*?minimap\\.*?)\.(?!dds)|(?!tga)");
            //Debug.Assert(reg.IsInRegexRules("data\\source\\player\\minimap\\autolodconfig.ini"));   // true
            //Debug.Assert(reg.IsInRegexRules("data\\source\\player\\minimap\\autolodconfig.tga"));   // false
            //bRet = Regex.IsMatch("data\\source\\npc_source\\e01003\\模型\\e01003_face_combi.jsoninspack", @"(?!data\\source\\maps\\).*?\.(inspack|jsoninspack|ini|mdl|mtl||tani|group|plight|modelst|jsonlight)"); // true
            //bRet = Regex.IsMatch("abc\\data\\source\\npc_source\\e01003\\模型\\e01003_face_combi.jsoninspack", @"^(?!data\\source\\maps\\).*?\.(inspack|jsoninspack|ini|mdl|mtl||tani|group|plight|modelst|jsonlight)"); // true
            //bRet = Regex.IsMatch("abc\\data\\source\\maps\\npc_source\\e01003\\模型\\e01003_face_combi.jsoninspack", @"^(?!data\\source\\maps)\\.*?\.(inspack|jsoninspack|ini|mdl|mtl||tani|group|plight|modelst|jsonlight)"); // true
            //bRet = Regex.IsMatch("data\\source\\maps\\npc_source\\e01003\\模型\\e01003_face_combi.jsoninspack", @"^(?!data\\source\\maps)\\.*?\.(inspack|jsoninspack|ini|mdl|mtl||tani|group|plight|modelst|jsonlight)"); // false
            //bRet = Regex.IsMatch("data\\source\\npc_source\\e01003\\模型\\e01003_face_combi.dds", @"^(?!data\\source\\maps)\\.*?\.(inspack|jsoninspack|ini|mdl|mtl||tani|group|plight|modelst|jsonlight)"); // false
            //bRet = Regex.IsMatch("data\\source\\maps\\npc_source\\e01003\\模型\\e01003_face_combi.jsoninspack", @"data\\source\\(?!maps)\\.*?\.(inspack|jsoninspack|ini|mdl|mtl||tani|group|plight|modelst|jsonlight)$"); // false
            //bRet = Regex.IsMatch("data\\source\\ps\\npc_source\\e01003\\模型\\e01003_face_combi.jsoninspack", @"data\\source\\(?!maps\\).*?\.(inspack|jsoninspack|ini|mdl|mtl||tani|group|plight|modelst|jsonlight)$"); // true
            //bRet = Regex.IsMatch("data\\source\\ps\\npc_source\\e01003\\模型\\e01003_face_combi.ps", @"data\\source\\(?!maps\\).*?\.(inspack|jsoninspack|ini|mdl|mtl||tani|group|plight|modelst|jsonlight)$"); // false
            //bRet = Regex.IsMatch("data\\source\\e01003_face_combi.ini", @"data\\source\\(?!maps\\).*?\.(inspack|jsoninspack|ini|mdl|mtl||tani|group|plight|modelst|jsonlight)$"); // false
            //var match = Regex.Match("data\\source\\ms\\npc_source\\e01003\\模型\\e01003_face_combi.jsoninspack", @"data\\source\\(?!maps\\).*?\.(inspack|jsoninspack|ini|mdl|mtl||tani|group|plight|modelst|jsonlight)$");
            //Debug.Assert(reg.IsInRegexRules("data\\source\\npc_source\\e01003\\模型\\e01003_face_combi.jsoninspack"));   // true
            //Debug.Assert(reg.IsInRegexRules("data\\source\\maps\\npc_source\\e01003\\模型\\e01003_face_combi.jsoninspack"));   // false
            //bRet = Regex.IsMatch("maps\\e01003_face_combi.aob", @"maps\\.*?\.(aob|cfg|doodad|entity|npc)$"); // true
            //var match = Regex.Match("maps\\e01003_face_combi.aob", @"maps\\.*?\.(aob|cfg|doodad|entity|npc)$");
            //bRet = Regex.IsMatch("ui\\e01003_face_combi.aob", @"ui\\(?!video\\).*"); // true
            //bRet = Regex.IsMatch("ui\\video\\e01003_face_combi.aob", @"ui\\(?!video\\).*"); // false
            //bRet = Regex.IsMatch("ui\\traits\\e01003_face_combi.aob", @"ui\\(?!traits\\mobilestreaming\\).*"); // true
            //bRet = Regex.IsMatch("ui\\traits\\mobilestreaming\\e01003_face_combi.aob", @"ui\\(?!traits\\mobilestreaming\\).*"); // false
            //var match = Regex.Match("ui\\traits\\e01003_face_combi.aob", @"ui\\(?!traits\\mobilestreaming\\).*");


            // 不在预期内的一些情况
            //bRet = Regex.IsMatch("bin64\\shaderlistupload\\gamedesignertoolssettings\\connectionconfig.ini", @"settings\\.*"); // true
            //bRet = Regex.IsMatch("bin64\\shaderlistupload\\gamedesignertoolssettings\\connectionconfig.ini", @"^settings\\.*"); // false， 修正后
            //bRet = Regex.IsMatch("settings\\connectionconfig.ini", @"^settings\\.*"); // true， 修正后

            bRet = Regex.IsMatch("data\\source\\m_prefab\\建筑\\治安建筑\\谋士府\\city_c_lv01_谋士府_3x3_a1.jsoninspack", @"^data\\source\\(?!m_prefab\\).*?\.(inspack|jsoninspack|ini|mdl|mtl||tani|group|plight|modelst|jsonlight)$"); // 

            DependenceData dd = new DependenceData();
            dd.LoadData("dependence_data.txt");
            ArrayList l = new ArrayList(dd.GetDependList("data/source/m_buildingfinal/mapbuilding/二版三号佛窟1.scenenode"));

            System.Console.Read();
        }
    }


}
