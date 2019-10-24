function [r] = KeplOrbitModel(rp,e,RAAN,inc,ArgPer,t)

% %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Keplarian Orbit Model - For Earth
% Kyle Rankin
%
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
%
% rp = perigee radius (km)
% e = eccentrisity
% delt = time step (s)
% RAAN = Initial Right Assention of the Assending Node (deg)
% inc = Initial Inclination (deg)
% ArgPer = Initial Argument of Perigee (deg)
% NumberOrbits = Number of Orbits
%
% %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%


%% Defign Variables
u = 398600.44189; % mue of earth (km^3 * s^-2)
a = (rp) / (1-e); % semi-major axis of leader (DetectorSat) (km)
T = (2*pi / sqrt(u)) * a^(3/2);  % Orbit period (s)
% t_f = T * NumberOrbits;% Simulation Period (s)
i = 0;
cnt = 0;
J2 = 1.08263e-3;
Re = 6378;% km


%% Calculate h
h = sqrt(a * u * (1-e^2));


%% Calculate Rotation Matrix
RAAN = RAAN * pi / 180;
inc = inc * pi / 180;
ArgPer = ArgPer * pi / 180;

PeriECI = @(RAAN,inc,ArgPer) [cos(-RAAN),sin(-RAAN),0; -sin(-RAAN),cos(-RAAN),0; 0,0,1]...
    * [1,0,0; 0,cos(-inc),sin(-inc); 0,-sin(-inc),cos(-inc)]...
    * [cos(-ArgPer),sin(-ArgPer),0; -sin(-ArgPer),cos(-ArgPer),0; 0,0,1];



    %% Calculate Mean Anomaly

    Me = (u^2 / h^3) * (1 - e^2)^(3/2) * t;

    % Reduce to be less then 2pi
    while Me > 2*pi
        Me = Me - 2*pi;
    end


    %% Calculate Eccentric Anomaly

    err = 1;

    % Initial Guess
    if Me < pi
        E = Me + e/2;
    else
        E = Me - e/2;
    end


    while err > (10^(-14))
        cnt = cnt+1;
        E_old = E;

        E = E_old - ((E_old - e * sin(E_old) - Me) / (1 - e * cos(E_old)));

        err = abs(E_old - E);
    end



    %% Calculate Position of detector Sat wrt, time
%     r_Polar(:,i) = [a * (1 - e*cos(E(i))), 2 * atan((-e*sqrt((1-e)/(1+e))*tan(E(i)/2) - sqrt((1-e)/(1+e))*tan(E(i)/2)) / (e-1)), 0];
%
%     if r_Polar(2,i) < 0
%         r_Polar(2,i) = r_Polar(2,i) + 2*pi;
%     end
%
%     r(:,i) = [r_Polar(1,i) * cos(r_Polar(2,i)), r_Polar(1,i) * sin(r_Polar(2,i)), 0]';

        r_Polar = [a * (1 - e*cos(E)), 2 * atan((-e*sqrt((1-e)/(1+e))*tan(E/2) - sqrt((1-e)/(1+e))*tan(E/2)) / (e-1)), 0];

    if r_Polar(2) < 0
        r_Polar(2) = r_Polar(2) + 2*pi;
    end

%     r(:,i) = [r_Polar(1,i) * cos(r_Polar(2,i)), r_Polar(1,i) * sin(r_Polar(2,i)), 0]';

      r = [r_Polar(1) * cos(r_Polar(2)), r_Polar(1) * sin(r_Polar(2)), 0]';


    %% J2 Effects - Assumes constant veriation
    RAAN_dot = -((3 * sqrt(u) * J2 * Re^2) / (2 * (1 - e^2)^2 * a^(7/2))) * cos(inc);
    ArgPer_dot = -((3 * sqrt(u) * J2 * Re^2) / (2 * (1 - e^2)^2 * a^(7/2))) * (5/2 * sin(inc)^2 - 2);

    RAAN = RAAN + RAAN_dot * t;
    ArgPer = ArgPer + ArgPer_dot * t;


    %% Rotate into ECI Coordinats
    r = PeriECI(RAAN,inc,ArgPer) * r;
