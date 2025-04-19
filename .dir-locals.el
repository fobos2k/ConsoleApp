((nil . ((lsp-clients-clangd-compile-commands-dir . "build")
         (clang-format-style . "file")
         (c-basic-offset . 4)
         (tab-width . 4)
         (indent-tabs-mode . nil)))

 (c++-mode . ((eval . (progn
                        (message "🔥 .dir-locals.el: adding before-save-hook (C++)")
                        (add-hook 'before-save-hook
                                  (lambda ()
                                    (message "🔥 formatting via clang-format-buffer")
                                    (clang-format-buffer))
                                  nil t)))))

 (c-mode . ((eval . (progn
                      (message "🔥 .dir-locals.el: adding before-save-hook (C)")
                      (add-hook 'before-save-hook
                                (lambda ()
                                  (message "🔥 formatting via clang-format-buffer")
                                  (clang-format-buffer))
                                nil t)))))
)
