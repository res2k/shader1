basicLight_compiled.cg
!!NVvp4.0
# cgc version 2.2.0017, build date Jan 29 2010
# command line args: -profile gp4vp
# source file: basicLight_compiled.cg
#vendor NVIDIA Corporation
#version 2.2.0.17
#profile gp4vp
#program vertex_main_
#semantic vertex_main_.imodelViewProj_
#semantic vertex_main_.iglobalAmbient_
#semantic vertex_main_.ilightColor_
#semantic vertex_main_.ilightPosition_
#semantic vertex_main_.ieyePosition_
#semantic vertex_main_.iKe_
#semantic vertex_main_.iKa_
#semantic vertex_main_.iKd_
#semantic vertex_main_.iKs_
#semantic vertex_main_.ishininess_
#var float3 v2f.i_tmp7_ : $vout.ATTR0 : ATTR0 : 0 : 1
#var float3 v2f.v_N_ : $vout.ATTR1 : ATTR1 : 0 : 1
#var float3 v2f.i_tmp16_ : $vout.ATTR2 : ATTR2 : 0 : 1
#var float4 iposition_ : $vin.ATTR0 : ATTR0 : 1 : 1
#var float3 inormal_ : $vin.ATTR1 : ATTR1 : 2 : 1
#var float4x4 imodelViewProj_ :  : c[0], 4 : 3 : 1
#var float3 iglobalAmbient_ :  :  : 4 : 0
#var float3 ilightColor_ :  :  : 5 : 0
#var float3 ilightPosition_ :  : c[4] : 6 : 1
#var float3 ieyePosition_ :  : c[5] : 7 : 1
#var float3 iKe_ :  :  : 8 : 0
#var float3 iKa_ :  :  : 9 : 0
#var float3 iKd_ :  :  : 10 : 0
#var float3 iKs_ :  :  : 11 : 0
#var float ishininess_ :  :  : 12 : 0
#var float4 ooPosition_ : $vout.POSITION : HPOS : 13 : 1
PARAM c[6] = { program.local[0..5] };
ATTRIB vertex_attrib[] = { vertex.attrib[0..1] };
OUTPUT result_attrib[] = { result.attrib[0..2] };
MOV.F result.attrib[1].xyz, vertex.attrib[1];
ADD.F result.attrib[0].xyz, -vertex.attrib[0], c[4];
ADD.F result.attrib[2].xyz, -vertex.attrib[0], c[5];
DP4.F result.position.w, vertex.attrib[0], c[3];
DP4.F result.position.z, vertex.attrib[0], c[2];
DP4.F result.position.y, vertex.attrib[0], c[1];
DP4.F result.position.x, vertex.attrib[0], c[0];
END
# 7 instructions, 0 R-regs
