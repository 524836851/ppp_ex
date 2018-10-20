/*****************************************************************************
Beta1.0
   �޸�ʱ�䣺2017.7.18
   �޸����ݣ�ʵ���˶�ȡGPS�۲������ļ�����

Beta1.11
   �޸�ʱ�䣺2017.7.18
   �޸����ݣ�1���޸��˱�������
            2��������һЩ������


*******************************************************************************/
#include "ObsFile.h"
using namespace std;



ObsFile::ObsFile()
{

}

ObsFile::ObsFile(const string &name)
{
	ReadFile(name);
}


ObsFile:: ~ObsFile()
{

}

/**************�����ȡ�۲������ļ�����*********/
/*
��������ReadFile()
���룺�۲������ļ���
�������ȡ�ļ��Ƿ�ɹ�
*/

bool ObsFile::ReadFile(const string &name)
{
	_filename = name;
	ifstream File;
	ofstream log;
	log.open("Point_Positioning.log", ios::app);
	File.open(_filename);
//	log << "�򿪹۲������ļ�..." << _filename << std::endl;
	int line_count = 0;//line_count ��¼����
	if (!File.good())
	{
//		log << "��ʧ�ܣ�" << endl;
		return false;
	}



	/*********��ȡ�۲�����ͷ�ļ�***************************/
	string buffer;//��ȡһ���е�����
//	std::stringstream ss;


	while (File.good())
	{
		line_count++;
		getline(File, buffer);
		
//		ss.clear();
//		ss.str("");   //���stringstream
//		ss << buffer;   //��buffer����ss
		
		if (buffer.find("RINEX VERSION / TYPE",60)!=string::npos)
		{
			_header.RINEX_version = std::atof(buffer.c_str());
			_header.RINEX_type = 'O';
		}
		else if (buffer.find("MARKER NAME", 60) != string::npos)
		{
//			ss >> _header.mark_name;

		}
		else if (buffer.find("MARKER NUMBER", 60) != string::npos)
		{
//			ss >> _header.mark_number;
		}
		else if (buffer.find("REC # / TYPE / VERS", 60) != string::npos)
		{
		}
		else if (buffer.find("ANT # / TYPE", 60) != string::npos)
		{
//			ss >> _header.antennaNumber;
//			ss >> _header.antennaType;
		}
		else if (buffer.find("APPROX POSITION XYZ", 60) != string::npos)
		{
            _header.approxPos.X = std::atof(buffer.substr(1,13).c_str());
            _header.approxPos.Y = std::atof(buffer.substr(15,13).c_str());
            _header.approxPos.Z = std::atof(buffer.substr(29,13).c_str());
            
//			ss >> _header.approxPos.X;
//			ss >> _header.approxPos.Y;
//			ss >> _header.approxPos.Z;

		}
		else if (buffer.find("ANTENNA: DELTA H/E/N", 60) != string::npos)
		{
            _header.antennaDelta.U = std::atof(buffer.substr(1,13).c_str());
            _header.antennaDelta.E = std::atof(buffer.substr(15,13).c_str());
            _header.antennaDelta.N = std::atof(buffer.substr(29,13).c_str());
//			ss >> _header.antennaDelta.U;
//			ss >> _header.antennaDelta.E;
//			ss >> _header.antennaDelta.N;

		}
		else if (buffer.find("WAVELENGTH FACT L1/2", 60) != string::npos)
		{
			//�ֽ׶�δ���� �����ز���������
		}
		else if (buffer.find("# / TYPES OF OBSERV", 60) != string::npos)
		{
//			ss >> _header.obsTypeNumber;
            _header.obsTypeNumber = std::atoi(buffer.substr(4,2).c_str());
			string tempA="";
			if (_header.obsTypeNumber <= 9)
			{
				for (int i = 0; i < _header.obsTypeNumber; i++)
				{
//					ss >> tempA;
                    strcpy(_header.obsType[i],buffer.substr(10+i*6,2).c_str());
				}
			}
			else
			{
				for (int i = 0; i <9; i++)
				{
//					ss >> tempA;
                    strcpy(_header.obsType[i],buffer.substr(10+i*6,2).c_str());
					//_header.obsType[i][0] = tempA[0];
					//_header.obsType[i][1] = tempA[1];
				}

				//��ȡ����
				getline(File, buffer);
//				ss.clear();
//				ss.str(buffer);

				for (int i = 0; i <_header.obsTypeNumber-9; i++)
				{
//					ss >> tempA;
                    strcpy(_header.obsType[i+9],buffer.substr(10+i*6,2).c_str());
					//_header.obsType[i+9][0] = tempA[0];
					//_header.obsType[i+9][1] = tempA[1];
				}
			}
		}
		else if (buffer.find("INTERVAL", 60) != string::npos)
		{
            _header.interval = std::atof(buffer.substr(1,9).c_str());
//			ss >> _header.interval;
		}
		else if (buffer.find("LEAP SECONDS", 60) != string::npos)
		{
//			ss >> _header.leap_sec;
		}
		else if (buffer.find("TIME OF FIRST OBS", 60) != string::npos)
		{
			CalendarTime ctime;
            ctime.year = atoi(buffer.substr(2,4).c_str());
            ctime.month = atoi(buffer.substr(10,2).c_str());
            ctime.day = atoi(buffer.substr(16,2).c_str());
            ctime.hour = atoi(buffer.substr(22,2).c_str());
            ctime.minute = atoi(buffer.substr(28,2).c_str());
            ctime.second = atof(buffer.substr(33,10).c_str());
//			ss >> ctime.year;
//			ss >> ctime.month;
//			ss >> ctime.day;
//			ss >> ctime.hour;
//			ss >> ctime.minute;
//			ss >> ctime.second;
            _header.startTime = TimeTrans::CalendarTime2GPSTime(ctime);
		}
		else if (buffer.find("TIME OF LAST OBS", 60) != string::npos)
		{
			CalendarTime ctime;
            ctime.year = atoi(buffer.substr(2,4).c_str());
            ctime.month = atoi(buffer.substr(10,2).c_str());
            ctime.day = atoi(buffer.substr(16,2).c_str());
            ctime.hour = atoi(buffer.substr(22,2).c_str());
            ctime.minute = atoi(buffer.substr(28,2).c_str());
            ctime.second = atof(buffer.substr(33,10).c_str());
//			ss >> ctime.year;
//			ss >> ctime.month;
//			ss >> ctime.day;
//			ss >> ctime.hour;
//			ss >> ctime.minute;
//			ss >> ctime.second;
            _header.endTime = TimeTrans::CalendarTime2GPSTime(ctime);
		}
		
		else if (buffer.find("END OF HEADER", 60) != string::npos)
		{
			_header.headLineNumber = line_count;
//			log << "ͷ�ļ���ȡ���!" << endl;
			break;
		}
	}


    
	line_count = 0;
	ObsRecord new_record;

    int test_count =0;
	/*********��ȡ�۲����ݼ�¼***************************/
	while (File.good())
	{
        test_count++;
        if (test_count >=387)
        {
            test_count =1;
        }
		string buffer;
		new_record = ObsRecord();

		/***************��ȡ�۲��ļ����ݼ�¼�ڵ���Ԫ���¼���־******************/
		getline(File, buffer);
        if (buffer =="")
            break;
        //ss.str("");   //���stringstream
		//ss.clear();
		//ss << buffer;   //��buffer����ss
		
		//��ȡ�۲���Ԫʱ��
        new_record.obstime_c.year = atoi(buffer.substr(1,2).c_str());
        new_record.obstime_c.month = atoi(buffer.substr(4,2).c_str());
        new_record.obstime_c.day = atoi(buffer.substr(7,2).c_str());
        new_record.obstime_c.hour = atoi(buffer.substr(10,2).c_str());
        new_record.obstime_c.minute = atoi(buffer.substr(13,2).c_str());
        new_record.obstime_c.second = atof(buffer.substr(16,10).c_str());
        
        
		if (new_record.obstime_c.year > 80)
		{
			new_record.obstime_c.year = 1900 + new_record.obstime_c.year;
		}
		else
		{
			new_record.obstime_c.year = 2000 + new_record.obstime_c.year;
		}
/*		ss >> new_record.obstime_c.month;
		ss >> new_record.obstime_c.day;
		ss >> new_record.obstime_c.hour;
		ss >> new_record.obstime_c.minute;	
		ss >> new_record.obstime_c.second; */
        new_record.obstime = TimeTrans::CalendarTime2GPSTime(new_record.obstime_c);
        
        new_record.epoch_mark = atoi(buffer.substr(27,2).c_str());
        new_record.sat_num = atoi(buffer.substr(30,2).c_str());
        
/*		ss >> new_record.epoch_mark;//��ȡ��Ԫ��־
		ss >> new_record.sat_num;//��ȡ���Ǹ���   */
        
		//��ȡ���ǵ�PRN�б�
		string PRN;
        int satnum_count1,satnum_count2;
        satnum_count1 = new_record.sat_num / 12;
        satnum_count2 = new_record.sat_num % 12;
        for (int i = 0;i<satnum_count1;i++)
        {
            if (i!=0)   getline(File,buffer);
            for (int j = 0;j<12;j++)
                strcpy(new_record.obsdata[i*12+j].PRN, buffer.substr(32+3*j,3).c_str());
        }
        if (satnum_count2!=0)
        {
            getline(File,buffer);
            for (int k = 0;k<satnum_count2;k++)
                strcpy(new_record.obsdata[satnum_count1*12+k].PRN, buffer.substr(32+3*k,3).c_str());
        }
        
        
//        if (new_record.sat_num > 12)//���������>12��Ҫ��ȡ����
//        {
//            //ss >> PRN;
//            for (int i = 0; i < 12; i++)
//            {
//                strcpy(new_record.obsdata[i].PRN, buffer.substr(32+3*i,3).c_str());
//                //new_record.obsdata[i].PRN[0] = PRN[3 * i];
//                //new_record.obsdata[i].PRN[1] = PRN[3 * i + 1];
//                //new_record.obsdata[i].PRN[2] = PRN[3 * i + 2];
//            }
//            //��ȡ����PRN�б�����
//            getline(File, buffer);
//
///*            ss.clear();
//            ss.str(buffer);
//            ss >> PRN;  */
//
//            for (int i = 0; i < new_record.sat_num-12; i++)
//            {
//                strcpy(new_record.obsdata[i+12].PRN, buffer.substr(32+3*i,3).c_str());
//                //new_record.obsdata[i+12].PRN[0] = PRN[3 * i];
//                //new_record.obsdata[i+12].PRN[1] = PRN[3 * i + 1];
//                //new_record.obsdata[i+12].PRN[2] = PRN[3 * i + 2];
//            }
//        }
//        else
//        {
//            //ss >> PRN;
//            for (int i = 0; i < new_record.sat_num; i++)
//            {
//                strcpy(new_record.obsdata[i].PRN, buffer.substr(32+3*i,3).c_str());
//                //new_record.obsdata[i].PRN[0] = PRN[3 * i];
//                //new_record.obsdata[i].PRN[1] = PRN[3 * i+1];
//                //new_record.obsdata[i].PRN[2] = PRN[3 * i+2];
//            }
//        }

		/****************��ȡ�۲������ļ����ݼ�¼��****************/
	
		int temp1 = _header.obsTypeNumber / 5; //��¼���ݼ�¼�ڵ�������
		int temp2 = _header.obsTypeNumber % 5; //��¼���ݼ�¼�ڵ�
		for (int i = 0; i < new_record.sat_num;i++)
		{
			for (int j = 0; j < temp1; j++)
			{
				getline(File, buffer);
				while (buffer.length() < 80)
				{
					buffer = buffer + " ";
				}
				for (int k = 0; k < 5; k++)
				{
				  new_record.obsdata[i].obs[j * 5 + k] = atof(buffer.substr(k * 16, 14).c_str());
				  new_record.obsdata[i].LLI[j * 5 + k] = atoi(buffer.substr(k * 16 + 14, 1).c_str());
				  new_record.obsdata[i].signal_str[j * 5 + k] = atoi(buffer.substr(k * 16 + 15, 1).c_str());
				}
			}
			if (temp2 != 0)
			{
				getline(File, buffer);
				while ((int)(buffer.length())< (temp2 * 16))
				{
					buffer = buffer + " ";
				}
				for (int k = 0; k < temp2; k++)
				{
					new_record.obsdata[i].obs[temp1 * 5 + k] = atof(buffer.substr(k * 16, 14).c_str());
					new_record.obsdata[i].LLI[temp1 * 5 + k] = atoi(buffer.substr(k * 16 + 14, 1).c_str());
					new_record.obsdata[i].signal_str[temp1 * 5 + k] = atoi(buffer.substr(k * 16 + 15, 1).c_str());
				}
			}
			
		}
		_data.push_back(new_record);       
	}

//	log << "���ݼ�¼��ȡ���!   ����ȡ" << _data.size() << "����¼��" << endl;
	log << endl;
	log.close();
	File.close();
   // new_record.~ObsRecord();
    
    convert();
	return true;
}

