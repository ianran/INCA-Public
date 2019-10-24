function [D] = INCA_Bdot_Controller(Kb, B, t, i)
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

persistent Bold told Dold i_old

if isempty(Bold) || t == 0
    Bold = B;
    told = 0;
    Dold = [0,0,0]';
    i_old = 0;
end

del_t = (t - told);


Bdot = (B - Bold) / del_t;

% BdotCut = abs(Bdot) <

D =  Kb * Bdot / norm(B);


if (sum(isnan(D)) ~= 0)
    D = [0,0,0]';
end


if del_t <= 0
    D = Dold;
end


% [i_old, i]

if i_old < i
    Bold = B;
    told = t;
    Dold = D;
    i_old = i;
end
