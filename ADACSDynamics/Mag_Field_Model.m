function [Binr] = Mag_Field_Model(rp, e, RAAN, inc, ArgPer, t)
%% Magnetic Field Model
% Kyle Rankin
% 09/10/19
% Copyright © 2019 New Mexico State Univeristy
%
% Permission is hereby granted, free of charge, to any person obtaining a copy of
% this software and associated documentation files (the “Software”), to deal in
% the Software without restriction, including without limitation the rights to use,
% copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the
% Software, and to permit persons to whom the Software is furnished to
% do so, subject to the following conditions:
%
% The above copyright notice and this permission notice shall be
% included in all copies or substantial portions of the Software.
%
% THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
% INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR
% PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE
% FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR
% OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
% OTHER DEALINGS IN THE SOFTWARE.

%% Init Program
%%%%%%%%%%%%%%%%%%%%%% Test Code %%%%%%%%%%%%%%%%%%%%%%
% clc; clear; close all;
% rp = 6738;
% e = 0;
% RAAN = 0;
% inc = 90;
% ArgPer = 0;
% t = 0;
%
%%%%%%%%%%%%%%%%%%%%%% Test Code %%%%%%%%%%%%%%%%%%%%%%

Re = 6378;% Radius of earth (km)
B0 = 3.12e-5;% Mean earth magnetic field at eaquator on surface (T)


%% Calculate Orbit Position
[rvec] = KeplOrbitModel(rp,e,RAAN,inc,ArgPer,t);

% rvec = [1,0,0]'; %%%%%%%%%%%%%%%%%%%%%% Test Code %%%%%%%%%%%%%%%%%%%%%%
% rvec = rvec / norm(rvec) * rp; %%%%%%%%%%%%%%%%%%%%%% Test Code %%%%%%%%%%%%%%%%%%%%%%

r = norm(rvec);
x = rvec(1)/r; y = rvec(2)/r; z = rvec(3)/r;

theta = acos(z);
phi = atan2(y, x);

if isnan(phi)
    phi = 0;
end




%% Calculate Magnetic Field - Dipole Model
Bsph = [-2 * B0 * (Re / r)^3 * cos(theta);
    -B0 * (Re / r)^3 * sin(theta)]; %Earth magnetic field in spherical coordinats (T)


%% Convert Magnetic Field to cartision Coordinats


R = [sin(theta) * cos(phi), cos(theta) * cos(phi);
     sin(theta) * sin(phi), cos(theta) * sin(phi);
     -cos(theta),            sin(theta)           ];


Binr = R * Bsph;

% norm(Bsph)
% norm(Binr)
