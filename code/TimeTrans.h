/*************************************************
Beta1.1
    �޸�ʱ�䣺2017.07.18
    �޸����ݣ�����ṹ�塢����

*************************************************/
#ifndef TimeTrans_H
#define TimeTrans_H


#include "P_Struct.h"




class TimeTrans
{
public:
	TimeTrans();
	~TimeTrans();

    static GPSTime CalendarTime2GPSTime(const CalendarTime &A);
	static JulianDay CalendarTime2JulianDay(const CalendarTime &A);//ͨ��ʱת��������
	static CalendarTime JulianDay2CalendarTime(const JulianDay &A);//������ת��ͨ��ʱ
	static GPSTime  JulianDay2GPSTime(const JulianDay &A);//������ת��GPSʱ
	static JulianDay GPSTime2JulianDay(const GPSTime &A);//GPSʱת��������
	static DayofYear CalendarTime2DayofYear(const CalendarTime &A);//ͨ��ʱת�������(������)
	
};

#endif;
