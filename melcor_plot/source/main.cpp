#include"MainForm.h"

using namespace melcor_plot;

//���Ͻ����� ����
[STAThread]

int main(array<System::String ^> ^args)
{
	Console::WriteLine("Coded by dmkim@fnctech.com");
	Console::WriteLine("Don't close this console window.");

	Form^ mform = gcnew MainForm();

	Application::EnableVisualStyles();
	Application::Run(mform);

	return 0;
}