#pragma once

using namespace System;

//struct
ref class dbl_arr
{
public:
	double u;
	double v;
};

ref class data_arr
{
public:
	String^ uname;
	String^ vname;

	array<dbl_arr^>^ value;

	Void extend_value(double u, double v);
};

//class
ref class UVData
{
public:
	UVData();
	~UVData();

	bool parsing(String^ str);

	void exportCSV(array<int>^ list, String^ path);

	//pointer
	String^ title;
	String^ u_label;
	String^ v_label;
	String^ u_unit;
	String^ v_unit;
	array<data_arr^>^ data;

private:
	Void extend_data();
};