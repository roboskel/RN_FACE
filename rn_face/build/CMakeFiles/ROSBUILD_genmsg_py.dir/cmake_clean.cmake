FILE(REMOVE_RECURSE
  "../msg_gen"
  "../src/rn_face/msg"
  "../msg_gen"
  "CMakeFiles/ROSBUILD_genmsg_py"
  "../src/rn_face/msg/__init__.py"
  "../src/rn_face/msg/_hcma.py"
  "../src/rn_face/msg/_head_cords_m.py"
  "../src/rn_face/msg/_head_cords_m_array.py"
  "../src/rn_face/msg/_hcm.py"
)

# Per-language clean rules from dependency scanning.
FOREACH(lang)
  INCLUDE(CMakeFiles/ROSBUILD_genmsg_py.dir/cmake_clean_${lang}.cmake OPTIONAL)
ENDFOREACH(lang)
