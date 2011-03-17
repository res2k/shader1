simple.cg
!!NVfp4.0
# cgc version 2.2.0017, build date Jan 29 2010
# command line args: -profile gp4fp
# source file: simple.cg
#vendor NVIDIA Corporation
#version 2.2.0.17
#profile gp4fp
#program pixel_main
#semantic pixel_main.Texture
#var float2 interpolatedVertexOutput.TexCoord : $vin.ATTR0 : ATTR0 : 0 : 1
#var float3 interpolatedVertexOutput.litColor : $vin.ATTR1 : ATTR1 : 0 : 1
#var sampler2D Texture :  : texunit 0 : 1 : 1
#var float4 outColor : $vout.COLOR : COL : 2 : 1
ATTRIB fragment_attrib[] = { fragment.attrib[0..1] };
TEMP R0;
OUTPUT oCol = result.color;
TEX.F R0.xyz, fragment.attrib[0], texture[0], 2D;
MUL.F oCol.xyz, R0, fragment.attrib[1];
MOV.F oCol.w, {1}.x;
END
# 3 instructions, 1 R-regs
