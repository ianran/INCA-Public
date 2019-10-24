function [D] = INCA_PID_Controller(Kp, Kd, Ko, Ki, r_sun, r_target, omegaTarg, Bbody, t, xhat, i)
%% INCA PID Controller
% Kyle Rankin
% 9/25/19
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

%% Init Function
persistent E_old E_sum t_old i_old xhatNoise r_sunNoise

% !!!!!!!!!!!!!!!!!!!!!!!!!! TEST CODE !!!!!!!!!!!!!!!!!!!!!!!!!!
% clc; clear; close all;
% Kp = eye(3);% Proportional Gain Matrix
% Kd = eye(3);% Derivitive Gain Matrix
% Ki = eye(3);% Integral Gain Matrix
% xhat = [0, 0, 0, 1, 0, 0, 0, 0]';% state vector [q; qdot]
% r_sun = [0,0,1]';% unit vector showint the desired pointint direction in the body frame
% r_target = [1,0,0]';% unit vector showing the desired axis alignment
% E_sum = [];% Initilizes sum of the error vector
% Bbody = [0,0,1]' * 3.12e-5;% Earth Magnetic Field in the body frame
% t = 15;
% !!!!!!!!!!!!!!!!!!!!!!!!!! TEST CODE !!!!!!!!!!!!!!!!!!!!!!!!!!

% Initilize E_sum
if isfinite(E_sum)
else
    E_sum = [0, 0, 0]';
    t_old = 0;
    E_old = [0,0,0]';
    i_old = 0;
%     xhatNoise = zeros(8,1);
%     r_sunNoise = zeros(3,1);
end


%% Add System Noise
% var = 0.001;
% if i_old < i
%     xhatNoise = [normrnd(0,var), normrnd(0,var), normrnd(0,var), normrnd(0,var), normrnd(0,var), normrnd(0,var), normrnd(0,var), normrnd(0,var)]';
%     r_sunNoise = [normrnd(0,var), normrnd(0,var), normrnd(0,var)]';
% end
%
%
% xhat = xhat + xhatNoise;
% r_sun = r_sun + r_sunNoise;


%% Normalize target and sun vectors
r_sun = r_sun / norm(r_sun);
r_target = r_target / norm(r_target);



%% Calculate Rotation Rate
Xi = @(X) [X(4), -X(3),  X(2);
    X(3),  X(4), -X(1);
    -X(2),  X(1),  X(4);
    -X(1), -X(2), -X(3)];

omega = -2 * Xi(xhat(1:4))' * xhat(5:8);

omegaErr = omega - omegaTarg;


%% Calculate Error Vector
TargCrossSun = cross(r_sun, r_target);


if norm(TargCrossSun) ~= 0
    E_des = (TargCrossSun / norm(TargCrossSun)) * acos(dot(r_sun, r_target)) / pi;% Error vector - alligned with desired torque

else% Handle error case error is 180deg off
    E_des = [1,0,0]'  * acos(dot(r_sun, r_target)) / pi;
end




%% Project Err_des and omegaT onto B plane
E_act = cross(Bbody, cross(E_des, Bbody)) / norm(Bbody)^2;
omegaErr = cross(Bbody, cross(omegaErr, Bbody)) / norm(Bbody)^2;


%% Calculate Desired Torque (PID Controller)
del_t = (t - t_old);

% Integral Term
E_sum = E_sum + E_des * del_t;

% Derivitive Term
if del_t ~= 0
    E_dot = cross(E_act, E_old) / del_t;
else
    E_dot = [0,0,0]';
end

% PID
tau = Kp * E_act + Kd * E_dot + Ko * omegaErr + Ki * E_sum;

% Update old terms
if i_old < i
    t_old = t;
    E_old = E_act;
    i_old = i;
end


%% Convert torque to dipole
D = (cross(Bbody, tau)) / (norm(Bbody)^2);
