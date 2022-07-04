using System;
using System.Data;
namespace csharp_study
{
    class TabFile
    {
        private DataTable m_table = null;

        public bool OpenTabFile(string fileName)
        {
            m_table = FileHelper.ReadTabFile(fileName);
            return true;
        }
        public bool SaveTabFile(string fileName)
        {
            return FileHelper.SaveTabFile(fileName, m_table);
        }
        public int GetRowNum()
        {
            return m_table.Rows.Count;
        }
        public int GetColumnNum()
        {
            return m_table.Columns.Count;
        }
        public bool GetString(int nRow, string strColumn, string strDefault, out string strReturn)
        {
            try
            {
                strReturn = m_table.Rows[nRow][strColumn].ToString();
                return true;
            }
            catch //(Exception e)
            {
                strReturn = strDefault;
                return false;
            }
        }

        public bool GetString(int nRow, int nColumn, string strDefault, out string strReturn)
        {
            try
            {
                strReturn = m_table.Rows[nRow][nColumn].ToString();
                return true;
            }
            catch //(Exception e)
            {
                strReturn = strDefault;
                return false;
            }
        }

        public bool GetInteger(int nRow, string strColumn, int nDefault, out int nReturn)
        {
            try
            {
                string strReturn = m_table.Rows[nRow][strColumn].ToString();
                nReturn = Int32.Parse(strReturn);
                return true;
            }
            catch //(Exception e)
            {
                nReturn = nDefault;
                return false;
            }
        }

        public bool GetInteger(int nRow, int nColumn, int nDefault, out int nReturn)
        {
            try
            {
                string strReturn = m_table.Rows[nRow][nColumn].ToString();
                nReturn = Int32.Parse(strReturn);
                return true;
            }
            catch //(Exception e)
            {
                nReturn = nDefault;
                return false;
            }
        }
    }
}
