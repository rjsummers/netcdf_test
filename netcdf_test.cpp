// netcdf_test.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <vector>
#include <complex>
#include <chrono>

#include <netcdf>
#include <fftw3.h>

int main()
{
	int fftw_init_threads(void);
	fftw_plan_with_nthreads(8);

	// Load data from datafile.
	netCDF::NcFile dataFile("in_data.nc", netCDF::NcFile::read);

	unsigned long long sample_count;
	auto data = dataFile.getVar("sample_count");
	data.getVar(&sample_count);

	double sampling_frequency;
	data = dataFile.getVar("sampling_frequency");
	data.getVar(&sampling_frequency);

	double sampling_period;
	data = dataFile.getVar("sampling_period");
	data.getVar(&sampling_period);

	std::vector<double> time(sample_count, 0.0);
	data = dataFile.getVar("time");
	data.getVar(&time[0]);

	// Initialize data vectors and create FFT plan before reading in rx_data
	std::vector<std::complex<double>> rx_data(sample_count, 0.0);
	std::vector<std::complex<double>> rx_dft(sample_count, 0.0);

	fftw_plan p = fftw_plan_dft_1d(sample_count, reinterpret_cast<fftw_complex*>(&rx_data[0]), reinterpret_cast<fftw_complex*>(&rx_dft[0]), FFTW_FORWARD, FFTW_ESTIMATE);

	data = dataFile.getVar("rx_data");
	data.getVar(&rx_data[0]);

	// Run the FFT

	auto start = std::chrono::high_resolution_clock::now();

	fftw_execute(p);
	fftw_destroy_plan(p);

	auto stop = std::chrono::high_resolution_clock::now();
	auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);

	std::cout << rx_dft[0] << std::endl;
	std::cout << rx_dft[1] << std::endl;
	std::cout << rx_dft[2] << std::endl;
	std::cout << rx_dft[3] << std::endl;

	std::cout << duration.count() << std::endl;

	netCDF::NcFile outputFile("out_data.nc", netCDF::NcFile::replace);

	auto tDim = outputFile.addDim("t", sample_count);
	auto complexDim = outputFile.addDim("complex", 2);
	data = outputFile.addVar("rx_dft", netCDF::ncDouble, {tDim, complexDim});
	data.putVar(&rx_dft[0]);

	return 0;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
