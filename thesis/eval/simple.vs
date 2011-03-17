simple.cg
!!NVvp4.0
# cgc version 2.2.0017, build date Jan 29 2010
# command line args: -profile gp4vp
# source file: simple.cg
#vendor NVIDIA Corporation
#version 2.2.0.17
#profile gp4vp
#program vertex_main
#semantic vertex_main.ModelViewProj
#semantic vertex_main.LightColor
#semantic vertex_main.LightDirObj
#var float4 Position : $vin.ATTR0 : ATTR0 : 0 : 1
#var float2 TexCoord : $vin.ATTR1 : ATTR1 : 1 : 1
#var float3 Normal : $vin.ATTR2 : ATTR2 : 2 : 1
#var float4x4 ModelViewProj :  : c[0], 4 : 3 : 1
#var float3 LightColor :  : c[4] : 4 : 1
#var float3 LightDirObj :  : c[5] : 5 : 1
#var float4 output.Position : $vout.POSITION : HPOS : 6 : 1
#var float2 output.TexCoord : $vout.ATTR0 : ATTR0 : 6 : 1
#var float3 output.litColor : $vout.ATTR1 : ATTR1 : 6 : 1
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
