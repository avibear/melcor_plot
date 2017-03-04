#include "MainForm.h"

using namespace System::IO;

melcor_plot::MainForm::MainForm(void)
{
	InitializeComponent();
	//
	//TODO: Add the constructor code here
	//
	
	uvdatas = gcnew UVData();
	//throw gcnew System::NotImplementedException();
}

System::Void melcor_plot::MainForm::button1_Click(System::Object ^ sender, System::EventArgs ^ e)
{
	textBox3->Text = "Select file\r\n";

	OpenFileDialog^ ofd = gcnew OpenFileDialog;

	ofd->Filter = "dat files (*.dat)|*.dat|txt files (*.txt)|*.txt|All files (*.*)|*.*";
	ofd->FilterIndex = 1;
	ofd->RestoreDirectory = true;

	if (ofd->ShowDialog() == System::Windows::Forms::DialogResult::OK)
	{
		String^ str = gcnew String(ofd->FileName);

		open_file(str);

		//
		array<String^>^ strarr = str->Split('\\');
		path = gcnew String("");


		for (int i = 0; i < strarr->Length - 1; i++)
		{
			path += strarr[i] + "\\";
		}
		openfile = strarr[strarr->Length - 1];

		strarr = openfile->Split('.');
		
		savefile = gcnew String(uvdatas->title->ToLower() + ".csv");
		
		textBox2->Text = path + savefile;
		button1->Enabled = false;
		button2->Enabled = true;
		button3->Enabled = true;
		button4->Enabled = true;
	}
	else
	{
		textBox1->Text = "";
	}

}

System::Void melcor_plot::MainForm::button2_Click(System::Object ^ sender, System::EventArgs ^ e)
{
	//체크리스트 정리
	array<int>^ lists;

	for (int i = 0; i < checkedListBox1->Items->Count; i++)
	{
		if (checkedListBox1->GetItemCheckState(i) == CheckState::Checked)
		{
			if (lists != nullptr)
			{
				int n = lists->Length;
				array<int>^ bk_lists = gcnew array<int>(n + 1);
				for (int j = 0; j < n; j++)
				{
					bk_lists[j] = lists[j];
				}
				lists = bk_lists;
				lists[n] = i;
			}
			else
			{
				lists = gcnew array<int>(1);
				lists[0] = i;
			}
		}
	}

	if (lists == nullptr)
	{
		textBox3->Text = "Must select 1 or more data.";
		return;
	}
	
	SaveFileDialog^ sfd = gcnew SaveFileDialog;

	sfd->Filter = "CSV files (*.csv)|*.csv";
	sfd->FilterIndex = 1;
	sfd->RestoreDirectory = true;
	sfd->InitialDirectory = path;
	sfd->FileName = savefile;

	if (sfd->ShowDialog() == System::Windows::Forms::DialogResult::OK)
	{
		//
		array<String^>^ strarr = sfd->FileName->Split('\\');

		path = "";
		for (int i = 0; i < strarr->Length - 1; i++)
		{
			path += strarr[i] + "\\";
		}
		savefile = strarr[strarr->Length - 1];

		//
		uvdatas->exportCSV(lists, path + savefile);

		textBox2->Text = path + savefile;
	}
	else
	{
	}
}

System::Void melcor_plot::MainForm::button3_Click(System::Object ^ sender, System::EventArgs ^ e)
{
	for (int i = 0; i < checkedListBox1->Items->Count; i++)
	{
		checkedListBox1->SetItemChecked(i, true);
	}
}

System::Void melcor_plot::MainForm::button4_Click(System::Object ^ sender, System::EventArgs ^ e)
{
	for (int i = 0; i < checkedListBox1->Items->Count; i++)
	{
		checkedListBox1->SetItemChecked(i, false);
	}
}

System::Void melcor_plot::MainForm::open_file(System::String^ path)
{
	if (File::Exists(path))
	{
		textBox1->Text = path;

		StreamReader^ din = File::OpenText(path);

		//최대 100메가 한도
		if (din->BaseStream->Length > 104857600)
		{
			textBox3->Text = "File over 100 mega characters.";
			return;
		}

		String^ str = din->ReadToEnd();

		if (uvdatas->parsing(str))
		{
			if (din->BaseStream->Length < 1048576)
			{
				textBox3->Text = str;

				this->Text = uvdatas->title;

				for (int i = 0; i < uvdatas->data->Length; i++)
				{
					checkedListBox1->Items->Add(uvdatas->data[i]->vname, CheckState::Checked);
				}
				
			}
			else
			{
				textBox3->Text += "Too much stream length [" + din->BaseStream->Length.ToString() + "]\r\n";
			}
		}
		else
		{
			textBox3->Text += "It is not MELCOR OUT file.\r\n";
		}


	}
}
