twoLights_compiled.cg
!!NVvp4.0
# cgc version 2.2.0017, build date Jan 29 2010
# command line args: -profile gp4vp
# source file: twoLights_compiled.cg
#vendor NVIDIA Corporation
#version 2.2.0.17
#profile gp4vp
#program vertex_main_
#semantic vertex_main_.imodelViewProj_
#semantic vertex_main_.ieyePosition_
#semantic vertex_main_.iglobalAmbient_
#semantic vertex_main_.ilights_position_
#semantic vertex_main_.ilights_color_
#semantic vertex_main_.imaterial_Ke_
#semantic vertex_main_.imaterial_Ka_
#semantic vertex_main_.imaterial_Kd_
#semantic vertex_main_.imaterial_Ks_
#semantic vertex_main_.imaterial_shininess_
#var float4 v2f.m_position_B0_ : $vout.ATTR0 : ATTR0 : 0 : 1
#var float3 v2f.m_normal_B0_ : $vout.ATTR1 : ATTR1 : 0 : 1
#var float4 iposition_ : $vin.ATTR0 : ATTR0 : 1 : 1
#var float3 inormal_ : $vin.ATTR1 : ATTR1 : 2 : 1
#var float4x4 imodelViewProj_ :  : c[0], 4 : 3 : 1
#var float3 ieyePosition_ :  :  : 4 : 0
#var float3 iglobalAmbient_ :  :  : 5 : 0
#var float3 ilights_position_[0] :  :  : 6 : 0
#var float3 ilights_position_[1] :  :  : 6 : 0
#var float3 ilights_color_[0] :  :  : 7 : 0
#var float3 ilights_color_[1] :  :  : 7 : 0
#var float3 imaterial_Ke_ :  :  : 8 : 0
#var float3 imaterial_Ka_ :  :  : 9 : 0
#var float3 imaterial_Kd_ :  :  : 10 : 0
#var float3 imaterial_Ks_ :  :  : 11 : 0
#var float imaterial_shininess_ :  :  : 12 : 0
#var float4 ooPosition_ : $vout.POSITION : HPOS : 13 : 1
PARAM c[4] = { program.local[0..3] };
ATTRIB vertex_attrib[] = { vertex.attrib[0..1] };
OUTPUT result_attrib[] = { result.attrib[0..1] };
MOV.F result.attrib[0], vertex.attrib[0];
MOV.F result.attrib[1].xyz, vertex.attrib[1];
DP4.F result.position.w, vertex.attrib[0], c[3];
DP4.F result.position.z, vertex.attrib[0], c[2];
DP4.F result.position.y, vertex.attrib[0], c[1];
DP4.F result.position.x, vertex.attrib[0], c[0];
END
# 6 instructions, 0 R-regs
