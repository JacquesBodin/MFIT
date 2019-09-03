#include "pch.h"
#include <fstream>
#include <iostream>
#include <iomanip> // std::setprecision
#include <boost/math/constants/constants.hpp>
#include <boost/math/special_functions/bessel.hpp>
#include <gsl/gsl_integration.h>
#include <gsl/gsl_errno.h>

using namespace std;

const double pi = boost::math::constants::pi<double>();

struct fun_params { double x, h, s, p, o, t; };

double fun1(double xi, void *p) // integral part of the MIM model, and integral part (i) of the derivative with respect to s-o, xi is the dummy integration variable
{
	fun_params &params = *reinterpret_cast<fun_params *>(p);
	return 0.1e1 / xi * sqrt(0.1e1 / (params.p * params.x * params.t / params.h - xi))
		* exp(-params.s * pow(params.p * params.x - xi, 0.2e1) / params.p / params.x / xi / 0.4e1 - params.o * xi / params.p - params.o * (params.p * params.x * params.t / params.h - xi) / (0.1e1 - params.p))
		* boost::math::cyl_bessel_i(1, 0.2e1 * params.o * sqrt(xi * (params.p * params.x * params.t / params.h - xi) / params.p / (0.1e1 - params.p)));
}

double fun2(double xi, void *p) // integral part (ii) of the derivative with respect to s
{
	fun_params &params = *reinterpret_cast<fun_params *>(p);
	return -pow(xi, -0.2e1) * sqrt(0.1e1 / (params.p * params.x * params.t / params.h - xi)) * pow(params.p * params.x - xi, 0.2e1) / params.p / params.x
		* exp(-params.s * pow(params.p * params.x - xi, 0.2e1) / params.p / params.x / xi / 0.4e1 - params.o * xi / params.p - params.o * (params.p * params.x * params.t / params.h - xi) / (0.1e1 - params.p))
		* boost::math::cyl_bessel_i(1, 0.2e1 * params.o * sqrt(xi * (params.p * params.x * params.t / params.h - xi) / params.p / (0.1e1 - params.p))) / 0.4e1;
}

double fun3(double xi, void *p) // integral part (ii) of the derivative with respect to o
{
	fun_params &params = *reinterpret_cast<fun_params *>(p);
	return 0.1e1 / xi * sqrt(0.1e1 / (params.p * params.x * params.t / params.h - xi))
		* (-xi / params.p - (params.p * params.x * params.t / params.h - xi) / (0.1e1 - params.p))
		* exp(-params.s * pow(params.p * params.x - xi, 0.2e1) / params.p / params.x / xi / 0.4e1
			- params.o * xi / params.p - params.o * (params.p * params.x * params.t / params.h - xi) / (0.1e1 - params.p))
		* boost::math::cyl_bessel_i(1, 0.2e1 * params.o * sqrt(xi * (params.p * params.x * params.t / params.h - xi) / params.p / (0.1e1 - params.p)))
		+ 0.2e1 / xi * sqrt(0.1e1 / (params.p * params.x * params.t / params.h - xi))
		* exp(-params.s * pow(params.p * params.x - xi, 0.2e1) / params.p / params.x / xi / 0.4e1
			- params.o * xi / params.p - params.o * (params.p * params.x * params.t / params.h - xi) / (0.1e1 - params.p))
		* (boost::math::cyl_bessel_i(0, 0.2e1 * params.o * sqrt(xi * (params.p * params.x * params.t / params.h - xi) / params.p / (0.1e1 - params.p)))
			- 0.1e1 / params.o * pow(xi * (params.p * params.x * params.t / params.h - xi) / params.p / (0.1e1 - params.p), -0.1e1 / 0.2e1)
			* boost::math::cyl_bessel_i(1, 0.2e1 * params.o * sqrt(xi * (params.p * params.x * params.t / params.h - xi) / params.p / (0.1e1 - params.p))) / 0.2e1)
		* sqrt(xi * (params.p * params.x * params.t / params.h - xi) / params.p / (0.1e1 - params.p));
}

