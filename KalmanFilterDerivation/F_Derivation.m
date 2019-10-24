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
%% INCA Dynamics Jacobian
% Kyle Rankin - Ian Rankin
% 08/30/19

%% Init Program
clc; clear; close all;


%% Imput Variables
syms q1 q2 q3 q4
syms q1_dot q2_dot q3_dot q4_dot

syms I11 I12 I13 I21 I22 I23 I31 I32 I33

syms d1 d2 d3
syms b1 b2 b3

assume(q1, 'real'); assume(q2, 'real'); assume(q3, 'real'); assume(q4, 'real');
assume(q1_dot, 'real'); assume(q2_dot, 'real'); assume(q3_dot, 'real'); assume(q4_dot, 'real');

assume(I11, 'real'); assume(I12, 'real'); assume(I13, 'real');
assume(I21, 'real'); assume(I22, 'real'); assume(I23, 'real');
assume(I31, 'real'); assume(I32, 'real'); assume(I33, 'real');

assume(d1, 'real'); assume(d2, 'real'); assume(d3, 'real');
assume(b1, 'real'); assume(b2, 'real'); assume(b3, 'real');

%% Imput Functions
square = @(x) [x(4), -x(3), x(2);
               x(3), x(4), -x(1);
               -x(2), x(1), x(4);
               -x(1), -x(2), -x(3)];
q = [q1; q2; q3; q4];
q_dot = [q1_dot; q2_dot; q3_dot; q4_dot];

x = [q; q_dot];

A = square(q_dot) * square(q)' * q_dot;
A = simplify(A)

q_flipped = [q4, q1, q2, q3];

%Inr = [I11, I12, I13;
%       I21, I22, I23;
%       I31, I32, I33];

Inr = [I11, 0, 0;
       0, I22, 0;
       0, 0, I33];

B = [0, b1, b2, b3];
D = [d1;d2;d3];

foo = [ zeros(3,1) eye(3,3) ];

C = (1/2) * square(q) * (Inr^-1) * (cross(D, foo * quatmultiply(quatmultiply(q_flipped, B), quatinv(q_flipped))'));
C = simplify(C)

%% Solve the jacobian
%Apartial = jacobian(A, x);
%Apartial = simplify(Apartial)

%Cpartial = jacobian(C, x);
%Cpartial = simplify(Cpartial)

q_dotdot = A + C;

x_dot = [q_dot; q_dotdot];
x_dot = simplify(x_dot)

F = jacobian(x_dot, x);
F = simplify(F);

%% Write Out Results
FID = fopen('Fout.txt', 'w');

for ii = 1 : 8
    for jj = 1 : 8
        fprintf(FID, 'F(%.0f, %.0f)\n',ii, jj);
        fprintf(FID, '%s', F(ii,jj));
        fprintf(FID, '\n\n\n\n\n');
    end
end

fclose(FID);
