QuercusError = [(100,0.0183364938),(250,3.13760723e-06),(500,1.52372838e-13),(1000,5.62975334e-14),(1500,3.80565944e-14),(2000,2.92540466e-14),(2500,2.38289494e-14),(3000,1.97520086e-14),(4000,1.25309419e-14),(5000,1.0156124e-14),(7500,8.48391337e-17)]
LobataeError = [(100,0.82465946),(250,0.707749851),(500,0.429951756),(1000,0.0209956516),(1500,0.00438592857),(2000,0.000505979073),(2500,1.09176849e-05),(3000,1.05487862e-06),(4000,4.65118744e-09),(5000,2.98357497e-11),(7500,1.93269324e-14)]
RojosError = [(100,0.598899501),(250,0.279524646),(500,0.0696577808),(1000,0.00150937489),(1500,3.23460085e-06),(2000,1.39334857e-08),(2500,2.06079752e-11),(3000,4.07161628e-13),(4000,6.79540865e-15),(5000,4.97539037e-15),(7500,2.19590099e-15)]
CentroError = [(100,0.955987045),(250,0.983372333),(500,0.645558228),(1000,0.344222954),(1500,0.0856765322),(2000,0.0206137958),(2500,0.00196377247),(3000,0.00042270488),(4000,9.94193288e-06),(5000,2.78497712e-07),(7500,4.61774657e-10)]


n = len(QuercusError)

P =  line(QuercusError, color = "green",thickness=1,legend_label="Quercus",legend_color="green",ymin=0.0,ymax=1.0) 
P += line(LobataeError,color="blue",thickness=1,legend_label="Lobatae",legend_color="blue")
P += line(RojosError,color="red",thickness=1,legend_label="Rojos",legend_color="red")
P += line(CentroError,color="magenta",thickness=1,legend_label="Centro",legend_color="magenta")

P += sum([point2d(c,size=20,color="green") for c in QuercusError])
P += sum([point2d(c,size=20,color="blue") for c in LobataeError])
P += sum([point2d(c,size=20,color="red") for c in RojosError])
P += sum([point2d(c,size=20,color="magenta") for c in CentroError])

P.show(dpi=200,axes_labels=("$n$", "worst error"),fontsize=12,show_legend=True,axes_labels_size=1.5)
