using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using System.Windows.Forms;
using System.Runtime.InteropServices;
using System.Data;


namespace csharp_study
{
    class FileHelper /*: System.Windows.Forms.Design.FolderNameEditor*/
    {
        private static Encoding fileEncoding = Encoding.GetEncoding("gb2312");
        // import
        [DllImport("kernel32")]
        private static extern long WritePrivateProfileString(string section, string key, string val, string filePath);

        [DllImport("kernel32")]
        private static extern int GetPrivateProfileString(string section, string key, string def, StringBuilder retVal, int size, string filePath);
#region File Path Function

        // 文件绝对路径名 -> 纯文件名
        static public string FullPathToFileName(string strFullPath)
        {
            int lio1 = strFullPath.LastIndexOf('/');
            int lio2 = strFullPath.LastIndexOf('\\');
            return strFullPath.Substring(lio1 > lio2 ? lio1 + 1 : lio2 + 1);
        }
        //从完整文件名到单个文件名（没有后缀）
        static public string GetFileNameNoExt(string strFileName)
        {
            int lastSlash = strFileName.LastIndexOfAny(new char[] { '\\', '/' });
            //if (lastSlash == -1)
            //    return "<未知文件名>";
            //else
            {
                string name = strFileName.Substring(lastSlash + 1);
                int firstdot = name.IndexOf('.');
                name = name.Substring(0, firstdot);
                return name;
            }
        }
        static public string GetRelativePath(string RootPath , string CurPath)
        {
            string RelativePath = CurPath;// "";
            if (!RootPath.EndsWith("\\"))
                RootPath += "\\";
            int nIndex = -1;
            int nPos = RootPath.IndexOf('\\');
            while(nPos >= 0)
            {
                nPos++;
                if (string.Compare(RootPath, 0, CurPath, 0, nPos, true) != 0)
                    break;
                nIndex = nPos;
                nPos = RootPath.IndexOf('\\', nPos);
            }
            if(nIndex >= 0)
            {
                RelativePath = CurPath.Substring(nIndex);
                nPos = RootPath.IndexOf('\\', nIndex);
                while (nPos >= 0)
                {
                    RelativePath = "..\\" + RelativePath;
                    nPos = RootPath.IndexOf("\\", nPos + 1);
                }
            }
            return RelativePath;
        }

        static public string GetRelativePathNormalize(string RootPath, string CurPath)
        {
            string RelativePath = CurPath;// "";
            if (!RootPath.EndsWith("/"))
                RootPath += "/";
            int nIndex = -1;
            int nPos = RootPath.IndexOf('/');
            while (nPos >= 0)
            {
                nPos++;
                if (string.Compare(RootPath, 0, CurPath, 0, nPos, true) != 0)
                    break;
                nIndex = nPos;
                nPos = RootPath.IndexOf('/', nPos);
            }
            if (nIndex >= 0)
            {
                RelativePath = CurPath.Substring(nIndex);
                nPos = RootPath.IndexOf('/', nIndex);
                while (nPos >= 0)
                {
                    RelativePath = "../" + RelativePath;
                    nPos = RootPath.IndexOf("/", nPos + 1);
                }
            }
            return RelativePath;
        }
#endregion
        #region Browse Files Function
        // private
        //static System.Windows.Forms.Design.FolderNameEditor.FolderBrowser folderBrowser = new System.Windows.Forms.Design.FolderNameEditor.FolderBrowser();
        public FileHelper()
        {
            //folderBrowser.Style = FolderBrowserStyles.BrowseForEverything | FolderBrowserStyles.ShowTextBox;
            //folderBrowser.StartLocation = FolderBrowserFolder.MyComputer;
        }
        ~FileHelper()
        {
            //folderBrowser.Dispose();
        }
        // public method
        // 浏览已存在文件
        static public string BrowseFile(string[] postfixes, string initDir)
        {
            string filter = "";
            foreach (string postfix in postfixes)
            {
                if (postfix != "")
                    filter += string.Format("{0} 文件|*.{1}|", postfix, postfix);
            }

            using (OpenFileDialog ofd = new OpenFileDialog())
            {
                ofd.InitialDirectory = initDir;
                ofd.Filter = filter + "全部文件|*.*";
                if (ofd.ShowDialog() == DialogResult.OK)
                    return ofd.FileName;
                else
                    return string.Empty;
            }
        }
        static public string BrowseFile()
        {
            //return BrowseFile(new string[] { "" }, EditorLayer.StartupPath);
            return "";
        }
        static public string BrowseFile(string[] postfixes)
        {
            //return BrowseFile(postfixes, EditorLayer.StartupPath);
            return "";
        }
        // 浏览要保存的文件
        static public string BrowseFileSave()
        {
            using (SaveFileDialog sfd = new SaveFileDialog())
            {
                sfd.Filter = "All Files|*.*";
                if (sfd.ShowDialog() == DialogResult.OK)
                    return sfd.FileName;
                else
                    return "";
            }
        }
        // 浏览文件夹
        static public string BrowseFolder(string description)
        {
            //folderBrowser.Description = description;
            //if (folderBrowser.ShowDialog() == DialogResult.OK)
            //    return folderBrowser.DirectoryPath;
            //else
                return "";
        }
#endregion


