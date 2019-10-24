function [xDot] = INCA_State_Model(t, Binr, xhat, InertialMatrixInv, Kb, Kp, Ko, Kd, Ki, r_sun, r_target, omegTarg,i)
%% INCA State Derivitive Function (f)
% Based on kalman filter EOM model
% Kyle Rankin - based on Ian Rankin C++ code
% 9/9/19


%% Init Function 
%%%%%%%%%%%%%%%%%%%%%%%% TEST CODE %%%%%%%%%%%%%%%%%%%%%%%%
% clc; clear; close all;
% D = [0,0,1]';
% Binr = [1,1,0]';
% xhat = [1,0,0,0,.001,0,0,-.004]';% x = [q,q_dot]'
% InertialMatrixInv = eye(3)^-1;
% 
% t = 5;
% Kb = [eye(3)];
% 
%%%%%%%%%%%%%%%%%%%%%%%% TEST CODE %%%%%%%%%%%%%%%%%%%%%%%%

% Function Handles
Xi = @(X) [X(4), -X(3),  X(2);
           X(3),  X(4), -X(1);
          -X(2),  X(1),  X(4);
          -X(1), -X(2), -X(3)];
      
quatInv = @(X) [-X(1), -X(2), -X(3), X(4)]';
      
quatTrans = @(q,v) hamMult(hamMult(q,v), quatInv(q))';
      

%% pull out q and qDot from the state vector.
q    = xhat(1:4);
qDot = xhat(5:8);

% xhat

%% Define D and B vectors. (calculate B vector in body frame)
Binr = [Binr; 0];

BbodyQuat = quatTrans(q, Binr);

Bbody = BbodyQuat(1:3);


%% Rotate Sun Vector into body frame
r_sunQuat = quatTrans(q, [r_sun; 0]);
r_sun = r_sunQuat(1:3);


%% Controller
[D_bdot] = INCA_Bdot_Controller(Kb, Bbody, t,i);
% D = [0.01, 0.01 ,0]';

[D_pid] = INCA_PID_Controller(Kp, Kd, Ko, Ki, r_sun, r_target, omegTarg, Bbody, t, xhat, i);

D = D_bdot + D_pid;


% Limit magnitude of D
if norm(D) > 0.01
    D = D / norm(D) * 0.01;
end



%% Generate q_dot protion of x_dot
xDot(1:4, 1) = qDot;


%% equation from derivation for q dot dot
% CORRECT solution.
xDot(5:8, 1) = (Xi(qDot) * Xi(q)' * qDot) + (0.5 * Xi(q) * InertialMatrixInv * cross(D, Bbody));

end

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
