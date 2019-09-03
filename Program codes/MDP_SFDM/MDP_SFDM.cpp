#include "pch.h"
#include <fstream>
#include <iostream>
#include <iomanip> // std::setprecision
#include <boost/math/constants/constants.hpp>
#include <gsl/gsl_integration.h>
#include <gsl/gsl_errno.h>

using namespace std;

const double pi = boost::math::constants::pi<double>();

struct fun_params { double b, h, s, t; };

double fun1(double xi, void *p) // integral part of the SFDM model by Maloszewski and Zuber (1990), and integral part (i) of the derivative with respect to b-h-s, xi is the dummy integration variable
{
	fun_params &params = *reinterpret_cast<fun_params *>(p);
	return exp(-params.s * pow(params.h - xi, 0.2e1) / xi / params.h / 0.4e1 - params.b * params.b * xi * xi / (params.t - xi)) * pow(xi * pow(params.t - xi, 0.3e1), -0.1e1 / 0.2e1);
}

double fun2(double xi, void *p) // integral part (ii) of the derivative with respect to b
{
	fun_params &params = *reinterpret_cast<fun_params *>(p);
	return -0.2e1 * params.b * xi * xi / (params.t - xi) * exp(-params.s * pow(params.h - xi, 0.2e1) / xi / params.h / 0.4e1 - params.b * params.b * xi * xi / (params.t - xi)) * pow(xi * pow(params.t - xi, 0.3e1), -0.1e1 / 0.2e1);
}

double fun3(double xi, void *p) // integral part (ii) of the derivative with respect to h
{
	fun_params &params = *reinterpret_cast<fun_params *>(p);
	return (-params.s * (params.h - xi) / xi / params.h / 0.2e1 + params.s * pow(params.h - xi, 0.2e1) / xi * pow(params.h, -0.2e1) / 0.4e1) * exp(-params.s * pow(params.h - xi, 0.2e1) / xi / params.h / 0.4e1 - params.b * params.b * xi * xi / (params.t - xi)) * pow(xi * pow(params.t - xi, 0.3e1), -0.1e1 / 0.2e1);
}

double fun4(double xi, void *p) // integral part (ii) of the derivative with respect to s
{
	fun_params &params = *reinterpret_cast<fun_params *>(p);
	return -pow(params.h - xi, 0.2e1) / xi / params.h * exp(-params.s * pow(params.h - xi, 0.2e1) / xi / params.h / 0.4e1 - params.b * params.b * xi * xi / (params.t - xi)) * pow(xi * pow(params.t - xi, 0.3e1), -0.1e1 / 0.2e1) / 0.4e1;
}

/*class fun1bis // functor
{
public:
fun1bis(double const Pe, double const t0, double const beta, double const t) : m_Pe(Pe), m_t0(t0), m_beta(beta), m_t(t) {} // constructor
double operator()(double xi) const // surcharge, xi is the dummy integration variable
{
return exp(-m_Pe*pow(m_t0 - xi, 2) / (4 * xi*m_t0) - pow(m_beta*xi, 2) / (m_t - xi)) / sqrt(xi*pow(m_t - xi, 3));
}
private:
double m_Pe, m_t0, m_beta, m_t;
};*/


