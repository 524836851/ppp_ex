/************************************************************************
Beta1.0
   �޸�ʱ�䣺2017.7.17
   �޸����ݣ�1������˸��ֽṹ��Ķ��塣
   
Beta1.1
   �޸�ʱ�䣺2017.7.18
   �޸����ݣ�1��������NavRecord�е�wn�����������ڱ�ʾ������




*************************************************************************************/

#ifndef P_STRUCT_H
#define P_STRUCT_H

#include <string>
#include <iostream>
#include <iomanip>
using namespace std;


/******************************************���峣��*************************************************************/
const double a = 6378137;
const double e2 = 0.00669437999013;
const double pi = 3.14159265358979324;

const int maxtypeofobs = 30;
const int maxsatnume = 64;

const double c = 2.99792458E8; //m/s
const double GM = 3.986005E14; //m^3/s^2
const double omega_e = 0.000072921151467;//rad/s

const double ura_eph[]={         /* ura values (ref [3] 20.3.3.3.1.1) */
    2.4,3.4,4.85,6.85,9.65,13.65,24.0,48.0,96.0,192.0,384.0,768.0,1536.0,
    3072.0,6144.0,0.0
};

/***************************************����ϵͳ����*************************/
#define MAXGPS 50    //GPS����ϵͳ�������



#define IB(s,opt)    ((s) + 3 + 1  + (opt.ion_corr_mode>=3?0:0) -1) //ȷ������������

#define SQR(x)      ((x)*(x))

#define EFACT_GPS   1.0                 /* error factor: GPS */

#define ERR_SAAS    0.3             /* saastamoinen model error std (m) */
#define ERR_BRDCI   0.5             /* broadcast iono model error factor */
#define ERR_CBIAS   0.3             /* code bias error std (m) */


#define VAR_POS     SQR( 30.0) /* initial variance of receiver position (m^2) */
#define VAR_CLK     SQR(100.0) /* initial variance of receiver clock (m^2) */
#define VAR_ZTD     SQR(  0.3) /* initial variance of ztd (m^2) */
#define VAR_GRA     SQR(0.001) /* initial variance of gradient (m^2) */
#define VAR_BIAS    SQR( 30.0) /* initial variance of phase-bias (m^2) */

const double lamda0 = c/(1575.42E6);  //GPS��L1�ز�������m��
const double lamda1 = c/120.0/10.23E6;  //GPS��L2�ز�������m��

/*****************************************�������ݽṹ��*******************************************************/

//ʱ��ṹ��

/*****��ǰʱ��*****/
struct TimeofDay
{
	int sn; //��
	double tos; //���С������
};

/*****��ǰ��*******/
struct TimeofWeek
{
	int sn;
	double tos;
};

/****ͨ��ʱ******/
struct CalendarTime
{
	int year;
	int month;
	int day;
	int hour;
	int minute;
	double second;

	friend ostream& operator<<(ostream& out, const CalendarTime &time) 
	{
		out << setw(6) << setiosflags(ios::right) << time.year;
		out << setw(2) << setiosflags(ios::left) << time.month;
		out << setw(4) << setiosflags(ios::left) << time.day;
		out << setw(3) << setiosflags(ios::left) << time.hour;
		out << setw(3) << setiosflags(ios::left) << time.minute;
		out << setw(11) << setiosflags(ios::right) <<setiosflags(ios::fixed) << setprecision(7) << setiosflags(ios::left) << time.second;
		return out;
	}
};

/****������*******/
struct JulianDay
{
	int day;
	TimeofDay tod; //��ǰʱ��
};

/****�����*******/
struct DayofYear
{
	int  year;
	int  day;
};

/****GPSʱ********/
struct GPSTime
{
	int wn; //����
	TimeofWeek tow;//ʱ��

	friend bool operator < (const GPSTime &t1, const GPSTime &t2)//������GPSTime��"<"���㣬�����Ļ�GPSTime�ṹ�Ϳ�����Ϊmap�����ļ�ֵ
	{
		return (t1.wn + (t1.tow.sn+t1.tow.tos) / 604800) < (t2.wn + (t2.tow.sn+t2.tow.tos) / 604800);
	}
    
    double operator -(const GPSTime &t2) const
    {
        double tt;
        tt  = tow.sn+tow.tos -(t2.tow.sn + t2.tow.tos);
        tt += (wn-t2.wn) *604800;
        return tt;
    }

