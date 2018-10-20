/************************************************************************
Beta1.0
   �޸�ʱ�䣺2017.7.17
   �޸����ݣ�1������˶Ե��������ļ��Ķ�ȡ���ܡ�
   
Beta1.1
   �޸�ʱ�䣺11:30 2017.7.18
   �޸����ݣ�1����stringstream�滻��isstream���������������޸ģ�������stringstream�Ĺ��������
             2�������˶���new_record.wn����䡣

Beta1.11
   �޸�ʱ�䣺2017.7.18
   �޸����ݣ�1���޸��˱�������
			2��������const��


*************************************************************************************/

#include "NavFile.h"

NavFile::NavFile()
{
	
}

NavFile::NavFile(const string & name)
{
	ReadFile(name);
}


NavFile::~NavFile()
{
}

/**************�����ȡ���������ļ�����*********/
/*
   ��������ReadFile()
   ���룺���������ļ���
   �������ȡ�ļ��Ƿ�ɹ�
*/

bool NavFile::ReadFile(const string & name)
{
    _filename = name;
    ifstream File;
    ofstream log;
    log.open("Point_Positioning.log", ios::app);
    File.open(_filename);
    log << "�򿪵��������ļ�..." << _filename<<std::endl;
    int line_count=0; //line_count��¼����
    if (!File.good())
    {
        log << "��ʧ�ܣ�" << endl;
        return false;
    }

    /*********��ȡ��������ͷ�ļ�***************************/
    string buffer;  //���ڶ�ȡһ���е�����
    while (File.good())
    {
        line_count++;
        std::getline(File, buffer);

        if (buffer.find("RINEX VERSION / TYPE", 60) != string::npos)
        {
            _header.RINEX_version = std::atof(buffer.c_str());
            _header.RINEX_type = 'N';
        }
        else if (buffer.find("ION ALPHA ", 60) != string::npos)
        {
            _header.ion_alpha[0] = atof(num_std(buffer.substr(2,12)).c_str());
            _header.ion_alpha[1] = atof(num_std(buffer.substr(13,12)).c_str());
            _header.ion_alpha[2] = atof(num_std(buffer.substr(24,12)).c_str());
            _header.ion_alpha[3] = atof(num_std(buffer.substr(35,12)).c_str());
        }
        else if (buffer.find("ION BETA", 60) != string::npos)
        {
            _header.ion_beta[0] = atof(num_std(buffer.substr(2,12)).c_str());
            _header.ion_beta[1] = atof(num_std(buffer.substr(14,12)).c_str());
            _header.ion_beta[2] = atof(num_std(buffer.substr(26,12)).c_str());
            _header.ion_beta[3] = atof(num_std(buffer.substr(38,12)).c_str());

        }
        else if (buffer.find("DELTA-UTC: A0,A1,T,W", 60) != string::npos)
        {
            _header.utc_a0 = atof(num_std(buffer.substr(3,19)).c_str());
            _header.utc_a1 = atof(num_std(buffer.substr(22,19)).c_str());
            _header.utc_t = atoi(num_std(buffer.substr(41,9)).c_str());
            _header.utc_w = atoi(num_std(buffer.substr(50,9)).c_str());
        }
        else if (buffer.find("LEAP SECONDS" ,60) != string::npos)
        {
            _header.leap_sec = atoi(buffer.substr(0,6).c_str());
        }
        else if (buffer.find("END OF HEADER", 60) != string::npos)
        {
            _header.nheader_len = line_count;
            log << "ͷ�ļ���ȡ���!" << endl;
            break;
        }

    }

    line_count = 0;
    int count=0;
    int Rec_count = 0;
    NavRecord new_record;

    /******************��ȡ�����������ݼ�¼*****************************/
    while (File.good())
    {
        std::getline(File, buffer);
        while (buffer.length() < 80)
        {
            buffer = buffer + " ";
        }


        if (buffer.length()<3) continue;  //�ж�buffer�Ƿ�Ϊ��

        count = count % 8; //count��¼һ����¼�е�����
        count++;
        line_count++;     //line_count��¼����������

        if (count == 1)
        {
            string temp;

            new_record = NavRecord();   //new_record��¼�������ݼ�¼

            new_record.PRN[0] = 'G';       //��ȡ���ǵ�PRN��
            if (buffer[0] == ' ') buffer[0] = '0';
            new_record.PRN[1] = buffer[0];
            new_record.PRN[2] = buffer[1];

            CalendarTime ctime = {};
            ctime.year = atoi(buffer.substr(3,2).c_str());
            ctime.month = atoi(buffer.substr(6,2).c_str());
            ctime.day = atoi(buffer.substr(9,2).c_str());
            ctime.hour = atoi(buffer.substr(12,2).c_str());
            ctime.minute = atoi(buffer.substr(15,2).c_str());
            ctime.second = atof(num_std(buffer.substr(17,5)).c_str());

            ctime.year += 2000;

            new_record.CalendarTime_0 = ctime;   //

            new_record.TOC = TimeTrans::CalendarTime2GPSTime(ctime);

            new_record.SClockBias = atof(num_std(buffer.substr(22,19)).c_str());
            new_record.SClockDri = atof(num_std(buffer.substr(41,19)).c_str());
            new_record.SClockDriV = atof(num_std(buffer.substr(60,19)).c_str());

        }
        if (count == 2)
        {
            new_record.IODE = atof(num_std(buffer.substr(3,19)).c_str());
            new_record.Crs = atof(num_std(buffer.substr(22,19)).c_str());
            new_record.deltan = atof(num_std(buffer.substr(41,19)).c_str());
            new_record.M0 = atof(num_std(buffer.substr(60,19)).c_str());
        }
        if (count == 3)
        {
            new_record.Cuc = atof(num_std(buffer.substr(3,19)).c_str());
            new_record.e = atof(num_std(buffer.substr(22,19)).c_str());
            new_record.Cus = atof(num_std(buffer.substr(41,19)).c_str());
            new_record.sqrtA = atof(num_std(buffer.substr(60,19)).c_str());
        }
        if (count == 4)
        {
            double secs;
            secs = atof(num_std(buffer.substr(3,19)).c_str());
            new_record.TOE = { 0,{ (int)secs,secs - (int)secs} };
            new_record.Cic = atof(num_std(buffer.substr(22,19)).c_str());
            new_record.omega = atof(num_std(buffer.substr(41,19)).c_str());
            new_record.Cis = atof(num_std(buffer.substr(60,19)).c_str());
        }
        if (count == 5)
        {
            new_record.i0 = atof(num_std(buffer.substr(3,19)).c_str());
            new_record.Crc = atof(num_std(buffer.substr(22,19)).c_str());
            new_record.omega1 = atof(num_std(buffer.substr(41,19)).c_str());
            new_record.dOmega= atof(num_std(buffer.substr(60,19)).c_str());
        }
        if (count == 6)
        {
            new_record.di = atof(num_std(buffer.substr(3,19)).c_str());
            new_record.l2_code = atof(num_std(buffer.substr(22,19)).c_str());
            new_record.wn= atof(num_std(buffer.substr(41,19)).c_str());
            new_record.l2_P_tag = atof(num_std(buffer.substr(60,19)).c_str());
        }
        if (count == 7)
        {
            new_record.SatAccur = atof(num_std(buffer.substr(3,19)).c_str());
            for (int i=0;i<15;i++) if (ura_eph[i]>=new_record.SatAccur)
            {
                new_record.SatAccur = i;
                break;
            }
            new_record.SatState = atof(num_std(buffer.substr(22,19)).c_str());
            new_record.TGD = atof(num_std(buffer.substr(41,19)).c_str());
            new_record.IODC = atof(num_std(buffer.substr(60,19)).c_str());
        }
        if (count == 8)
        {
            new_record.SendTime = atof(num_std(buffer.substr(3,19)).c_str());
            new_record.FitRange = atof(num_std(buffer.substr(22,19)).c_str());
            new_record.spare1 = atof(num_std(buffer.substr(41,19)).c_str());
            new_record.spare2 = atof(num_std(buffer.substr(60,19)).c_str());
            Rec_count++;
            new_record.TOC = TimeChange(new_record.TOC);
            _data[new_record.PRN].insert(make_pair(new_record.TOC,new_record));  //��������¼�洢��data��
            //_data[new_record.PRN][new_record.TOC] = new_record;
        }
    }
    _header.nrec_len = line_count;

    log << "���ݼ�¼��ȡ���!   ����ȡ" <<Rec_count<<"����¼��"<< endl;
    log << endl;
    log.close();
    File.close();
      return true;
}

