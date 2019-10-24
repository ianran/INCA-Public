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
%% INCA Dynamics Simulation - based on Kalman Filter EOM's
% Kyle Rankin
% 09/09/19


%% Init Program
clc; clear; close all;


%% Input Constants
RunTime = 16 * 3600 * 1;% Run Time in Seconds


Inr = [0.031000 0        0        ;
       0        0.031134 0        ;
       0        0        0.0183645];% INCA Inertial Matrix (kg*m^2)

% Inr = [0.03     0        0    ;
%        0        0.03     0    ;
%        0        0        0.03];% INCA Inertial Matrix (kg*m^2) !!!!!!!!!!!!!!! TEST CODE !!!!!!!!!!!!!!!
%

V = [1, .5, 0]'; % Initial vecter of rotation (will be normalized later)

theta = (pi / 180) * 120;% initial rotation angle (rad)

omega = (pi / 180) * [1, 5, -30]';% Initial Rotation Vector (rad/s)

% D = [0.01, 0.01, 0.00]';% S/C Magnetic Dipole (A * m^2)

% tOut = linspace(0, RunTime, 500);% Output time Vector (s)
% tOut = 0 : .01 : RunTime;

NumOutPoints = 15;% Output Time Step (s)


% Orbit Perameters
rp = 6878;
ecc = 0;
RAAN = 0;
inc = 90;
ArgPer = 0;


% Desired rotation vector
r_target = [0,0,1]';


% Controller Gains
Kb = [1, 0, 0;
      0, 1, 0;
      0, 0, 1] * 0e-0;

Kp = [1, 0, 0;
      0, 1, 0;
      0, 0, 1] * 1e-6;

Kd = [1, 0, 0;
      0, 1, 0;
      0, 0, 1] * 1e-5;

Ko = [10, 0, 0;
      0, 10, 0;
      0, 0, 0] * 1e-4;

Ki = [1, 0, 0;
      0, 1, 0;
      0, 0, 1/3] * 0e-10;

omegaTarg = 180 / pi * [0, 0, 0]';% Rotation Rate (rad/s)


%% Quaternion Inverse Functions
quatInv = @(X) [-X(1), -X(2), -X(3), X(4)]';
quatTrans = @(q,v) hamMult(hamMult(q,v), quatInv(q))';


%% Calculate Constants
InertialMatrixInv = Inr^-1;

Xi = @(X) [X(4), -X(3),  X(2);
    X(3),  X(4), -X(1);
    -X(2),  X(1),  X(4);
    -X(1), -X(2), -X(3)];


V = V / norm(V);% Normalize V

q = [V * sin(theta/2); cos(theta/2)];

qdot = 0.5 * Xi(q) * omega;

xhat_init = [q; qdot]

tSpan = [0, RunTime];


%% Generate function handle f
% Magnetic Field Vector
% Binr = @(t) [0, 0, 1]' * 3.18e-5;% !!!!!!!!!!!!!!!! TEST VALUE !!!!!!!!!!!!!!!!
[Binr] = @(t) Mag_Field_Model(rp, ecc, RAAN, inc, ArgPer, t);

% Sun Direction
r_sun = [1, 0, 0]';% !!!!!!!!!!!!!!!! TEST VALUE !!!!!!!!!!!!!!!!



f = @ (t, xhat, i) INCA_State_Model(t, Binr(t), xhat, InertialMatrixInv, Kb, Kp, Ko, Kd, Ki, r_sun, r_target, omegaTarg, i);



%% Solve with Dormand Prince 4/5 order pair
% Defign initial conditions

T = 1e-8; % Tolerance
w(:,1) = xhat_init;
t(1) = tSpan(1);
s7 = f(t(1),w(:,1), 1);
e = ones(8,1);
i = 1;
stepFail = 0;
LastWrite = 0;
PreAloc = 10000;
MaxTimeStep = 20;

while t(i) < tSpan(2)
    chk = 1;



    %% Prealocate Variables for speed in chunks
    if mod(i,PreAloc) == 0
        w(:, i+1 : i+PreAloc) = zeros(8,PreAloc);
        e(:, i+1 : i+PreAloc) = zeros(8,PreAloc);
        t(i+1 : i+PreAloc) = zeros(1,PreAloc);
        stepFail(i+1 : i+PreAloc) = zeros(1,PreAloc);
    end


    %% Calculate Step Size
    if i == 1
        h = 0.0001; % Initial Step Size
    else
        h = 0.8 * min((T * (abs(w(:,i)) + 1e-3) ./ e(:,i)).^(1/5)) * h;


        % Set max time step
        if (h > MaxTimeStep) || (isnan(h)) || (h == 0)
            h = MaxTimeStep;

        end
    end

    %% Normalize w(1:4) (q) vector
    w(1:4, i) = w(1:4, i) / norm(w(1:4, i));

    %% Run Itteration
    stepFail(i) = 0;
    while chk == 1
        t(i+1) = t(i) + h;
        [w(:,i+1), s7, e(:,i+1)] = DormandPrince45(f,h,t(i+1),w(:,i),s7, i);

        % Check solution tolerence
        if (max(e(:,i+1) ./ (10^-1 + abs(w(:,i+1)))) < T)
            chk = 0;
        else
            h = h / 15;
            stepFail(i) = stepFail(i) + 1;
        end
    end

    %% Write Out Status
    if mod(i,10000) == 0
        fprintf('\n Completed: %.1f h of %.1f h', t(i)/3600, tSpan(2)/3600)
        fprintf('\n Percent Complete: %.1f%% \n',t(i)/tSpan(2) * 100)
    end


    %% Break simulation if solution System exceads permitable perameters
    if (max(abs(w(:,i))) > 10^4) || (max(isnan(w(:,i))))
        fprintf('\n\n???????????????????????????????????????\n')
        fprintf('Solution Exceaded Permitable Perameters\n')
        fprintf('???????????????????????????????????????\n')
        break
    end

    %% Write last 10,000 variables of solution to text file
