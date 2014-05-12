#pragma once

#ifndef _SERVERMENU_
#define _SERVERMENU_

#include "main.h"

#define FILE_SIZE 255

#define PACKETSRECV_LISTBOX WM_APP+1
#define BYTESRECV_LISTBOX   WM_APP+2
#define STRING_LISTBOX      WM_APP+3

namespace TCPIP {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;
	using namespace System::Security::Permissions;
	using namespace System::IO;

	/// <summary>
	/// Summary for server_menu
	/// </summary>
	public ref class server_menu : public System::Windows::Forms::Form
	{
		int i;
		HWND hwnd;
	private: System::Windows::Forms::Button^  button1;
			 char *fileName;
	private: System::Windows::Forms::Label^  label1;
	private: System::Windows::Forms::TextBox^  ipAddressTextBox;

	public:
		String^ errMsg;
		String^ ipAddress;
		server_menu(void):ipAddress("234.234.234.7")
		{
			
			InitializeComponent();
			if (!BASS_Init(-1,44100, 0,hwnd,NULL))
			{
				MessageBox::Show("Can't initialize device");
				return;
			}
			//
			//TODO: Add the constructor code here
			//
		}
		virtual void WndProc(Message% m) override
		{
			if(!(WindowProc((HWND)m.HWnd.ToPointer(), m.Msg, (WPARAM)m.WParam.ToPointer(), (LPARAM)m.LParam.ToPointer())))
				Form::WndProc(m);
		}
		BOOL WindowProc(HWND hwnd, int uMsg, WPARAM wParam, LPARAM lParam)
		{
			if(uMsg == PACKETSRECV_LISTBOX)
			{
				if(wParam != NULL)
				{
					int buf = (int)wParam;
					Object ^Obj = buf; 
					outputListBox->Items->Add("Packets received: " + Obj);
				}
				return 1;
			}
			if(uMsg == BYTESRECV_LISTBOX)
			{
				if(wParam != NULL)
				{
					int buf = (int)wParam;
					Object ^Obj = buf; 
					outputListBox->Items->Add("Bytes received: " + Obj);
				}
				return 1;
			}
			if(uMsg == STRING_LISTBOX)
			{
				if(wParam != NULL)
				{
					char *buf = (char*)wParam;
					String^ s = gcnew String(buf);
					Object ^Obj = s; 
					outputListBox->Items->Add(s);
				}
				return 1;
			}
			return 0;
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~server_menu()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::Button^  ListenButton;
	protected: 

	protected: 

	private: System::Windows::Forms::TextBox^  PortTextBox;
	private: System::Windows::Forms::Label^  PortLabel;
	private: Boolean appActive;
	private: System::Windows::Forms::ListBox^  outputListBox;
	private: System::Windows::Forms::Button^  CloseButton;
	private: System::Windows::Forms::OpenFileDialog^  openFileDialog1;
	private: System::ComponentModel::BackgroundWorker^  backgroundWorker;
	private: System::Windows::Forms::Label^  FileLabel;
	private: System::Windows::Forms::TextBox^  FileTextBox;
	private: System::Windows::Forms::Button^  SendButton;
	private: System::Windows::Forms::Button^  playButton;
	private: System::Windows::Forms::Button^  stopButton;
	private: System::Windows::Forms::Button^  pauseButton;
	private: System::Windows::Forms::Button^  FileDirButton;
	private: System::ComponentModel::BackgroundWorker^  backgroundWorker1;
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
			this->ListenButton = (gcnew System::Windows::Forms::Button());
			this->PortTextBox = (gcnew System::Windows::Forms::TextBox());
			this->PortLabel = (gcnew System::Windows::Forms::Label());
			this->outputListBox = (gcnew System::Windows::Forms::ListBox());
			this->CloseButton = (gcnew System::Windows::Forms::Button());
			this->openFileDialog1 = (gcnew System::Windows::Forms::OpenFileDialog());
			this->backgroundWorker = (gcnew System::ComponentModel::BackgroundWorker());
			this->FileLabel = (gcnew System::Windows::Forms::Label());
			this->FileTextBox = (gcnew System::Windows::Forms::TextBox());
			this->SendButton = (gcnew System::Windows::Forms::Button());
			this->playButton = (gcnew System::Windows::Forms::Button());
			this->stopButton = (gcnew System::Windows::Forms::Button());
			this->pauseButton = (gcnew System::Windows::Forms::Button());
			this->FileDirButton = (gcnew System::Windows::Forms::Button());
			this->backgroundWorker1 = (gcnew System::ComponentModel::BackgroundWorker());
			this->button1 = (gcnew System::Windows::Forms::Button());
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->ipAddressTextBox = (gcnew System::Windows::Forms::TextBox());
			this->SuspendLayout();
			// 
			// ListenButton
			// 
			this->ListenButton->Location = System::Drawing::Point(72, 71);
			this->ListenButton->Name = L"ListenButton";
			this->ListenButton->Size = System::Drawing::Size(75, 23);
			this->ListenButton->TabIndex = 0;
			this->ListenButton->Text = L"Stream";
			this->ListenButton->UseVisualStyleBackColor = true;
			this->ListenButton->Click += gcnew System::EventHandler(this, &server_menu::ListenButton_Click);
			// 
			// PortTextBox
			// 
			this->PortTextBox->Location = System::Drawing::Point(72, 43);
			this->PortTextBox->Name = L"PortTextBox";
			this->PortTextBox->Size = System::Drawing::Size(237, 20);
			this->PortTextBox->TabIndex = 1;
			this->PortTextBox->Text = L"8190";
			// 
			// PortLabel
			// 
			this->PortLabel->AutoSize = true;
			this->PortLabel->Location = System::Drawing::Point(37, 46);
			this->PortLabel->Name = L"PortLabel";
			this->PortLabel->Size = System::Drawing::Size(29, 13);
			this->PortLabel->TabIndex = 2;
			this->PortLabel->Text = L"Port:";
			// 
			// outputListBox
			// 
			this->outputListBox->FormattingEnabled = true;
			this->outputListBox->Location = System::Drawing::Point(73, 146);
			this->outputListBox->Name = L"outputListBox";
			this->outputListBox->Size = System::Drawing::Size(237, 186);
			this->outputListBox->TabIndex = 3;
			// 
			// CloseButton
			// 
			this->CloseButton->Enabled = false;
			this->CloseButton->Location = System::Drawing::Point(234, 71);
			this->CloseButton->Name = L"CloseButton";
			this->CloseButton->Size = System::Drawing::Size(75, 23);
			this->CloseButton->TabIndex = 8;
			this->CloseButton->Text = L"Disconnect";
			this->CloseButton->UseVisualStyleBackColor = true;
			this->CloseButton->Click += gcnew System::EventHandler(this, &server_menu::CloseButton_Click);
			// 
			// backgroundWorker
			// 
			this->backgroundWorker->WorkerSupportsCancellation = true;
			this->backgroundWorker->DoWork += gcnew System::ComponentModel::DoWorkEventHandler(this, &server_menu::backgroundWorker_DoWork);
			// 
			// FileLabel
			// 
			this->FileLabel->AutoSize = true;
			this->FileLabel->Location = System::Drawing::Point(41, 350);
			this->FileLabel->Name = L"FileLabel";
			this->FileLabel->Size = System::Drawing::Size(26, 13);
			this->FileLabel->TabIndex = 13;
			this->FileLabel->Text = L"File:";
			// 
			// FileTextBox
			// 
			this->FileTextBox->Location = System::Drawing::Point(73, 347);
			this->FileTextBox->Name = L"FileTextBox";
			this->FileTextBox->Size = System::Drawing::Size(237, 20);
			this->FileTextBox->TabIndex = 12;
			// 
			// SendButton
			// 
			this->SendButton->Location = System::Drawing::Point(73, 373);
			this->SendButton->Name = L"SendButton";
			this->SendButton->Size = System::Drawing::Size(75, 23);
			this->SendButton->TabIndex = 11;
			this->SendButton->Text = L"Send";
			this->SendButton->UseVisualStyleBackColor = true;
			// 
			// playButton
			// 
			this->playButton->Location = System::Drawing::Point(134, 107);
			this->playButton->Name = L"playButton";
			this->playButton->Size = System::Drawing::Size(58, 23);
			this->playButton->TabIndex = 14;
			this->playButton->Text = L"Play";
			this->playButton->UseVisualStyleBackColor = true;
			this->playButton->Click += gcnew System::EventHandler(this, &server_menu::playButton_Click);
			// 
			// stopButton
			// 
			this->stopButton->Location = System::Drawing::Point(198, 107);
			this->stopButton->Name = L"stopButton";
			this->stopButton->Size = System::Drawing::Size(52, 23);
			this->stopButton->TabIndex = 15;
			this->stopButton->Text = L"Stop";
			this->stopButton->UseVisualStyleBackColor = true;
			this->stopButton->Click += gcnew System::EventHandler(this, &server_menu::stopButton_Click);
			// 
			// pauseButton
			// 
			this->pauseButton->Location = System::Drawing::Point(256, 107);
			this->pauseButton->Name = L"pauseButton";
			this->pauseButton->Size = System::Drawing::Size(53, 23);
			this->pauseButton->TabIndex = 16;
			this->pauseButton->Text = L"Pause";
			this->pauseButton->UseVisualStyleBackColor = true;
			this->pauseButton->Click += gcnew System::EventHandler(this, &server_menu::pauseButton_Click);
			// 
			// FileDirButton
			// 
			this->FileDirButton->FlatStyle = System::Windows::Forms::FlatStyle::Popup;
			this->FileDirButton->Location = System::Drawing::Point(287, 347);
			this->FileDirButton->Name = L"FileDirButton";
			this->FileDirButton->Size = System::Drawing::Size(23, 20);
			this->FileDirButton->TabIndex = 17;
			this->FileDirButton->Text = L"..";
			this->FileDirButton->UseVisualStyleBackColor = true;
			this->FileDirButton->Click += gcnew System::EventHandler(this, &server_menu::FileDirButton_Click_1);
			// 
			// backgroundWorker1
			// 
			this->backgroundWorker1->WorkerSupportsCancellation = true;
			this->backgroundWorker1->DoWork += gcnew System::ComponentModel::DoWorkEventHandler(this, &server_menu::backgroundWorker1_DoWork);

			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Location = System::Drawing::Point(5, 17);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(61, 13);
			this->label1->TabIndex = 19;
			this->label1->Text = L"IP Address:";
			// 
			// ipAddressTextBox
			// 
			this->ipAddressTextBox->Location = System::Drawing::Point(72, 17);
			this->ipAddressTextBox->Name = L"ipAddressTextBox";
			this->ipAddressTextBox->Size = System::Drawing::Size(237, 20);
			this->ipAddressTextBox->TabIndex = 20;
			this->ipAddressTextBox->Text = L"234.234.234.7";
			// 
			// server_menu
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(373, 436);
			this->Controls->Add(this->ipAddressTextBox);
			this->Controls->Add(this->label1);
			this->Controls->Add(this->FileDirButton);
			this->Controls->Add(this->pauseButton);
			this->Controls->Add(this->stopButton);
			this->Controls->Add(this->playButton);
			this->Controls->Add(this->FileLabel);
			this->Controls->Add(this->FileTextBox);
			this->Controls->Add(this->SendButton);
			this->Controls->Add(this->CloseButton);
			this->Controls->Add(this->outputListBox);
			this->Controls->Add(this->PortLabel);
			this->Controls->Add(this->PortTextBox);
			this->Controls->Add(this->ListenButton);
			this->Name = L"server_menu";
			this->Text = L"server_menu";
			this->FormClosing += gcnew System::Windows::Forms::FormClosingEventHandler(this, &server_menu::server_menu_FormClosing);
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
	private: System::Void ListenButton_Click(System::Object^  sender, System::EventArgs^  e) 
			 {
				 this->PortTextBox->Enabled = false;
				 this->ListenButton->Enabled = false;
				 this->CloseButton->Enabled = true;

				 hwnd = static_cast<HWND>(Handle.ToPointer());

				 backgroundWorker->RunWorkerAsync();
			 }
	private: System::Void ClearButton_Click(System::Object^  sender, System::EventArgs^  e) 
			 {
				 outputListBox->Items->Clear();
			 }
	private: System::Void backgroundWorker_DoWork(System::Object^  sender, System::ComponentModel::DoWorkEventArgs^  e) 
			 {
				 int port;
				 char* result;


				 if(PortTextBox->Text != "")
					 port = System::Int32::Parse(PortTextBox->Text);
				 
				if ((ipAddress = (System::String^) ipAddressTextBox->Text) != ""){
					result = (char*)(void*)System::Runtime::InteropServices::Marshal::StringToHGlobalAnsi(ipAddress);
				}

				 init_server(result,port);
				 run_server();
			 }
	private: System::Void CloseButton_Click(System::Object^  sender, System::EventArgs^  e) 
			 {
				 this->backgroundWorker->CancelAsync();

				 // Disable the Cancel button.
				 this->PortTextBox->Enabled = true;
				 this->ListenButton->Enabled = true;
				 this->CloseButton->Enabled = false;
			 }
	private: System::Void server_menu_FormClosing(System::Object^  sender, System::Windows::Forms::FormClosingEventArgs^  e) 
			 {
				 exit(1);
			 }
	private: System::Void clearButton_Click_1(System::Object^  sender, System::EventArgs^  e) 
			 {
				 outputListBox->Items->Clear();
			 }
	private: System::Void playButton_Click(System::Object^  sender, System::EventArgs^  e) 
			 {

				 if(i == 0)
				 {
					 backgroundWorker1->RunWorkerAsync();
				 }else
					 BASS_Start();


				 pin_ptr<const wchar_t> tempFileName;

				 if(FileTextBox->Text != "")
				 {
					 tempFileName = PtrToStringChars(this->FileTextBox->Text);
					 fileName = (char*) malloc(sizeof(tempFileName));
					 wcstombs_s(0, fileName, wcslen(tempFileName) + 1, tempFileName, _TRUNCATE);
				 }
				 play(fileName);
				 this->pauseButton->Text = L"Pause";

			 }
	private: System::Void stopButton_Click(System::Object^  sender, System::EventArgs^  e) 
			 {
				 String^ errMsg		= gcnew String("error");
				 errMsg = "Can't Create Stream File";
				 this->pauseButton->Text = L"Pause";

				 stop(fileName);
			 }
	private: System::Void pauseButton_Click(System::Object^  sender, System::EventArgs^  e) 
			 {
				 if (this->pauseButton->Text == L"Pause"){
					 String^ errMsg		= gcnew String("error");
					 errMsg				= "Can't Create Stream File";
					 pause(fileName);
					 this->pauseButton->Text = L"Continue";
				 }

			 }
	private: System::Void FileDirButton_Click_1(System::Object^  sender, System::EventArgs^  e) 
			 {
				 openFileDialog1->ShowDialog();
				 FileTextBox->Text = openFileDialog1->FileName;
			 }
	private: System::Void backgroundWorker1_DoWork(System::Object^  sender, System::ComponentModel::DoWorkEventArgs^  e) 
			 {
				 char *fileName;
				 pin_ptr<const wchar_t> tempFileName;

				 if(FileTextBox->Text != "")
				 {
					 tempFileName = PtrToStringChars(this->FileTextBox->Text);
					 fileName = (char*) malloc(sizeof(tempFileName));
					 wcstombs_s(0, fileName, wcslen(tempFileName) + 1, tempFileName, _TRUNCATE);
				 }
				 play(fileName);

			 }

};
}

#endif
