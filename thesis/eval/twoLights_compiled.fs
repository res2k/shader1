twoLights_compiled.cg
!!NVfp4.0
# cgc version 2.2.0017, build date Jan 29 2010
# command line args: -profile gp4fp
# source file: twoLights_compiled.cg
#vendor NVIDIA Corporation
#version 2.2.0.17
#profile gp4fp
#program fragment_main_
#semantic fragment_main_.imodelViewProj_
#semantic fragment_main_.ieyePosition_
#semantic fragment_main_.iglobalAmbient_
#semantic fragment_main_.ilights_position_
#semantic fragment_main_.ilights_color_
#semantic fragment_main_.imaterial_Ke_
#semantic fragment_main_.imaterial_Ka_
#semantic fragment_main_.imaterial_Kd_
#semantic fragment_main_.imaterial_Ks_
#semantic fragment_main_.imaterial_shininess_
#var float4 v2f.m_position_B0_ : $vin.ATTR0 : ATTR0 : 0 : 1
#var float3 v2f.m_normal_B0_ : $vin.ATTR1 : ATTR1 : 0 : 1
#var float4x4 imodelViewProj_ :  : , 4 : 1 : 0
#var float3 ieyePosition_ :  : c[0] : 2 : 1
#var float3 iglobalAmbient_ :  : c[1] : 3 : 1
#var float3 ilights_position_[0] :  : c[2] : 4 : 1
#var float3 ilights_position_[1] :  : c[3] : 4 : 1
#var float3 ilights_color_[0] :  : c[4] : 5 : 1
#var float3 ilights_color_[1] :  : c[5] : 5 : 1
#var float3 imaterial_Ke_ :  : c[6] : 6 : 1
#var float3 imaterial_Ka_ :  : c[7] : 7 : 1
#var float3 imaterial_Kd_ :  : c[8] : 8 : 1
#var float3 imaterial_Ks_ :  : c[9] : 9 : 1
#var float imaterial_shininess_ :  : c[10] : 10 : 1
#var float4 ocolor_ : $vout.COLOR : COL : 11 : 1
PARAM c[11] = { program.local[0..10] };
ATTRIB fragment_attrib[] = { fragment.attrib[0..1] };
TEMP R0, R1, R2, R3, R4;
TEMP RC, HC;
OUTPUT oCol = result.color;
MOV.F R0.xyz, c[1];
MUL.F R0.xyz, R0, c[7];
ADD.F R0.xyz, R0, c[6];
MOV.F R1.xyz, {0}.x;
MOV.F R2.xyz, {0}.x;
MOV.S R0.w, {0}.x;
MOV.U R1.w, {1}.x;
REP.S ;
SEQ.U R1.w, R1, {0}.x;
MOV.U.CC RC.x, -R1.w;
BRK   (GT.x);
MOV.U R1.w, R0;
ADD.F R3.xyz, c[R1.w + 2], -fragment.attrib[0];
DP3.F R2.w, R3, R3;
RSQ.F R3.w, R2.w;
ADD.F R4.xyz, -fragment.attrib[0], c[0];
DP3.F R2.w, R4, R4;
MUL.F R3.xyz, R3.w, R3;
RSQ.F R2.w, R2.w;
MAD.F R4.xyz, R2.w, R4, R3;
DP3.F R2.w, R4, R4;
RSQ.F R2.w, R2.w;
MUL.F R4.xyz, R2.w, R4;
DP3.F R2.w, fragment.attrib[1], R4;
DP3.F R3.x, fragment.attrib[1], R3;
MAX.F R3.w, R3.x, {0}.x;
MOV.F R3.xyz, c[R1.w + 4];
MAX.F R2.w, R2, {0}.x;
ADD.S R0.w, R0, {1}.x;
MUL.F R4.xyz, R3, R3.w;
SLT.S R1.w, R0, {2}.x;
SLE.F R3.w, R3, {0}.x;
TRUNC.U.CC HC.x, R3.w;
POW.F R2.w, R2.w, c[10].x;
MOV.S R1.w, -R1;
IF    NE.x;
MOV.F R2.w, {0}.x;
ELSE;
ENDIF;
MAD.F R2.xyz, R3, R2.w, R2;
ADD.F R1.xyz, R1, R4;
ENDREP;
MAD.F R0.xyz, R1, c[8], R0;
MAD.F oCol.xyz, R2, c[9], R0;
MOV.F oCol.w, {1}.x;
END
# 45 instructions, 5 R-regs
