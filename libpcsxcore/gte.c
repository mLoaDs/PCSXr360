/***************************************************************************
 *   PCSX-Revolution - PlayStation Emulator for Nintendo Wii               *
 *   Copyright (C) 2009-2010  PCSX-Revolution Dev Team                     *
 *   <http://code.google.com/p/pcsx-revolution/>                           *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   51 Franklin Street, Fifth Floor, Boston, MA 02111-1307 USA.           *
 ***************************************************************************/

/*
* GTE functions.
*/
#include "gte.h"
#include "psxmem.h"

static __inline u32 MFC2(int reg) {
	switch (reg) {
		case 1:
		case 3:
		case 5:
		case 8:
		case 9:
		case 10:
		case 11:
			psxRegs.CP2D.r[reg] = (s32)psxRegs.CP2D.p[reg].sw.l;
			break;

		case 7:
		case 16:
		case 17:
		case 18:
		case 19:
			psxRegs.CP2D.r[reg] = (u32)psxRegs.CP2D.p[reg].w.l;
			break;

		case 15:
			psxRegs.CP2D.r[reg] = gteSXY2;
			break;

		case 28:
		case 29:
			psxRegs.CP2D.r[reg] = LIM(gteIR1 >> 7, 0x1f, 0, 0) |
									(LIM(gteIR2 >> 7, 0x1f, 0, 0) << 5) |
									(LIM(gteIR3 >> 7, 0x1f, 0, 0) << 10);
			break;
	}
	return psxRegs.CP2D.r[reg];
}

static __inline void MTC2(u32 value, int reg) {
	switch (reg) {
		case 15:
			gteSXY0 = gteSXY1;
			gteSXY1 = gteSXY2;
			gteSXY2 = value;
			gteSXYP = value;
			break;

		case 28:
			gteIRGB = value;
			gteIR1 = (value & 0x1f) << 7;
			gteIR2 = (value & 0x3e0) << 2;
			gteIR3 = (value & 0x7c00) >> 3;
			break;

		case 30:
			{
#if 1
				int a;
				gteLZCS = value;

				// non-MAME code
				a = gteLZCS;
				if (a > 0) {
					int i;
					for (i = 31; (a & (1 << i)) == 0 && i >= 0; i--);
					gteLZCR = 31 - i;
				} else if (a < 0) {
					int i;
					a ^= 0xffffffff;
					for (i = 31; (a & (1 << i)) == 0 && i >= 0; i--);
					gteLZCR = 31 - i;
				} else {
					gteLZCR = 32;
				}
#else
				// MAME
				u32 lzcs = value;
				u32 lzcr = 0;

				gteLZCS = value;

				if( ( lzcs & 0x80000000 ) == 0 )
				{
					lzcs = ~lzcs;
				}
				while( ( lzcs & 0x80000000 ) != 0 )
				{
					lzcr++;
					lzcs <<= 1;
				}
				gteLZCR = lzcr;
#endif
			}
			break;

#if 1
		case 31:
			return;
#else
		case 31:
			value = psxRegs.CP2D.r[reg];
			break;
#endif

		default:
			psxRegs.CP2D.r[reg] = value;
	}
}

static __inline void CTC2(u32 value, int reg) {
	switch (reg) {
		case 4:
		case 12:
		case 20:
		case 26:
		case 27:
		case 29:
		case 30:
			value = (s32)(s16)value;
			break;

		case 31:
			value = value & 0x7ffff000;
			if (value & 0x7f87e000) value |= 0x80000000;
			break;
	}

	psxRegs.CP2C.r[reg] = value;
}

void gteMFC2() {
	if (!_Rt_) return;
	psxRegs.GPR.r[_Rt_] = MFC2(_Rd_);
}

void gteCFC2() {
	if (!_Rt_) return;
	psxRegs.GPR.r[_Rt_] = psxRegs.CP2C.r[_Rd_];
}

void gteMTC2() {
	MTC2(psxRegs.GPR.r[_Rt_], _Rd_);
}

void gteCTC2() {
	CTC2(psxRegs.GPR.r[_Rt_], _Rd_);
}

