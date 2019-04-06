<%@ Page Language="C#" AutoEventWireup="true" CodeBehind="utf8encode.aspx.cs" Inherits="IISTest.utf8encode" %>

<!DOCTYPE html>

<html xmlns="http://www.w3.org/1999/xhtml">
<head runat="server">
	<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
	<title>UTF-8 编码器</title>
	<meta name="viewport" content="width=device-width, initial-scale=1,maximum-scale=1, user-scalable=no"/>
</head>
<body>
	<form id="mainForm" runat="server">
		<div style="font-size:1em">
			<h3>UTF-8 编码器</h3>
			<div>请将需要编码的字符输入到下面的文本框，然后单击“编码”按钮。</div>
			<div>
				<asp:TextBox ID="txCode" runat="server" />
				<asp:Button ID="btnEncode" Text="编码" runat="server" OnClick="btnEncode_Click" />
			</div>
			<br />
			<div><asp:Literal ID="lResult" runat="server">编码后的结果为：</asp:Literal></div>
		</div>
	</form>
</body>
</html>
