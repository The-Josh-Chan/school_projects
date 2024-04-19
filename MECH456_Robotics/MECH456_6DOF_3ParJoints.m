% Forward Displacement Analysis and Jacobian Matrix of the 6 DOF Manipulator 
% with Three Parallel Revolute Joints(6R)
% The manipulator consists of 6 revolute joints, joints 2, 3 and 4 being parallel.
% The first joint is located normal to the ground (z axis) while the second revolute joint is perpendicular to and intersects the first joint.
% The third and fourth joint axes are parallel to the second joint axis...
%
% Original code provided by the MECH456 Course.
% Edited by Joshua Chan, Matt Birchard, and Chris Roper to simulate groups designed project. 

% The DH parameters of the manipulator are tabulated :

%         theta_i       d_i                   a_i               alpha_i
%         theta1=0      d_1 = s              a_1 = 0           alpha_1 = 0
%         theta2        d_2 = 0              a_2 = 0           alpha_2 = -90
%         theta3        d_3 = l2             a_3 = l3          alpha_3 = 0
%         theta4        d_4 = l4             a_4 = 0           alpha_4 = 90
%         theta5        d_5 = l5             a_5 = 0           alpha_5 = 0

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

close all
clc

function penroseCheck(check)
    % This function takes a variable check1 and prints a message 
    % based on the value of check1.
    simp = simplify(check);
    if simp == 0
        disp('Passed');
    else 
        disp('Failed');
    end
end

% Define symbolic variables as real values:
% Joint Displacements % Theta1 is 0 since joint 1 is a prismatic joint
t1 = 0; t2 = sym('t2','real'); t3 = sym('t3','real'); t4 = sym('t4','real'); t5 = sym('t5','real'); 
% Prismatic joint and Link Lengths
s = sym('s', 'real'); l2 = sym('l2','real'); l3 = sym('l3','real'); l4 = sym('l4','real'); l5 = sym('l5','real');


% Define DH parameters:
d1 = s;  a1 = 0;  alph1 = 0;
d2 = 0;  a2 = 0;  alph2 = -90;
d3 = l2; a3 = l3; alph3 = 0;
d4 = l4; a4 = 0;  alph4 = 90;
d5 = l5; a5 = 0;  alph5 = 0;


% Define shorthands:
s1=sin(t1); s2=sin(t2); s3=sin(t3); s4=sin(t4); s5=sin(t5);
c1=cos(t1); c2=cos(t2); c3=cos(t3); c4=cos(t4); c5=cos(t5); 
s23=sin(t2+t3); s34=sin(t3+t4); c23=cos(t2+t3); c34=cos(t3+t4);
s234=sin(t2+t3+t4); c234=cos(t2+t3+t4);

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% CALCULATE TRANSFORMATION MATRICES AND END EFFECTOR POSE (A_06)

A01 = [c1 (-cosd(alph1)*s1)  (sind(alph1)*s1) (a1*c1);
       s1  (cosd(alph1)*c1) (-sind(alph1)*c1) (a1*s1);
        0       sind(alph1)       cosd(alph1)      d1;
        0                  0                  0       1];

A12 = [c2 (-cosd(alph2)*s2)  (sind(alph2)*s2) (a2*c2);
       s2  (cosd(alph2)*c2) (-sind(alph2)*c2) (a2*s2);
        0       sind(alph2)       cosd(alph2)      d2;
        0                  0                  0       1];

A23 = [c3 (-cosd(alph3)*s3)  (sind(alph3)*s3) (a3*c3);
       s3  (cosd(alph3)*c3) (-sind(alph3)*c3) (a3*s3);
        0       sind(alph3)       cosd(alph3)      d3;
        0                  0                  0       1];

A34 = [c4 (-cosd(alph4)*s4)  (sind(alph4)*s4) (a4*c4);
       s4  (cosd(alph4)*c4) (-sind(alph4)*c4) (a4*s4);
        0       sind(alph4)       cosd(alph4)      d4;
        0                  0                  0       1];

A45 = [c5 (-cosd(alph5)*s5)  (sind(alph5)*s5) (a5*c5);
       s5  (cosd(alph5)*c5) (-sind(alph5)*c5) (a5*s5);
        0       sind(alph5)       cosd(alph5)      d5;
        0                  0                  0       1];

A02 = (A01*A12);
A03 = simplify(A02*A23);
A04 = simplify(A03*A34);
A05 = simplify(A04*A45);
disp("Rotation Matrix");
disp(A05);

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% CALCULATE JACOBIAN MATRIX:

% Define position vector of end effector:
P05 = A05(1:3,4);