int main(int argc, char *argv[])
{
	bool drvOption(false); // indicates whether the program must compute BOTH c(t) values AND derivatives
	int quadKey(6); // value must be between 1 and 6, determines the integration rule of the QAG adaptative integration routine, see https://www.gnu.org/software/gsl/doc/html/integration.html#qng-non-adaptive-gauss-kronrod-integration
	bool tplFileError(false); // file reading error flag
	bool quadError(false); // numerical integration (quadrature) error flag
	vector<string> vectPestParams;

	//gsl_integration_glfixed_table * tGL = gsl_integration_glfixed_table_alloc(10000); // memory allocation for a Gauss-Legendre (fixed order) integration scheme
	gsl_integration_workspace * w = gsl_integration_workspace_alloc(1000); // memory allocation for the QAG adaptative integration routine: see https://www.gnu.org/software/gsl/doc/html/integration.html

	// remove possible error messages while computing integral during Pest optimization trials 
	gsl_set_error_handler_off(); //*** GSL errors
	//_set_abort_behavior(0, _WRITE_ABORT_MSG); //*** Visual C++ errors

	if (argc > 1)
	{
		if (string(argv[1]) == "/d") // the command line 'MDP_2RNE /d' indicates that both c(t) values and derivatives must be computed
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
		vector< vector<double> > vectPaths; // noFlowpaths x 6 parameters (m, x, h, s, p, o) = (mass, x, T0, Pe, psi, omega)
		vector<double> vectMXHSPO(6, 0); // a single flow path, initialized as [0,0,0,0,0,0]
		bool inputFileError(false); // file reading error flag

		if (inputFile >> tMin >> tMax >> noTimeSteps >> qt >> noFlowpaths)
		{
			if ((tMin >= 0) && (tMax > tMin) && (noTimeSteps >= 1) && (qt > 0) && (noFlowpaths >= 1))
			{
				for (int i = 0; i < noFlowpaths; i++)
				{
					if (inputFile >> vectMXHSPO[0] >> vectMXHSPO[1] >> vectMXHSPO[2]
						>> vectMXHSPO[3] >> vectMXHSPO[4] >> vectMXHSPO[5]) // m >> x >> h >> s >> p >> o
					{
						if ((vectMXHSPO[0] >= 0) && (vectMXHSPO[1] > 0) && (vectMXHSPO[2] > 0)
							&& (vectMXHSPO[3] >= 0) && (vectMXHSPO[4] > 0) && (vectMXHSPO[4] < 1) && (vectMXHSPO[5] >= 0))
						{
							vectPaths.push_back(vectMXHSPO);
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
						double xj = vectPaths[j][1]; // x
						double hj = vectPaths[j][2]; // T0
						double sj = vectPaths[j][3]; // Pe
						double pj = vectPaths[j][4]; // psi
						double oj = vectPaths[j][5]; // omega
						if (time > 0)
						{
							struct fun_params paramsj = { xj, hj, sj, pj, oj, time };
							gsl_function f1j;
							f1j.function = &fun1;
							f1j.params = &paramsj;
							double  quadres1j, error;
							//quadres1j = gsl_integration_glfixed(&f1j, 0, time, tGL); // Gauss-Legendre integration
							gsl_integration_qag(&f1j, 0, pj*xj*time / hj, 0, 1e-2, 1000, quadKey, w, &quadres1j, &error); // QAG adaptative integration with a 99% relative accuracy
							concentration += mj / qt * (0.1e1 / hj * pow(pi / sj * pow(time, 0.3e1) * pow(hj, -0.3e1), -0.1e1 / 0.2e1)
								* exp(-sj * pow(0.1e1 - time / hj, 0.2e1) / time * hj / 0.4e1 - oj * xj * time / hj) / 0.2e1
								+ oj * pj / hj * sqrt(sj * pow(xj, 0.3e1) / pi / (0.1e1 - pj)) * quadres1j / 0.2e1);
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
									double xk = vectPaths[k][1];
									double hk = vectPaths[k][2];
									double sk = vectPaths[k][3];
									double pk = vectPaths[k][4];
									double ok = vectPaths[k][5];
									if (time > 0)
									{
										struct fun_params paramsk = { xk, hk, sk, pk, ok, time };
										gsl_function f1k;
										f1k.function = &fun1;
										f1k.params = &paramsk;
										double  quadres1k, error;
										gsl_integration_qag(&f1k, 0, pk*xk*time / hk, 0, 1e-2, 1000, quadKey, w, &quadres1k, &error);
										deriv_qt += (-1 / pow(qt, 2))*(mk * (0.1e1 / hk * pow(pi / sk * pow(time, 0.3e1) * pow(hk, -0.3e1), -0.1e1 / 0.2e1)
											* exp(-sk * pow(0.1e1 - time / hk, 0.2e1) / time * hk / 0.4e1 - ok * xk * time / hk) / 0.2e1
											+ ok * pk / hk * sqrt(sk * pow(xk, 0.3e1) / pi / (0.1e1 - pk)) * quadres1k / 0.2e1));
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
										double xk = vectPaths[idFlowpath - 1][1];
										double hk = vectPaths[idFlowpath - 1][2];
										double sk = vectPaths[idFlowpath - 1][3];
										double pk = vectPaths[idFlowpath - 1][4];
										double ok = vectPaths[idFlowpath - 1][5];
										if (time > 0)
										{
											struct fun_params paramsk = { xk, hk, sk, pk, ok, time };
											gsl_function f1k;
											f1k.function = &fun1;
											f1k.params = &paramsk;
											double  quadres1k, error;
											gsl_integration_qag(&f1k, 0, pk*xk*time / hk, 0, 1e-2, 1000, quadKey, w, &quadres1k, &error);
											deriv_m = 0.1e1 / qt * (0.1e1 / hk * pow(pi / sk * pow(time, 0.3e1) * pow(hk, -0.3e1), -0.1e1 / 0.2e1)
												* exp(-sk * pow(0.1e1 - time / hk, 0.2e1) / time * hk / 0.4e1 - ok * xk * time / hk) / 0.2e1
												+ ok * pk / hk * sqrt(sk * pow(xk, 0.3e1) / pi / (0.1e1 - pk)) * quadres1k / 0.2e1);
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
							else if ((paramType == 'x') || (paramType == 'h') || (paramType == 'p'))
							{
								drvFile << -1.11E33;  // Pest flag code indicating that the derivative must be computed numerically (because the derivative analytical expression is undefined) 
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
							else if (paramType == 's')
							{
								int idFlowpath;
								if (istringstream(pestParamName.substr(1, pestParamName.size() - 1)) >> idFlowpath)
								{
									if ((idFlowpath >= 1) && (idFlowpath <= noFlowpaths))
									{
										double deriv_s = 0;
										double mk = vectPaths[idFlowpath - 1][0];
										double xk = vectPaths[idFlowpath - 1][1];
										double hk = vectPaths[idFlowpath - 1][2];
										double sk = vectPaths[idFlowpath - 1][3];
										double pk = vectPaths[idFlowpath - 1][4];
										double ok = vectPaths[idFlowpath - 1][5];
										if (time > 0)
										{
											struct fun_params paramsk = { xk, hk, sk, pk, ok, time };
											gsl_function f1k, f2k;
											f1k.function = &fun1;
											f1k.params = &paramsk;
											f2k.function = &fun2;
											f2k.params = &paramsk;
											double  quadres1k, quadres2k, error;
											gsl_integration_qag(&f1k, 0, pk*xk*time / hk, 0, 1e-2, 1000, quadKey, w, &quadres1k, &error);
											gsl_integration_qag(&f2k, 0, pk*xk*time / hk, 0, 1e-2, 1000, quadKey, w, &quadres2k, &error);

											deriv_s = mk * (pow(hk, -0.4e1) * pow(pi / sk * pow(time, 0.3e1) * pow(hk, -0.3e1), -0.3e1 / 0.2e1)
												* exp(-sk * pow(0.1e1 - time / hk, 0.2e1) / time * hk / 0.4e1 - ok * xk * time / hk)
												* pi * pow(sk, -0.2e1) * pow(time, 0.3e1) / 0.4e1
												- pow(pi / sk * pow(time, 0.3e1) * pow(hk, -0.3e1), -0.1e1 / 0.2e1)
												* pow(0.1e1 - time / hk, 0.2e1) / time
												* exp(-sk * pow(0.1e1 - time / hk, 0.2e1) / time * hk / 0.4e1 - ok * xk * time / hk) / 0.8e1
												+ ok * pk * quadres1k * pow(xk, 0.3e1) / hk * pow(sk * pow(xk, 0.3e1) / pi / (0.1e1 - pk), -0.1e1 / 0.2e1) / pi / (0.1e1 - pk) / 0.4e1
												+ ok * pk * sqrt(sk * pow(xk, 0.3e1) / pi / (0.1e1 - pk)) * quadres2k / hk / 0.2e1) / qt;
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
							else if (paramType == 'o')
							{
								int idFlowpath;
								if (istringstream(pestParamName.substr(1, pestParamName.size() - 1)) >> idFlowpath)
								{
									if ((idFlowpath >= 1) && (idFlowpath <= noFlowpaths))
									{
										double deriv_o(0);
										double mk = vectPaths[idFlowpath - 1][0];
										double xk = vectPaths[idFlowpath - 1][1];
										double hk = vectPaths[idFlowpath - 1][2];
										double sk = vectPaths[idFlowpath - 1][3];
										double pk = vectPaths[idFlowpath - 1][4];
										double ok = vectPaths[idFlowpath - 1][5];
										if (time > 0)
										{
											struct fun_params paramsk = { xk, hk, sk, pk, ok, time };
											gsl_function f1k, f3k;
											f1k.function = &fun1;
											f1k.params = &paramsk;
											f3k.function = &fun3;
											f3k.params = &paramsk;
											double  quadres1k, quadres3k, error;
											gsl_integration_qag(&f1k, 0, pk*xk*time / hk, 0, 1e-2, 1000, quadKey, w, &quadres1k, &error);
											gsl_integration_qag(&f3k, 0, pk*xk*time / hk, 0, 1e-2, 1000, quadKey, w, &quadres3k, &error);

											deriv_o = mk * (-pow(hk, -0.2e1) * pow(pi / sk * pow(time, 0.3e1) * pow(hk, -0.3e1), -0.1e1 / 0.2e1) * xk * time
												* exp(-sk * pow(0.1e1 - time / hk, 0.2e1) / time * hk / 0.4e1 - ok * xk * time / hk) / 0.2e1
												+ pk * sqrt(sk * pow(xk, 0.3e1) / pi / (0.1e1 - pk)) * quadres1k / hk / 0.2e1
												+ ok * pk * sqrt(sk * pow(xk, 0.3e1) / pi / (0.1e1 - pk)) * quadres3k / hk / 0.2e1) / qt;
										}
										drvFile << deriv_o;
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
					cerr << "Numerical error while computing integral during Pest optimization trials" << endl;
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

