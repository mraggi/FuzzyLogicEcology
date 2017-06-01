#pragma once

#include "TimeHelpers.hpp"
#include "Mu.hpp"
#include "ReadFile.hpp"
#include "Point.hpp"

const real RadioTierraEnKm = 6371.0;
const real ValorDe1Grado = (2*pi*RadioTierraEnKm)/360.0;

class GraphCalculator
{
public:
	GraphCalculator(size_t _malla, real RangoVisibilidadEnKm, const vector<vector<Point>>& UEnGrados) : malla(_malla)
	{
// 		cout << "Valor de 1 grado: " << ValorDe1Grado << endl;
// 		real sigma = RangoVisibilidadEnKm/ValorDe1Grado;
// 		cout << "sigma = " << sigma*ValorDe1Grado << "km" << endl;
// 		
// 		Cx = 1/(2*sigma*sigma);
// 		Cy = Cx;
// 		bx = MaximaLongitudQueNoEs0(Cx);
// 		cout << "El borde vale " << bx*ValorDe1Grado << "km" << endl;
// 		by = MaximaLongitudQueNoEs0(Cy);
// 		
// 		cout << "Cx = " << Cx << "/grado^2" << endl;
		
		real sigma = RangoVisibilidadEnKm;
// 		cout << "sigma = " << sigma << "km" << endl;

		Cx = 1/(2*sigma*sigma);
		Cy = Cx;
// 		cout << "Cx = " << Cx << "/km^2" << endl;
		bx = MaximaLongitudQueNoEs0(Cx);
		by = MaximaLongitudQueNoEs0(Cy);
// 		cout << "bx = " << bx << "km" << endl;
		
		auto UEnKilometros = UEnGrados;
		for (auto& conjDePuntos : UEnKilometros)
		{
			for (auto& punto : conjDePuntos)
			{
				punto *= ValorDe1Grado;
			
			}
		}
		
			
			
		E = Normalized(UEnKilometros);
	}
	
	Matrix CalculateGraph();
	
private:
	vector<vector<Point>> Normalized(const vector<vector<Point>>& U);
	size_t malla;
	real Cx; //constantes en continuo
	real Cy;
	
	real Cnx; // constantes normalizada
	real Cny;
	
	real Cmx; //constantes en malla
	real Cmy;

	real bx; //borde en continuo
	real by; 
		
	Point O;
	Point W;
	Point F;

	vector<vector<Point>> E;
};