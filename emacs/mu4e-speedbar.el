;;; mu4e-speedbar --- Speedbar support for mu4e

;; Copyright (C) 2012 Antono Vasiljev
;;
;; Author: Antono Vasiljev <self@antono.info>
;; Version: 0.1
;; Keywords: file, tags, tools
;;
;; This file is not part of GNU Emacs.
;;
;; This program is free software; you can redistribute it and/or modify
;; it under the terms of the GNU General Public License as published by
;; the Free Software Foundation; either version 3, or (at your option)
;; any later version.
;;
;; This program is distributed in the hope that it will be useful,
;; but WITHOUT ANY WARRANTY; without even the implied warranty of
;; MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
;; GNU General Public License for more details.
;;
;; You should have received a copy of the GNU General Public License
;; along with this program; if not, you can either send email to this
;; program's author (see below) or write to:
;;
;;              The Free Software Foundation, Inc.
;;              675 Mass Ave.
;;              Cambridge, MA 02139, USA.
;;
;; Please send bug reports, etc. to self@antono.info
;;

;;; Commentary:
;;
;; Speedbar provides a frame in which files, and locations in files
;; are displayed.  These functions provide mu4e specific support,
;; showing maildir list in the side-bar.
;;
;;   This file requires speedbar.

;;; Code:

(defvar mu4e-main-speedbar-key-map nil
  "Keymap used when in mu4e display mode.")

(defvar mu4e-main-speedbar-menu-items nil
  "Additional menu-items to add to speedbar frame.")

(defun mu4e-install-speedbar-variables ()
  "Install those variables used by speedbar to enhance mu4e."
  (if mu4e-main-speedbar-key-map
      nil
    (setq mu4e-main-speedbar-key-map (speedbar-make-specialized-keymap))
    (define-key mu4e-main-speedbar-key-map "RET" 'speedbar-edit-line)
    (define-key mu4e-main-speedbar-key-map "e" 'speedbar-edit-line)
    (define-key mu4e-main-speedbar-key-map "e" 'speedbar-edit-line)))

;; Make sure our special speedbar major mode is loaded
(if (featurep 'speedbar)
    (mu4e-install-speedbar-variables)
  (add-hook 'speedbar-load-hook 'mu4e-install-speedbar-variables))

(defun mu4e-render-maildir-list ()
  (interactive)
  (mapcar (lambda (maildir-name)
            (speedbar-insert-button maildir-name
                                    'speedbar-directory-face
                                    'highlight
                                    'mu4e-main-speedbar-find-file
                                    maildir-name))
          (mu4e-get-maildirs mu4e-maildir)))

(defun mu4e-main-speedbar-find-file (&optional text token ident)
  "Load in the mu4e file TEXT. TOKEN and INDENT are not used."
  (speedbar-with-attached-buffer
    (speedbar-message "Loading in MU4E maildir %s..." text)
    (mu4e-search (concat "\"maildir:" token "\""))))

;;;###autoload
(defun mu4e-main-speedbar-buttons (buffer)
  "Create buttons for any mu4e BUFFER."
  (interactive)
  (erase-buffer)
  (mu4e-render-maildir-list))

(provide 'mu4e-speedbar)
;;; mu4e-speedbar.el ends here
