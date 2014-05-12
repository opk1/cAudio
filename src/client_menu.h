#pragma once

#ifndef _CLIENTMENU_
#define _CLIENTMENU_

#include "main.h"

namespace TCPIP 
{

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;
	using namespace System::IO;

	/// <summary>
	/// Summary for client_menu
	/// </summary>
	public ref class client_menu : public System::Windows::Forms::Form
	{
		HWND hwnd;
		float val;
	private: System::Windows::Forms::Button^  button2;

	private: System::ComponentModel::BackgroundWorker^  backgroundWorker1;
	private: System::Windows::Forms::Button^  button3;
	private: System::Windows::Forms::Button^  pause;
	private: System::Windows::Forms::TrackBar^  trackBar1;
	private: System::Windows::Forms::Label^  label1;
	public:
		String^ errMsg;
		client_menu(void)
		{
			InitializeComponent();
			if (!BASS_Init(-1,44100, 0,hwnd,NULL))
			{
				MessageBox::Show("Can't initialize device");
				return;
			}
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~client_menu()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::Button^  ConnectButton;


	private: System::Windows::Forms::Label^  IPLabel;
	private: System::Windows::Forms::Label^  PortLabel;

	private: System::Windows::Forms::TextBox^  IPTextBox;
	private: System::Windows::Forms::TextBox^  PortTextBox;

	





	private: System::Windows::Forms::ListBox^  outputListBox;
	private: System::Windows::Forms::SaveFileDialog^  saveFileDialog1;


	private: System::Windows::Forms::Button^  DisconnectButton;
	private: System::Windows::Forms::Button^  ClearButton;
	private: System::Windows::Forms::Button^  button1;
	private: System::Windows::Forms::Label^  SaveAsLabel;
	private: System::Windows::Forms::TextBox^  SaveAsTextBox;
	private: System::ComponentModel::BackgroundWorker^  backgroundWorker;




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
			this->ConnectButton = (gcnew System::Windows::Forms::Button());
			this->IPLabel = (gcnew System::Windows::Forms::Label());
			this->PortLabel = (gcnew System::Windows::Forms::Label());
			this->IPTextBox = (gcnew System::Windows::Forms::TextBox());
			this->PortTextBox = (gcnew System::Windows::Forms::TextBox());
			this->outputListBox = (gcnew System::Windows::Forms::ListBox());
			this->saveFileDialog1 = (gcnew System::Windows::Forms::SaveFileDialog());
			this->DisconnectButton = (gcnew System::Windows::Forms::Button());
			this->ClearButton = (gcnew System::Windows::Forms::Button());
			this->button1 = (gcnew System::Windows::Forms::Button());
			this->SaveAsLabel = (gcnew System::Windows::Forms::Label());
			this->SaveAsTextBox = (gcnew System::Windows::Forms::TextBox());
			this->backgroundWorker = (gcnew System::ComponentModel::BackgroundWorker());
			this->button2 = (gcnew System::Windows::Forms::Button());
			this->backgroundWorker1 = (gcnew System::ComponentModel::BackgroundWorker());
			this->button3 = (gcnew System::Windows::Forms::Button());
			this->pause = (gcnew System::Windows::Forms::Button());
			this->trackBar1 = (gcnew System::Windows::Forms::TrackBar());
			this->label1 = (gcnew System::Windows::Forms::Label());
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->trackBar1))->BeginInit();
			this->SuspendLayout();
			// 
			// ConnectButton
			// 
			this->ConnectButton->Location = System::Drawing::Point(64, 71);
			this->ConnectButton->Name = L"ConnectButton";
			this->ConnectButton->Size = System::Drawing::Size(75, 23);
			this->ConnectButton->TabIndex = 0;
			this->ConnectButton->Text = L"Connect";
			this->ConnectButton->UseVisualStyleBackColor = true;
			this->ConnectButton->Click += gcnew System::EventHandler(this, &client_menu::ConnectButton_Click);
			// 
			// IPLabel
			// 
			this->IPLabel->AutoSize = true;
			this->IPLabel->Location = System::Drawing::Point(38, 22);
			this->IPLabel->Name = L"IPLabel";
			this->IPLabel->Size = System::Drawing::Size(20, 13);
			this->IPLabel->TabIndex = 2;
			this->IPLabel->Text = L"IP:";
			// 
			// PortLabel
			// 
			this->PortLabel->AutoSize = true;
			this->PortLabel->Location = System::Drawing::Point(32, 48);
			this->PortLabel->Name = L"PortLabel";
			this->PortLabel->Size = System::Drawing::Size(29, 13);
			this->PortLabel->TabIndex = 3;
			this->PortLabel->Text = L"Port:";
			// 
			// IPTextBox
			// 
			this->IPTextBox->Location = System::Drawing::Point(64, 19);
			this->IPTextBox->Name = L"IPTextBox";
			this->IPTextBox->Size = System::Drawing::Size(100, 20);
			this->IPTextBox->TabIndex = 4;
			this->IPTextBox->Text = L"234.234.234.7";
			// 
			// PortTextBox
			// 
			this->PortTextBox->Location = System::Drawing::Point(64, 45);
			this->PortTextBox->Name = L"PortTextBox";
			this->PortTextBox->Size = System::Drawing::Size(100, 20);
			this->PortTextBox->TabIndex = 5;
			this->PortTextBox->Text = L"8190";
			// 
			// outputListBox
			// 
			this->outputListBox->FormattingEnabled = true;
			this->outputListBox->Location = System::Drawing::Point(170, 19);
			this->outputListBox->Name = L"outputListBox";
			this->outputListBox->Size = System::Drawing::Size(211, 43);
			this->outputListBox->TabIndex = 11;
			// 
			// DisconnectButton
			// 
			this->DisconnectButton->Location = System::Drawing::Point(306, 242);
			this->DisconnectButton->Name = L"DisconnectButton";
			this->DisconnectButton->Size = System::Drawing::Size(75, 23);
			this->DisconnectButton->TabIndex = 14;
			this->DisconnectButton->Text = L"Disconnect";
			this->DisconnectButton->UseVisualStyleBackColor = true;
			this->DisconnectButton->Click += gcnew System::EventHandler(this, &client_menu::DisconnectButton_Click);
			// 
			// ClearButton
			// 
			this->ClearButton->FlatStyle = System::Windows::Forms::FlatStyle::Popup;
			this->ClearButton->Location = System::Drawing::Point(170, 74);
			this->ClearButton->Name = L"ClearButton";
			this->ClearButton->Size = System::Drawing::Size(211, 20);
			this->ClearButton->TabIndex = 15;
			this->ClearButton->Text = L"clear";
			this->ClearButton->UseVisualStyleBackColor = true;
			this->ClearButton->Click += gcnew System::EventHandler(this, &client_menu::ClearButton_Click);
			// 
			// button1
			// 
			this->button1->FlatStyle = System::Windows::Forms::FlatStyle::Popup;
			this->button1->Location = System::Drawing::Point(360, 216);
			this->button1->Name = L"button1";
			this->button1->Size = System::Drawing::Size(21, 20);
			this->button1->TabIndex = 18;
			this->button1->Text = L"..";
			this->button1->UseVisualStyleBackColor = true;
			this->button1->Click += gcnew System::EventHandler(this, &client_menu::button1_Click);
			// 
			// SaveAsLabel
			// 
			this->SaveAsLabel->AutoSize = true;
			this->SaveAsLabel->Location = System::Drawing::Point(9, 219);
			this->SaveAsLabel->Name = L"SaveAsLabel";
			this->SaveAsLabel->Size = System::Drawing::Size(49, 13);
			this->SaveAsLabel->TabIndex = 17;
			this->SaveAsLabel->Text = L"Save as:";
			// 
			// SaveAsTextBox
			// 
			this->SaveAsTextBox->Location = System::Drawing::Point(64, 216);
			this->SaveAsTextBox->Name = L"SaveAsTextBox";
			this->SaveAsTextBox->Size = System::Drawing::Size(317, 20);
			this->SaveAsTextBox->TabIndex = 16;
			// 
			// backgroundWorker
			// 
			this->backgroundWorker->WorkerSupportsCancellation = true;
			this->backgroundWorker->DoWork += gcnew System::ComponentModel::DoWorkEventHandler(this, &client_menu::backgroundWorker_DoWork);
			// 
			// button2
			// 
			this->button2->Location = System::Drawing::Point(64, 242);
			this->button2->Name = L"button2";
			this->button2->Size = System::Drawing::Size(75, 23);
			this->button2->TabIndex = 19;
			this->button2->Text = L"Talk";
			this->button2->UseVisualStyleBackColor = true;
			this->button2->Click += gcnew System::EventHandler(this, &client_menu::micTalk);
			// 
			// backgroundWorker1
			// 
			this->backgroundWorker1->WorkerSupportsCancellation = true;
			this->backgroundWorker1->DoWork += gcnew System::ComponentModel::DoWorkEventHandler(this, &client_menu::backgroundWorker1_DoWork);
			// 
			// button3
			// 
			this->button3->Location = System::Drawing::Point(248, 167);
			this->button3->Name = L"button3";
			this->button3->Size = System::Drawing::Size(133, 39);
			this->button3->TabIndex = 20;
			this->button3->Text = L"resume";
			this->button3->UseVisualStyleBackColor = true;
			this->button3->Click += gcnew System::EventHandler(this, &client_menu::resumeButton);
			// 
			// pause
			// 
			this->pause->Location = System::Drawing::Point(64, 167);
			this->pause->Name = L"pause";
			this->pause->Size = System::Drawing::Size(135, 39);
			this->pause->TabIndex = 21;
			this->pause->Text = L"pause";
			this->pause->UseVisualStyleBackColor = true;
			this->pause->Click += gcnew System::EventHandler(this, &client_menu::pauseButton);
			// 
			// trackBar1
			// 
			this->trackBar1->Location = System::Drawing::Point(64, 120);
			this->trackBar1->Name = L"trackBar1";
			this->trackBar1->Size = System::Drawing::Size(317, 45);
			this->trackBar1->TabIndex = 22;
			this->trackBar1->Scroll += gcnew System::EventHandler(this, &client_menu::volume);
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Location = System::Drawing::Point(13, 125);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(45, 13);
			this->label1->TabIndex = 23;
			this->label1->Text = L"Volume:";
			// 
			// client_menu
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(398, 278);
			this->Controls->Add(this->label1);
			this->Controls->Add(this->trackBar1);
			this->Controls->Add(this->pause);
			this->Controls->Add(this->button3);
			this->Controls->Add(this->button2);
			this->Controls->Add(this->button1);
			this->Controls->Add(this->SaveAsLabel);
			this->Controls->Add(this->SaveAsTextBox);
			this->Controls->Add(this->ClearButton);
			this->Controls->Add(this->DisconnectButton);
			this->Controls->Add(this->outputListBox);
			this->Controls->Add(this->PortTextBox);
			this->Controls->Add(this->IPTextBox);
			this->Controls->Add(this->PortLabel);
			this->Controls->Add(this->IPLabel);
			this->Controls->Add(this->ConnectButton);
			this->Name = L"client_menu";
			this->Text = L"client_menu";
			this->FormClosing += gcnew System::Windows::Forms::FormClosingEventHandler(this, &client_menu::client_menu_FormClosing);
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->trackBar1))->EndInit();
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion

	private: System::Void ConnectButton_Click(System::Object^  sender, System::EventArgs^  e) 
			 {
				 this->button1->Enabled = false;
				 this->SaveAsTextBox->Enabled = false;
				 backgroundWorker->RunWorkerAsync();
			 }

	private: System::Void DisconnectButton_Click(System::Object^  sender, System::EventArgs^  e) 
			 {
				 this->backgroundWorker->CancelAsync();
			 }
	private: System::Void ClearButton_Click(System::Object^  sender, System::EventArgs^  e) 
			 {
				 outputListBox->Items->Clear();
			 }


	private: System::Void client_menu_FormClosing(System::Object^  sender, System::Windows::Forms::FormClosingEventArgs^  e) 
			 {
				 exit(1);
			 }
	private: System::Void backgroundWorker_DoWork(System::Object^  sender, System::ComponentModel::DoWorkEventArgs^  e) 
			 {
				 int port = 0;
				 char *fileName;
				 char ip[32]= "";

				 pin_ptr<const wchar_t> tempIP;
				 pin_ptr<const wchar_t> tempFileName;

				 tempIP = PtrToStringChars(this->IPTextBox->Text);
				 if(PortTextBox->Text != "")
					 port = System::Int32::Parse(PortTextBox->Text);

				 wcstombs_s(0, ip, wcslen(tempIP) + 1, tempIP, _TRUNCATE);

				 if(SaveAsTextBox->Text != "")
				 {
					 tempFileName = PtrToStringChars(this->SaveAsTextBox->Text);
					 fileName = (char*) malloc(sizeof(tempFileName));
					 wcstombs_s(0, fileName, wcslen(tempFileName) + 1, tempFileName, _TRUNCATE);
				 }

				 multicast_connect(outputListBox, ip, port);
				 run_client(fileName);
			 }

private: System::Void micTalk(System::Object^  sender, System::EventArgs^  e) {
			 
				 backgroundWorker1->RunWorkerAsync();
		 }
private: System::Void backgroundWorker1_DoWork(System::Object^  sender, System::ComponentModel::DoWorkEventArgs^  e) {
			  recordMic();
		 }
private: System::Void button1_Click(System::Object^  sender, System::EventArgs^  e) {
			 saveFileDialog1->Filter = "WAV files (*.wav)|*.wav";

				 saveFileDialog1->RestoreDirectory = true;
				 saveFileDialog1->ShowDialog();

				 SaveAsTextBox->Text = saveFileDialog1->FileName;
		 }
private: System::Void pauseButton(System::Object^  sender, System::EventArgs^  e) {
			 pauseSong();
		 }
private: System::Void resumeButton(System::Object^  sender, System::EventArgs^  e) {
			 resume();
		 }
private: System::Void volume(System::Object^  sender, System::EventArgs^  e) {
			 val = this->trackBar1->Value;
			 volumeChange(val);
		 }
};
}

#endif
