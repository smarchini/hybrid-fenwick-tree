(TeX-add-style-hook
 "alltogether"
 (lambda ()
   (TeX-add-to-alist 'LaTeX-provided-package-options
                     '(("geometry" "paperwidth=30cm" "paperheight=25cm" "left=5mm" "top=5mm" "bottom=5mm" "right=5mm" "margin=5mm")))
   (TeX-run-style-hooks
    "latex2e"
    "article"
    "art10"
    "geometry"
    "graphicx"))
 :latex)

