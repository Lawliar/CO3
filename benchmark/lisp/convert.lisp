;;;; CGC PoV converter

;; Unfortunately, this tool is a bit hard to use without prior Lisp experience.
;; In this comment, I'll try to summarize how to get it to run on a fresh Ubuntu
;; installation; the steps will be similar on other systems:
;;
;; 1. Use the package manager to install SBCL, an implementation of Common Lisp:
;;    sudo apt install sbcl rlwrap. The second tool isn't really necessary but
;;    gives you a history and the ability to move around in the Lisp shell.
;;
;; 2. Get Quicklisp, the Lisp package manager. We use it to download
;;    dependencies, like pip in the Python world. The gray box at
;;    https://www.quicklisp.org/ contains all the instructions; it boils down to
;;    downloading a ".lisp" file, running "rlwrap sbcl --load
;;    the-downloaded-file.lisp" (which will drop you in a Lisp shell), and then
;;    calling the two functions "(quicklisp-quickstart:install)" and
;;    "(ql:add-to-init-file)". The latter is required so that Quicklisp is
;;    available every time you start SBCL.
;;
;; 3. Then, in the same Lisp shell or a fresh one, install the dependencies that
;;    we need and run the tool:
;;
;;    - "(ql:quickload '(:alexandria :serapeum :xmls :ironclad))" will download
;;      the dependencies the first time and load them from cache afterwards.
;;
;;    - "(in-package cgc-pov-converter)" sets your default namespace so that you
;;      don't have to prefix every function name.
;;
;;    - Finally, this snippet performs the actual conversion:
;;
;;        (let ((*cgc-repository-path* "samples/cqe-challenges"))
;;          (persist-converted-povs (convert-cgc-povs :on-error :continue) "bin2" t))
;;
;;      Replace the path with the path to
;;      https://github.com/CyberGrandChallenge/samples/tree/master/cqe-challenges
;;      on your system. The function will just return "NIL", but you should now
;;      have a file with the extension "bin2" next to each PoV XML file, e.g.,
;;      "cqe-challenges/CROMU_00001/pov/POV_00000.xml.bin2".

(defpackage cgc-pov-converter
  (:use :cl :alexandria :serapeum :xmls)
  (:export #:convert-cgc-povs
           #:persist-converted-povs
           #:all-povs))

(in-package cgc-pov-converter)

;;; Converting PoVs

(defun read-xml (filename)
  "Return the parsed XML contents."
  (with-open-file (f filename)
    (parse f :compress-whitespace nil)))

(defparameter *cgc-repository-path*
  "/path/to/cgc/samples/cqe-challenges/")

(defparameter *cgc-string-escapes*
  '((#\n . #\Newline) (#\r . #\Return) (#\t . #\Tab) (#\\ . #\\)))

(defun handle-escape-sequences (s)
  "Return an array of bytes corresponding to the content of S with escape
sequences replaced."
  (flet ((parse-one (offset)
           (cond
             ;; Hex escape
             ((string^= "\\x" s :start2 offset)
              (values (+ offset 4)
                      (parse-integer s :radix 16 :start (+ offset 2) :end (+ offset 4))))
             ;; Simple escape
             ((char= #\\ (elt s offset))
              (let* ((escape-char (elt s (1+ offset)))
                     (replacement (assocdr escape-char *cgc-string-escapes*)))
                (unless replacement
                  (error "Unknown escape character ~a." escape-char))
                (values (+ offset 2) (char-code replacement))))
             ;; Regular character
             (t (values (1+ offset) (char-code (elt s offset)))))))
    (loop with offset = 0
          and result = (make-array (length s) :element-type '(unsigned-byte 8) :fill-pointer 0)
          while (< offset (length s))
          do (multiple-value-bind (new-offset byte) (parse-one offset)
               (when byte (vector-push byte result))
               (setq offset new-offset))
          finally (return result))))

(defun handle-data (elt)
  "Return the data of ELT, handling the various expressions for strings, e.g.,
hex-encoded data or escape sequences."
  (let ((text (xmlrep-string-child elt)))
    (string-ecase (xmlrep-attrib-value "format" elt "asciic")
      ("asciic" (handle-escape-sequences text))
      ("hex" (ironclad:hex-string-to-byte-array (remove-if #'whitespacep text))))))

(defun process-value (instruction vars)
  "Determine a value, either as part of a write instruction or in a variable
declaration."
  (loop for elt in (xmlrep-children instruction)
        unless (stringp elt)           ; ignore whitespace between data elements
          collecting (string-ecase (xmlrep-tag elt)
                       ("data" (handle-data elt))
                       ("var" (or (assocdr (xmlrep-string-child elt) vars :test #'equal)
                                  (error "Variable ~a is unknown."
                                         (xmlrep-string-child elt)))))
            into output
        finally (return (apply (curry #'concatenate 'vector) output))))

(defun process-decl (instruction vars)
  "Process a declaration instruction, returning a CONS of the variable name and
its value."
  (let ((name (xmlrep-string-child (xmlrep-find-child-tag "var" instruction)))
        (value (process-value (xmlrep-find-child-tag "value" instruction) vars)))
    (cons name value)))

(defun parse-replay (xml)
  "Parse the replay encoded in the given XML and return the data sent to the
target application."
  (assert (xmlrep-tagmatch "pov" xml) (xml)
          "The XML root element should be <pov>, but it is <~a>." (xmlrep-tag xml))
  (loop for instruction in (xmlrep-children (xmlrep-find-child-tag "replay" xml))
        when (xmlrep-tagmatch "decl" instruction)
          collect (process-decl instruction vars) into vars
        when (xmlrep-tagmatch "write" instruction)
          collect (process-value instruction vars) into output
        finally (return (apply (curry #'concatenate 'vector) output))))

(defun all-povs ()
  "Return a list of all PoV files."
  (remove-if-not (op (in _ "xml" "povxml"))
                 (uiop:directory-files (path-join *cgc-repository-path* "**/pov/"))
                 :key #'pathname-type))

(defun convert-cgc-povs (&key (on-error :error))
  "Iterate through cgc-repository-path and convert all XML files that look like
PoVs. The result is a hash table associating file names with the corresponding
PoV data (as array of bytes). Errors are signaled with the option to continue;
if ON-ERROR is :CONTINUE, errors are ignored."
  (handler-bind ((error (lambda (e)
                          (declare (ignore e))
                          (when (and (eq on-error :continue)
                                     (find-restart 'ignore-pov))
                            (invoke-restart 'ignore-pov)))))
    (loop for file in (all-povs)
          with result = (make-hash-table :test 'equal)
          do (restart-case (setf (@ result file)
                                 (parse-replay (read-xml file)))
               (ignore-pov ()
                 :report (lambda (s) (format s "Ignore ~a." file))
                 nil))
          finally (return result))))

;;; Utilities

(defun persist-converted-povs (povs &optional (extension "bin2") supersede)
  "Write converted PoVs to files with the same name as the PoV XML but EXTENSION
appended. If SUPERSEDE is T then overwrite the target files if they exist."
  (loop for file being the hash-keys of povs using (hash-value data)
        do (let ((destination (format nil "~a.~a" (namestring file) extension)))
             (write-byte-vector-into-file (coerce data '(vector (unsigned-byte 8)))
                                          destination
                                          :if-exists (if supersede :supersede :error)))))
