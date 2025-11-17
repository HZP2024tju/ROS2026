; Auto-generated. Do not edit!


(cl:in-package phone-srv)


;//! \htmlinclude link-request.msg.html

(cl:defclass <link-request> (roslisp-msg-protocol:ros-message)
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

(cl:defclass link-request (<link-request>)
  ())

(cl:defmethod cl:initialize-instance :after ((m <link-request>) cl:&rest args)
  (cl:declare (cl:ignorable args))
  (cl:unless (cl:typep m 'link-request)
    (roslisp-msg-protocol:msg-deprecation-warning "using old message class name phone-srv:<link-request> is deprecated: use phone-srv:link-request instead.")))

(cl:ensure-generic-function 'num1-val :lambda-list '(m))
(cl:defmethod num1-val ((m <link-request>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader phone-srv:num1-val is deprecated.  Use phone-srv:num1 instead.")
  (num1 m))

(cl:ensure-generic-function 'num2-val :lambda-list '(m))
(cl:defmethod num2-val ((m <link-request>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader phone-srv:num2-val is deprecated.  Use phone-srv:num2 instead.")
  (num2 m))
(cl:defmethod roslisp-msg-protocol:serialize ((msg <link-request>) ostream)
  "Serializes a message object of type '<link-request>"
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
(cl:defmethod roslisp-msg-protocol:deserialize ((msg <link-request>) istream)
  "Deserializes a message object of type '<link-request>"
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
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql '<link-request>)))
  "Returns string type for a service object of type '<link-request>"
  "phone/linkRequest")
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql 'link-request)))
  "Returns string type for a service object of type 'link-request"
  "phone/linkRequest")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql '<link-request>)))
  "Returns md5sum for a message object of type '<link-request>"
  "4781436a0c2affec8025955a6041e481")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql 'link-request)))
  "Returns md5sum for a message object of type 'link-request"
  "4781436a0c2affec8025955a6041e481")
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql '<link-request>)))
  "Returns full string definition for message of type '<link-request>"
  (cl:format cl:nil "~%int32 num1~%int32 num2~%~%~%~%"))
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql 'link-request)))
  "Returns full string definition for message of type 'link-request"
  (cl:format cl:nil "~%int32 num1~%int32 num2~%~%~%~%"))
(cl:defmethod roslisp-msg-protocol:serialization-length ((msg <link-request>))
  (cl:+ 0
     4
     4
))
(cl:defmethod roslisp-msg-protocol:ros-message-to-list ((msg <link-request>))
  "Converts a ROS message object to a list"
  (cl:list 'link-request
    (cl:cons ':num1 (num1 msg))
    (cl:cons ':num2 (num2 msg))
))
;//! \htmlinclude link-response.msg.html

(cl:defclass <link-response> (roslisp-msg-protocol:ros-message)
  ((sum
    :reader sum
    :initarg :sum
    :type cl:integer
    :initform 0))
)

(cl:defclass link-response (<link-response>)
  ())

(cl:defmethod cl:initialize-instance :after ((m <link-response>) cl:&rest args)
  (cl:declare (cl:ignorable args))
  (cl:unless (cl:typep m 'link-response)
    (roslisp-msg-protocol:msg-deprecation-warning "using old message class name phone-srv:<link-response> is deprecated: use phone-srv:link-response instead.")))

(cl:ensure-generic-function 'sum-val :lambda-list '(m))
(cl:defmethod sum-val ((m <link-response>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader phone-srv:sum-val is deprecated.  Use phone-srv:sum instead.")
  (sum m))
(cl:defmethod roslisp-msg-protocol:serialize ((msg <link-response>) ostream)
  "Serializes a message object of type '<link-response>"
  (cl:let* ((signed (cl:slot-value msg 'sum)) (unsigned (cl:if (cl:< signed 0) (cl:+ signed 4294967296) signed)))
    (cl:write-byte (cl:ldb (cl:byte 8 0) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) unsigned) ostream)
    )
)
(cl:defmethod roslisp-msg-protocol:deserialize ((msg <link-response>) istream)
  "Deserializes a message object of type '<link-response>"
    (cl:let ((unsigned 0))
      (cl:setf (cl:ldb (cl:byte 8 0) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) unsigned) (cl:read-byte istream))
      (cl:setf (cl:slot-value msg 'sum) (cl:if (cl:< unsigned 2147483648) unsigned (cl:- unsigned 4294967296))))
  msg
)
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql '<link-response>)))
  "Returns string type for a service object of type '<link-response>"
  "phone/linkResponse")
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql 'link-response)))
  "Returns string type for a service object of type 'link-response"
  "phone/linkResponse")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql '<link-response>)))
  "Returns md5sum for a message object of type '<link-response>"
  "4781436a0c2affec8025955a6041e481")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql 'link-response)))
  "Returns md5sum for a message object of type 'link-response"
  "4781436a0c2affec8025955a6041e481")
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql '<link-response>)))
  "Returns full string definition for message of type '<link-response>"
  (cl:format cl:nil "int32 sum~%~%~%"))
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql 'link-response)))
  "Returns full string definition for message of type 'link-response"
  (cl:format cl:nil "int32 sum~%~%~%"))
(cl:defmethod roslisp-msg-protocol:serialization-length ((msg <link-response>))
  (cl:+ 0
     4
))
(cl:defmethod roslisp-msg-protocol:ros-message-to-list ((msg <link-response>))
  "Converts a ROS message object to a list"
  (cl:list 'link-response
    (cl:cons ':sum (sum msg))
))
(cl:defmethod roslisp-msg-protocol:service-request-type ((msg (cl:eql 'link)))
  'link-request)
(cl:defmethod roslisp-msg-protocol:service-response-type ((msg (cl:eql 'link)))
  'link-response)
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql 'link)))
  "Returns string type for a service object of type '<link>"
  "phone/link")