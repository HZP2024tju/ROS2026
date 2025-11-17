; Auto-generated. Do not edit!


(cl:in-package phone-srv)


;//! \htmlinclude linux-request.msg.html

(cl:defclass <linux-request> (roslisp-msg-protocol:ros-message)
  ((num1
    :reader num1
    :initarg :num1
    :type cl:integer
    :initform 0)
   (num2
    :reader num2
    :initarg :num2
    :type cl:integer
    :initform 0))
)

(cl:defclass linux-request (<linux-request>)
  ())

(cl:defmethod cl:initialize-instance :after ((m <linux-request>) cl:&rest args)
  (cl:declare (cl:ignorable args))
  (cl:unless (cl:typep m 'linux-request)
    (roslisp-msg-protocol:msg-deprecation-warning "using old message class name phone-srv:<linux-request> is deprecated: use phone-srv:linux-request instead.")))

(cl:ensure-generic-function 'num1-val :lambda-list '(m))
(cl:defmethod num1-val ((m <linux-request>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader phone-srv:num1-val is deprecated.  Use phone-srv:num1 instead.")
  (num1 m))

(cl:ensure-generic-function 'num2-val :lambda-list '(m))
(cl:defmethod num2-val ((m <linux-request>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader phone-srv:num2-val is deprecated.  Use phone-srv:num2 instead.")
  (num2 m))
(cl:defmethod roslisp-msg-protocol:serialize ((msg <linux-request>) ostream)
  "Serializes a message object of type '<linux-request>"
  (cl:let* ((signed (cl:slot-value msg 'num1)) (unsigned (cl:if (cl:< signed 0) (cl:+ signed 4294967296) signed)))
    (cl:write-byte (cl:ldb (cl:byte 8 0) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) unsigned) ostream)
    )
  (cl:let* ((signed (cl:slot-value msg 'num2)) (unsigned (cl:if (cl:< signed 0) (cl:+ signed 4294967296) signed)))
    (cl:write-byte (cl:ldb (cl:byte 8 0) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) unsigned) ostream)
    )
)
(cl:defmethod roslisp-msg-protocol:deserialize ((msg <linux-request>) istream)
  "Deserializes a message object of type '<linux-request>"
    (cl:let ((unsigned 0))
      (cl:setf (cl:ldb (cl:byte 8 0) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) unsigned) (cl:read-byte istream))
      (cl:setf (cl:slot-value msg 'num1) (cl:if (cl:< unsigned 2147483648) unsigned (cl:- unsigned 4294967296))))
    (cl:let ((unsigned 0))
      (cl:setf (cl:ldb (cl:byte 8 0) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) unsigned) (cl:read-byte istream))
      (cl:setf (cl:slot-value msg 'num2) (cl:if (cl:< unsigned 2147483648) unsigned (cl:- unsigned 4294967296))))
  msg
)
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql '<linux-request>)))
  "Returns string type for a service object of type '<linux-request>"
  "phone/linuxRequest")
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql 'linux-request)))
  "Returns string type for a service object of type 'linux-request"
  "phone/linuxRequest")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql '<linux-request>)))
  "Returns md5sum for a message object of type '<linux-request>"
  "4781436a0c2affec8025955a6041e481")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql 'linux-request)))
  "Returns md5sum for a message object of type 'linux-request"
  "4781436a0c2affec8025955a6041e481")
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql '<linux-request>)))
  "Returns full string definition for message of type '<linux-request>"
  (cl:format cl:nil "~%int32 num1~%int32 num2~%~%~%~%"))
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql 'linux-request)))
  "Returns full string definition for message of type 'linux-request"
  (cl:format cl:nil "~%int32 num1~%int32 num2~%~%~%~%"))
(cl:defmethod roslisp-msg-protocol:serialization-length ((msg <linux-request>))
  (cl:+ 0
     4
     4
))
(cl:defmethod roslisp-msg-protocol:ros-message-to-list ((msg <linux-request>))
  "Converts a ROS message object to a list"
  (cl:list 'linux-request
    (cl:cons ':num1 (num1 msg))
    (cl:cons ':num2 (num2 msg))
))
;//! \htmlinclude linux-response.msg.html

(cl:defclass <linux-response> (roslisp-msg-protocol:ros-message)
  ((sum
    :reader sum
    :initarg :sum
    :type cl:integer
    :initform 0))
)

(cl:defclass linux-response (<linux-response>)
  ())

(cl:defmethod cl:initialize-instance :after ((m <linux-response>) cl:&rest args)
  (cl:declare (cl:ignorable args))
  (cl:unless (cl:typep m 'linux-response)
    (roslisp-msg-protocol:msg-deprecation-warning "using old message class name phone-srv:<linux-response> is deprecated: use phone-srv:linux-response instead.")))

(cl:ensure-generic-function 'sum-val :lambda-list '(m))
(cl:defmethod sum-val ((m <linux-response>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader phone-srv:sum-val is deprecated.  Use phone-srv:sum instead.")
  (sum m))
(cl:defmethod roslisp-msg-protocol:serialize ((msg <linux-response>) ostream)
  "Serializes a message object of type '<linux-response>"
  (cl:let* ((signed (cl:slot-value msg 'sum)) (unsigned (cl:if (cl:< signed 0) (cl:+ signed 4294967296) signed)))
    (cl:write-byte (cl:ldb (cl:byte 8 0) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) unsigned) ostream)
    )
)
(cl:defmethod roslisp-msg-protocol:deserialize ((msg <linux-response>) istream)
  "Deserializes a message object of type '<linux-response>"
    (cl:let ((unsigned 0))
      (cl:setf (cl:ldb (cl:byte 8 0) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) unsigned) (cl:read-byte istream))
      (cl:setf (cl:slot-value msg 'sum) (cl:if (cl:< unsigned 2147483648) unsigned (cl:- unsigned 4294967296))))
  msg
)
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql '<linux-response>)))
  "Returns string type for a service object of type '<linux-response>"
  "phone/linuxResponse")
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql 'linux-response)))
  "Returns string type for a service object of type 'linux-response"
  "phone/linuxResponse")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql '<linux-response>)))
  "Returns md5sum for a message object of type '<linux-response>"
  "4781436a0c2affec8025955a6041e481")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql 'linux-response)))
  "Returns md5sum for a message object of type 'linux-response"
  "4781436a0c2affec8025955a6041e481")
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql '<linux-response>)))
  "Returns full string definition for message of type '<linux-response>"
  (cl:format cl:nil "~%int32 sum~%~%~%"))
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql 'linux-response)))
  "Returns full string definition for message of type 'linux-response"
  (cl:format cl:nil "~%int32 sum~%~%~%"))
(cl:defmethod roslisp-msg-protocol:serialization-length ((msg <linux-response>))
  (cl:+ 0
     4
))
(cl:defmethod roslisp-msg-protocol:ros-message-to-list ((msg <linux-response>))
  "Converts a ROS message object to a list"
  (cl:list 'linux-response
    (cl:cons ':sum (sum msg))
))
(cl:defmethod roslisp-msg-protocol:service-request-type ((msg (cl:eql 'linux)))
  'linux-request)
(cl:defmethod roslisp-msg-protocol:service-response-type ((msg (cl:eql 'linux)))
  'linux-response)
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql 'linux)))
  "Returns string type for a service object of type '<linux>"
  "phone/linux")