	GPSTime operator +(const double &t2) const//��û����
	{
		GPSTime new_t = { wn,tow };
		new_t.tow.tos += t2;
		if (new_t.tow.tos >= 1)
		{
			new_t.tow.sn += (int)new_t.tow.tos;
			new_t.tow.tos -= (int)new_t.tow.tos;
		}
		else if (new_t.tow.tos < 0)
		{
			new_t.tow.sn += (int)new_t.tow.tos-1;
			new_t.tow.tos -= (int)new_t.tow.tos - 1;
		}

		if (new_t.tow.sn >= 604800)
		{
			new_t.wn += (int)(new_t.tow.sn / 604800.0);
			new_t.tow.sn -= (int)(new_t.tow.sn / 604800.0) * 604800;
		}
		else if (new_t.tow.sn < 0)
		{
			new_t.wn += (int)(new_t.tow.sn / 604800.0)-1;
			new_t.tow.sn -= ((int)(new_t.tow.sn / 604800.0)-1) * 604800;
		}

		return new_t;
	}

};



//����ϵ�ṹ

/*****�ѿ�������ϵ******/
struct Cartesian
{
	double X;
	double Y;
	double Z;
    
    Cartesian operator+(Cartesian &value)
    {
        Cartesian new_value ;
        new_value.X = X + value.X;
        new_value.Y = Y + value.Y;
        new_value.Z = Z + value.Z;
        return new_value;
    }
    
    Cartesian operator-(Cartesian &value)
    {
        Cartesian new_value ;
        new_value.X = X - value.X;
        new_value.Y = Y - value.Y;
        new_value.Z = Z - value.Z;
        return new_value;
    }

};

/*******�������ϵ******/
struct Geodetic
{
	double B;//��ؾ��ȣ��ȣ�
	double L;//���γ�ȣ��ȣ�
	double H;//��ظߣ��ף�
};

/*******վ�ĵ�ƽ����ϵ�������꣩***/
struct Topocentric
{
	double N;//��������)
	double E;//�������ף�
	double U;//�춥����(��)
};

/*******վ�ĵ�ƽ����ϵ�������꣩***/
struct Topopolar
{
	double S;//��վ�ľ��루�ף�
	double E;//�������ǣ��ȣ�
	double A;//���ӷ���ǣ��ȣ�
};

//���ǹ۲����ݽṹ��
/*******�۲�����ͷ�ļ�*************/
struct GPSObsHdr
{
	double RINEX_version;//�汾��
	char RINEX_type;
	string mark_name;//���߱�־�����ƣ�������
	string mark_number;//���߱�־��ţ�
	string receiverNumber; //���ջ����к�
	string receiverType;   //���ջ�����
	string recerverVersion; //���ջ��汾��
	string antennaNumber;//�������к�
	string antennaType;//��������
	Cartesian approxPos; //��վ��־�Ľ���λ�ã�WGS-84��
	Topocentric antennaDelta; //������������ڲ�վ��־��λ��
	int obsTypeNumber;//�۲�ֵ������Ŀ
	char obsType[maxtypeofobs][2]; //����۲�����
	double interval; //�۲�ֵ����Ԫ������룩
	int leap_sec; //��1980��1��6��������������
	GPSTime startTime; //�����ļ���һ���۲��¼��ʱ��
	GPSTime endTime; //�����ļ����һ���۲��¼��ʱ��
	int headLineNumber; //ͷ�ļ����һ���к�
};
/******ÿ���۲������ļ��Ľṹ��*****/
struct GpsObservation
{
	char PRN[3];//������
    double P[3]; //α��۲�ֵ
    double L[3]; //�ز���λ�۲�ֵ
    double D[3]; //�����չ۲�ֵ
	double obs[maxtypeofobs];    //�۲�ֵ
	int LLI[maxtypeofobs];       //LLI(ʧ����ʶ����
	int signal_str[maxtypeofobs]; //�ź�ǿ��
};

/******�����۲��¼�Ľṹ��**********/
struct ObsRecord
{
	GPSTime obstime;      //�۲���Ԫʱ��GPSʱ
	CalendarTime obstime_c;  //�۲���Ԫʱ��ͨ��ʱ
	int epoch_mark;       // ��Ԫ��־
	int sat_num;          // ��ǰ��Ԫ���۲⵽��������
	GpsObservation obsdata[maxsatnume];
};



//�������Ĺ۲����ݽṹ��
/*******��������ͷ�ļ�***********/
struct GPSNavHdr
{
	double RINEX_version;    //RINEX�汾
	char RINEX_type;         //�ļ�����
	double ion_alpha[4], ion_beta[4]; //��������
	double utc_a0, utc_a1;   //���ڼ���UTCʱ����������
	int utc_t, utc_w;       //���ڼ���UTCʱ����������
	int leap_sec;           //������
	int nheader_len, nrec_len;    //�ļ�ͷ��������������
};

