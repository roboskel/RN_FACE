; Auto-generated. Do not edit!


(cl:in-package rn_face-msg)


;//! \htmlinclude hcm.msg.html

(cl:defclass <hcm> (roslisp-msg-protocol:ros-message)
  ((timestamp
    :reader timestamp
    :initarg :timestamp
    :type cl:integer
    :initform 0)
   (id
    :reader id
    :initarg :id
    :type cl:integer
    :initform 0)
   (x
    :reader x
    :initarg :x
    :type (cl:vector cl:fixnum)
   :initform (cl:make-array 10 :element-type 'cl:fixnum :initial-element 0))
   (y
    :reader y
    :initarg :y
    :type (cl:vector cl:fixnum)
   :initform (cl:make-array 10 :element-type 'cl:fixnum :initial-element 0))
   (z
    :reader z
    :initarg :z
    :type (cl:vector cl:fixnum)
   :initform (cl:make-array 10 :element-type 'cl:fixnum :initial-element 0)))
)

(cl:defclass hcm (<hcm>)
  ())

(cl:defmethod cl:initialize-instance :after ((m <hcm>) cl:&rest args)
  (cl:declare (cl:ignorable args))
  (cl:unless (cl:typep m 'hcm)
    (roslisp-msg-protocol:msg-deprecation-warning "using old message class name rn_face-msg:<hcm> is deprecated: use rn_face-msg:hcm instead.")))

(cl:ensure-generic-function 'timestamp-val :lambda-list '(m))
(cl:defmethod timestamp-val ((m <hcm>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader rn_face-msg:timestamp-val is deprecated.  Use rn_face-msg:timestamp instead.")
  (timestamp m))

(cl:ensure-generic-function 'id-val :lambda-list '(m))
(cl:defmethod id-val ((m <hcm>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader rn_face-msg:id-val is deprecated.  Use rn_face-msg:id instead.")
  (id m))

(cl:ensure-generic-function 'x-val :lambda-list '(m))
(cl:defmethod x-val ((m <hcm>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader rn_face-msg:x-val is deprecated.  Use rn_face-msg:x instead.")
  (x m))

(cl:ensure-generic-function 'y-val :lambda-list '(m))
(cl:defmethod y-val ((m <hcm>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader rn_face-msg:y-val is deprecated.  Use rn_face-msg:y instead.")
  (y m))

(cl:ensure-generic-function 'z-val :lambda-list '(m))
(cl:defmethod z-val ((m <hcm>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader rn_face-msg:z-val is deprecated.  Use rn_face-msg:z instead.")
  (z m))
(cl:defmethod roslisp-msg-protocol:serialize ((msg <hcm>) ostream)
  "Serializes a message object of type '<hcm>"
  (cl:let* ((signed (cl:slot-value msg 'timestamp)) (unsigned (cl:if (cl:< signed 0) (cl:+ signed 18446744073709551616) signed)))
    (cl:write-byte (cl:ldb (cl:byte 8 0) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 32) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 40) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 48) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 56) unsigned) ostream)
    )
  (cl:let* ((signed (cl:slot-value msg 'id)) (unsigned (cl:if (cl:< signed 0) (cl:+ signed 18446744073709551616) signed)))
    (cl:write-byte (cl:ldb (cl:byte 8 0) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 32) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 40) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 48) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 56) unsigned) ostream)
    )
  (cl:map cl:nil #'(cl:lambda (ele) (cl:let* ((signed ele) (unsigned (cl:if (cl:< signed 0) (cl:+ signed 256) signed)))
    (cl:write-byte (cl:ldb (cl:byte 8 0) unsigned) ostream)
    ))
   (cl:slot-value msg 'x))
  (cl:map cl:nil #'(cl:lambda (ele) (cl:let* ((signed ele) (unsigned (cl:if (cl:< signed 0) (cl:+ signed 256) signed)))
    (cl:write-byte (cl:ldb (cl:byte 8 0) unsigned) ostream)
    ))
   (cl:slot-value msg 'y))
  (cl:map cl:nil #'(cl:lambda (ele) (cl:let* ((signed ele) (unsigned (cl:if (cl:< signed 0) (cl:+ signed 256) signed)))
    (cl:write-byte (cl:ldb (cl:byte 8 0) unsigned) ostream)
    ))
   (cl:slot-value msg 'z))
)
(cl:defmethod roslisp-msg-protocol:deserialize ((msg <hcm>) istream)
  "Deserializes a message object of type '<hcm>"
    (cl:let ((unsigned 0))
      (cl:setf (cl:ldb (cl:byte 8 0) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 32) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 40) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 48) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 56) unsigned) (cl:read-byte istream))
      (cl:setf (cl:slot-value msg 'timestamp) (cl:if (cl:< unsigned 9223372036854775808) unsigned (cl:- unsigned 18446744073709551616))))
    (cl:let ((unsigned 0))
      (cl:setf (cl:ldb (cl:byte 8 0) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 32) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 40) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 48) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 56) unsigned) (cl:read-byte istream))
      (cl:setf (cl:slot-value msg 'id) (cl:if (cl:< unsigned 9223372036854775808) unsigned (cl:- unsigned 18446744073709551616))))
  (cl:setf (cl:slot-value msg 'x) (cl:make-array 10))
  (cl:let ((vals (cl:slot-value msg 'x)))
    (cl:dotimes (i 10)
    (cl:let ((unsigned 0))
      (cl:setf (cl:ldb (cl:byte 8 0) unsigned) (cl:read-byte istream))
      (cl:setf (cl:aref vals i) (cl:if (cl:< unsigned 128) unsigned (cl:- unsigned 256))))))
  (cl:setf (cl:slot-value msg 'y) (cl:make-array 10))
  (cl:let ((vals (cl:slot-value msg 'y)))
    (cl:dotimes (i 10)
    (cl:let ((unsigned 0))
      (cl:setf (cl:ldb (cl:byte 8 0) unsigned) (cl:read-byte istream))
      (cl:setf (cl:aref vals i) (cl:if (cl:< unsigned 128) unsigned (cl:- unsigned 256))))))
  (cl:setf (cl:slot-value msg 'z) (cl:make-array 10))
  (cl:let ((vals (cl:slot-value msg 'z)))
    (cl:dotimes (i 10)
    (cl:let ((unsigned 0))
      (cl:setf (cl:ldb (cl:byte 8 0) unsigned) (cl:read-byte istream))
      (cl:setf (cl:aref vals i) (cl:if (cl:< unsigned 128) unsigned (cl:- unsigned 256))))))
  msg
)
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql '<hcm>)))
  "Returns string type for a message object of type '<hcm>"
  "rn_face/hcm")
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql 'hcm)))
  "Returns string type for a message object of type 'hcm"
  "rn_face/hcm")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql '<hcm>)))
  "Returns md5sum for a message object of type '<hcm>"
  "eba503a94c55b57a103896678be7b05a")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql 'hcm)))
  "Returns md5sum for a message object of type 'hcm"
  "eba503a94c55b57a103896678be7b05a")
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql '<hcm>)))
  "Returns full string definition for message of type '<hcm>"
  (cl:format cl:nil "int64 timestamp~%int64 id~%int8[10] x~%int8[10] y~%int8[10] z~%~%~%"))
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql 'hcm)))
  "Returns full string definition for message of type 'hcm"
  (cl:format cl:nil "int64 timestamp~%int64 id~%int8[10] x~%int8[10] y~%int8[10] z~%~%~%"))
(cl:defmethod roslisp-msg-protocol:serialization-length ((msg <hcm>))
  (cl:+ 0
     8
     8
     0 (cl:reduce #'cl:+ (cl:slot-value msg 'x) :key #'(cl:lambda (ele) (cl:declare (cl:ignorable ele)) (cl:+ 1)))
     0 (cl:reduce #'cl:+ (cl:slot-value msg 'y) :key #'(cl:lambda (ele) (cl:declare (cl:ignorable ele)) (cl:+ 1)))
     0 (cl:reduce #'cl:+ (cl:slot-value msg 'z) :key #'(cl:lambda (ele) (cl:declare (cl:ignorable ele)) (cl:+ 1)))
))
(cl:defmethod roslisp-msg-protocol:ros-message-to-list ((msg <hcm>))
  "Converts a ROS message object to a list"
  (cl:list 'hcm
    (cl:cons ':timestamp (timestamp msg))
    (cl:cons ':id (id msg))
    (cl:cons ':x (x msg))
    (cl:cons ':y (y msg))
    (cl:cons ':z (z msg))
))