/**************�����ȡ�����������ݼ�¼�ĺ���*********/
/*
��������Data()
���룺����Ҫ�ĵ����������ݼ�¼�����
����������������ݼ�¼
*/
//const NavRecord& NavFile::Data(int num)
//{
//	if (num >= 0 && num < _data.size())
//	{
//		return NavRecord();
//	}
//
//	else return _data[num];
//}

/**************�����ȡ�����������ݼ�¼���ĺ���*********/
/*
��������getDataNum()
���룺
����������������ݼ�¼��
*/
int NavFile::getDataNum() const
{
	return _data.size();
}


/************�����ȡ���������ļ�������**************/
/*
��������getName()
���룺
��������������ļ����ļ���
*/
string NavFile::FileName() const
{
	return _filename;
}

/***************�����ȡ�������ļ�¼�ĺ���***************************/
/*
   ��������GetRecord()
   ���룺t    ���Ƿ����źŵ�GPSʱ
        prn  ���ǵ�PRN��
   ��������ǵ�������¼
*/
NavRecord NavFile::GetRecord(GPSTime t, string prn) const
{

	GPSTime temp;
	temp = NavFile::TimeChange(t);
    if (_data.find(prn) == _data.end())
        return NavRecord();
	if (_data.at(prn).find(temp) != _data.at(prn).end())
	{
		return _data.at(prn).at(temp);
	}
	else
	{
		return NavRecord();
	}
	
	//return NavRecord();
}

/******************ʱ��ת������*******************************/
/*
   ��������TimeChange()
   ���룺t  ���Ƿ����źŵ�GPSʱ
   �������Ӧ�����������ο�ʱ��
*/
GPSTime NavFile::TimeChange(GPSTime t)
{
	int n,m;
	GPSTime ans;
	n = t.tow.sn % 7200;
	m = t.tow.sn / 7200;
	if (n >= 3600) m++;
	if (m >= 84)
	{
		ans.wn = t.wn + 1;
		ans.tow.sn = 0;
		ans.tow.tos = 0;
	}
	else
	{
		ans.wn = t.wn;
		ans.tow.sn = m * 7200;
		ans.tow.tos = 0;
	}
	return ans;
}