#define _oB_ (psxRegs.GPR.r[_Rs_] + _Imm_)

void gteLWC2() {
//if(use_vm){
//	MTC2(psxMemRead32(_oB_), _Rt_);}
//else{
	MTC2(psxMemRead32_2(_oB_), _Rt_);//}//teste
}

void gteSWC2() {
//if(use_vm){
//	psxMemWrite32(_oB_, MFC2(_Rt_));}
//else{
	psxMemWrite32_2(_oB_, MFC2(_Rt_));//}//teste
}

void gteMVMVA() {
	int shift = 12 * GTE_SF(gteop);
	int mx = GTE_MX(gteop);
	int v = GTE_V(gteop);
	int cv = GTE_CV(gteop);
	int lm = GTE_LM(gteop);
	s32 vx = VX(v);
	s32 vy = VY(v);
	s32 vz = VZ(v);

#ifdef GTE_LOG
	GTE_LOG("GTE MVMVA\n");
#endif
	gteFLAG = 0;

	gteMAC1 = A1((((s64)CV1(cv) << 12) + (MX11(mx) * vx) + (MX12(mx) * vy) + (MX13(mx) * vz)) >> shift);
	gteMAC2 = A2((((s64)CV2(cv) << 12) + (MX21(mx) * vx) + (MX22(mx) * vy) + (MX23(mx) * vz)) >> shift);
	gteMAC3 = A3((((s64)CV3(cv) << 12) + (MX31(mx) * vx) + (MX32(mx) * vy) + (MX33(mx) * vz)) >> shift);

	gteIR1 = limB1(gteMAC1, lm);
	gteIR2 = limB2(gteMAC2, lm);
	gteIR3 = limB3(gteMAC3, lm);
}

void gteNCLIP() {
#ifdef GTE_LOG
	GTE_LOG("GTE NCLIP\n");
#endif
	gteFLAG = 0;

	gteMAC0 = F((s64)gteSX0 * (gteSY1 - gteSY2) +
				gteSX1 * (gteSY2 - gteSY0) +
				gteSX2 * (gteSY0 - gteSY1));
}

void gteAVSZ3() {
#ifdef GTE_LOG
	GTE_LOG("GTE AVSZ3\n");
#endif
	gteFLAG = 0;

	gteMAC0 = F((s64)(gteZSF3 * gteSZ1) + (gteZSF3 * gteSZ2) + (gteZSF3 * gteSZ3));
	gteOTZ = limD(gteMAC0 >> 12);
}

void gteAVSZ4() {
#ifdef GTE_LOG
	GTE_LOG("GTE AVSZ4\n");
#endif
	gteFLAG = 0;

	gteMAC0 = F((s64)(gteZSF4 * (gteSZ0 + gteSZ1 + gteSZ2 + gteSZ3)));
	gteOTZ = limD(gteMAC0 >> 12);
}

void gteSQR() {
	int shift = 12 * GTE_SF(gteop);
	int lm = GTE_LM(gteop);

#ifdef GTE_LOG
	GTE_LOG("GTE SQR\n");
#endif
	gteFLAG = 0;

	gteMAC1 = A1((gteIR1 * gteIR1) >> shift);
	gteMAC2 = A2((gteIR2 * gteIR2) >> shift);
	gteMAC3 = A3((gteIR3 * gteIR3) >> shift);
	gteIR1 = limB1(gteMAC1, lm);
	gteIR2 = limB2(gteMAC2, lm);
	gteIR3 = limB3(gteMAC3, lm);
}

