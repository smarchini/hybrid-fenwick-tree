from collections import OrderedDict

# Data from: benchresults/rankselect/data/rankselect_nohugetlb/bitsize.csv
# Numbers to LaTeX regex: :s/\([0-9]\.?[0-9]*\)\([,\" ]\)/$\1$\2/g

lbls = "Elements,fixed[F],fixed[$\ell$],byte[F],byte[$\ell$],bit[F],bit[$\ell$],fixed[$16$]fixed,byte[$16$]byte,bit[$16$]bit,fixed[$16$]byte,fixed[$16$]bit,byte[$16$]bit,fixed[F]$8$,fixed[$\ell$]$8$,byte[F]$8$,byte[$\ell$]$8$,bit[F]$8$,bit[$\ell$]$8$,fixed[$16$]fixed$8$,byte[$16$]byte$8$,bit[$16$]bit$8$,fixed[$16$]byte$8$,fixed[$16$]bit$8$,byte[$16$]bit$8$,fixed[F]$16$,fixed[$\ell$]$16$,byte[F]$16$,byte[$\ell$]$16$,bit[F]$16$,bit[$\ell$]$16$,fixed[$16$]fixed$16$,byte[$16$]byte$16$,bit[$16$]bit$16$,fixed[$16$]byte$16$,fixed[$16$]bit$16$,byte[$16$]bit$16$,fixed[F]$32$,fixed[$\ell$]$32$,byte[F]$32$,byte[$\ell$]$32$,bit[F]$32$,bit[$\ell$]$32$,fixed[$16$]fixed$32$,byte[$16$]byte$32$,bit[$16$]bit$32$,fixed[$16$]byte$32$,fixed[$16$]bit$32$,byte[$16$]bit$32$,fixed[F]$64$,fixed[$\ell$]$64$,byte[F]$64$,byte[$\ell$]$64$,bit[F]$64$,bit[$\ell$]$64$,fixed[$16$]fixed$64$,byte[$16$]byte$64$,bit[$16$]bit$64$,fixed[$16$]byte$64$,fixed[$16$]bit$64$,byte[$16$]bit$64$,\\emph{Prezza}"

vals = [ 32_000_000_000,2.000000,2.000000,1.156982,1.156373,1.125000,1.125000,2.000073,1.157061,1.125095,1.157070,1.125105,1.125096,1.125000,1.125000,1.031982,1.031373,1.021484,1.021484,1.125009,1.031992,1.021496,1.031993,1.021497,1.021496,1.062500,1.062500,1.016357,1.015748,1.011719,1.011719,1.062505,1.016362,1.011725,1.016363,1.011725,1.011725,1.031250,1.031250,1.008545,1.007935,1.006348,1.006348,1.031252,1.008547,1.006351,1.008548,1.006351,1.006351,1.015625,1.015625,1.004639,1.004029,1.003418,1.003418,1.015626,1.004640,1.003419,1.004640,1.003420,1.003420,1.127441 ]


#if __name__ == '__main__':
mapped = dict(zip(lbls.split(',')[1:], vals[1:]))
ordered = OrderedDict(sorted(mapped.items(), key=lambda x: x[1]))

length = 7
keys, vals = list(ordered.keys()), [ "${:0.4f}$".format(i) for i in ordered.values() ]
for i in range(0, len(keys), length):
   print("\\begin{tabular}{" + "|x{2.35cm}"*len(keys[i:i+length])  + "|}")
   print("\\hline")
   print(" & ".join(keys[i:i+length]) + " \\\\")
   print("\\hline")
   print(" & ".join(vals[i:i+length]) + " \\\\")
   print("\\hline")
   print("\\end{tabular} \\vspace{0.2cm}\n")