%     if mod(i,10000) == 0
%         fprintf('\n Starting Text File Write')
%         WriteToTextFile(FileName,w(:,LastWrite+1 : i),t(LastWrite+1 : i),e(:,LastWrite+1 : i),stepFail(LastWrite+1 : i));
%         LastWrite = i;
%         fprintf('\n Text File Write Compleated\n')
%     end

        i = i + 1;
end


fprintf('\n Percent Complete: %.1f%% \n\n Simulation Finished!\n\n',t(i)/tSpan(2) * 100)


%% Format Output
% for ii =  1 : length(t)-1
%     delta(ii) = -t(ii) + t(ii+1);
% end
%
% delta

tOut = tSpan(1) : NumOutPoints : tSpan(2);
xOut = spline(t(:, 1:i), w(:, 1:i), tOut);



%% Plot Ouput - Quaternion
figure()

plot(tOut / 3600, xOut(1:4, :), 'linewidth', 2)

xlabel('Time (h)', 'fontsize', 15)
ylabel('Quaternion', 'fontsize', 15)
title('Quaternion', 'fontsize', 20)

legend('q_1', 'q_2', 'q_3', 'q_4')
grid on


%% Plot Ouput - Quaternion Dot
figure()

plot(tOut / 3600, xOut(5:8, :), 'linewidth', 2)

xlabel('Time (h)', 'fontsize', 15)
ylabel('Quaternion Dot', 'fontsize', 15)
title('Quaternion Dot', 'fontsize', 20)

legend('q_1', 'q_2', 'q_3', 'q_4')
grid on



%% Calc/Plot- Omega
omega_out = zeros(3, length(tOut));% Pre-alocate for speed

for ii = 1 : length(tOut)
    omega_out(:,ii) = 2 * Xi(xOut(1:4, ii))' * xOut(5:8, ii);
    omega_norm(ii) = norm(omega_out(:,ii));
end

figure()

plot(tOut / 3600, omega_out * 180 / pi, 'linewidth', 2); hold all;
plot(tOut / 3600, omega_norm * 180 / pi, 'linewidth', 2)

xlabel('Time (h)', 'fontsize', 15)
ylabel('\omega (deg/s)', 'fontsize', 15)
title('Rotation Rate', 'fontsize', 20)

legend('\omega_1', '\omega_2', '\omega_3', '\omega_n_o_r_m')
grid on


%% Calc/Plot q norm
q_norm = zeros(1, length(tOut));% Pre-alocate for speed

for ii = 1 : length(tOut)
    q_norm(ii) = norm(xOut(1:4, ii));
end

figure()

plot(tOut / 3600, q_norm, 'linewidth', 2)

xlabel('Time (h)', 'fontsize', 15)
ylabel('norm(q)', 'fontsize', 15)
title('Quaternion Norm', 'fontsize', 20)

grid on


%% Calculate/Plot B field
% Bout = zeros(3, length(tOut));
% for ii = 1 : length(tOut)
%
%     Bout(:,ii) = Binr(tOut(ii));
% end
%
%
% figure()
%
% plot(tOut / 3600, Bout, 'linewidth', 2)
%
% xlabel('Time (h)', 'fontsize', 15)
% ylabel('B Field (T)', 'fontsize', 15)
% title('Earth''s Magnetic Field', 'fontsize', 20)
%
% legend('B_x', 'B_y', 'B_z')
%
% grid on



%% Calculate/Plot Orbit Position
% rVec = zeros(3, length(tOut));
% for ii = 1 : length(tOut)
%     rVec(:,ii) = KeplOrbitModel(rp,ecc,RAAN,inc,ArgPer,tOut(ii));
%
% end
%
%
% figure()
%
% plot(tOut / 3600, rVec, 'linewidth', 2)
%
% xlabel('Time (h)', 'fontsize', 15)
% ylabel('Orbit Position (km)', 'fontsize', 15)
% title('Orbit Plot', 'fontsize', 20)
%
% legend('r_x', 'r_y', 'r_z')
%
% grid on



figure(3)

%% Calculate / Plot Error Angle
figure()
r_sun_body = zeros(3, length(xOut)); ErrorAngle = zeros(1, length(xOut));

for ii = 1 : length(xOut)
    r_sun_Temp = quatTrans(xOut(1:4, ii), [r_sun; 0]);

    r_sun_body(:,ii) = r_sun_Temp(1:3);

   ErrorAngle(ii) = acosd((r_sun_body(:, ii)' * r_target) / (norm(r_sun_body(:, ii)) * norm(r_target)));
end

plot(tOut / 3600, ErrorAngle, 'linewidth', 2)

xlabel('Time (h)', 'fontsize', 15)
ylabel('Error Angle (deg)', 'fontsize', 15)
title('Pointing Error', 'fontsize', 20)

grid on




%% hamiltonian multiplication function
function [z] = hamMult(x, y)

   % i component
   z(1) = x(4) * y(1) + x(1) * y(4) + x(2) * y(3) - x(3) * y(2);

   % j component
   z(2) = x(4) * y(2) - x(1) * y(3) + x(2) * y(4) + x(3) * y(1);

   % k component
   z(3) = x(4) * y(3) + x(1) * y(2) - x(2) * y(1) + x(3) * y(4);

   % scalar component
   z(4) = x(4) * y(4) - x(1) * y(1) - x(2) * y(2) - x(3) * y(3);
end
