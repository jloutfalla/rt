(nil . ((c-mode . ((eval . (setq-local company-c-headers-path-user
                                       (list "."
                                             (concat (projectile-project-root)
                                                     "include/"))))
                   (eval . (setq-local flycheck-clang-include-path
                                       company-c-headers-path-user))
                   (eval . (setq-local company-clang-arguments
                                       (list (concat "-I"
                                                     (concat (projectile-project-root)
                                                             "include/")))))))))
