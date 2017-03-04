#include"uvdata.h"

Void data_arr::extend_value(double u, double v)
{
	if (value != nullptr)
	{
		int length = value->Length;
		array<dbl_arr^>^ new_value = gcnew array<dbl_arr^>(length + 1);
		for (int i = 0; i < length; i++)
		{
			new_value[i] = value[i];
		}
		value = new_value;
		value[length] = gcnew dbl_arr;
		value[length]->u = u;
		value[length]->v = v;
	}
	else
	{
		value = gcnew array<dbl_arr^>(1);
		value[0] = gcnew dbl_arr;
		value[0]->u = u;
		value[0]->v = v;
	}
}


UVData::UVData()
{
}

UVData::~UVData()
{
}

bool UVData::parsing(String ^ str)
{
	using namespace Text::RegularExpressions;

	array<String^>^ seperator = gcnew array<String^>(1);
	seperator[0] = gcnew String("\r\n");

	array<String^>^ strs = str->Split(seperator, StringSplitOptions::None);

	/* HISPLT version 3.40 <9/25/94> MELCOR configuration                    */
	if (Regex::IsMatch(strs[0], "^[a-zA-Z ]+[a-zA-Z0-9.<>/ ]+MELCOR[a-zA-Z ]+[\r]?$"))
	{
		int plt = 0;
		int plt_no;

		for (size_t i = 1; i < strs->Length; i++)
		{
			//plot 검색
			if (Regex::IsMatch(strs[i], "^ PLOT[- ]+"))
			{
				plt = 1; //PLOT
				extend_data();
				plt_no = data->Length - 1;
				i++;
			}
			else if (Regex::IsMatch(strs[i], "^ CPLOT[- ]+"))
			{
				plt = 2; //CPLOT
				extend_data();
				plt_no = data->Length - 1;
				i++;
			}

			while (plt != 0)
			{
				if (Regex::IsMatch(strs[i], "^ U =[^*]+V =[^*]+"))
				{
					//name					
					int iu = strs[i]->IndexOf("U =");
					int iv = strs[i]->IndexOf("V =");

					String^ uname = gcnew String(strs[i]->ToCharArray(), iu + 3, iv - iu - 3);
					String^ vname = gcnew String(strs[i]->ToCharArray(), iv + 3, strs[i]->Length - iv - 3);

					data[plt_no]->uname = uname->Trim(' ');
					data[plt_no]->vname = vname->Trim(' ');
				}
				else if (plt == 1 && Regex::IsMatch(strs[i], "^ ULABEL"))
				{
					//plot 전용
					String^ str = gcnew String(strs[i]->ToCharArray(), 7, strs[i]->Length - 7);
					array<String^>^ ar = str->Trim(' ')->Split(' ');					
					//label
					u_label = ar[0];
					for (int i = 1; i < ar->Length - 1; i++) u_label += " " + ar[i];
					//unit
					u_unit = ar[ar->Length - 1]->Trim('(', ')');
				}
				else if (plt == 1 && Regex::IsMatch(strs[i], "^ VLABEL"))
				{
					//plot 전용
					String^ str = gcnew String(strs[i]->ToCharArray(), 7, strs[i]->Length - 7);
					array<String^>^ ar = str->Trim(' ')->Split(' ');
					v_label = gcnew String("");
					//label
					v_label = ar[0];
					for (int i = 1; i < ar->Length - 1; i++) v_label += " " + ar[i];
					//unit
					v_unit = ar[ar->Length - 1]->Trim('(', ')');
				}
				else if (plt == 1 && Regex::IsMatch(strs[i], "^ TITLE"))
				{
					//plot 전용
					String^ str = gcnew String(strs[i]->ToCharArray(), 6, strs[i]->Length - 6);
					title = str->Trim(' ');
				}
				/*
				else if (Regex::IsMatch(strs[i], "^ NUMBER OF POINTS PLOTTED"))
				{
				}
				*/
				else if (Regex::IsMatch(strs[i], "^[ ]+[0-9+-.E]+[ ]+[0-9+-.E]+"))
				{
					//data
					array<String^>^ sarr = strs[i]->Trim()->Split(' ');
					data[plt_no]->extend_value(Convert::ToDouble(sarr[0]), Convert::ToDouble(sarr[sarr->Length - 1]));
				}
				else if (Regex::IsMatch(strs[i], "^ PLOTTED FRAME[0-9 ]+"))
				{
					plt = 0;
				}
				i++;
			}
		}
		return true; //melcor 맞음
	}
	else
	{
		return false; //melcor 아님
	}
}

void UVData::exportCSV(array<int>^ list, String^ path)
{
	String^ out_buff = gcnew String("");

	int no = list->Length;

	//name
	out_buff += data[0]->uname;
	for (int i = 0; i < no; i++)
	{
		out_buff += "," + data[list[i]]->vname;
	}
	//label
	out_buff += Environment::NewLine + u_label;
	for (int i = 0; i < no; i++)
	{
		out_buff += "," + v_label;
	}
	//unit
	out_buff += Environment::NewLine + u_unit;
	for (int i = 0; i < no; i++)
	{
		out_buff += "," + v_unit;
	}
	//value
	for (int j = 0; j < data[0]->value->Length; j++)
	{
		out_buff += Environment::NewLine + Convert::ToDouble(data[0]->value[j]->u);
		for (int i = 0; i < no; i++)
		{
			out_buff += "," + Convert::ToDouble(data[list[i]]->value[j]->v);
		}
	}
	out_buff += Environment::NewLine;

	//save
	System::IO::File::WriteAllText(path, out_buff);
}

Void UVData::extend_data()
{
	if (data != nullptr)
	{
		int length = data->Length;
		array<data_arr^>^ new_data = gcnew array<data_arr^>(length + 1);
		for (int i = 0; i < length; i++)
		{
			new_data[i] = data[i];
		}
		data = new_data;
		data[length] = gcnew data_arr;
	}
	else
	{
		data = gcnew array<data_arr^>(1);
		data[0] = gcnew data_arr;
	}
}
