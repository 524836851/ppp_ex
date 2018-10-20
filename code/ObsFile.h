/*****************************************************************************
Beta1.0
    �޸�ʱ�䣺2017.7.18
    �޸����ݣ�1�������˶�ȡGPS�۲������ļ�����
	         2����ObsFile.CPP������˶�ȡGPS�۲������ļ�����

Beta1.11
	�޸�ʱ�䣺2017.7.18
	�޸����ݣ�1���޸��˱�������
			 2��������һЩ������


*******************************************************************************/
#ifndef OBSFILE_H
#define OBSFILE_H
#include <string>
#include <vector>
#include "P_Struct.h"
#include<math.h>
#include<fstream>
#include"TimeTrans.h"

using namespace std;
class ObsFile
{
public:
	ObsFile();
	ObsFile(const string &name);//name��ʾ�۲������ļ���
	~ObsFile();

private:
	string _filename;//�۲������ļ���
	vector<ObsRecord> _data;//�۲����ݼ�¼
	GPSObsHdr _header;//�۲�����ͷ�ļ�
    void convert(); //���۲������ļ����뵽P��L��D��

public:
	bool ReadFile(const string &name);//��ȡ�۲������ļ�
	string getName() const;//��ȡ�۲������ļ���
	ObsRecord Data(int num) const; //ȡ��һ����¼
	int getDataNum() const; //�õ���¼����
    int getObstype(string type); //�õ��۲�����λ��
	const  GPSObsHdr &Header() const;
    
};

#endif;//!OBSFILE_H