void gteNCCS() {
#ifdef GTE_LOG
	GTE_LOG("GTE NCCS\n");
#endif
	gteFLAG = 0;

	gteMAC1 = A1((((s64)gteL11 * gteVX0) + (gteL12 * gteVY0) + (gteL13 * gteVZ0)) >> 12);
	gteMAC2 = A2((((s64)gteL21 * gteVX0) + (gteL22 * gteVY0) + (gteL23 * gteVZ0)) >> 12);
	gteMAC3 = A3((((s64)gteL31 * gteVX0) + (gteL32 * gteVY0) + (gteL33 * gteVZ0)) >> 12);
	gteIR1 = limB1(gteMAC1, 1);
	gteIR2 = limB2(gteMAC2, 1);
	gteIR3 = limB3(gteMAC3, 1);
	gteMAC1 = A1((((s64)gteRBK << 12) + (gteLR1 * gteIR1) + (gteLR2 * gteIR2) + (gteLR3 * gteIR3)) >> 12);
	gteMAC2 = A2((((s64)gteGBK << 12) + (gteLG1 * gteIR1) + (gteLG2 * gteIR2) + (gteLG3 * gteIR3)) >> 12);
	gteMAC3 = A3((((s64)gteBBK << 12) + (gteLB1 * gteIR1) + (gteLB2 * gteIR2) + (gteLB3 * gteIR3)) >> 12);
	gteIR1 = limB1(gteMAC1, 1);
	gteIR2 = limB2(gteMAC2, 1);
	gteIR3 = limB3(gteMAC3, 1);
	gteMAC1 = A1(((s64)gteR * gteIR1) >> 8);
	gteMAC2 = A2(((s64)gteG * gteIR2) >> 8);
	gteMAC3 = A3(((s64)gteB * gteIR3) >> 8);
	gteIR1 = limB1(gteMAC1, 1);
	gteIR2 = limB2(gteMAC2, 1);
	gteIR3 = limB3(gteMAC3, 1);

	gteRGB0 = gteRGB1;
	gteRGB1 = gteRGB2;
	gteCODE2 = gteCODE;
	gteR2 = limC1(gteMAC1 >> 4);
	gteG2 = limC2(gteMAC2 >> 4);
	gteB2 = limC3(gteMAC3 >> 4);
}

void gteNCCT() {
	int v;
	s32 vx, vy, vz;

#ifdef GTE_LOG
	GTE_LOG("GTE NCCT\n");
#endif
	gteFLAG = 0;

	for (v = 0; v < 3; v++) {
		vx = VX(v);
		vy = VY(v);
		vz = VZ(v);
		gteMAC1 = A1((((s64)gteL11 * vx) + (gteL12 * vy) + (gteL13 * vz)) >> 12);
		gteMAC2 = A2((((s64)gteL21 * vx) + (gteL22 * vy) + (gteL23 * vz)) >> 12);
		gteMAC3 = A3((((s64)gteL31 * vx) + (gteL32 * vy) + (gteL33 * vz)) >> 12);
		gteIR1 = limB1(gteMAC1, 1);
		gteIR2 = limB2(gteMAC2, 1);
		gteIR3 = limB3(gteMAC3, 1);
		gteMAC1 = A1((((s64)gteRBK << 12) + (gteLR1 * gteIR1) + (gteLR2 * gteIR2) + (gteLR3 * gteIR3)) >> 12);
		gteMAC2 = A2((((s64)gteGBK << 12) + (gteLG1 * gteIR1) + (gteLG2 * gteIR2) + (gteLG3 * gteIR3)) >> 12);
		gteMAC3 = A3((((s64)gteBBK << 12) + (gteLB1 * gteIR1) + (gteLB2 * gteIR2) + (gteLB3 * gteIR3)) >> 12);
		gteIR1 = limB1(gteMAC1, 1);
		gteIR2 = limB2(gteMAC2, 1);
		gteIR3 = limB3(gteMAC3, 1);
		gteMAC1 = A1(((s64)gteR * gteIR1) >> 8);
		gteMAC2 = A2(((s64)gteG * gteIR2) >> 8);
		gteMAC3 = A3(((s64)gteB * gteIR3) >> 8);

		gteRGB0 = gteRGB1;
		gteRGB1 = gteRGB2;
		gteCODE2 = gteCODE;
		gteR2 = limC1(gteMAC1 >> 4);
		gteG2 = limC2(gteMAC2 >> 4);
		gteB2 = limC3(gteMAC3 >> 4);
	}
	gteIR1 = limB1(gteMAC1, 1);
	gteIR2 = limB2(gteMAC2, 1);
	gteIR3 = limB3(gteMAC3, 1);
}

