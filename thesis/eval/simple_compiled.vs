simple_compiled.cg
!!NVvp4.0
# cgc version 2.2.0017, build date Jan 29 2010
# command line args: -profile gp4vp
# source file: simple_compiled.cg
#vendor NVIDIA Corporation
#version 2.2.0.17
#profile gp4vp
#program vertex_main_
#semantic vertex_main_.iModelViewProj_
#semantic vertex_main_.iLightColor_
#semantic vertex_main_.iLightDirObj_
#var float2 v2f.m_TexCoord_B0_ : $vout.ATTR0 : ATTR0 : 0 : 1
#var float3 v2f.v_litColor1_cqa : $vout.ATTR1 : ATTR1 : 0 : 1
#var float4 iPosition_ : $vin.ATTR0 : ATTR0 : 1 : 1
#var float2 iTexCoord_ : $vin.ATTR1 : ATTR1 : 2 : 1
#var float3 iNormal_ : $vin.ATTR2 : ATTR2 : 3 : 1
#var float4x4 iModelViewProj_ :  : c[0], 4 : 4 : 1
#var float3 iLightColor_ :  : c[4] : 5 : 1
#var float3 iLightDirObj_ :  : c[5] : 6 : 1
#var float4 ooutPosition_ : $vout.POSITION : HPOS : 7 : 1
PARAM c[6] = { program.local[0..5] };
ATTRIB vertex_attrib[] = { vertex.attrib[0..2] };
OUTPUT result_attrib[] = { result.attrib[0..1] };
TEMP R0;
DP3.F R0.x, vertex.attrib[2], c[5];
MUL.F R0.xyz, R0.x, c[4];
ADD.F result.attrib[1].xyz, R0, {0.40000001}.x;
MOV.F result.attrib[0].xy, vertex.attrib[1];
DP4.F result.position.w, vertex.attrib[0], c[3];
DP4.F result.position.z, vertex.attrib[0], c[2];
DP4.F result.position.y, vertex.attrib[0], c[1];
DP4.F result.position.x, vertex.attrib[0], c[0];
END
# 8 instructions, 1 R-regs