/**************�����ȡ�۲������ļ����ĺ���*********/
/*
��������getName()
���룺
������۲������ļ���
*/
string ObsFile::getName() const
{
	return _filename;
}

/**************�����ȡ�۲����ݼ�¼�ĺ���*********/
/*
��������Data()
���룺����Ҫ�Ĺ۲����ݼ�¼�����
������۲����ݼ�¼
*/
ObsRecord ObsFile::Data(int num) const
{
	if (num < 0 && num >= _data.size())
	{
		return ObsRecord();
	}
	return _data[num];
}

/**************�����ȡ�۲����ݼ�¼���ĺ���*********/
/*
��������getDataNum()
���룺
������۲����ݼ�¼��
*/
int ObsFile::getDataNum() const
{
	return _data.size();
}

const GPSObsHdr & ObsFile::Header() const
{
	// TODO: insert return statement here
	return _header;
}

int ObsFile::getObstype(string type)
{
    for (int i = 0; i < _header.obsTypeNumber; i++)
    {
        if (_header.obsType[i][0] == type[0] && _header.obsType[i][1] == type[1])
        {
            return i;
        }
    }
    return -1;
}

void ObsFile::convert()
{
    int P1,P2,L1,L2,D1,D2;
    P1 = getObstype("P1");
    if (P1 == -1)
        P1 = getObstype("C1");
    P2 = getObstype("P2");
    L1 = getObstype("L1");
    L2 = getObstype("L2");
    D1 = getObstype("D1");
    D2 = getObstype("D2");
    
    for (int i= 0;i <_data.size();i++)
        for (int j =0;j <_data[i].sat_num;j++)
        {
            if (P1 != -1)
                _data[i].obsdata[j].P[0] = _data[i].obsdata[j].obs[P1];
            if (P2 != -1)
                _data[i].obsdata[j].P[1] = _data[i].obsdata[j].obs[P2];
            if (L1 != -1)
                _data[i].obsdata[j].L[0] = _data[i].obsdata[j].obs[L1];
            if (L2 != -1)
                _data[i].obsdata[j].L[1] = _data[i].obsdata[j].obs[L2];
            if (D1 != -1)
                _data[i].obsdata[j].D[0] = _data[i].obsdata[j].obs[D1];
            if (D2 != -1)
                _data[i].obsdata[j].D[1] = _data[i].obsdata[j].obs[D2];
        }
}