void gteNCDS() {
#ifdef GTE_LOG
	GTE_LOG("GTE NCDS\n");
#endif
	gteFLAG = 0;

	gteMAC1 = A1((((s64)gteL11 * gteVX0) + (gteL12 * gteVY0) + (gteL13 * gteVZ0)) >> 12);
	gteMAC2 = A2((((s64)gteL21 * gteVX0) + (gteL22 * gteVY0) + (gteL23 * gteVZ0)) >> 12);
	gteMAC3 = A3((((s64)gteL31 * gteVX0) + (gteL32 * gteVY0) + (gteL33 * gteVZ0)) >> 12);
	gteIR1 = limB1(gteMAC1, 1);
	gteIR2 = limB2(gteMAC2, 1);
	gteIR3 = limB3(gteMAC3, 1);
	gteMAC1 = A1((((s64)gteRBK << 12) + (gteLR1 * gteIR1) + (gteLR2 * gteIR2) + (gteLR3 * gteIR3)) >> 12);
	gteMAC2 = A2((((s64)gteGBK << 12) + (gteLG1 * gteIR1) + (gteLG2 * gteIR2) + (gteLG3 * gteIR3)) >> 12);
	gteMAC3 = A3((((s64)gteBBK << 12) + (gteLB1 * gteIR1) + (gteLB2 * gteIR2) + (gteLB3 * gteIR3)) >> 12);
	gteIR1 = limB1(gteMAC1, 1);
	gteIR2 = limB2(gteMAC2, 1);
	gteIR3 = limB3(gteMAC3, 1);
	gteMAC1 = A1(((((s64)gteR << 4) * gteIR1) + (gteIR0 * limB1(gteRFC - ((gteR * gteIR1) >> 8), 0))) >> 12);
	gteMAC2 = A2(((((s64)gteG << 4) * gteIR2) + (gteIR0 * limB2(gteGFC - ((gteG * gteIR2) >> 8), 0))) >> 12);
	gteMAC3 = A3(((((s64)gteB << 4) * gteIR3) + (gteIR0 * limB3(gteBFC - ((gteB * gteIR3) >> 8), 0))) >> 12);
	gteIR1 = limB1(gteMAC1, 1);
	gteIR2 = limB2(gteMAC2, 1);
	gteIR3 = limB3(gteMAC3, 1);

	gteRGB0 = gteRGB1;
	gteRGB1 = gteRGB2;
	gteCODE2 = gteCODE;
	gteR2 = limC1(gteMAC1 >> 4);
	gteG2 = limC2(gteMAC2 >> 4);
	gteB2 = limC3(gteMAC3 >> 4);
}

void gteNCDT() {
	int v;
	s32 vx, vy, vz;

#ifdef GTE_LOG
	GTE_LOG("GTE NCDT\n");
#endif
	gteFLAG = 0;

	for (v = 0; v < 3; v++) {
		vx = VX(v);
		vy = VY(v);
		vz = VZ(v);
		gteMAC1 = A1((((s64)gteL11 * vx) + (gteL12 * vy) + (gteL13 * vz)) >> 12);
		gteMAC2 = A2((((s64)gteL21 * vx) + (gteL22 * vy) + (gteL23 * vz)) >> 12);
		gteMAC3 = A3((((s64)gteL31 * vx) + (gteL32 * vy) + (gteL33 * vz)) >> 12);
		gteIR1 = limB1(gteMAC1, 1);
		gteIR2 = limB2(gteMAC2, 1);
		gteIR3 = limB3(gteMAC3, 1);
		gteMAC1 = A1((((s64)gteRBK << 12) + (gteLR1 * gteIR1) + (gteLR2 * gteIR2) + (gteLR3 * gteIR3)) >> 12);
		gteMAC2 = A2((((s64)gteGBK << 12) + (gteLG1 * gteIR1) + (gteLG2 * gteIR2) + (gteLG3 * gteIR3)) >> 12);
		gteMAC3 = A3((((s64)gteBBK << 12) + (gteLB1 * gteIR1) + (gteLB2 * gteIR2) + (gteLB3 * gteIR3)) >> 12);
		gteIR1 = limB1(gteMAC1, 1);
		gteIR2 = limB2(gteMAC2, 1);
		gteIR3 = limB3(gteMAC3, 1);
		gteMAC1 = A1(((((s64)gteR << 4) * gteIR1) + (gteIR0 * limB1(gteRFC - ((gteR * gteIR1) >> 8), 0))) >> 12);
		gteMAC2 = A2(((((s64)gteG << 4) * gteIR2) + (gteIR0 * limB2(gteGFC - ((gteG * gteIR2) >> 8), 0))) >> 12);
		gteMAC3 = A3(((((s64)gteB << 4) * gteIR3) + (gteIR0 * limB3(gteBFC - ((gteB * gteIR3) >> 8), 0))) >> 12);

		gteRGB0 = gteRGB1;
		gteRGB1 = gteRGB2;
		gteCODE2 = gteCODE;
		gteR2 = limC1(gteMAC1 >> 4);
		gteG2 = limC2(gteMAC2 >> 4);
		gteB2 = limC3(gteMAC3 >> 4);
	}
	gteIR1 = limB1(gteMAC1, 1);
	gteIR2 = limB2(gteMAC2, 1);
	gteIR3 = limB3(gteMAC3, 1);
}

