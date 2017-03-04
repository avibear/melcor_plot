#include"MainForm.h"

using namespace melcor_plot;

//단일스레드 선언
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