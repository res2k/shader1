simple_compiled.cg
!!NVfp4.0
# cgc version 2.2.0017, build date Jan 29 2010
# command line args: -profile gp4fp
# source file: simple_compiled.cg
#vendor NVIDIA Corporation
#version 2.2.0.17
#profile gp4fp
#program fragment_main_
#semantic fragment_main_.iModelViewProj_
#semantic fragment_main_.iLightColor_
#semantic fragment_main_.iLightDirObj_
#semantic fragment_main_.iTexture_
#var float2 v2f.m_TexCoord_B0_ : $vin.ATTR0 : ATTR0 : 0 : 1
#var float3 v2f.v_litColor1_cqa : $vin.ATTR1 : ATTR1 : 0 : 1
#var float4x4 iModelViewProj_ :  : , 4 : 1 : 0
#var float3 iLightColor_ :  :  : 2 : 0
#var float3 iLightDirObj_ :  :  : 3 : 0
#var sampler2D iTexture_ :  : texunit 0 : 4 : 1
#var float4 ooutColor_ : $vout.COLOR : COL : 5 : 1
ATTRIB fragment_attrib[] = { fragment.attrib[0..1] };
TEMP R0;
OUTPUT oCol = result.color;
TEX.F R0.xyz, fragment.attrib[0], texture[0], 2D;
MUL.F oCol.xyz, R0, fragment.attrib[1];
MOV.F oCol.w, {1}.x;
END
# 3 instructions, 1 R-regs
