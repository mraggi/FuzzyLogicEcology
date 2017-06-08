#pragma once

#include "TimeHelpers.hpp"
#include "ReadFile.hpp"
#include "Point.hpp"

constexpr double RadioTierraEnKm = 6371.0;
constexpr double ValorDe1Grado = (pi*RadioTierraEnKm)/180.0;

class GraphCalculator
{
public:
	GraphCalculator(size_t _malla, double RangoVisibilidadEnKm, const vector<vector<Point>>& UEnGrados) : malla(_malla)
	{
		double sigma = RangoVisibilidadEnKm;

		Cx = 1/(2*sigma*sigma);
		Cy = Cx;
		bx = MaximaLongitudQueNoEs0(Cx);
		by = MaximaLongitudQueNoEs0(Cy);
		
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
	double Cx; //constantes en continuo
	double Cy;
	
	double Cnx; // constantes normalizada
	double Cny;
	
	double Cmx; //constantes en malla
	double Cmy;

	double bx; //borde en continuo
	double by; 
		
	Point O;
	Point W;
	Point F;

	vector<vector<Point>> E;
};