void gteOP() {
	int shift = 12 * GTE_SF(gteop);
	int lm = GTE_LM(gteop);

#ifdef GTE_LOG
	GTE_LOG("GTE OP\n");
#endif
	gteFLAG = 0;

	gteMAC1 = A1(((s64)(gteR22 * gteIR3) - (gteR33 * gteIR2)) >> shift);
	gteMAC2 = A2(((s64)(gteR33 * gteIR1) - (gteR11 * gteIR3)) >> shift);
	gteMAC3 = A3(((s64)(gteR11 * gteIR2) - (gteR22 * gteIR1)) >> shift);
	gteIR1 = limB1(gteMAC1, lm);
	gteIR2 = limB2(gteMAC2, lm);
	gteIR3 = limB3(gteMAC3, lm);
}

void gteDCPL() {
	int lm = GTE_LM(gteop);

	s64 RIR1 = ((s64)gteR * gteIR1) >> 8;
	s64 GIR2 = ((s64)gteG * gteIR2) >> 8;
	s64 BIR3 = ((s64)gteB * gteIR3) >> 8;

#ifdef GTE_LOG
	GTE_LOG("GTE DCPL\n");
#endif
	gteFLAG = 0;

	gteMAC1 = A1(RIR1 + ((gteIR0 * limB1(gteRFC - RIR1, 0)) >> 12));
	gteMAC2 = A2(GIR2 + ((gteIR0 * limB1(gteGFC - GIR2, 0)) >> 12));
	gteMAC3 = A3(BIR3 + ((gteIR0 * limB1(gteBFC - BIR3, 0)) >> 12));

	gteIR1 = limB1(gteMAC1, lm);
	gteIR2 = limB2(gteMAC2, lm);
	gteIR3 = limB3(gteMAC3, lm);

	gteRGB0 = gteRGB1;
	gteRGB1 = gteRGB2;
	gteCODE2 = gteCODE;
	gteR2 = limC1(gteMAC1 >> 4);
	gteG2 = limC2(gteMAC2 >> 4);
	gteB2 = limC3(gteMAC3 >> 4);
}

void gteGPF() {
	int shift = 12 * GTE_SF(gteop);

#ifdef GTE_LOG
	GTE_LOG("GTE GPF\n");
#endif
	gteFLAG = 0;

	gteMAC1 = A1(((s64)gteIR0 * gteIR1) >> shift);
	gteMAC2 = A2(((s64)gteIR0 * gteIR2) >> shift);
	gteMAC3 = A3(((s64)gteIR0 * gteIR3) >> shift);
	gteIR1 = limB1(gteMAC1, 0);
	gteIR2 = limB2(gteMAC2, 0);
	gteIR3 = limB3(gteMAC3, 0);

	gteRGB0 = gteRGB1;
	gteRGB1 = gteRGB2;
	gteCODE2 = gteCODE;
	gteR2 = limC1(gteMAC1 >> 4);
	gteG2 = limC2(gteMAC2 >> 4);
	gteB2 = limC3(gteMAC3 >> 4);
}

