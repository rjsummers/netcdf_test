clear; close all; clc;

rx_dft = ncread('out_data.nc', 'rx_dft');

xs = rx_dft(1,:) + 1j * rx_dft(2,:);

plot(abs(xs))
