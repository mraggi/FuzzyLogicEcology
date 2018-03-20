QuercusTime = [(100,0.00775),(500,0.0709),(1000,0.234),(2000,0.951),(2500,1.34),(3000,1.92),(4000,3.48),(5000,5.41),(7500,11.9),(10000,22.8)]
LobataeTime = [(100,0.0124),(500,0.109),(1000,0.547),(2000,1.72),(2500,2.64),(3000,3.74),(4000,6.42),(5000,10.1),(7500,22.6),(10000,39.9)]
RojosTime = [(100,0.00906),(500,0.0875),(1000,0.483),(2000,1.58),(2500,2.34),(3000,3.16),(4000,5.64),(5000,8.47),(7500,19.1),(10000,33.5)]
CentroTime = [(100,0.0216),(500,0.239),(1000,0.933),(2000,3.64),(2500,5.27),(3000,7.41),(4000,13),(5000,20.4),(7500,45.6),(10000,81)]


n = len(QuercusTime)

P =  line(QuercusTime, color = "green",thickness=1,legend_label="Quercus Time",legend_color="green") 
P += line(LobataeTime,color="blue",thickness=1,legend_label="Lobatae Time",legend_color="blue")
P += line(RojosTime,color="red",thickness=1,legend_label="Rojos Time",legend_color="red")
P += line(CentroTime,color="magenta",thickness=1,legend_label="Centro Time",legend_color="magenta")

P += sum([point2d(c,size=20,color="green") for c in QuercusTime])
P += sum([point2d(c,size=20,color="blue") for c in LobataeTime])
P += sum([point2d(c,size=20,color="red") for c in RojosTime])
P += sum([point2d(c,size=20,color="magenta") for c in CentroTime])

P.show(dpi=200,axes_labels=("$n$", "seconds taken"),fontsize=12,show_legend=True,axes_labels_size=1.5)