void gteGPL() {
	int shift = 12 * GTE_SF(gteop);

#ifdef GTE_LOG
	GTE_LOG("GTE GPL\n");
#endif
	gteFLAG = 0;

	gteMAC1 = A1((((s64)gteMAC1 << shift) + (gteIR0 * gteIR1)) >> shift);
	gteMAC2 = A2((((s64)gteMAC2 << shift) + (gteIR0 * gteIR2)) >> shift);
	gteMAC3 = A3((((s64)gteMAC3 << shift) + (gteIR0 * gteIR3)) >> shift);
	gteIR1 = limB1(gteMAC1, 0);
	gteIR2 = limB2(gteMAC2, 0);
	gteIR3 = limB3(gteMAC3, 0);

	gteRGB0 = gteRGB1;
	gteRGB1 = gteRGB2;
	gteCODE2 = gteCODE;
	gteR2 = limC1(gteMAC1 >> 4);
	gteG2 = limC2(gteMAC2 >> 4);
	gteB2 = limC3(gteMAC3 >> 4);
}

void gteDPCS() {
	int shift = 12 * GTE_SF(gteop);

#ifdef GTE_LOG
	GTE_LOG("GTE DPCS\n");
#endif
	gteFLAG = 0;

	gteMAC1 = A1(((gteR << 16) + (gteIR0 * limB1(A1((s64)gteRFC - (gteR << 4)) << (12 - shift), 0))) >> 12);
	gteMAC2 = A2(((gteG << 16) + (gteIR0 * limB2(A2((s64)gteGFC - (gteG << 4)) << (12 - shift), 0))) >> 12);
	gteMAC3 = A3(((gteB << 16) + (gteIR0 * limB3(A3((s64)gteBFC - (gteB << 4)) << (12 - shift), 0))) >> 12);

	gteIR1 = limB1(gteMAC1, 0);
	gteIR2 = limB2(gteMAC2, 0);
	gteIR3 = limB3(gteMAC3, 0);
	gteRGB0 = gteRGB1;
	gteRGB1 = gteRGB2;
	gteCODE2 = gteCODE;
	gteR2 = limC1(gteMAC1 >> 4);
	gteG2 = limC2(gteMAC2 >> 4);
	gteB2 = limC3(gteMAC3 >> 4);
}

void gteDPCT() {
	int v;

#ifdef GTE_LOG
	GTE_LOG("GTE DPCT\n");
#endif
	gteFLAG = 0;

	for (v = 0; v < 3; v++) {
		gteMAC1 = A1((((s64)gteR0 << 16) + ((s64)gteIR0 * (limB1(gteRFC - (gteR0 << 4), 0)))) >> 12);
		gteMAC2 = A2((((s64)gteG0 << 16) + ((s64)gteIR0 * (limB1(gteGFC - (gteG0 << 4), 0)))) >> 12);
		gteMAC3 = A3((((s64)gteB0 << 16) + ((s64)gteIR0 * (limB1(gteBFC - (gteB0 << 4), 0)))) >> 12);

		gteRGB0 = gteRGB1;
		gteRGB1 = gteRGB2;
		gteCODE2 = gteCODE;
		gteR2 = limC1(gteMAC1 >> 4);
		gteG2 = limC2(gteMAC2 >> 4);
		gteB2 = limC3(gteMAC3 >> 4);
	}
	gteIR1 = limB1(gteMAC1, 0);
	gteIR2 = limB2(gteMAC2, 0);
	gteIR3 = limB3(gteMAC3, 0);
}

