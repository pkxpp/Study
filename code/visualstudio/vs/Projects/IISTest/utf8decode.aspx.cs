using System;
using System.Collections.Generic;
using System.Web;
using System.Web.UI;
using System.Web.UI.WebControls;

namespace IISTest
{
	/// <summary>
	/// UTF-8 解码器
	/// </summary>
	public partial class utf8decode : System.Web.UI.Page
	{
		/// <summary>
		/// 解码按钮按下时
		/// </summary>
		protected void btnDecode_Click(object sender, EventArgs e)
		{
			try
			{
				string code = this.txCode.Text.Trim(' ', '{', '}', '\t', '[', ']');
				if (string.IsNullOrEmpty(code))
				{
					this.lResult.Text = "<b style=\"color:Red\">请输入UTF-8编码！</b>";
					return;
				}
				byte[] bytes = new byte[code.Length / 2];
				for (int i = 0; i < bytes.Length; i++)
				{
					bytes[i] = (byte)((GetCharValue16(code[i * 2]) << 4) + GetCharValue16(code[i * 2 + 1]));
				}
				this.lResult.Text = "解码后的结果为：<br/><b style=\"color:Red\">" + System.Text.Encoding.UTF8.GetString(bytes) + "</b>";
			}
			catch
			{
				this.lResult.Text = "<b style=\"color:Red\">请输入正确的编码！</b>";
			}
		}

		///<summary>
		///获取十六进制字符对应的数值
		///</summary>
		///<params name="ch">要转换的十六进制字符</params>
		///<exception cref="FormatException">当ch不是十六进制字符时抛出</exception>
		public static byte GetCharValue16(char ch)
		{
			if (ch < '0')
				throw new FormatException("不正确的16进制格式！");
			else if (ch <= '9')
				return (byte)(ch - '0');
			else if (ch < 'A')
				throw new FormatException("不正确的16进制格式！");
			else if (ch <= 'F')
				return (byte)(ch - 'A' + 10);
			else if (ch < 'a')
				throw new FormatException("不正确的16进制格式！");
			else if (ch <= 'f')
				return (byte)(ch - 'a' + 10);
			else
				throw new FormatException("不正确的16进制格式！");
		}
	}//class
}