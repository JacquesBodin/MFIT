#include "pch.h"
#include <iostream>
#include <iomanip> // std::setprecision
#include <fstream>
#include <cmath>
#include <vector>
#include <boost/math/constants/constants.hpp>
//#include <Windows.h> // sleep

using namespace std;

const double pi = boost::math::constants::pi<double>();

int main(int argc, char *argv[])
{
	bool drvOption(false); // indicates whether the program must compute BOTH c(t) values AND derivatives
	bool tplFileError(false); // file reading error flag
	bool computError(false); // numerical error flag
	vector<string> vectPestParams;

	if ((argc > 1) && (string(argv[1]) == "/d")) // the command line 'MDMed /d' indicates that both c(t) values and derivatives must be computed
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

	ifstream inputFile("Input.txt", ios::in); // opening the file "Input.txt" for reading model parameters
	if (inputFile)
	{
		double tMin, tMax; // minimum and maximum simulation times
		int noTimeSteps; // number of simulated time-concentration values
		double c0; // source(injection) concentration
		int noFlowpaths; // number of flow (and transport) paths
		vector< vector<double> > vectPaths; // noFlowpaths x 4 parameters (f, h, s, g) = (flowpath discharge ratio, T0, Pe, gamma coef.)
		vector<double> vectFHSG(4, 0); // a single flow path, initialized as [0,0,0,0]
		bool inputFileError(false); // file reading error flag

		if (inputFile >> tMin >> tMax >> noTimeSteps >> c0 >> noFlowpaths)
		{
			if ((tMin >= 0) && (tMax > tMin) && (noTimeSteps >= 1) && (c0 > 0) && (noFlowpaths >= 1))
			{
				for (int i = 0; i < noFlowpaths; i++)
				{
					if (inputFile >> vectFHSG[0] >> vectFHSG[1] >> vectFHSG[2] >> vectFHSG[3]) // f >> h >> s >> g
					{
						if ((vectFHSG[0] >= 0) && (vectFHSG[0] <= 1) && (vectFHSG[1] > 0) && (vectFHSG[2] >= 0) && (vectFHSG[3] >= 0) && (vectFHSG[3] <= 1))
						{
							vectPaths.push_back(vectFHSG);
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

				// concentration value computation and writing to file
				double concentration(0);
				for (int j = 0; j < noFlowpaths; j++)
				{
					double fj = vectPaths[j][0]; // flowpath discharge ratio
					double hj = vectPaths[j][1]; // T0
					double sj = vectPaths[j][2]; // Pe
					double gj = vectPaths[j][3]; // gamma coef.
					if (time > 0)
					{
						double cj = fj * c0 * (exp(gj * sj / 0.2e1) * erfc(sqrt(sj) * (0.1e1 + gj * time / hj) * pow(time / hj, -0.1e1 / 0.2e1) / 0.2e1)
							+ exp(-gj * sj / 0.2e1) * erfc(sqrt(sj) * (0.1e1 - gj * time / hj) * pow(time / hj, -0.1e1 / 0.2e1) / 0.2e1))
							* exp(sj * (0.1e1 - (-gj * gj + 0.1e1) * time / hj / 0.2e1) / 0.2e1) / 0.2e1;
						if (cj != cj) // ... if nan ...
						{
							computError = true;
							break;
						}
						else
						{
							concentration += cj;
						}
					}
				}
				if (computError)
				{
					break;
				}
				else
				{
					outputFile << time << ";" << concentration << endl;
				}

				// derivative computation and writing to file
				if ((drvOption) && (!tplFileError))
				{
					int noWrittenValPerLine(0);
					for (int j = 0; j < vectPestParams.size(); j++)
					{
						string pestParamName = vectPestParams[j];
						char paramType = pestParamName[0];
						if (paramType == 'c') //c0
						{
							double deriv_c0(0);
							for (int k = 0; k < noFlowpaths; k++)
							{
								double fk = vectPaths[k][0];
								double hk = vectPaths[k][1];
								double sk = vectPaths[k][2];
								double gk = vectPaths[k][3];
								if (time > 0)
								{
									double deriv_c0_k = fk * (exp(gk * sk / 0.2e1) * erfc(sqrt(sk) * (0.1e1 + gk * time / hk) * pow(time / hk, -0.1e1 / 0.2e1) / 0.2e1)
										+ exp(-gk * sk / 0.2e1) * erfc(sqrt(sk) * (0.1e1 - gk * time / hk) * pow(time / hk, -0.1e1 / 0.2e1) / 0.2e1))
										* exp(sk * (0.1e1 - (-gk * gk + 0.1e1) * time / hk / 0.2e1) / 0.2e1) / 0.2e1;
									if (deriv_c0_k != deriv_c0_k) // ... if nan ...
									{
										computError = true;
										break;
									}
									else
									{
										deriv_c0 += deriv_c0_k;
									}
								}
							}
							if (computError)
							{
								break;
							}
							else
							{
								drvFile << deriv_c0;
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
						}
						else if (paramType == 'f')
						{
							int idFlowpath;
							if (istringstream(pestParamName.substr(1, pestParamName.size() - 1)) >> idFlowpath)
							{
								if ((idFlowpath >= 1) && (idFlowpath <= noFlowpaths))
								{
									double deriv_f = 0;
									double hk = vectPaths[idFlowpath - 1][1];
									double sk = vectPaths[idFlowpath - 1][2];
									double gk = vectPaths[idFlowpath - 1][3];
									if (time > 0)
									{
										double deriv_f_k = c0 * (exp(gk * sk / 0.2e1) * erfc(sqrt(sk) * (0.1e1 + gk * time / hk) * pow(time / hk, -0.1e1 / 0.2e1) / 0.2e1)
											+ exp(-gk * sk / 0.2e1) * erfc(sqrt(sk) * (0.1e1 - gk * time / hk) * pow(time / hk, -0.1e1 / 0.2e1) / 0.2e1))
											* exp(sk * (0.1e1 - (-gk * gk + 0.1e1) * time / hk / 0.2e1) / 0.2e1) / 0.2e1;
										if (deriv_f_k != deriv_f_k) // ... if nan ...
										{
											computError = true;
											break;
										}
										else
										{
											deriv_f = deriv_f_k;
										}
									}
									if (computError)
									{
										break;
									}
									else
									{
										drvFile << deriv_f;
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
									double fk = vectPaths[idFlowpath - 1][0];
									double hk = vectPaths[idFlowpath - 1][1];
									double sk = vectPaths[idFlowpath - 1][2];
									double gk = vectPaths[idFlowpath - 1][3];
									if (time > 0)
									{
										double deriv_h_k = fk * c0 * (-0.2e1 * exp(gk * sk / 0.2e1) * pow(pi, -0.1e1 / 0.2e1)
											* exp(-sk * pow(0.1e1 + gk * time / hk, 0.2e1) / time * hk / 0.4e1)
											* (-sqrt(sk) * gk * time * pow(hk, -0.2e1) * pow(time / hk, -0.1e1 / 0.2e1) / 0.2e1
												+ sqrt(sk) * (0.1e1 + gk * time / hk) * pow(time / hk, -0.3e1 / 0.2e1) * time * pow(hk, -0.2e1) / 0.4e1)
											- 0.2e1 * exp(-gk * sk / 0.2e1) * pow(pi, -0.1e1 / 0.2e1)
											* exp(-sk * pow(0.1e1 - gk * time / hk, 0.2e1) / time * hk / 0.4e1)
											* (sqrt(sk) * gk * time * pow(hk, -0.2e1) * pow(time / hk, -0.1e1 / 0.2e1) / 0.2e1
												+ sqrt(sk) * (0.1e1 - gk * time / hk) * pow(time / hk, -0.3e1 / 0.2e1) * time * pow(hk, -0.2e1) / 0.4e1))
											* exp(sk * (0.1e1 - (-gk * gk + 0.1e1) * time / hk / 0.2e1) / 0.2e1) / 0.2e1
											+ fk * c0 * (exp(gk * sk / 0.2e1) * erfc(sqrt(sk) * (0.1e1 + gk * time / hk) * pow(time / hk, -0.1e1 / 0.2e1) / 0.2e1)
												+ exp(-gk * sk / 0.2e1) * erfc(sqrt(sk) * (0.1e1 - gk * time / hk) * pow(time / hk, -0.1e1 / 0.2e1) / 0.2e1))
											* sk * (-gk * gk + 0.1e1) * time * pow(hk, -0.2e1) * exp(sk * (0.1e1 - (-gk * gk + 0.1e1) * time / hk / 0.2e1) / 0.2e1) / 0.8e1;
										if (deriv_h_k != deriv_h_k) // ... if nan ...
										{
											computError = true;
											break;
										}
										else
										{
											deriv_h = deriv_h_k;
										}
									}
									if (computError)
									{
										break;
									}
									else
									{
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
									double fk = vectPaths[idFlowpath - 1][0];
									double hk = vectPaths[idFlowpath - 1][1];
									double sk = vectPaths[idFlowpath - 1][2];
									double gk = vectPaths[idFlowpath - 1][3];
									if (time > 0)
									{
										double deriv_s_k = fk * c0 * (gk * exp(gk * sk / 0.2e1) * erfc(sqrt(sk) * (0.1e1 + gk * time / hk) * pow(time / hk, -0.1e1 / 0.2e1) / 0.2e1) / 0.2e1
											- exp(gk * sk / 0.2e1) * pow(pi, -0.1e1 / 0.2e1) * exp(-sk * pow(0.1e1 + gk * time / hk, 0.2e1) / time * hk / 0.4e1)
											* pow(sk, -0.1e1 / 0.2e1) * (0.1e1 + gk * time / hk) * pow(time / hk, -0.1e1 / 0.2e1) / 0.2e1
											- gk * exp(-gk * sk / 0.2e1) * erfc(sqrt(sk) * (0.1e1 - gk * time / hk) * pow(time / hk, -0.1e1 / 0.2e1) / 0.2e1) / 0.2e1
											- exp(-gk * sk / 0.2e1) * pow(pi, -0.1e1 / 0.2e1) * exp(-sk * pow(0.1e1 - gk * time / hk, 0.2e1) / time * hk / 0.4e1)
											* pow(sk, -0.1e1 / 0.2e1) * (0.1e1 - gk * time / hk) * pow(time / hk, -0.1e1 / 0.2e1) / 0.2e1)
											* exp(sk * (0.1e1 - (-gk * gk + 0.1e1) * time / hk / 0.2e1) / 0.2e1) / 0.2e1
											+ fk * c0 * (exp(gk * sk / 0.2e1) * erfc(sqrt(sk) * (0.1e1 + gk * time / hk) * pow(time / hk, -0.1e1 / 0.2e1) / 0.2e1)
												+ exp(-gk * sk / 0.2e1) * erfc(sqrt(sk) * (0.1e1 - gk * time / hk) * pow(time / hk, -0.1e1 / 0.2e1) / 0.2e1))
											* (0.1e1 / 0.2e1 - (-gk * gk + 0.1e1) * time / hk / 0.4e1) * exp(sk * (0.1e1 - (-gk * gk + 0.1e1) * time / hk / 0.2e1) / 0.2e1) / 0.2e1;
										if (deriv_s_k != deriv_s_k) // ... if nan ...
										{
											computError = true;
											break;
										}
										else
										{
											deriv_s = deriv_s_k;
										}
									}
									if (computError)
									{
										break;
									}
									else
									{
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
						else if (paramType == 'g')
						{
							int idFlowpath;
							if (istringstream(pestParamName.substr(1, pestParamName.size() - 1)) >> idFlowpath)
							{
								if ((idFlowpath >= 1) && (idFlowpath <= noFlowpaths))
								{
									double deriv_g = 0;
									double fk = vectPaths[idFlowpath - 1][0];
									double hk = vectPaths[idFlowpath - 1][1];
									double sk = vectPaths[idFlowpath - 1][2];
									double gk = vectPaths[idFlowpath - 1][3];
									if (time > 0)
									{
										double deriv_g_k = fk * c0 * (sk * exp(gk * sk / 0.2e1) * erfc(sqrt(sk) * (0.1e1 + gk * time / hk) * pow(time / hk, -0.1e1 / 0.2e1) / 0.2e1) / 0.2e1
											- exp(gk * sk / 0.2e1) * pow(pi, -0.1e1 / 0.2e1) * exp(-sk * pow(0.1e1 + gk * time / hk, 0.2e1) / time * hk / 0.4e1)
											* sqrt(sk) * time / hk * pow(time / hk, -0.1e1 / 0.2e1) - sk * exp(-gk * sk / 0.2e1)
											* erfc(sqrt(sk) * (0.1e1 - gk * time / hk) * pow(time / hk, -0.1e1 / 0.2e1) / 0.2e1) / 0.2e1
											+ exp(-gk * sk / 0.2e1) * pow(pi, -0.1e1 / 0.2e1) * exp(-sk * pow(0.1e1 - gk * time / hk, 0.2e1) / time * hk / 0.4e1)
											* sqrt(sk) * time / hk * pow(time / hk, -0.1e1 / 0.2e1)) * exp(sk * (0.1e1 - (-gk * gk + 0.1e1) * time / hk / 0.2e1) / 0.2e1) / 0.2e1
											+ fk * c0 * (exp(gk * sk / 0.2e1) * erfc(sqrt(sk) * (0.1e1 + gk * time / hk) * pow(time / hk, -0.1e1 / 0.2e1) / 0.2e1)
												+ exp(-gk * sk / 0.2e1) * erfc(sqrt(sk) * (0.1e1 - gk * time / hk) * pow(time / hk, -0.1e1 / 0.2e1) / 0.2e1))
											* sk * gk * time / hk * exp(sk * (0.1e1 - (-gk * gk + 0.1e1) * time / hk / 0.2e1) / 0.2e1) / 0.4e1;
										if (deriv_g_k != deriv_g_k) // ... if nan ...
										{
											computError = true;
											break;
										}
										else
										{
											deriv_g = deriv_g_k;
										}
									}
									if (computError)
									{
										break;
									}
									else
									{
										drvFile << deriv_g;
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
			if (inputFileError)
			{
				cerr << "Input file error: check file structure and parameter values" << endl;
			}
			if ((inputFileError) || (computError))
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
	//int value_returned = std::system("PAUSE"); // waiting for user key press
	//Sleep(100);
	return 0;
}