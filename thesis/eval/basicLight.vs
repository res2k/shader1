C5E1v_basicLight.cg
!!NVvp4.0
# cgc version 2.2.0017, build date Jan 29 2010
# command line args: -profile gp4vp
# source file: C5E1v_basicLight.cg
#vendor NVIDIA Corporation
#version 2.2.0.17
#profile gp4vp
#program C5E1v_basicLight
#semantic C5E1v_basicLight.modelViewProj
#semantic C5E1v_basicLight.globalAmbient
#semantic C5E1v_basicLight.lightColor
#semantic C5E1v_basicLight.lightPosition
#semantic C5E1v_basicLight.eyePosition
#semantic C5E1v_basicLight.Ke
#semantic C5E1v_basicLight.Ka
#semantic C5E1v_basicLight.Kd
#semantic C5E1v_basicLight.Ks
#semantic C5E1v_basicLight.shininess
#var float4 position : $vin.POSITION : ATTR0 : 0 : 1
#var float3 normal : $vin.NORMAL : ATTR2 : 1 : 1
#var float4 oPosition : $vout.POSITION : HPOS : 2 : 1
#var float4 color : $vout.COLOR : COL0 : 3 : 1
#var float4x4 modelViewProj :  : c[0], 4 : 4 : 1
#var float3 globalAmbient :  : c[4] : 5 : 1
#var float3 lightColor :  : c[5] : 6 : 1
#var float3 lightPosition :  : c[6] : 7 : 1
#var float3 eyePosition :  : c[7] : 8 : 1
#var float3 Ke :  : c[8] : 9 : 1
#var float3 Ka :  : c[9] : 10 : 1
#var float3 Kd :  : c[10] : 11 : 1
#var float3 Ks :  : c[11] : 12 : 1
#var float shininess :  : c[12] : 13 : 1
PARAM c[13] = { program.local[0..12] };
ATTRIB vertex_attrib[] = { vertex.attrib[0..2] };
TEMP R0, R1, R2;
TEMP RC, HC;
ADD.F R0.xyz, -vertex.attrib[0], c[6];
DP3.F R0.w, R0, R0;
ADD.F R1.xyz, -vertex.attrib[0], c[7];
RSQ.F R0.w, R0.w;
MUL.F R0.xyz, R0.w, R0;
DP3.F R1.w, R1, R1;
RSQ.F R0.w, R1.w;
MAD.F R1.xyz, R0.w, R1, R0;
DP3.F R0.w, R1, R1;
RSQ.F R0.w, R0.w;
MUL.F R1.xyz, R0.w, R1;
DP3.F R0.w, vertex.attrib[2], R1;
DP3.F R1.x, vertex.attrib[2], R0;
MAX.F R0.w, R0, {0}.x;
MOV.F R0.xyz, c[5];
MAX.F R1.w, R1.x, {0}.x;
MUL.F R0.xyz, R0, c[10];
MUL.F R1.xyz, R0, R1.w;
MOV.F R0.xyz, c[4];
SLE.F R1.w, R1, {0}.x;
TRUNC.U.CC HC.x, R1.w;
POW.F R0.w, R0.w, c[12].x;
MUL.F R0.xyz, R0, c[9];
DP4.F result.position.w, vertex.attrib[0], c[3];
DP4.F result.position.z, vertex.attrib[0], c[2];
DP4.F result.position.y, vertex.attrib[0], c[1];
DP4.F result.position.x, vertex.attrib[0], c[0];
IF    NE.x;
MOV.F R0.w, {0}.x;
ENDIF;
ADD.F R2.xyz, R0, c[8];
MOV.F R0.xyz, c[5];
ADD.F R1.xyz, R2, R1;
MUL.F R0.xyz, R0, c[11];
MAD.F result.color.xyz, R0, R0.w, R1;
MOV.F result.color.w, {1}.x;
END
# 36 instructions, 3 R-regs
