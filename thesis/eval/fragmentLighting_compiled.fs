fragmentLighting_compiled.cg
!!NVfp4.0
# cgc version 2.2.0017, build date Jan 29 2010
# command line args: -profile gp4fp
# source file: fragmentLighting_compiled.cg
#vendor NVIDIA Corporation
#version 2.2.0.17
#profile gp4fp
#program fragment_main_
#semantic fragment_main_.imodelViewProj_
#semantic fragment_main_.iglobalAmbient_
#semantic fragment_main_.ilightColor_
#semantic fragment_main_.ilightPosition_
#semantic fragment_main_.ieyePosition_
#semantic fragment_main_.iKe_
#semantic fragment_main_.iKa_
#semantic fragment_main_.iKd_
#semantic fragment_main_.iKs_
#semantic fragment_main_.ishininess_
#var float3 v2f.m_normal_B0_ : $vin.ATTR0 : ATTR0 : 0 : 1
#var float3 v2f.i_tmp8_ : $vin.ATTR1 : ATTR1 : 0 : 1
#var float3 v2f.i_tmp17_ : $vin.ATTR2 : ATTR2 : 0 : 1
#var float4x4 imodelViewProj_ :  : , 4 : 1 : 0
#var float3 iglobalAmbient_ :  : c[0] : 2 : 1
#var float3 ilightColor_ :  : c[1] : 3 : 1
#var float3 ilightPosition_ :  :  : 4 : 0
#var float3 ieyePosition_ :  :  : 5 : 0
#var float3 iKe_ :  : c[2] : 6 : 1
#var float3 iKa_ :  : c[3] : 7 : 1
#var float3 iKd_ :  : c[4] : 8 : 1
#var float3 iKs_ :  : c[5] : 9 : 1
#var float ishininess_ :  : c[6] : 10 : 1
#var float4 ocolor_ : $vout.COLOR : COL : 11 : 1
PARAM c[7] = { program.local[0..6] };
ATTRIB fragment_attrib[] = { fragment.attrib[0..2] };
TEMP R0, R1, R2;
TEMP RC, HC;
OUTPUT oCol = result.color;
DP3.F R0.x, fragment.attrib[1], fragment.attrib[1];
RSQ.F R0.x, R0.x;
DP3.F R0.w, fragment.attrib[0], fragment.attrib[0];
MUL.F R1.xyz, R0.x, fragment.attrib[1];
DP3.F R0.y, fragment.attrib[2], fragment.attrib[2];
RSQ.F R0.x, R0.y;
MAD.F R0.xyz, R0.x, fragment.attrib[2], R1;
DP3.F R1.w, R0, R0;
RSQ.F R1.w, R1.w;
MUL.F R2.xyz, R1.w, R0;
RSQ.F R0.w, R0.w;
MUL.F R0.xyz, R0.w, fragment.attrib[0];
DP3.F R0.w, R0, R2;
DP3.F R0.x, R1, R0;
MAX.F R0.w, R0, {0}.x;
MAX.F R1.w, R0.x, {0}.x;
MOV.F R1.xyz, c[1];
MUL.F R0.xyz, R1, c[4];
MUL.F R2.xyz, R0, R1.w;
MOV.F R0.xyz, c[0];
SLE.F R1.w, R1, {0}.x;
MUL.F R0.xyz, R0, c[3];
TRUNC.U.CC HC.x, R1.w;
POW.F R0.w, R0.w, c[6].x;
MUL.F R1.xyz, R1, c[5];
ADD.F R0.xyz, R0, c[2];
IF    NE.x;
MOV.F R0.w, {0}.x;
ELSE;
ENDIF;
ADD.F R0.xyz, R0, R2;
MAD.F oCol.xyz, R1, R0.w, R0;
MOV.F oCol.w, {1}.x;
END
# 33 instructions, 3 R-regs
