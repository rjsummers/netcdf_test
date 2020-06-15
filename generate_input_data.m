clear; close all; clc;

Fs = 100e6;
Ts = 1 / Fs;
t = (0:Ts:1e-4).';
Ns = uint64(length(t));

f0 = 82.7e6;
w0 = 2 * pi * f0;
x = exp(1j * w0 * t);

figure();
plot(t, real(x));

figure();
plot(t, imag(x));

delete in_data.nc

nccreate('in_data.nc', 'sampling_frequency',...
          'Format', 'netcdf4');
ncwrite('in_data.nc', 'sampling_frequency', Fs);

nccreate('in_data.nc', 'sampling_period');
ncwrite('in_data.nc', 'sampling_period', Ts);

nccreate('in_data.nc', 'sample_count');
ncwrite('in_data.nc', 'sample_count', Ns);

nccreate('in_data.nc', 'time',...
         'Dimensions', {'t', Ns});
ncwrite('in_data.nc', 'time', t);

nccreate('in_data.nc', 'rx_data',...
         'Dimensions', {'complex', 2, 't', Ns});
ncwrite('in_data.nc', 'rx_data', [real(x) imag(x)].');

tic
xs = fft(x);
toc