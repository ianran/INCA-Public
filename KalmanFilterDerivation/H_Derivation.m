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

%% Solve Jacobian of Rate Gyro
% Kyle Rankin & Ian Rankin

%% Init Code
clc; clear; close all;

%% Init Variables
syms q1 q2 q3 q4 q1_dot q2_dot q3_dot q4_dot

assume(q1,'real'); assume(q2,'real'); assume(q3,'real'); assume(q4,'real');
assume(q1_dot,'real'); assume(q2_dot,'real'); assume(q3_dot,'real'); assume(q4_dot,'real');


%% Input Matrixis
R_eb = [q1^2 - q2^2 - q3^2 + q4^2, 2 * (q1*q2 + q3*q4), 2 * (q1*q3 - q2*q4);
        2 * (q2*q1 - q3*q4), -q1^2 + q2^2 - q3^2 + q4^2, 2 * (q2*q3 + q1*q4);
        2 * (q3*q1 + q2*q4), 2 * (q3*q2 - q1*q4), -q1^2 - q2^2 + q3^2 + q4^2];

B = [q4, -q3, q2; q3, q4, -q1; -q2, q1, q4; -q1, -q2, -q3];

q_dot = [q1_dot, q2_dot, q3_dot, q4_dot]';

q = [q1, q2, q3, q4]';

% G = [eye(3),[0,0,0]'];


%% Math from hell

A = R_eb * B' * q_dot;


Sol = [diff(A,q1), diff(A,q2), diff(A,q3), diff(A,q4)];

Sol = simplify(Sol);

pretty(Sol)


%% Write Out Results
FID = fopen('Hout.txt', 'w');

[n,m] = size(Sol);

for ii = 1 : n
    for jj = 1 : m
        fprintf(FID, 'H(%.0f, %.0f)\n',ii, jj);
        fprintf(FID, '%s', Sol(ii,jj));
        fprintf(FID, '\n\n\n\n\n');
    end
end

fclose(FID);
