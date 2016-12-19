#pragma once


namespace DemoDraw {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	/// <summary>
	/// Summary for Form1
	///
	/// WARNING: If you change the name of this class, you will need to change the
	///          'Resource File Name' property for the managed resource compiler tool
	///          associated with all .resx files this class depends on.  Otherwise,
	///          the designers will not be able to interact properly with localized
	///          resources associated with this form.
	/// </summary>
	public ref class Form1 : public System::Windows::Forms::Form
	{
	public:
		Form1(void)
		{
			InitializeComponent();
			//
			//TODO: Add the constructor code here
			//
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~Form1()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::MenuStrip^  menuStrip1;
	private: System::Windows::Forms::ToolStripMenuItem^  toolsToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  circlesToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  boxesToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  helpToolStripMenuItem;
	private: System::Windows::Forms::ToolStrip^  toolStrip1;
	private: System::Windows::Forms::ToolStripButton^  DrawCircleButton;


	protected: 

	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>
		System::ComponentModel::Container ^components;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			System::ComponentModel::ComponentResourceManager^  resources = (gcnew System::ComponentModel::ComponentResourceManager(Form1::typeid));
			this->menuStrip1 = (gcnew System::Windows::Forms::MenuStrip());
			this->toolsToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->circlesToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->boxesToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->helpToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->toolStrip1 = (gcnew System::Windows::Forms::ToolStrip());
			this->DrawCircleButton = (gcnew System::Windows::Forms::ToolStripButton());
			this->menuStrip1->SuspendLayout();
			this->toolStrip1->SuspendLayout();
			this->SuspendLayout();
			// 
			// menuStrip1
			// 
			this->menuStrip1->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(2) {this->toolsToolStripMenuItem, 
				this->helpToolStripMenuItem});
			this->menuStrip1->Location = System::Drawing::Point(0, 0);
			this->menuStrip1->Name = L"menuStrip1";
			this->menuStrip1->Size = System::Drawing::Size(284, 25);
			this->menuStrip1->TabIndex = 0;
			this->menuStrip1->Text = L"menuStrip1";
			// 
			// toolsToolStripMenuItem
			// 
			this->toolsToolStripMenuItem->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(2) {this->circlesToolStripMenuItem, 
				this->boxesToolStripMenuItem});
			this->toolsToolStripMenuItem->Name = L"toolsToolStripMenuItem";
			this->toolsToolStripMenuItem->Size = System::Drawing::Size(52, 21);
			this->toolsToolStripMenuItem->Text = L"Tools";
			// 
			// circlesToolStripMenuItem
			// 
			this->circlesToolStripMenuItem->Name = L"circlesToolStripMenuItem";
			this->circlesToolStripMenuItem->Size = System::Drawing::Size(152, 22);
			this->circlesToolStripMenuItem->Text = L"Circles";
			this->circlesToolStripMenuItem->ToolTipText = L"画一个圆";
			// 
			// boxesToolStripMenuItem
			// 
			this->boxesToolStripMenuItem->Name = L"boxesToolStripMenuItem";
			this->boxesToolStripMenuItem->Size = System::Drawing::Size(152, 22);
			this->boxesToolStripMenuItem->Text = L"Boxes";
			this->boxesToolStripMenuItem->ToolTipText = L"画一个正方形";
			// 
			// helpToolStripMenuItem
			// 
			this->helpToolStripMenuItem->Name = L"helpToolStripMenuItem";
			this->helpToolStripMenuItem->Size = System::Drawing::Size(47, 21);
			this->helpToolStripMenuItem->Text = L"Help";
			// 
			// toolStrip1
			// 
			this->toolStrip1->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(1) {this->DrawCircleButton});
			this->toolStrip1->Location = System::Drawing::Point(0, 25);
			this->toolStrip1->Name = L"toolStrip1";
			this->toolStrip1->Size = System::Drawing::Size(284, 25);
			this->toolStrip1->TabIndex = 1;
			this->toolStrip1->Text = L"toolStrip1";
			// 
			// DrawCircleButton
			// 
			this->DrawCircleButton->DisplayStyle = System::Windows::Forms::ToolStripItemDisplayStyle::Image;
			this->DrawCircleButton->Image = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"DrawCircleButton.Image")));
			this->DrawCircleButton->ImageTransparentColor = System::Drawing::Color::Chartreuse;
			this->DrawCircleButton->Name = L"DrawCircleButton";
			this->DrawCircleButton->Size = System::Drawing::Size(23, 22);
			this->DrawCircleButton->Text = L"画一个圆";
			this->DrawCircleButton->Click += gcnew System::EventHandler(this, &Form1::DrawCircleButton_Click);
			// 
			// Form1
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 12);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(284, 262);
			this->Controls->Add(this->toolStrip1);
			this->Controls->Add(this->menuStrip1);
			this->MainMenuStrip = this->menuStrip1;
			this->Name = L"Form1";
			this->Text = L"Form1";
			this->menuStrip1->ResumeLayout(false);
			this->menuStrip1->PerformLayout();
			this->toolStrip1->ResumeLayout(false);
			this->toolStrip1->PerformLayout();
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
	private: System::Void DrawCircleButton_Click(System::Object^  sender, System::EventArgs^  e) {
				 System::Drawing::SolidBrush^ myBrush = gcnew
					 System::Drawing::SolidBrush(System::Drawing::Color::Blue);
				 System::Drawing::Graphics^ formGraphics;
				 formGraphics = this->CreateGraphics();
				 formGraphics->FillEllipse(myBrush, Rectangle(100, 100, 200, 200));
				 delete myBrush;
				 delete formGraphics;
			 }
};
}

