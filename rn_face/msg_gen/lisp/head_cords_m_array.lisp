; Auto-generated. Do not edit!


(cl:in-package rn_face-msg)


;//! \htmlinclude head_cords_m_array.msg.html

(cl:defclass <head_cords_m_array> (roslisp-msg-protocol:ros-message)
  ((head_co
    :reader head_co
    :initarg :head_co
    :type (cl:vector rn_face-msg:head_cords_m)
   :initform (cl:make-array 0 :element-type 'rn_face-msg:head_cords_m :initial-element (cl:make-instance 'rn_face-msg:head_cords_m))))
)

(cl:defclass head_cords_m_array (<head_cords_m_array>)
  ())

(cl:defmethod cl:initialize-instance :after ((m <head_cords_m_array>) cl:&rest args)
  (cl:declare (cl:ignorable args))
  (cl:unless (cl:typep m 'head_cords_m_array)
    (roslisp-msg-protocol:msg-deprecation-warning "using old message class name rn_face-msg:<head_cords_m_array> is deprecated: use rn_face-msg:head_cords_m_array instead.")))

(cl:ensure-generic-function 'head_co-val :lambda-list '(m))
(cl:defmethod head_co-val ((m <head_cords_m_array>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader rn_face-msg:head_co-val is deprecated.  Use rn_face-msg:head_co instead.")
  (head_co m))
(cl:defmethod roslisp-msg-protocol:serialize ((msg <head_cords_m_array>) ostream)
  "Serializes a message object of type '<head_cords_m_array>"
  (cl:let ((__ros_arr_len (cl:length (cl:slot-value msg 'head_co))))
    (cl:write-byte (cl:ldb (cl:byte 8 0) __ros_arr_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) __ros_arr_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) __ros_arr_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) __ros_arr_len) ostream))
  (cl:map cl:nil #'(cl:lambda (ele) (roslisp-msg-protocol:serialize ele ostream))
   (cl:slot-value msg 'head_co))
)
(cl:defmethod roslisp-msg-protocol:deserialize ((msg <head_cords_m_array>) istream)
  "Deserializes a message object of type '<head_cords_m_array>"
  (cl:let ((__ros_arr_len 0))
    (cl:setf (cl:ldb (cl:byte 8 0) __ros_arr_len) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 8) __ros_arr_len) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 16) __ros_arr_len) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 24) __ros_arr_len) (cl:read-byte istream))
  (cl:setf (cl:slot-value msg 'head_co) (cl:make-array __ros_arr_len))
  (cl:let ((vals (cl:slot-value msg 'head_co)))
    (cl:dotimes (i __ros_arr_len)
    (cl:setf (cl:aref vals i) (cl:make-instance 'rn_face-msg:head_cords_m))
  (roslisp-msg-protocol:deserialize (cl:aref vals i) istream))))
  msg
)
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql '<head_cords_m_array>)))
  "Returns string type for a message object of type '<head_cords_m_array>"
  "rn_face/head_cords_m_array")
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql 'head_cords_m_array)))
  "Returns string type for a message object of type 'head_cords_m_array"
  "rn_face/head_cords_m_array")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql '<head_cords_m_array>)))
  "Returns md5sum for a message object of type '<head_cords_m_array>"
  "389fffe2ce004da6b2e2a56f91e2a9c5")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql 'head_cords_m_array)))
  "Returns md5sum for a message object of type 'head_cords_m_array"
  "389fffe2ce004da6b2e2a56f91e2a9c5")
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql '<head_cords_m_array>)))
  "Returns full string definition for message of type '<head_cords_m_array>"
  (cl:format cl:nil "head_cords_m[] head_co~%~%================================================================================~%MSG: rn_face/head_cords_m~%int64 timestamp~%int64 id~%float64 x~%float64 y~%float64 z~%~%~%"))
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql 'head_cords_m_array)))
  "Returns full string definition for message of type 'head_cords_m_array"
  (cl:format cl:nil "head_cords_m[] head_co~%~%================================================================================~%MSG: rn_face/head_cords_m~%int64 timestamp~%int64 id~%float64 x~%float64 y~%float64 z~%~%~%"))
(cl:defmethod roslisp-msg-protocol:serialization-length ((msg <head_cords_m_array>))
  (cl:+ 0
     4 (cl:reduce #'cl:+ (cl:slot-value msg 'head_co) :key #'(cl:lambda (ele) (cl:declare (cl:ignorable ele)) (cl:+ (roslisp-msg-protocol:serialization-length ele))))
))
(cl:defmethod roslisp-msg-protocol:ros-message-to-list ((msg <head_cords_m_array>))
  "Converts a ROS message object to a list"
  (cl:list 'head_cords_m_array
    (cl:cons ':head_co (head_co msg))
))
