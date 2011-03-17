C2E2f_passthru.cg
!!NVfp4.0
# cgc version 2.2.0017, build date Jan 29 2010
# command line args: -profile gp4fp
# source file: C2E2f_passthru.cg
#vendor NVIDIA Corporation
#version 2.2.0.17
#profile gp4fp
#program C2E2f_passthru
#var float4 color : $vin.COLOR : COL0 : 0 : 1
#var float4 C2E2f_passthru.color : $vout.COLOR : COL : -1 : 1
OUTPUT oCol = result.color;
MOV.F oCol, fragment.color;
END
# 1 instructions, 0 R-regs