void gteNCS() {
#ifdef GTE_LOG
	GTE_LOG("GTE NCS\n");
#endif
	gteFLAG = 0;

	gteMAC1 = A1((((s64)gteL11 * gteVX0) + (gteL12 * gteVY0) + (gteL13 * gteVZ0)) >> 12);
	gteMAC2 = A2((((s64)gteL21 * gteVX0) + (gteL22 * gteVY0) + (gteL23 * gteVZ0)) >> 12);
	gteMAC3 = A3((((s64)gteL31 * gteVX0) + (gteL32 * gteVY0) + (gteL33 * gteVZ0)) >> 12);
	gteIR1 = limB1(gteMAC1, 1);
	gteIR2 = limB2(gteMAC2, 1);
	gteIR3 = limB3(gteMAC3, 1);
	gteMAC1 = A1((((s64)gteRBK << 12) + (gteLR1 * gteIR1) + (gteLR2 * gteIR2) + (gteLR3 * gteIR3)) >> 12);
	gteMAC2 = A2((((s64)gteGBK << 12) + (gteLG1 * gteIR1) + (gteLG2 * gteIR2) + (gteLG3 * gteIR3)) >> 12);
	gteMAC3 = A3((((s64)gteBBK << 12) + (gteLB1 * gteIR1) + (gteLB2 * gteIR2) + (gteLB3 * gteIR3)) >> 12);
	gteIR1 = limB1(gteMAC1, 1);
	gteIR2 = limB2(gteMAC2, 1);
	gteIR3 = limB3(gteMAC3, 1);

	gteRGB0 = gteRGB1;
	gteRGB1 = gteRGB2;
	gteCODE2 = gteCODE;
	gteR2 = limC1(gteMAC1 >> 4);
	gteG2 = limC2(gteMAC2 >> 4);
	gteB2 = limC3(gteMAC3 >> 4);
}

void gteNCT() {
	int v;
	s32 vx, vy, vz;

#ifdef GTE_LOG
	GTE_LOG("GTE NCT\n");
#endif
	gteFLAG = 0;

	for (v = 0; v < 3; v++) {
		vx = VX(v);
		vy = VY(v);
		vz = VZ(v);
		gteMAC1 = A1((((s64)gteL11 * vx) + (gteL12 * vy) + (gteL13 * vz)) >> 12);
		gteMAC2 = A2((((s64)gteL21 * vx) + (gteL22 * vy) + (gteL23 * vz)) >> 12);
		gteMAC3 = A3((((s64)gteL31 * vx) + (gteL32 * vy) + (gteL33 * vz)) >> 12);
		gteIR1 = limB1(gteMAC1, 1);
		gteIR2 = limB2(gteMAC2, 1);
		gteIR3 = limB3(gteMAC3, 1);
		gteMAC1 = A1((((s64)gteRBK << 12) + (gteLR1 * gteIR1) + (gteLR2 * gteIR2) + (gteLR3 * gteIR3)) >> 12);
		gteMAC2 = A2((((s64)gteGBK << 12) + (gteLG1 * gteIR1) + (gteLG2 * gteIR2) + (gteLG3 * gteIR3)) >> 12);
		gteMAC3 = A3((((s64)gteBBK << 12) + (gteLB1 * gteIR1) + (gteLB2 * gteIR2) + (gteLB3 * gteIR3)) >> 12);
		gteRGB0 = gteRGB1;
		gteRGB1 = gteRGB2;
		gteCODE2 = gteCODE;
		gteR2 = limC1(gteMAC1 >> 4);
		gteG2 = limC2(gteMAC2 >> 4);
		gteB2 = limC3(gteMAC3 >> 4);
	}
	gteIR1 = limB1(gteMAC1, 1);
	gteIR2 = limB2(gteMAC2, 1);
	gteIR3 = limB3(gteMAC3, 1);
}

