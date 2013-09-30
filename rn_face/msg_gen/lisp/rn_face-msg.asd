
(cl:in-package :asdf)

(defsystem "rn_face-msg"
  :depends-on (:roslisp-msg-protocol :roslisp-utils )
  :components ((:file "_package")
    (:file "hcma" :depends-on ("_package_hcma"))
    (:file "_package_hcma" :depends-on ("_package"))
    (:file "head_cords_m" :depends-on ("_package_head_cords_m"))
    (:file "_package_head_cords_m" :depends-on ("_package"))
    (:file "head_cords_m_array" :depends-on ("_package_head_cords_m_array"))
    (:file "_package_head_cords_m_array" :depends-on ("_package"))
    (:file "hcm" :depends-on ("_package_hcm"))
    (:file "_package_hcm" :depends-on ("_package"))
  ))