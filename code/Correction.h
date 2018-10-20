/***************************************************





******************************************************/

#ifndef CORRECTION_H
#define CORRECTION_H

#include "P_Struct.h"

class Correction
{

public:
	Correction();
	~Correction();

	/************�����Ӳ����******************************/

	/************�����ЧӦ����****************************/

	/************�������ӳٸ���****************************/
	static double tropSimple(const double E);    //�����������ӳٵļ�ģ��
	static double tropSaas(const Cartesian &X1, const Cartesian &X2);      //�����������ӳٵ�SAASģ�� 


	/************������ӳٸ���***************************/
	static double ionKlobuchar(const double a[4],const double b[4],const int &gpstime,
		const Cartesian &X1,const Cartesian &X2);  //Klobucharģ�ͶԶԵ�����ӳ�

	/************������ת����*****************************/
	static Cartesian EarthRotatCorr(double t, Cartesian SatPos);//tΪ�����ź���ʵ����ʱ�䣬�����ջ�����ʱ+���ջ��Ӳ�/c��-(���Ƿ�������ʱ)



};




#endif;
