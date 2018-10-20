/*****************************************************************************
Beta1.0
    �޸�ʱ�䣺2017.7.18
    �޸����ݣ���������ת������




*******************************************************************************/
#ifndef COORDINATRANS_H
#define COORDINATRANS_H

#include "P_Struct.h"

class CoordTrans
{
public:
	CoordTrans();
	~CoordTrans();

public:
	/*******************************��������ת������**************************************/
	static Geodetic Cart2Geod(const Cartesian &X); //�ѿ�������ϵ->�������ϵ
	static Cartesian Geod2Cart(const Geodetic &G); //�������ϵ->�ѿ�������ϵ
	static Topocentric Cart2Topo(const Cartesian &X1, const Cartesian &X2); //�ѿ�������ϵ->վ��������ϵ
	static Cartesian Topo2Cart(const Cartesian &X1, const Topocentric &T2); //վ��������ϵ->�ѿ�������ϵ
	static Topopolar Topo2Topop(const Topocentric &Tc); //վ��������ϵ->վ�ļ�����ϵ
	static Topocentric Topop2Topo(const Topopolar &Tp); //վ�ļ�����ϵ->վ��������ϵ

};

#endif;