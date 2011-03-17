C5E2v_fragmentLighting.cg
!!NVvp4.0
# cgc version 2.2.0017, build date Jan 29 2010
# command line args: -profile gp4vp
# source file: C5E2v_fragmentLighting.cg
#vendor NVIDIA Corporation
#version 2.2.0.17
#profile gp4vp
#program C5E2v_fragmentLighting
#semantic C5E2v_fragmentLighting.modelViewProj
#var float4 position : $vin.POSITION : ATTR0 : 0 : 1
#var float3 normal : $vin.NORMAL : ATTR2 : 1 : 1
#var float4 oPosition : $vout.POSITION : HPOS : 2 : 1
#var float3 objectPos : $vout.TEXCOORD0 : TEX0 : 3 : 1
#var float3 oNormal : $vout.TEXCOORD1 : TEX1 : 4 : 1
#var float4x4 modelViewProj :  : c[0], 4 : 5 : 1
PARAM c[4] = { program.local[0..3] };
ATTRIB vertex_attrib[] = { vertex.attrib[0..2] };
OUTPUT result_texcoord[] = { result.texcoord[0..1] };
MOV.F result.texcoord[0].xyz, vertex.attrib[0];
MOV.F result.texcoord[1].xyz, vertex.attrib[2];
DP4.F result.position.w, vertex.attrib[0], c[3];
DP4.F result.position.z, vertex.attrib[0], c[2];
DP4.F result.position.y, vertex.attrib[0], c[1];
DP4.F result.position.x, vertex.attrib[0], c[0];
END
# 6 instructions, 0 R-regs
