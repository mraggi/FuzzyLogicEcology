QuercusAvgError = [(100,0.000170310471),(250,5.82218803e-09),(500,5.85567737e-16),(1000,3.58075259e-16),(1500,2.89160805e-16),(2000,2.33095278e-16),(2500,1.9219866e-16),(3000,1.48427515e-16),(4000,1.05757169e-16),(5000,7.66403157e-17),(7500,4.09857807e-19)]
LobataeAvgError = [(100,0.00533598546),(250,0.00273635904),(500,0.000702631448),(1000,6.70140599e-05),(1500,5.54008968e-06),(2000,4.09773628e-07),(2500,1.65433923e-08),(3000,9.18124661e-10),(4000,5.13265231e-12),(5000,1.95941977e-14),(7500,4.66156092e-17)]
RojosAvgError = [(100,0.00518326023),(250,0.00141242646),(500,0.000236230945),(1000,1.92148504e-06),(1500,5.68403265e-09),(2000,2.65164194e-11),(2500,1.135032e-13),(3000,6.83129652e-16),(4000,6.38807562e-17),(5000,4.37265568e-17),(7500,9.41065802e-18)]
CentroAvgError = [(100,0.0098384106),(250,0.00958371488),(500,0.00449586586),(1000,0.00105538794),(1500,0.000234695054),(2000,3.40054823e-05),(2500,5.20072001e-06),(3000,8.05824932e-07),(4000,2.07356679e-08),(5000,5.96308206e-10),(7500,1.73209377e-13)]

n = len(QuercusAvgError)

P =  line(QuercusAvgError, color = "green",thickness=1,legend_label="Quercus",legend_color="green",ymin=0.0,ymax=0.013) 
P += line(LobataeAvgError,color="blue",thickness=1,legend_label="Lobatae",legend_color="blue")
P += line(RojosAvgError,color="red",thickness=1,legend_label="Rojos",legend_color="red")
P += line(CentroAvgError,color="magenta",thickness=1,legend_label="Centro",legend_color="magenta")

P += sum([point2d(c,size=20,color="green") for c in QuercusAvgError])
P += sum([point2d(c,size=20,color="blue") for c in LobataeAvgError])
P += sum([point2d(c,size=20,color="red") for c in RojosAvgError])
P += sum([point2d(c,size=20,color="magenta") for c in CentroAvgError])

P.show(dpi=200,axes_labels=("$n$", "average error"),fontsize=12,show_legend=True,axes_labels_size=1.5)
