/************************************************************************
Beta1.0
   �޸�ʱ�䣺2017.7.17
   �޸����ݣ�1�������NavFile��Ķ��塣
   
Beta1.1
   �޸�ʱ�䣺2017.7.18
   �޸����ݣ�1���޸��˶�Ӧ��cpp�ļ���

Beta1.11
   �޸�ʱ�䣺2017.7.18
   �޸����ݣ�1���޸��˱�������
            2��������const��

Beta1.2
   �޸�ʱ�䣺
   �޸����ݣ�1����¼��Ϊ��map�ķ�ʽ�洢 ��������Ĳ���


*************************************************************************************/
#ifndef NAVFILE_H
#define NAVFILE_H
#include <string>
#include <vector>
#include <map>
#include <math.h>
#include <fstream>
#include "TimeTrans.h"
#include <iostream>

using namespace std;

class NavFile
{
public:
	NavFile();
	NavFile(const string &name); //name��ʾ���������ļ���
	~NavFile();

private:
	string _filename; //���������ļ���
	map<string ,map<GPSTime,NavRecord>> _data;  //�����������ݼ�¼
	GPSNavHdr _header;  //��������ͷ�ļ�

public:
	bool ReadFile(const string &name); //��ȡ���������ļ�
	//const NavRecord & Data(int num);  //ȡ��һ����¼
	int getDataNum() const;  //�õ���¼����
	string FileName() const;  //��ȡ���������ļ���
	NavRecord GetRecord(GPSTime t, string prn) const;  //������Ӧ�ķ���ʱ���Լ�PRN�Ż�ȡ��Ӧ�ĵ������ĵļ�¼
	static GPSTime TimeChange(GPSTime t);    //ʱ��ת������
    static string num_std(string s)
    {
        std::size_t found = s.find('D');
        if (found == string::npos)
            return s;
        s.replace(found, 1, "E");
        return s;
    }

	const GPSNavHdr &Header() const { return _header; }
};


#endif // !NAVFILE_H
