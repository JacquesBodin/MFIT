#include "pch.h"
#include <iostream>
#include <iomanip> // std::setprecision
#include <fstream>
#include <vector>
#include <boost/math/constants/constants.hpp>
//#include <Windows.h> // sleep

using namespace std;

const double pi = boost::math::constants::pi<double>();

int main(int argc, char *argv[])
{
	bool drvOption(false); // indicates whether the program must compute BOTH c(t) values AND derivatives
	bool tplFileError(false); // file reading error flag
	vector<string> vectPestParams;

	if ((argc > 1) && (string(argv[1]) == "/d")) // the command line 'MDMi /d' indicates that both c(t) values and derivatives must be computed
	{
		drvOption = true;
		ifstream tplFile("MFIT.tpl", ios::in); // reading the file "MFIT.tpl" and implementing vectPestParams (list of parameters for which optimization is required)
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
		double qt; // total flow rate
		int noFlowpaths; // number of flow (and transport) paths
		vector< vector<double> > vectPaths; // noFlowpaths x 3 parameters (m, h, s) = (mass, T0, Pe)
		vector<double> vectMHS(3, 0); // a single flow path, initialized as [0,0,0]
		bool inputFileError(false); // file reading error flag

		if (inputFile >> tMin >> tMax >> noTimeSteps >> qt >> noFlowpaths)
		{
			if ((tMin >= 0) && (tMax > tMin) && (noTimeSteps >= 1) && (qt > 0) && (noFlowpaths >= 1))
			{
				for (int i = 0; i < noFlowpaths; i++)
				{
					if (inputFile >> vectMHS[0] >> vectMHS[1] >> vectMHS[2]) // m >> h >> s
					{
						if ((vectMHS[0] >= 0) && (vectMHS[1] > 0) && (vectMHS[2] >= 0))
						{
							vectPaths.push_back(vectMHS);
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
					double mj = vectPaths[j][0]; // mass
					double hj = vectPaths[j][1]; // T0
					double sj = vectPaths[j][2]; // Pe
					if (time > 0)
					{
						concentration += mj / qt / hj * pow(pi / sj * pow(time, 0.3e1) * pow(hj, -0.3e1), -0.1e1 / 0.2e1)
							* exp(-sj * hj * pow(0.1e1 - time / hj, 0.2e1) / time / 0.4e1) / 0.2e1;
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
								double hk = vectPaths[k][1];
								double sk = vectPaths[k][2];
								if (time > 0)
								{
									deriv_qt += -mk * pow(qt, -0.2e1) / hk
										* pow(pi / sk * pow(time, 0.3e1) * pow(hk, -0.3e1), -0.1e1 / 0.2e1)
										* exp(-sk * hk * pow(0.1e1 - time / hk, 0.2e1) / time / 0.4e1) / 0.2e1;
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
									double hk = vectPaths[idFlowpath - 1][1];
									double sk = vectPaths[idFlowpath - 1][2];
									if (time > 0)
									{
										deriv_m = 0.1e1 / qt / hk * pow(pi / sk * pow(time, 0.3e1) * pow(hk, -0.3e1), -0.1e1 / 0.2e1)
											* exp(-sk * hk * pow(0.1e1 - time / hk, 0.2e1) / time / 0.4e1) / 0.2e1;
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
						else if (paramType == 'h')
						{
							int idFlowpath;
							if (istringstream(pestParamName.substr(1, pestParamName.size() - 1)) >> idFlowpath)
							{
								if ((idFlowpath >= 1) && (idFlowpath <= noFlowpaths))
								{
									double deriv_h = 0;
									double mk = vectPaths[idFlowpath - 1][0];
									double hk = vectPaths[idFlowpath - 1][1];
									double sk = vectPaths[idFlowpath - 1][2];
									if (time > 0)
									{
										deriv_h = -mk * exp(-sk / hk * pow(hk - time, 0.2e1) / time / 0.4e1) / time
											* (sk * hk * hk - sk * time * time - 0.2e1 * hk * time) * pow(hk, -0.3e1)
											* pow(pi / sk * pow(time, 0.3e1) * pow(hk, -0.3e1), -0.1e1 / 0.2e1) / qt / 0.8e1;
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
									double hk = vectPaths[idFlowpath - 1][1];
									double sk = vectPaths[idFlowpath - 1][2];
									if (time > 0)
									{
										deriv_s = -mk * (pow(time - hk, 0.2e1) * sk - 0.2e1 * hk * time)
											* pow(pi / sk * pow(time, 0.3e1) * pow(hk, -0.3e1), -0.1e1 / 0.2e1)
											* exp(-sk / hk * pow(hk - time, 0.2e1) / time / 0.4e1) / time * pow(hk, -0.2e1) / sk / qt / 0.8e1;
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
			if (inputFileError)
			{
				cerr << "Input file error: check file structure and parameter values" << endl;
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
	//Sleep(3000);
	return 0;
}