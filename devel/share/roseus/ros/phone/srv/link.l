;; Auto-generated. Do not edit!


(when (boundp 'phone::link)
  (if (not (find-package "PHONE"))
    (make-package "PHONE"))
  (shadow 'link (find-package "PHONE")))
(unless (find-package "PHONE::LINK")
  (make-package "PHONE::LINK"))
(unless (find-package "PHONE::LINKREQUEST")
  (make-package "PHONE::LINKREQUEST"))
(unless (find-package "PHONE::LINKRESPONSE")
  (make-package "PHONE::LINKRESPONSE"))

(in-package "ROS")





(defclass phone::linkRequest
  :super ros::object
  :slots (_num1 _num2 ))

(defmethod phone::linkRequest
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

(defclass phone::linkResponse
  :super ros::object
  :slots (_sum ))

(defmethod phone::linkResponse
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

(defclass phone::link
  :super ros::object
  :slots ())

(setf (get phone::link :md5sum-) "4781436a0c2affec8025955a6041e481")
(setf (get phone::link :datatype-) "phone/link")
(setf (get phone::link :request) phone::linkRequest)
(setf (get phone::link :response) phone::linkResponse)

(defmethod phone::linkRequest
  (:response () (instance phone::linkResponse :init)))

(setf (get phone::linkRequest :md5sum-) "4781436a0c2affec8025955a6041e481")
(setf (get phone::linkRequest :datatype-) "phone/linkRequest")
(setf (get phone::linkRequest :definition-)
      "
int32 num1
int32 num2

---
int32 sum
")

(setf (get phone::linkResponse :md5sum-) "4781436a0c2affec8025955a6041e481")
(setf (get phone::linkResponse :datatype-) "phone/linkResponse")
(setf (get phone::linkResponse :definition-)
      "
int32 num1
int32 num2

---
int32 sum
")



(provide :phone/link "4781436a0c2affec8025955a6041e481")


