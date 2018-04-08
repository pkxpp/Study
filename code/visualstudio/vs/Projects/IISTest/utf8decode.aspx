<%@ Page Language="C#" AutoEventWireup="true" CodeBehind="utf8decode.aspx.cs" Inherits="IISTest.utf8decode" %>

<!DOCTYPE html>

<html xmlns="http://www.w3.org/1999/xhtml">
<head runat="server">
	<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
	<title>UTF-8 解码器</title>
	<meta name="viewport" content="width=device-width, initial-scale=1,maximum-scale=1, user-scalable=no"/>
</head>
<body>
	<form id="mainForm" runat="server">
		<div style="font-size:1em">
			<h3>UTF-8 解码器</h3>
			<div>请将UTF-8的16进制编码输入到下面的文本框，然后单击“解码”按钮。</div>
			<div>
				<asp:TextBox ID="txCode" runat="server" />
				<asp:Button ID="btnDecode" Text="解码" runat="server" OnClick="btnDecode_Click" />
			</div>
			<br />
			<div><asp:Literal ID="lResult" runat="server">解码后的结果为：</asp:Literal></div>
		</div>
	</form>
</body>
</html>
