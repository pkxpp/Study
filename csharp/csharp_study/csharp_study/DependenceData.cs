using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Collections;
using System.Collections.Concurrent;
using System.IO;

namespace csharp_study
{
    class DependenceDataItem
    {
        public int ID = 0;
        public string FileName = "";
        public string DependFile = "";
    }

    class DependenceData
    {
        ConcurrentDictionary<string, ConcurrentDictionary<string, bool>> m_Datas = null;

        public void LoadData(string strFileName)
        {
            if (!File.Exists(strFileName))
                return;

            TabFile t = new TabFile();
            if (!t.OpenTabFile(strFileName))
                return;

            int nNum = t.GetRowNum();
            if (nNum <= 0)
                return;

            m_Datas = new ConcurrentDictionary<string, ConcurrentDictionary<string, bool>>();

            for (var i = 0; i < nNum; ++i)
            {
                var item = new DependenceDataItem();
                t.GetInteger(i, "ID", 0, out item.ID);
                t.GetString(i, "FileName", "", out item.FileName);
                t.GetString(i, "DependFile", "", out item.DependFile);
                //if (m_Datas[item.FileName].IsEmpty)
                if (!m_Datas.ContainsKey(item.FileName))
                {
                    m_Datas[item.FileName] = new ConcurrentDictionary<string, bool>();
                }
                m_Datas[item.FileName][item.DependFile] = true;
            }

        }

        public string[] GetDependList(string strFileName)
        {
            return m_Datas[strFileName].Keys.ToArray<string>();
        }
    }
}
