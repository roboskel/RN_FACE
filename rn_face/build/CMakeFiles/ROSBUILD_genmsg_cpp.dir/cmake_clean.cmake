FILE(REMOVE_RECURSE
  "../msg_gen"
  "../src/rn_face/msg"
  "../msg_gen"
  "CMakeFiles/ROSBUILD_genmsg_cpp"
  "../msg_gen/cpp/include/rn_face/hcma.h"
  "../msg_gen/cpp/include/rn_face/head_cords_m.h"
  "../msg_gen/cpp/include/rn_face/head_cords_m_array.h"
  "../msg_gen/cpp/include/rn_face/hcm.h"
)

# Per-language clean rules from dependency scanning.
FOREACH(lang)
  INCLUDE(CMakeFiles/ROSBUILD_genmsg_cpp.dir/cmake_clean_${lang}.cmake OPTIONAL)
ENDFOREACH(lang)