int main(int argc, char *argv[])
{
	bool drvOption(false); // indicates whether the program must compute BOTH c(t) values AND derivatives
	int quadKey(6); // value must be between 1 and 6, determines the integration rule of the QAG adaptative integration routine, see https://www.gnu.org/software/gsl/doc/html/integration.html#qng-non-adaptive-gauss-kronrod-integration
	bool tplFileError(false); // file reading error flag
	bool quadError(false); // numerical integration (quadrature) error flag
	vector<string> vectPestParams;

	//gsl_integration_glfixed_table * tGL = gsl_integration_glfixed_table_alloc(10000); // memory allocation for a Gauss-Legendre (fixed order) integration scheme
	gsl_integration_workspace * w = gsl_integration_workspace_alloc(1000); // memory allocation for the QAG adaptative integration routine: see https://www.gnu.org/software/gsl/doc/html/integration.html

	gsl_set_error_handler_off(); //*** GSL errors: remove possible error messages while computing integral during Pest optimization trials 

	if (argc > 1)
	{
		if (string(argv[1]) == "/d") // the command line 'MDP_SFDM /d' indicates that both c(t) values and derivatives must be computed
		{
			drvOption = true;
			ifstream tplFile("MFIT.tpl", ios::in); // reading the file "MFIT.tpl" and implementing vectPestParam (list of parameters for which optimization is required)
			if (tplFile)
			{
				string line, paramName;
				while (getline(tplFile, line))
				{
					if (line[0] == '#')
					{
						istringstream(line.substr(1, line.size() - 1)) >> paramName;
						vectPestParams.push_back(paramName);
					}
				}
			}
			else
			{
				tplFileError = true;
				cerr << "Error: the file MFIT.tpl can't be opened" << endl;
			}
		}
		else if (string(argv[1]) == "/15") quadKey = 1; // no derivative but quadrature tuning option 
		else if (string(argv[1]) == "/21") quadKey = 2; // The default quadKey value is 6, corresponding to 61 point Gauss-Kronrod rule
		else if (string(argv[1]) == "/31") quadKey = 3;
		else if (string(argv[1]) == "/41") quadKey = 4;
		else if (string(argv[1]) == "/51") quadKey = 5;
		else if (string(argv[1]) == "/61") quadKey = 6;
		if ((argc > 2) && (string(argv[1]) == "/d"))
		{
			if (string(argv[2]) == "/15") quadKey = 1; // both derivatives AND quadrature tuning options
			else if (string(argv[2]) == "/21") quadKey = 2;
			else if (string(argv[2]) == "/31") quadKey = 3;
			else if (string(argv[2]) == "/41") quadKey = 4;
			else if (string(argv[2]) == "/51") quadKey = 5;
			else if (string(argv[2]) == "/61") quadKey = 6;
		}
	}

	ifstream inputFile("Input.txt", ios::in); // opening the file "Input.txt" for reading model parameters
	if (inputFile)
	{
		double tMin, tMax; // minimum and maximum simulation times
		int noTimeSteps; // number of simulated time-concentration values
		double qt; // total flow rate
		int noFlowpaths; // number of flow (and transport) paths
		vector< vector<double> > vectPaths; // noFlowpaths x 4 parameters (m, b, h, s) = (mass, beta, T0, Pe)
		vector<double> vectMBHS(4, 0); // a single flow path, initialized as [0,0,0,0]
		bool inputFileError(false); // file reading error flag

		if (inputFile >> tMin >> tMax >> noTimeSteps >> qt >> noFlowpaths)
		{
			if ((tMin >= 0) && (tMax > tMin) && (noTimeSteps >= 1) && (qt > 0) && (noFlowpaths >= 1))
			{
				for (int i = 0; i < noFlowpaths; i++)
				{
					if (inputFile >> vectMBHS[0] >> vectMBHS[1] >> vectMBHS[2] >> vectMBHS[3]) // m >> b >> h >> s
					{
						if ((vectMBHS[0] >= 0) && (vectMBHS[1] > 0) && (vectMBHS[2] > 0) && (vectMBHS[3] >= 0))
						{
							vectPaths.push_back(vectMBHS);
						}
						else
						{
							inputFileError = true;
							break;
						}
					}
					else
					{
						inputFileError = true;
						break;
					}
				}
			}
			else inputFileError = true;
		}
		else inputFileError = true;

		if (inputFileError)
		{
			cerr << "Input file error: check file structure and parameter values" << endl;
		}
		else // (inputFileError == false )
		{
			ofstream outputFile("Output.txt", ios::out | ios::trunc); // list of (time;concentration) values
			outputFile << "TIME VALUE ; CONCENTRATION VALUE" << endl; // headline
			outputFile << setprecision(15); // managing the precision of numbers written to the file

			ofstream drvFile;
			if ((drvOption) && (!tplFileError))
			{
				drvFile.open("Deriv.txt", ios::out | ios::trunc); // creating derivative file
				drvFile << vectPestParams.size() << "  " << noTimeSteps << endl;
				drvFile << setprecision(15);
			}

			for (int i = 0; i < noTimeSteps; i++) // number of time-concentration values
			{
				double time = tMin + i * (tMax - tMin) / double(noTimeSteps - 1);
				try
				{
					// concentration value computation and writing to file
					double concentration(0);
					for (int j = 0; j < noFlowpaths; j++)
					{
						double mj = vectPaths[j][0]; // mass
						double bj = vectPaths[j][1]; // beta (mass transfert coef.)
						double hj = vectPaths[j][2]; // T0
						double sj = vectPaths[j][3]; // Pe
						if (time > 0)
						{
							struct fun_params paramsj = { bj, hj, sj, time };
							gsl_function f1j;
							f1j.function = &fun1;
							f1j.params = &paramsj;
							double  quadres1j, error;
							//quadres1j = gsl_integration_glfixed(&f1j, 0, time, tGL); // Gauss-Legendre integration
							int status = gsl_integration_qag(&f1j, 0, time, 0, 1e-2, 1000, quadKey, w, &quadres1j, &error); // QAG adaptative integration with a 99% relative accuracy
							if (status)
							{
								quadError = true;
								cerr << "Nuperical error while computing integral during Pest optimization trials" << endl;
							}
							concentration += mj * bj*sqrt(sj*hj)*quadres1j / (2 * pi*qt);
						}
					}
					outputFile << time << ";" << concentration << endl;

					// derivative computation and writing to file
					if ((drvOption) && (!tplFileError))
					{
						int noWrittenValPerLine(0);
						for (int j = 0; j < vectPestParams.size(); j++)
						{
							string pestParamName = vectPestParams[j];
							char paramType = pestParamName[0];
							if (paramType == 'q') //qt
							{
								double deriv_qt(0);
								for (int k = 0; k < noFlowpaths; k++)
								{
									double mk = vectPaths[k][0];
									double bk = vectPaths[k][1];
									double hk = vectPaths[k][2];
									double sk = vectPaths[k][3];
									if (time > 0)
									{
										struct fun_params paramsk = { bk, hk, sk, time };
										gsl_function f1k;
										f1k.function = &fun1;
										f1k.params = &paramsk;
										double  quadres1k, error;
										gsl_integration_qag(&f1k, 0, time, 0, 1e-2, 1000, quadKey, w, &quadres1k, &error);
										deriv_qt += (-1 / pow(qt, 2))*(mk*bk*sqrt(sk*hk)*quadres1k / (2 * pi));
									}
								}
								drvFile << deriv_qt;
								noWrittenValPerLine++;
								if (noWrittenValPerLine < vectPestParams.size())
								{
									drvFile << "  ";
								}
								else
								{
									drvFile << endl;
								}
							}
							else if (paramType == 'm')
							{
								int idFlowpath;
								if (istringstream(pestParamName.substr(1, pestParamName.size() - 1)) >> idFlowpath)
								{
									if ((idFlowpath >= 1) && (idFlowpath <= noFlowpaths))
									{
										double deriv_m = 0;
										double mk = vectPaths[idFlowpath - 1][0];
										double bk = vectPaths[idFlowpath - 1][1];
										double hk = vectPaths[idFlowpath - 1][2];
										double sk = vectPaths[idFlowpath - 1][3];
										if (time > 0)
										{
											struct fun_params paramsk = { bk, hk, sk, time };
											gsl_function f1k;
											f1k.function = &fun1;
											f1k.params = &paramsk;
											double  quadres1k, error;
											gsl_integration_qag(&f1k, 0, time, 0, 1e-2, 1000, quadKey, w, &quadres1k, &error);
											deriv_m = bk * sqrt(sk*hk)*quadres1k / (2 * pi*qt);
										}
										drvFile << deriv_m;
										noWrittenValPerLine++;
										if (noWrittenValPerLine < vectPestParams.size())
										{
											drvFile << "  ";
										}
										else
										{
											drvFile << endl;
										}
									}
									else
									{
										inputFileError = true;
										break;
									}
								}
								else
								{
									inputFileError = true;
									break;
								}
							}
							else if (paramType == 'b')
							{
								int idFlowpath;
								if (istringstream(pestParamName.substr(1, pestParamName.size() - 1)) >> idFlowpath)
								{
									if ((idFlowpath >= 1) && (idFlowpath <= noFlowpaths))
									{
										double deriv_b = 0;
										double mk = vectPaths[idFlowpath - 1][0];
										double bk = vectPaths[idFlowpath - 1][1];
										double hk = vectPaths[idFlowpath - 1][2];
										double sk = vectPaths[idFlowpath - 1][3];
										if (time > 0)
										{
											struct fun_params paramsk = { bk, hk, sk, time };
											gsl_function f1k, f2k;
											f1k.function = &fun1;
											f1k.params = &paramsk;
											f2k.function = &fun2;
											f2k.params = &paramsk;
											double  quadres1k, quadres2k, error;
											gsl_integration_qag(&f1k, 0, time, 0, 1e-2, 1000, quadKey, w, &quadres1k, &error);
											gsl_integration_qag(&f2k, 0, time, 0, 1e-2, 1000, quadKey, w, &quadres2k, &error);
											deriv_b = mk * sqrt(sk * hk) * quadres1k / pi / qt / 0.2e1 + mk * bk * sqrt(sk * hk) * quadres2k / pi / qt / 0.2e1;
										}
										drvFile << deriv_b;
										noWrittenValPerLine++;
										if (noWrittenValPerLine < vectPestParams.size())
										{
											drvFile << "  ";
										}
										else
										{
											drvFile << endl;
										}
									}
									else
									{
										inputFileError = true;
										break;
									}
								}
								else
								{
									inputFileError = true;
									break;
								}
							}
							else if (paramType == 'h')
							{
								int idFlowpath;
								if (istringstream(pestParamName.substr(1, pestParamName.size() - 1)) >> idFlowpath)
								{
									if ((idFlowpath >= 1) && (idFlowpath <= noFlowpaths))
									{
										double deriv_h = 0;
										double mk = vectPaths[idFlowpath - 1][0];
										double bk = vectPaths[idFlowpath - 1][1];
										double hk = vectPaths[idFlowpath - 1][2];
										double sk = vectPaths[idFlowpath - 1][3];
										if (time > 0)
										{
											struct fun_params paramsk = { bk, hk, sk, time };
											gsl_function f1k, f3k;
											f1k.function = &fun1;
											f1k.params = &paramsk;
											f3k.function = &fun3;
											f3k.params = &paramsk;
											double  quadres1k, quadres3k, error;
											gsl_integration_qag(&f1k, 0, time, 0, 1e-2, 1000, quadKey, w, &quadres1k, &error);
											gsl_integration_qag(&f3k, 0, time, 0, 1e-2, 1000, quadKey, w, &quadres3k, &error);
											deriv_h = mk * bk * quadres1k * sk / pi / qt * pow(sk * hk, -0.1e1 / 0.2e1) / 0.4e1 + mk * bk * sqrt(sk * hk) * quadres3k / pi / qt / 0.2e1;
										}
										drvFile << deriv_h;
										noWrittenValPerLine++;
										if (noWrittenValPerLine < vectPestParams.size())
										{
											drvFile << "  ";
										}
										else
										{
											drvFile << endl;
										}
									}
									else
									{
										inputFileError = true;
										break;
									}
								}
								else
								{
									inputFileError = true;
									break;
								}
							}
							else if (paramType == 's')
							{
								int idFlowpath;
								if (istringstream(pestParamName.substr(1, pestParamName.size() - 1)) >> idFlowpath)
								{
									if ((idFlowpath >= 1) && (idFlowpath <= noFlowpaths))
									{
										double deriv_s = 0;
										double mk = vectPaths[idFlowpath - 1][0];
										double bk = vectPaths[idFlowpath - 1][1];
										double hk = vectPaths[idFlowpath - 1][2];
										double sk = vectPaths[idFlowpath - 1][3];
										if (time > 0)
										{
											struct fun_params paramsk = { bk, hk, sk, time };
											gsl_function f1k, f4k;
											f1k.function = &fun1;
											f1k.params = &paramsk;
											f4k.function = &fun4;
											f4k.params = &paramsk;
											double  quadres1k, quadres4k, error;
											gsl_integration_qag(&f1k, 0, time, 0, 1e-2, 1000, quadKey, w, &quadres1k, &error);
											gsl_integration_qag(&f4k, 0, time, 0, 1e-2, 1000, quadKey, w, &quadres4k, &error);
											deriv_s = mk * bk * quadres1k * hk / pi / qt * pow(sk * hk, -0.1e1 / 0.2e1) / 0.4e1 + mk * bk * sqrt(sk * hk) * quadres4k / pi / qt / 0.2e1;
										}
										drvFile << deriv_s;
										noWrittenValPerLine++;
										if (noWrittenValPerLine < vectPestParams.size())
										{
											drvFile << "  ";
										}
										else
										{
											drvFile << endl;
										}
									}
									else
									{
										inputFileError = true;
										break;
									}
								}
								else
								{
									inputFileError = true;
									break;
								}
							}
						}
					}
					if (inputFileError) break;
				}
				catch (boost::exception const&  ex)
				{
					quadError = true;
					cerr << "Nurerical error while computing integral during Pest optimization trials" << endl;
					break;
				}
			}
			if (inputFileError)
			{
				cerr << "Input file error: check file structure and parameter values" << endl;
			}
			if ((inputFileError) || (quadError))
			{
				outputFile.close();
				remove("Output.txt");
				drvFile.close();
				remove("Deriv.txt");
			}
		}
	}
	else
	{
		cerr << "Error: the file Input.txt can't be opened" << endl;
		remove("Output.txt"); // cleaning possible old files
		remove("Deriv.txt");
	}
	//gsl_integration_glfixed_table_free(tGL); // if the Gauss-Legendre integration routine has been used
	gsl_integration_workspace_free(w);
	return 0;
}