% Define unit vectors for joint axes (3rd column of rotation matrices):
zero1 = [0 0 0].'; % pjoint
z01 = A01(1:3,3);
z02 = A02(1:3,3);
z03 = A03(1:3,3);
z04 = A04(1:3,3);
z05 = A05(1:3,3);
v0 = [0;0;0];

% Formulate the last 3 rows of Jacobian matrix using derivatives of P06 with respect to theta_i:
P05_s = [0;0;1];  % Partial derivative of P06 with respect to s, since prismatic joint, there is no contribution to the z-axis rotation
P05_t2 = simplify(diff(P05,t2));  % Partial derivative of P06 with respect to t2
P05_t3 = simplify(diff(P05,t3));  % Partial derivative of P06 with respect to t3
P05_t4 = simplify(diff(P05,t4));  % Partial derivative of P06 with respect to t4
P05_t5 = simplify(diff(P05,t5));  % Partial derivative of P06 with respect to t4

% Construct Jacobian matrix and its determinant, check its rank
Jac = [zero1 z01 z02 z03 z04; P05_s P05_t2 P05_t3 P05_t4 P05_t5];



% Calculate A_05 and J matrix when s and k are zero
%s = 0.0;
A05_compact = simplify(eval(A05));

J = simplify(eval(Jac));
disp("Jacobian Matrix");
disp(J);
rankJ = rank(J);
disp("Rank of J");
disp(rankJ);

% Forward Velocity Analysis
sdot = sym('sdot', 'real'); 
t2dot = sym('t2dot', 'real'); t3dot = sym('t3dot', 'real'); t4dot = sym('t4dot', 'real'); t5dot = sym('t5dot', 'real'); 
joint_divs = [sdot;t2dot;t3dot;t4dot;t5dot];
V = J * joint_divs;
disp("FVA Analysis: Components of EE velocity")
disp(V)

% To get the spin matrix, we will use the rotation matrix and its derivitive to get Omega
r05 =  A05(1:3,1:3);
rdt2 = simplify(diff(r05,t2));
rdt3 = simplify(diff(r05,t3));
rdt4 = simplify(diff(r05,t4));
rdt5 = simplify(diff(r05,t5));
rdot = simplify(rdt2 + rdt3 + rdt4 + rdt5);
spin_matrix = (rdot*r05.');
disp("Spin Matrix using Spin Matrix Method with rotation matricies");
disp(spin_matrix);

% IVA
% Generalized Inverse of Jacobian Matrix minimum 2-norm Joint Velocity
% Vector
Jt = J.';
disp("Determinant of Jacobian Matrix")
detJ = simplify(det(Jt * J));
disp(detJ)
Jgi = simplify((inv(Jt*J))*Jt);
qdot_2norm = Jgi * V; 
disp("Generalized Inverse of Jacobian Matrix (2-norm joint velocity vector");
disp(qdot_2norm);

% IVA using n independent end effector motuions
%use first 5 rows of the jacobian matrix (p-joint, t2, t3, t4, t5)
J_2 = J(1:5,1:5);
V_f = V(1:5);
qdot_5x5 = pinv(J_2) * V_f;
disp("Generalized Inverse of Jacobian Matrix (5x5 independant");
disp(simplify(qdot_5x5));

%Penrose Conditions
disp("Checking Penrose Conditions")
check1 = J*Jgi*J - J;
disp("Penrose Condition 1")
penroseCheck(check1);

check2 = Jgi*J*Jgi - Jgi;
disp("Penrose Condition 2")
penroseCheck(check2);

check3 = (J*Jgi).' - J*Jgi;
disp("Penrose Condition 3")
penroseCheck(check3);

check4 = (Jgi*J).' - Jgi*J;
disp("Penrose Condition 4")
penroseCheck(check4);
    
%Compliance Matrix
%Stiffness matrix - for compliance matrix calculation
k1 = sym('k1','real'); k2 = sym('k2','real'); k3 = sym('k3','real'); k4 = sym('k4','real'); k5 = sym('k5','real'); k6 = sym('k6','real');

k = [k1 0 0 0 0;
     0 k2 0 0 0;
     0 0 k3 0 0;
     0 0 0 k4 0;
     0 0 0 0 k5];

disp("Stiffness Matrix");
disp(k);

%Compliance Matrix --> [C] = [J][K]^-1[J]^T
k_inv = inv(k);
[C] = (J*k_inv)*Jt;
disp(C)

%End Effector Stiffness Matrix
[k_ee] = inv(C);

size(C);
rank(C);

% Write these as disp statements with titles so the TA knows what they're
% looking at when they print. 

