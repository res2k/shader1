C5E3f_basicLight.cg
!!NVfp4.0
# cgc version 2.2.0017, build date Jan 29 2010
# command line args: -profile gp4fp
# source file: C5E3f_basicLight.cg
#vendor NVIDIA Corporation
#version 2.2.0.17
#profile gp4fp
#program C5E3f_basicLight
#semantic C5E3f_basicLight.globalAmbient
#semantic C5E3f_basicLight.lightColor
#semantic C5E3f_basicLight.lightPosition
#semantic C5E3f_basicLight.eyePosition
#semantic C5E3f_basicLight.Ke
#semantic C5E3f_basicLight.Ka
#semantic C5E3f_basicLight.Kd
#semantic C5E3f_basicLight.Ks
#semantic C5E3f_basicLight.shininess
#var float4 position : $vin.TEXCOORD0 : TEX0 : 0 : 1
#var float3 normal : $vin.TEXCOORD1 : TEX1 : 1 : 1
#var float4 color : $vout.COLOR : COL : 2 : 1
#var float3 globalAmbient :  : c[0] : 3 : 1
#var float3 lightColor :  : c[1] : 4 : 1
#var float3 lightPosition :  : c[2] : 5 : 1
#var float3 eyePosition :  : c[3] : 6 : 1
#var float3 Ke :  : c[4] : 7 : 1
#var float3 Ka :  : c[5] : 8 : 1
#var float3 Kd :  : c[6] : 9 : 1
#var float3 Ks :  : c[7] : 10 : 1
#var float shininess :  : c[8] : 11 : 1
PARAM c[9] = { program.local[0..8] };
ATTRIB fragment_texcoord[] = { fragment.texcoord[0..1] };
TEMP R0, R1, R2;
TEMP RC, HC;
OUTPUT oCol = result.color;
ADD.F R1.xyz, -fragment.texcoord[0], c[2];
DP3.F R0.w, R1, R1;
ADD.F R0.xyz, -fragment.texcoord[0], c[3];
RSQ.F R0.w, R0.w;
MUL.F R1.xyz, R0.w, R1;
DP3.F R1.w, R0, R0;
RSQ.F R0.w, R1.w;
MAD.F R0.xyz, R0.w, R0, R1;
DP3.F R1.w, R0, R0;
RSQ.F R1.w, R1.w;
DP3.F R0.w, fragment.texcoord[1], fragment.texcoord[1];
MUL.F R2.xyz, R1.w, R0;
RSQ.F R0.w, R0.w;
MUL.F R0.xyz, R0.w, fragment.texcoord[1];
DP3.F R1.x, R1, R0;
DP3.F R0.w, R0, R2;
MAX.F R0.w, R0, {0}.x;
MOV.F R0.xyz, c[1];
MAX.F R1.w, R1.x, {0}.x;
MUL.F R0.xyz, R0, c[6];
MUL.F R1.xyz, R0, R1.w;
MOV.F R0.xyz, c[0];
SLE.F R1.w, R1, {0}.x;
TRUNC.U.CC HC.x, R1.w;
POW.F R0.w, R0.w, c[8].x;
MUL.F R0.xyz, R0, c[5];
IF    NE.x;
MOV.F R0.w, {0}.x;
ENDIF;
ADD.F R2.xyz, R0, c[4];
MOV.F R0.xyz, c[1];
ADD.F R1.xyz, R2, R1;
MUL.F R0.xyz, R0, c[7];
MAD.F oCol.xyz, R0, R0.w, R1;
MOV.F oCol.w, {1}.x;
END
# 35 instructions, 3 R-regs
