;; Auto-generated. Do not edit!


(when (boundp 'phone::linux)
  (if (not (find-package "PHONE"))
    (make-package "PHONE"))
  (shadow 'linux (find-package "PHONE")))
(unless (find-package "PHONE::LINUX")
  (make-package "PHONE::LINUX"))
(unless (find-package "PHONE::LINUXREQUEST")
  (make-package "PHONE::LINUXREQUEST"))
(unless (find-package "PHONE::LINUXRESPONSE")
  (make-package "PHONE::LINUXRESPONSE"))

(in-package "ROS")





(defclass phone::linuxRequest
  :super ros::object
  :slots (_num1 _num2 ))

(defmethod phone::linuxRequest
  (:init
   (&key
    ((:num1 __num1) 0)
    ((:num2 __num2) 0)
    )
   (send-super :init)
   (setq _num1 (round __num1))
   (setq _num2 (round __num2))
   self)
  (:num1
   (&optional __num1)
   (if __num1 (setq _num1 __num1)) _num1)
  (:num2
   (&optional __num2)
   (if __num2 (setq _num2 __num2)) _num2)
  (:serialization-length
   ()
   (+
    ;; int32 _num1
    4
    ;; int32 _num2
    4
    ))
  (:serialize
   (&optional strm)
   (let ((s (if strm strm
              (make-string-output-stream (send self :serialization-length)))))
     ;; int32 _num1
       (write-long _num1 s)
     ;; int32 _num2
       (write-long _num2 s)
     ;;
     (if (null strm) (get-output-stream-string s))))
  (:deserialize
   (buf &optional (ptr- 0))
   ;; int32 _num1
     (setq _num1 (sys::peek buf ptr- :integer)) (incf ptr- 4)
   ;; int32 _num2
     (setq _num2 (sys::peek buf ptr- :integer)) (incf ptr- 4)
   ;;
   self)
  )

(defclass phone::linuxResponse
  :super ros::object
  :slots (_sum ))

(defmethod phone::linuxResponse
  (:init
   (&key
    ((:sum __sum) 0)
    )
   (send-super :init)
   (setq _sum (round __sum))
   self)
  (:sum
   (&optional __sum)
   (if __sum (setq _sum __sum)) _sum)
  (:serialization-length
   ()
   (+
    ;; int32 _sum
    4
    ))
  (:serialize
   (&optional strm)
   (let ((s (if strm strm
              (make-string-output-stream (send self :serialization-length)))))
     ;; int32 _sum
       (write-long _sum s)
     ;;
     (if (null strm) (get-output-stream-string s))))
  (:deserialize
   (buf &optional (ptr- 0))
   ;; int32 _sum
     (setq _sum (sys::peek buf ptr- :integer)) (incf ptr- 4)
   ;;
   self)
  )

(defclass phone::linux
  :super ros::object
  :slots ())

(setf (get phone::linux :md5sum-) "4781436a0c2affec8025955a6041e481")
(setf (get phone::linux :datatype-) "phone/linux")
(setf (get phone::linux :request) phone::linuxRequest)
(setf (get phone::linux :response) phone::linuxResponse)

(defmethod phone::linuxRequest
  (:response () (instance phone::linuxResponse :init)))

(setf (get phone::linuxRequest :md5sum-) "4781436a0c2affec8025955a6041e481")
(setf (get phone::linuxRequest :datatype-) "phone/linuxRequest")
(setf (get phone::linuxRequest :definition-)
      "
int32 num1
int32 num2

---

int32 sum
")

(setf (get phone::linuxResponse :md5sum-) "4781436a0c2affec8025955a6041e481")
(setf (get phone::linuxResponse :datatype-) "phone/linuxResponse")
(setf (get phone::linuxResponse :definition-)
      "
int32 num1
int32 num2

---

int32 sum
")



(provide :phone/linux "4781436a0c2affec8025955a6041e481")


