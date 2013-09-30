; Auto-generated. Do not edit!


(cl:in-package rn_face-msg)


;//! \htmlinclude hcma.msg.html

(cl:defclass <hcma> (roslisp-msg-protocol:ros-message)
  ((head_co
    :reader head_co
    :initarg :head_co
    :type (cl:vector rn_face-msg:hcm)
   :initform (cl:make-array 0 :element-type 'rn_face-msg:hcm :initial-element (cl:make-instance 'rn_face-msg:hcm))))
)

(cl:defclass hcma (<hcma>)
  ())

(cl:defmethod cl:initialize-instance :after ((m <hcma>) cl:&rest args)
  (cl:declare (cl:ignorable args))
  (cl:unless (cl:typep m 'hcma)
    (roslisp-msg-protocol:msg-deprecation-warning "using old message class name rn_face-msg:<hcma> is deprecated: use rn_face-msg:hcma instead.")))

(cl:ensure-generic-function 'head_co-val :lambda-list '(m))
(cl:defmethod head_co-val ((m <hcma>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader rn_face-msg:head_co-val is deprecated.  Use rn_face-msg:head_co instead.")
  (head_co m))
(cl:defmethod roslisp-msg-protocol:serialize ((msg <hcma>) ostream)
  "Serializes a message object of type '<hcma>"
  (cl:let ((__ros_arr_len (cl:length (cl:slot-value msg 'head_co))))
    (cl:write-byte (cl:ldb (cl:byte 8 0) __ros_arr_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) __ros_arr_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) __ros_arr_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) __ros_arr_len) ostream))
  (cl:map cl:nil #'(cl:lambda (ele) (roslisp-msg-protocol:serialize ele ostream))
   (cl:slot-value msg 'head_co))
)
(cl:defmethod roslisp-msg-protocol:deserialize ((msg <hcma>) istream)
  "Deserializes a message object of type '<hcma>"
  (cl:let ((__ros_arr_len 0))
    (cl:setf (cl:ldb (cl:byte 8 0) __ros_arr_len) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 8) __ros_arr_len) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 16) __ros_arr_len) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 24) __ros_arr_len) (cl:read-byte istream))
  (cl:setf (cl:slot-value msg 'head_co) (cl:make-array __ros_arr_len))
  (cl:let ((vals (cl:slot-value msg 'head_co)))
    (cl:dotimes (i __ros_arr_len)
    (cl:setf (cl:aref vals i) (cl:make-instance 'rn_face-msg:hcm))
  (roslisp-msg-protocol:deserialize (cl:aref vals i) istream))))
  msg
)
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql '<hcma>)))
  "Returns string type for a message object of type '<hcma>"
  "rn_face/hcma")
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql 'hcma)))
  "Returns string type for a message object of type 'hcma"
  "rn_face/hcma")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql '<hcma>)))
  "Returns md5sum for a message object of type '<hcma>"
  "5965750697474fb92efa297d80e9d602")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql 'hcma)))
  "Returns md5sum for a message object of type 'hcma"
  "5965750697474fb92efa297d80e9d602")
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql '<hcma>)))
  "Returns full string definition for message of type '<hcma>"
  (cl:format cl:nil "hcm[] head_co~%~%================================================================================~%MSG: rn_face/hcm~%int64 timestamp~%int64 id~%int8[10] x~%int8[10] y~%int8[10] z~%~%~%"))
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql 'hcma)))
  "Returns full string definition for message of type 'hcma"
  (cl:format cl:nil "hcm[] head_co~%~%================================================================================~%MSG: rn_face/hcm~%int64 timestamp~%int64 id~%int8[10] x~%int8[10] y~%int8[10] z~%~%~%"))
(cl:defmethod roslisp-msg-protocol:serialization-length ((msg <hcma>))
  (cl:+ 0
     4 (cl:reduce #'cl:+ (cl:slot-value msg 'head_co) :key #'(cl:lambda (ele) (cl:declare (cl:ignorable ele)) (cl:+ (roslisp-msg-protocol:serialization-length ele))))
))
(cl:defmethod roslisp-msg-protocol:ros-message-to-list ((msg <hcma>))
  "Converts a ROS message object to a list"
  (cl:list 'hcma
    (cl:cons ':head_co (head_co msg))
))