/******�������ļ�¼*************/
struct  NavRecord
{
	char PRN[3];   //���ǵ�PRN��
	//��һ��
	CalendarTime CalendarTime_0;   //����ʱ
	GPSTime TOC;       //�����ӵĲο�ʱ��
	double SClockBias;//�����ӵ�ƫ��(s)
	double SClockDri;//�����ӵ�Ư��(s/s)
	double SClockDriV;//�����ӵ�Ư���ٶ�(s/s^2)
	//�ڶ���
	double IODE;//���ݡ���������ʱ��
	double Crs;//(m)
	double deltan;//(rad/s)
	double M0;//(rad)
	//������
	double Cuc;//(rad)
	double e;//���ƫ����
	double Cus;//(rad)
	double sqrtA;//(m^0.5)
	//������
	GPSTime TOE;//�����Ĳο�ʱ�̣�GPS���ڵ�������
	double Cic;//(rad)
	double omega;//(rad)
	double Cis;//(rad)
	//������
	double i0;//(rad)
	double Crc;//(m)
	double omega1;//(rad)
	double dOmega;//(rad/s)
	//������
	double di;
	double l2_code;
	double wn;//GPS��������TOEһͬ��ʾʱ�䣩
	double l2_P_tag;
	//������
	double SatAccur;//���Ǿ���(m)
	double SatState;//���ǽ���״̬
	double TGD;//(sec)
	double IODC;//�ӵ���������
	//�ڰ���
	double SendTime;//���ķ���ʱ�̣���λΪGPS�ܵ��룬ͨ�������֣�HOW���е�Z�����ó���
	double FitRange;
	double spare1;
	double spare2;
};

/***********����λ�ü�������****************/
//struct SatInfo
//{
//	Cartesian SatPos;//����λ��
//	
//	double delta_t;//�Ӳ�
//};

//�������ӳٸ���
/*******����������*********/
struct meteodata
{
	double temp;//�¶ȣ�K��
	double pres;//ѹǿ��hpa��
	double RH;
	double height;//�߶ȣ�km��
};

//ƽ���
struct  Adjust_Scheme
{
    int mode;                 //positioning model : 0 SPP 1 Static_PPP 2 Dynamic_PPP
    
    double cut_off_angle;     // ��ֹ�߶Ƚ�
	int trop_corr_mode;       // ���������ѡ��
	int ion_corr_mode;        // ��������ѡ�� 0 ������ 1 �򻯸���ģ�� 2 SAASģ�� 3 ZTDģ�� 4 ZTD+Grad ģ��
    int niter;                // �˲���������
    
    double thresslip = 0.5;         // �޼��ξ����������̽����ֵ
};

//������Ϣ�ͽ��ջ���Ϣ�ṹ��
/*******������Ϣ********/
struct  SatInfo
{
	string prn;  //���ǵ�PRN��
	GPSTime t;   //���ڵ�ʱ��
	Cartesian pos;  //��ʱ�����ǵ�λ��
    Cartesian velocity; //
	double delta_t;  //���ǵĸ������Ӳ����
    double drift;
    double var;
    bool flag;
};

/*******���㶨λ�õ��Ľ��ջ���Ϣ*******/
struct ssat_t
{
    int slip[2] ;    //������ʶ 1 ������
    double azel[2]; //��ʾ��λ��/���� {az,el} (rad)
    
    double gf;    //����Ͼ���
    double phw;
    bool vs;    //valid of satllite 
};

struct RcvInfo
{
	GPSTime rcvtime; //����ʱ��
	Cartesian pos;   //���ջ�λ�õĵѿ�������ϵ
    Cartesian velocity; //���ջ��ٶȵѿ�������ϵ
    Topocentric diff; //����õ��Ľ��ջ�λ����۲��ļ������Ľ���λ�õĲ�
	double delta_t;  //���ջ��Ӳ�
    double clock_drift;
	int validSatNum; //��Ч������
	double sigma0;
	double  GDOP;
	double  PDOP;
	double  TDOP;
	double  HDOP;
    double  VDOP;
    int  nx;
    double  x[50] = {0.0};       //������״̬����
    double  P[50][50] = {0.0};   // ״̬�����ķ���
    double tt;  //�����ϸ�ʱ�̵�ʱ���
    ssat_t ssat[MAXGPS] = {{0}};
    Adjust_Scheme opt;
};



#endif
