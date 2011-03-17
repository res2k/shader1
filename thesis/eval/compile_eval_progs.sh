#!/bin/sh
S1C=../../../s1-build/apps/s1c/s1c

compile_program()
{
  cgvp_source=$1
  shift
  cgvp_entry=$1
  shift
  cgfp_source=$1
  shift
  cgfp_entry=$1
  shift
  programName=$1
  shift
  compilerArgs=$*
  
  cgc ${cgvp_source}.cg -entry $cgvp_entry -profile gp4vp > ${programName}.vs
  cgc ${cgfp_source}.cg -entry $cgfp_entry -profile gp4fp > ${programName}.fs
  
  $S1C $programName.s1 $compilerArgs > ${programName}_compiled.cg
  
  cgc ${programName}_compiled.cg -entry vertex_main_ -profile gp4vp > ${programName}_compiled.vs
  cgc ${programName}_compiled.cg -entry fragment_main_ -profile gp4fp > ${programName}_compiled.fs
}

compile_program simple vertex_main simple pixel_main simple \
  --param-vertex Position \
  --param-vertex TexCoord \
  --param-vertex Normal \
  --param-uniform LightColor \
  --param-uniform LightDirObj \
  $*

compile_program C5E1v_basicLight C5E1v_basicLight C2E2f_passthru C2E2f_passthru basicLight \
  --param-vertex position \
  --param-vertex normal \
  --param-uniform globalAmbient \
  --param-uniform lightColor \
  --param-uniform lightPosition \
  --param-uniform eyePosition \
  --param-uniform Ke \
  --param-uniform Ka \
  --param-uniform Kd \
  --param-uniform Ks \
  --param-uniform shininess \
  $*

compile_program C5E2v_fragmentLighting C5E2v_fragmentLighting C5E3f_basicLight C5E3f_basicLight fragmentLighting \
  --param-vertex position \
  --param-vertex normal \
  --param-uniform globalAmbient \
  --param-uniform lightColor \
  --param-uniform lightPosition \
  --param-uniform eyePosition \
  --param-uniform Ke \
  --param-uniform Ka \
  --param-uniform Kd \
  --param-uniform Ks \
  --param-uniform shininess \
  $*

compile_program C5E4v_twoLights C5E4v_twoLights C2E2f_passthru C2E2f_passthru twoLights \
  --param-vertex position \
  --param-vertex normal \
  --param-uniform globalAmbient \
  --param-uniform lights_color \
  --param-uniform lights_position \
  --param-uniform eyePosition \
  --param-uniform material_Ke \
  --param-uniform material_Ka \
  --param-uniform material_Kd \
  --param-uniform material_Ks \
  --param-uniform material_shininess \
  --param-size lights_position 2 \
  --param-size lights_color 2 \
  $*