        // 保存string到文件
        static public void StringToFile(string strContent, string strFileName)
        {
            StringToFile(strContent, strFileName, fileEncoding); // 解决乱码关键在于Encoding
        }
        static public void WriteStringToFile(string strContent, string strFileName)
        {
            StringToFile(strContent, strFileName); // 解决乱码关键在于Encoding
        }
        static public void StringToFile(string strContent, string strFileName, Encoding encoding)
        {
            if (!File.Exists(strFileName))
            {
                File.Create(strFileName).Close();
            }
            using (StreamWriter writer = new StreamWriter(strFileName, false, encoding))
            {
                writer.Write(strContent);
            }
        }
        // 读取文件到string
        static public string FileToString(string strFileName)
        {
            return FileToString(strFileName, fileEncoding);
        }
        static public string FileToString(string strFileName, Encoding encoding)
        {
            string strContent = string.Empty;
            try
            {
                using (StreamReader reader = new StreamReader(strFileName, encoding))
                {
                    strContent = reader.ReadToEnd();
                }
            }
            catch //(Exception e)
            {
                //MessageBox.Show(e.Message);
                return "";
            }

            return strContent;
        }
        // ini文件操作

        static public int SafeReadFloatFromIniFile(string section, string key, string def, string filePath, out float fValue)
        {
            StringBuilder strTmp = new StringBuilder(260);
            int nRetCode = 0;

            nRetCode = FileHelper.ReadIniFile(section, key, def, strTmp, 260, filePath);
            if (nRetCode != 0)
            {
                float.TryParse(strTmp.ToString().Trim(), out fValue);
            }
            else
            {
                fValue = 0.0f;
            }


            return nRetCode;
        }

        static public int SafeReadIntFromIniFile(string section, string key, string def, string filePath, out int nValue)
        {
            StringBuilder strTmp = new StringBuilder(260);
            int nRetCode = 0;

            nRetCode = FileHelper.ReadIniFile(section, key, def, strTmp, 260, filePath);
            if (nRetCode != 0)
            {
                int.TryParse(strTmp.ToString().Trim(), out nValue);
            }
            else
            {
                nValue = 0;
            }


            return nRetCode;
        }

        static public int ReadIniFile(string section, string key, string def, StringBuilder retVal, int size, string filePath)
        {
            return GetPrivateProfileString(section, key, def, retVal, size, filePath);
        }
        static public long WriteIniFile(string section, string key, string val, string filePath)
        {
            return WritePrivateProfileString(section, key, val, filePath);
        }

        public static string GetFileContent(string filePath, Encoding encoding)
        {
            string content = string.Empty;

            if (File.Exists(filePath))
            {
                using (FileStream fs = new FileStream(filePath, FileMode.Open, FileAccess.Read, FileShare.ReadWrite))
                using (StreamReader reader = new StreamReader(fs, encoding))
                {
                    //StreamReader reader = new StreamReader(filePath, encoding);
                    content = reader.ReadToEnd();
                }
            }
            else
            {
                MessageBox.Show(string.Format("文件{0}不存在！", filePath), "读取数据文件",
                                MessageBoxButtons.OK, MessageBoxIcon.Information);
            }

            return content;
        }
#region 文件保存
        public static bool SaveFile(string fileName, string content)
        {
            return SaveFile(fileName, content, fileEncoding, false);
        }

        public static bool SaveFile(string fileName, string content, bool append)
        {
            return SaveFile(fileName, content, fileEncoding, append);
        }

