;; Auto-generated. Do not edit!


(when (boundp 'phone::person)
  (if (not (find-package "PHONE"))
    (make-package "PHONE"))
  (shadow 'person (find-package "PHONE")))
(unless (find-package "PHONE::PERSON")
  (make-package "PHONE::PERSON"))

(in-package "ROS")
;;//! \htmlinclude person.msg.html


(defclass phone::person
  :super ros::object
  :slots (_name _age _hight ))

(defmethod phone::person
  (:init
   (&key
    ((:name __name) "")
    ((:age __age) 0)
    ((:hight __hight) 0.0)
    )
   (send-super :init)
   (setq _name (string __name))
   (setq _age (round __age))
   (setq _hight (float __hight))
   self)
  (:name
   (&optional __name)
   (if __name (setq _name __name)) _name)
  (:age
   (&optional __age)
   (if __age (setq _age __age)) _age)
  (:hight
   (&optional __hight)
   (if __hight (setq _hight __hight)) _hight)
  (:serialization-length
   ()
   (+
    ;; string _name
    4 (length _name)
    ;; int32 _age
    4
    ;; float32 _hight
    4
    ))
  (:serialize
   (&optional strm)
   (let ((s (if strm strm
              (make-string-output-stream (send self :serialization-length)))))
     ;; string _name
       (write-long (length _name) s) (princ _name s)
     ;; int32 _age
       (write-long _age s)
     ;; float32 _hight
       (sys::poke _hight (send s :buffer) (send s :count) :float) (incf (stream-count s) 4)
     ;;
     (if (null strm) (get-output-stream-string s))))
  (:deserialize
   (buf &optional (ptr- 0))
   ;; string _name
     (let (n) (setq n (sys::peek buf ptr- :integer)) (incf ptr- 4) (setq _name (subseq buf ptr- (+ ptr- n))) (incf ptr- n))
   ;; int32 _age
     (setq _age (sys::peek buf ptr- :integer)) (incf ptr- 4)
   ;; float32 _hight
     (setq _hight (sys::peek buf ptr- :float)) (incf ptr- 4)
   ;;
   self)
  )

(setf (get phone::person :md5sum-) "56af71e4d5123f01ffc8a50934f9ea28")
(setf (get phone::person :datatype-) "phone/person")
(setf (get phone::person :definition-)
      "string name
int32 age
float32 hight

")



(provide :phone/person "56af71e4d5123f01ffc8a50934f9ea28")


