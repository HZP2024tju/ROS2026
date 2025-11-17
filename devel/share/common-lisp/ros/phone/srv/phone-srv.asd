
(cl:in-package :asdf)

(defsystem "phone-srv"
  :depends-on (:roslisp-msg-protocol :roslisp-utils )
  :components ((:file "_package")
    (:file "link" :depends-on ("_package_link"))
    (:file "_package_link" :depends-on ("_package"))
  ))