        public static bool SaveFile(string fileName, string content, Encoding encoding)
        {
            return SaveFile(fileName, content, encoding, false);
        }
        public static bool SaveFile(string fileName,string content,Encoding encoding,bool append)
        {
            bool saveSuccess = false;

            string directoryPath = Path.GetDirectoryName(fileName);
            if(!Directory.Exists(directoryPath))
            {
                Directory.CreateDirectory(directoryPath);
            }

             using (StreamWriter streamWriter = new StreamWriter(fileName, append, encoding))
            {
                streamWriter.Write(content);
                saveSuccess = true;
            }

            return saveSuccess;
        }
#endregion
        
#region DataTable 相关方法

        /// <summary>
        /// 创建数据表
        /// </summary>
        /// <param name="content">内容</param>
        /// <returns>数据表</returns>
        /// 
        public static DataTable CreateDataTable(string content,string tableName)
        {
            DataTable table = new DataTable(tableName);
            List<string> fieldList = new List<string>();

            bool firstLine = true;
            string[] lines = content.Split(new char[] { '\n' });

            foreach (string s in lines)
            {
                string line = s.TrimEnd(new char[] { '\r' });

                if (line != "") // 跳过空的数据行
                {
                    string[] data = line.Split(new char[] { '\t' });

                    if (firstLine)
                    {
                        firstLine = false;

                        for (int i = 0; i < data.Length; i++)
                        {
                            DataColumn dataColumn = new DataColumn(data[i]);
                            table.Columns.Add(dataColumn);
                            fieldList.Add(data[i]);
                        }
                    }
                    else
                    {
                        DataRow dataRow = table.NewRow();

                        for (int i = 0; i < data.Length; i++)
                        {
                            dataRow[i] = data[i];
                        }

                        table.Rows.Add(dataRow);
                    }
                }

            }

            return table;
        }

        public static string DataTableToString(DataTable dataTable)
        {
            StringBuilder content = new StringBuilder();
            StringBuilder titleLine = new StringBuilder();

            foreach (DataColumn dataColumn in dataTable.Columns)
            {
                titleLine.Append(string.Format("{0}\t", dataColumn.ColumnName));
            }

            titleLine.Remove(titleLine.Length - 1, 1);
            content.AppendLine(titleLine.ToString());

            foreach (DataRow dataRow in dataTable.Rows)
            {
                StringBuilder line = new StringBuilder();

                for (int i = 0; i < dataTable.Columns.Count; i++)
                {
                    string str = dataRow[i].ToString();
                    if (dataTable.Columns[i].DataType == typeof(bool) && dataRow[i] != DBNull.Value)
                    {
                        if (str == "True")
                            str = "1";
                        else str = "0";
                    }
                    line.Append(string.Format("{0}\t", str));
                }

                line.Remove(line.Length - 1, 1);
                content.AppendLine(line.ToString());
            }

            return content.ToString();
        }

        public static DataTable ReadTabFile(string fileName)
        {
            return ReadTabFile(fileName, fileEncoding);
        }
        public static DataTable ReadTabFile(string fileName, Encoding encoding)
        {
            DataTable tabData = null;
            string content = GetFileContent(fileName,encoding);
            string name = GetFileNameNoExt(fileName);

            if (!string.IsNullOrEmpty(content))
            {
                tabData = CreateDataTable(content, name);
            }
            return tabData;
        }
        public static bool SaveTabFile(string fileName, DataTable tabData, Encoding encoding)
        {
            string content = DataTableToString(tabData);
            return SaveFile(fileName, content, encoding);
        }

        public static bool SaveTabFile(string fileName, DataTable tabData)
        {
            string content = DataTableToString(tabData);
            return SaveFile(fileName, content);
        }
#endregion
#region 获取文件名，扩展名，目录
        public static string GetFileName(String path)
        {
            //int nPos = path.LastIndexOfAny(spl);
            return System.IO.Path.GetFileName(path);
        }
        public static string GetFileExt(string strFileName)
        {
            return strFileName.Substring(strFileName.LastIndexOf(".") + 1);
        }
        public static String GetDirectory(string filename)
        {
            char[] spl = { '/', '\\' };
            int nPos = filename.LastIndexOfAny(spl);
            return filename.Substring(0, nPos);
        }
        public static string ReplaceFileExt(string filename, string ext)
        {
            int nPos = filename.LastIndexOf(".");
            return filename.Substring(0, nPos + 1) + ext;
        }

        public static string NormalizeSplitChar(string _path)
        {
            return _path.Replace("\\", "/");
        }

        #endregion

    }
}