void gteCC() {
#ifdef GTE_LOG
	GTE_LOG("GTE CC\n");
#endif
	gteFLAG = 0;

	gteMAC1 = A1((((s64)gteRBK << 12) + (gteLR1 * gteIR1) + (gteLR2 * gteIR2) + (gteLR3 * gteIR3)) >> 12);
	gteMAC2 = A2((((s64)gteGBK << 12) + (gteLG1 * gteIR1) + (gteLG2 * gteIR2) + (gteLG3 * gteIR3)) >> 12);
	gteMAC3 = A3((((s64)gteBBK << 12) + (gteLB1 * gteIR1) + (gteLB2 * gteIR2) + (gteLB3 * gteIR3)) >> 12);
	gteIR1 = limB1(gteMAC1, 1);
	gteIR2 = limB2(gteMAC2, 1);
	gteIR3 = limB3(gteMAC3, 1);
	gteMAC1 = A1(((s64)gteR * gteIR1) >> 8);
	gteMAC2 = A2(((s64)gteG * gteIR2) >> 8);
	gteMAC3 = A3(((s64)gteB * gteIR3) >> 8);
	gteIR1 = limB1(gteMAC1, 1);
	gteIR2 = limB2(gteMAC2, 1);
	gteIR3 = limB3(gteMAC3, 1);

	gteRGB0 = gteRGB1;
	gteRGB1 = gteRGB2;
	gteCODE2 = gteCODE;
	gteR2 = limC1(gteMAC1 >> 4);
	gteG2 = limC2(gteMAC2 >> 4);
	gteB2 = limC3(gteMAC3 >> 4);
}

void gteINTPL() {
	int shift = 12 * GTE_SF(gteop);
	int lm = GTE_LM(gteop);

#ifdef GTE_LOG
	GTE_LOG("GTE INTPL\n");
#endif
	gteFLAG = 0;

	gteMAC1 = A1(((gteIR1 << 12) + (gteIR0 * limB1(((s64)gteRFC - gteIR1), 0))) >> shift);
	gteMAC2 = A2(((gteIR2 << 12) + (gteIR0 * limB2(((s64)gteGFC - gteIR2), 0))) >> shift);
	gteMAC3 = A3(((gteIR3 << 12) + (gteIR0 * limB3(((s64)gteBFC - gteIR3), 0))) >> shift);
	gteIR1 = limB1(gteMAC1, lm);
	gteIR2 = limB2(gteMAC2, lm);
	gteIR3 = limB3(gteMAC3, lm);
	gteRGB0 = gteRGB1;
	gteRGB1 = gteRGB2;
	gteCODE2 = gteCODE;
	gteR2 = limC1(gteMAC1 >> 4);
	gteG2 = limC2(gteMAC2 >> 4);
	gteB2 = limC3(gteMAC3 >> 4);
}

void gteCDP() {
#ifdef GTE_LOG
	GTE_LOG("GTE CDP\n");
#endif
	gteFLAG = 0;

	gteMAC1 = A1((((s64)gteRBK << 12) + (gteLR1 * gteIR1) + (gteLR2 * gteIR2) + (gteLR3 * gteIR3)) >> 12);
	gteMAC2 = A2((((s64)gteGBK << 12) + (gteLG1 * gteIR1) + (gteLG2 * gteIR2) + (gteLG3 * gteIR3)) >> 12);
	gteMAC3 = A3((((s64)gteBBK << 12) + (gteLB1 * gteIR1) + (gteLB2 * gteIR2) + (gteLB3 * gteIR3)) >> 12);
	gteIR1 = limB1(gteMAC1, 1);
	gteIR2 = limB2(gteMAC2, 1);
	gteIR3 = limB3(gteMAC3, 1);
	gteMAC1 = A1(((((s64)gteR << 4) * gteIR1) + (gteIR0 * limB1(gteRFC - ((gteR * gteIR1) >> 8), 0))) >> 12);
	gteMAC2 = A2(((((s64)gteG << 4) * gteIR2) + (gteIR0 * limB2(gteGFC - ((gteG * gteIR2) >> 8), 0))) >> 12);
	gteMAC3 = A3(((((s64)gteB << 4) * gteIR3) + (gteIR0 * limB3(gteBFC - ((gteB * gteIR3) >> 8), 0))) >> 12);
	gteIR1 = limB1(gteMAC1, 1);
	gteIR2 = limB2(gteMAC2, 1);
	gteIR3 = limB3(gteMAC3, 1);

	gteRGB0 = gteRGB1;
	gteRGB1 = gteRGB2;
	gteCODE2 = gteCODE;
	gteR2 = limC1(gteMAC1 >> 4);
	gteG2 = limC2(gteMAC2 >> 4);
	gteB2 = limC3(gteMAC3 >> 4);
}
