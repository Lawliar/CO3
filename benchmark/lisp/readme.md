- `sudo apt install sbcl rlwrap`
- `rlwrap sbcl --load quicklisp.lisp`
- in the shell that pops up:
    - `(quicklisp-quickstart:install)`
    - `(ql:add-to-init-file)`
    - `(ql:quickload '(:alexandria :serapeum :xmls :ironclad))`
    - `(load <path to convert.lisp>)`
    - `(in-package cgc-pov-converter)`
    - `(let ((*cgc-repository-path* "<path to the cqe-challenge>"))
        (persist-converted-povs (convert-cgc-povs :on-error :continue) "bin2" t))`