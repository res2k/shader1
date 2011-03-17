C5E4v_twoLights.cg
!!NVvp4.0
# cgc version 2.2.0017, build date Jan 29 2010
# command line args: -profile gp4vp
# source file: C5E4v_twoLights.cg
#vendor NVIDIA Corporation
#version 2.2.0.17
#profile gp4vp
#program C5E4v_twoLights
#semantic C5E4v_twoLights.modelViewProj
#semantic C5E4v_twoLights.eyePosition
#semantic C5E4v_twoLights.globalAmbient
#semantic C5E4v_twoLights.lights
#semantic C5E4v_twoLights.material
#var float4 position : $vin.POSITION : ATTR0 : 0 : 1
#var float3 normal : $vin.NORMAL : ATTR2 : 1 : 1
#var float4 oPosition : $vout.POSITION : HPOS : 2 : 1
#var float4 color : $vout.COLOR : COL0 : 3 : 1
#var float4x4 modelViewProj :  : c[0], 4 : 4 : 1
#var float3 eyePosition :  : c[4] : 5 : 1
#var float3 globalAmbient :  : c[5] : 6 : 1
#var float3 lights[0].position :  : c[6] : 7 : 1
#var float3 lights[0].color :  : c[7] : 7 : 1
#var float3 lights[1].position :  : c[8] : 7 : 1
#var float3 lights[1].color :  : c[9] : 7 : 1
#var float3 material.Ke :  : c[10] : 8 : 1
#var float3 material.Ka :  : c[11] : 8 : 1
#var float3 material.Kd :  : c[12] : 8 : 1
#var float3 material.Ks :  : c[13] : 8 : 1
#var float material.shininess :  : c[14] : 8 : 1
PARAM c[15] = { program.local[0..14] };
ATTRIB vertex_attrib[] = { vertex.attrib[0..2] };
TEMP R0, R1, R2, R3;
TEMP RC, HC;
ADD.F R1.xyz, -vertex.attrib[0], c[6];
DP3.F R0.w, R1, R1;
ADD.F R0.xyz, -vertex.attrib[0], c[4];
RSQ.F R0.w, R0.w;
MUL.F R1.xyz, R0.w, R1;
DP3.F R1.w, R0, R0;
RSQ.F R0.w, R1.w;
MAD.F R0.xyz, R0.w, R0, R1;
DP3.F R0.w, R0, R0;
RSQ.F R0.w, R0.w;
MUL.F R0.xyz, R0.w, R0;
DP3.F R0.y, vertex.attrib[2], R0;
DP3.F R0.x, vertex.attrib[2], R1;
MAX.F R0.y, R0, {0}.x;
MOV.F R1.xyz, c[5];
MAX.F R1.w, R0.x, {0}.x;
POW.F R0.w, R0.y, c[14].x;
MUL.F R0.xyz, R1.w, c[7];
SLE.F R1.w, R1, {0}.x;
TRUNC.U.CC HC.x, R1.w;
MUL.F R1.xyz, R1, c[11];
DP4.F result.position.w, vertex.attrib[0], c[3];
DP4.F result.position.z, vertex.attrib[0], c[2];
DP4.F result.position.y, vertex.attrib[0], c[1];
DP4.F result.position.x, vertex.attrib[0], c[0];
IF    NE.x;
MOV.F R0.w, {0}.x;
ENDIF;
ADD.F R2.xyz, -vertex.attrib[0], c[8];
DP3.F R1.w, R2, R2;
RSQ.F R2.w, R1.w;
ADD.F R3.xyz, -vertex.attrib[0], c[4];
DP3.F R1.w, R3, R3;
MUL.F R2.xyz, R2.w, R2;
RSQ.F R1.w, R1.w;
MAD.F R3.xyz, R1.w, R3, R2;
DP3.F R1.w, R3, R3;
RSQ.F R1.w, R1.w;
MUL.F R3.xyz, R1.w, R3;
DP3.F R1.w, vertex.attrib[2], R3;
MAX.F R2.w, R1, {0}.x;
DP3.F R1.w, vertex.attrib[2], R2;
MUL.F R3.xyz, R0.w, c[7];
MAX.F R1.w, R1, {0}.x;
POW.F R0.w, R2.w, c[14].x;
MOV.F R2.xyz, R0;
SLE.F R2.w, R1, {0}.x;
TRUNC.U.CC HC.x, R2.w;
MUL.F R0.xyz, R1.w, c[9];
IF    NE.x;
MOV.F R0.w, {0}.x;
ENDIF;
ADD.F R1.xyz, R1, c[10];
ADD.F R0.xyz, R2, R0;
MAD.F R0.xyz, R0, c[12], R1;
MAD.F R1.xyz, R0.w, c[9], R3;
MAD.F result.color.xyz, R1, c[13], R0;
MOV.F result.color.w, {1}.x;
END
# 58 instructions, 4 R-regs
