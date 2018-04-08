using System;
using System.Collections.Generic;
using System.Web;
using System.Web.UI;
using System.Web.UI.WebControls;

namespace IISTest
{
	/// <summary>
	/// UTF-8 编码器
	/// </summary>
	public partial class utf8encode : System.Web.UI.Page
	{
		/// <summary>
		/// 编码按钮按下时
		/// </summary>
		protected void btnEncode_Click(object sender, EventArgs e)
		{
			string text = this.txCode.Text;
			if (string.IsNullOrEmpty(text))
			{
				this.lResult.Text = "<b style=\"color:Red\">请输入要编码的字符串！</b>";
				return;
			}
			byte[] bytes = System.Text.Encoding.UTF8.GetBytes(text);
			System.Text.StringBuilder rBuf = new System.Text.StringBuilder();
			rBuf.Append("编码后的结果为：<br/><b style=\"color:Red\">{");
			for (int i = 0; i < bytes.Length; i++)
				rBuf.Append(bytes[i].ToString("X2"));
			rBuf.Append("}</b>");
			this.lResult.Text = rBuf.ToString();
		}
	}//class
}